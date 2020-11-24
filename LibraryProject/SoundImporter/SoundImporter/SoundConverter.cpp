#include "SoundConverter.h"

//SoundConverter::SoundConverter(const char* Input, const char* output, const long MAX_ADUIO_IN_MS /*= 5000*/)
//{
//	// Main goes here may need a function for input. Data in and out.
//}

bool SoundConverter::Convert(std::string a_input, std::string a_output, const long MAX_ADUIO_IN_MS)
{

	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	const char* input = a_input.c_str();
	size_t size1 = strlen(input) + 1;
	wchar_t* in = new wchar_t[size1];
	size_t inSize;
	mbstowcs_s(&inSize, in, size1, input, size1 - 1);

	//wchar_t wtext2[20];
	//mbstowcs(wtext2, input.c_str(), input.length());//includes null
	//LPWSTR out = wtext2;

	const char* output = a_output.c_str();
	size_t size2 = strlen(output) + 1;
	wchar_t* out = new wchar_t[size2];
	size_t outSize;
	mbstowcs_s(&outSize, out, size2, output, size2 - 1);


	 


	// Error code? could be changed to a bool? Depends what the funcitons return.
	HRESULT hr = S_OK; // Success code. Handle.

	IMFSourceReader* pReader = NULL;	 // The thing that will give me the data.
	HANDLE hFile = INVALID_HANDLE_VALUE; // Handle for file to write to. For some context about what handles are - https://stackoverflow.com/questions/1303123/what-is-a-handle-in-c

	// Initialize the COM library?
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	// Initalize media foundation platform - https://docs.microsoft.com/en-us/windows/win32/medfound/media-foundation-platform-apis
	if (SUCCEEDED(hr))
	{
		hr = MFStartup(MF_VERSION);
	}

	// Create the file reader - Note that you create different kinds of source readers.
	if (SUCCEEDED(hr))
	{
		//hr = MFCreateSourceReaderFromURL(in, NULL, &pReader);
		hr = MFCreateSourceReaderFromURL(in, NULL, &pReader);
		//hr = MFCreateSourceReaderFromByteStream(in, NULL, &pReader);
		if (FAILED(hr))
		{
			printf("error opening input file: %S\n", in); // ,hr third argument?
		}
	}

	// Open output file. - Example of what create file does https://docs.microsoft.com/en-us/windows/win32/fileio/opening-a-file-for-reading-or-writing
	if (SUCCEEDED(hr))
	{
		//  Some info about createFile and it's other versions - https://stackoverflow.com/questions/51462048/what-is-the-difference-between-createfile-and-createfilea
		hFile = CreateFile(out, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_ALWAYS, 0, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			printf("Cannont create output file: %S\n", out); // ,hr third argument?
		}
	}

	if (SUCCEEDED(hr))
	{
		// The function that writes the file. Needs to be added above.
		hr = WriteWaveFile(pReader, hFile, MAX_ADUIO_IN_MS);
	}

	if (FAILED(hr))
	{	// This prints a hex code... not very useful for me
		printf("Failed, hr = 0x%X\n", hr);
	}

	// Close file?
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		delete[] in;
		delete[] out;
		return false;
	}

	SafeRelease(&pReader);
	MFShutdown();
	CoUninitialize();

	delete[] in;
	delete[] out;
	//delete input;
	//delete output;

	return true;
}



// Safe release is not included in headers. - https://docs.microsoft.com/en-us/windows/win32/medfound/saferelease
// Releases COM interface pointers. Alternate is smart pointers.
template <class T> void SoundConverter::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}


//-------------------------------------------------------------------
// WriteWaveFile - Function
// Writes a WAVE file by getting audio data from the source reader.
//-------------------------------------------------------------------

HRESULT SoundConverter::WriteWaveFile(IMFSourceReader* a_pReader, 
	HANDLE a_hfile, LONG a_msecAudioData)
{
	// For error checking?
	HRESULT hr = S_OK;

	// Dword being a "Double word" windows type 32 bit data type. Can contain integers.
	DWORD cbHeader = 0;
	DWORD cbAudioData = 0;
	DWORD cbMaxAudioData = 0;


	IMFMediaType* pAudioType = NULL; // What type of data to convert the file being read from into.

	hr = ConfigureAudioStream(a_pReader, &pAudioType); // What does the in file get converted into.

	// Write the wav header - Look at wavHeader.h for what the layout is.
	if (SUCCEEDED(hr))
	{
		hr = WriteWaveHeader(a_hfile, pAudioType, &cbHeader);
	}

	if (SUCCEEDED(hr))
	{
		// cb is what?
		cbMaxAudioData = CalculateMaxAudioDataSize(pAudioType, cbHeader, a_msecAudioData); // How much of the audio to decode in bytes 

		// Decode audio data to the file.
		hr = WriteWaveData(a_hfile, a_pReader, cbMaxAudioData, &cbAudioData);
	}

	if (SUCCEEDED(hr))
	{
		hr = FixChunkSize(a_hfile, cbHeader, cbAudioData);  // Adjusts the header based on what has occured during the conversion
	}

	SafeRelease(&pAudioType); // Release pointers.
	return hr;
}

//-------------------------------------------------------------------
// ConfigureAudioStream
// Selects an audio stream from the source file, and configures the
// stream to deliver decoded PCM audio. 
//-------------------------------------------------------------------

HRESULT SoundConverter::ConfigureAudioStream(
	IMFSourceReader* a_pReader,		// Pointer to the Source reader.
	IMFMediaType** ppPCMAuido		// Receives the audio format.
)
{
	// For the difference between complete and partial media types - https://docs.microsoft.com/en-us/windows/win32/medfound/complete-and-partial-media-types
	IMFMediaType* pUnconmressedAudioType = NULL; // This will be PCM.
	IMFMediaType* pPartialType = NULL; // A partial type will lack some attributes.

	// Deselect all the other streams.
	HRESULT hr = a_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, FALSE);

	if (SUCCEEDED(hr))
	{
		// Select First audio stream.
		hr = a_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
	}

	// Create the partial media type and it's attributes.
	hr = MFCreateMediaType(&pPartialType);
	if (SUCCEEDED(hr))
	{
		hr = pPartialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio); // Major Type: Audio.
	}

	if (SUCCEEDED(hr))
	{
		hr = pPartialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM); // Sub Type: Pcm
	}

	// Now we set this type to the source reader.
	// I think windows chooses a media type that works best from the information provided.
	if (SUCCEEDED(hr))
	{
		hr = a_pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, pPartialType);
	}
	// This should get the media type(which is now a complete type) out of the source reader
	if (SUCCEEDED(hr))
	{
		hr = a_pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pUnconmressedAudioType);
	}

	// I Feel like this is a safety.
	// Ensures the 1st audio stream is selected.
	if (SUCCEEDED(hr))
	{
		hr = a_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
	}

	// Return the format to the caller.
	if (SUCCEEDED(hr))
	{
		*ppPCMAuido = pUnconmressedAudioType;
		(*ppPCMAuido)->AddRef();
	}

	SafeRelease(&pUnconmressedAudioType);
	SafeRelease(&pPartialType);

	return hr;
}

//-------------------------------------------------------------------
// WriteWaveHeader - Depending on what file type you wanted to write out
// you would adjust this file to take in the file type.
// Write the WAVE file header.
// The details of the file will need to be adjusted later
//-------------------------------------------------------------------

HRESULT SoundConverter::WriteWaveHeader(
	HANDLE a_hFile,				// Output file
	IMFMediaType* a_pMediaType,	// PCM audio format. Provided by ConfigureAudioStream()
	DWORD* a_pcbWritten			// Size of the header
)
{
	HRESULT hr = S_OK;
	UINT32 cbFormat = 0;

	// The wavformat. Need to find the equivalant in other media types.
	// I think: MPEG1WAVEFORMAT may be the equiv in mp3? - https://en.wikipedia.org/wiki/MP3

	// Check Wav header for WaveFormat.h or mmeapi.h
	WAVEFORMATEX* pWav = NULL;

	*a_pcbWritten = 0;

	// Media Foundation framework background- https://en.wikipedia.org/wiki/Media_Foundation
	// Convert pcm into the correct structure. 
 	hr = MFCreateWaveFormatExFromMFMediaType(a_pMediaType, &pWav, &cbFormat);

	// The start of the header.
	if (SUCCEEDED(hr))
	{
		DWORD header[] = {
			FCC('RIFF'),
			0, //File size not known yet
			FCC('WAVE'),
			FCC('fmt '), // FMT chunk start
			cbFormat	// Should be modified by the above function MFCreateWaveFormatExFromMFMediaType.
		};

		DWORD dataHeader[] = { FCC('data'), 0 }; // Start of the data chunk. Goes after fmt. Unfilled becuase no data yet

		hr = WriteToFile(a_hFile, header, sizeof(header)); // 

		// Write the WAVEFORMATEX structure.
		if (SUCCEEDED(hr))
		{
			hr = WriteToFile(a_hFile, pWav, cbFormat);
		}

		if (SUCCEEDED(hr))
		{
			hr = WriteToFile(a_hFile, dataHeader, sizeof(dataHeader));
		}

		if (SUCCEEDED(hr))
		{
			*a_pcbWritten = sizeof(header) + cbFormat + sizeof(dataHeader); // Total size of header.
		}

	}


	// Different from the other releases?
	CoTaskMemFree(pWav);

	return hr;
}

//-------------------------------------------------------------------
// Writes a block of data to a file
// hFile: Handle to the file.
// p: Pointer to the buffer to write.
// cb: Size of the buffer, in bytes.
// Not really neccesary. Called multiple times so more for reuse
// and to cut down on bloat.
//-------------------------------------------------------------------

HRESULT SoundConverter::WriteToFile(HANDLE a_hFile, void* a_p, DWORD a_cb)
{
	DWORD cbWritten = 0;
	HRESULT hr = S_OK;
	// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
	BOOL bResult = WriteFile(a_hFile, a_p, a_cb, &cbWritten, NULL);

	if (!bResult)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	return hr;
}

//-------------------------------------------------------------------
// CalculateMaxAudioDataSize
//
// Calculates how much audio to write to the WAVE file, given the
// audio format and the maximum duration of the WAVE file.
// This function also works out how much data it can write to the 
// output file.
// Max size of a WAVE is 4GB.
//-------------------------------------------------------------------

DWORD SoundConverter::CalculateMaxAudioDataSize(
	IMFMediaType* a_pAudioType,		// Pcm audio format worked out by ConfigureAudioStream
	DWORD a_cbHeader,				// Size of the wav header worked out by WriteWaveHeader
	DWORD a_msecAudioData			// Maximum duration to read in. From Main.
)
{
	UINT cbBlockSize = 0;		// Audio frame size in bytes. Not sure if frame and sample are interchangeable.
	UINT cbBytesPerSecond = 0;	// Bytes per second.

	// Get the audio block size and number of bytes/second from the audio format.
	// Note that there are other get attribute functions that return different types.

	cbBlockSize = MFGetAttributeUINT32(a_pAudioType, MF_MT_AUDIO_BLOCK_ALIGNMENT, 0);
	cbBytesPerSecond = MFGetAttributeUINT32(a_pAudioType, MF_MT_AUDIO_AVG_BYTES_PER_SECOND, 0);


	// Desired size of file.
	DWORD cbAudioClipSize = (DWORD)MulDiv(cbBytesPerSecond, a_msecAudioData, 1000);

	// Largest file size after header file.
	DWORD cbMaxSize = MAXDWORD - a_cbHeader;


	// Whichever is smaller is the amount that is written to the file.
	// I feel like this should also throw an error or message
	// if you hit the max size limit.
	cbAudioClipSize = min(cbAudioClipSize, cbMaxSize);

	// Rounds the clip size to avoid partial audio frame/sample
	cbAudioClipSize = (cbAudioClipSize / cbBlockSize) * cbBlockSize;

	return cbAudioClipSize;
}

//-------------------------------------------------------------------
// WriteWaveData
//
// Decodes PCM audio data from the source file and writes it to
// the WAVE file.
// If I were to add reverb it would be somewhere in here since data is
// decoded. Would I need to get ALL the data out then apply the verb?
// I would need to adjust the maxAudioData. OR I could check the 
// amplitude of the pcm and then apply the verb.
//-------------------------------------------------------------------

HRESULT SoundConverter::WriteWaveData(
	HANDLE a_hfile,					// Output file
	IMFSourceReader* a_pReader,		// Source reader
	DWORD a_cbMaxAudioData,			// Maximum amount of adio data
	DWORD* pcbDataWritten			// Receives the data written.
)
{
	HRESULT hr = S_OK;
	DWORD cbAudioData = 0;			// The data read out in bits?
	DWORD cbBuffer = 0;				// Buffer to write to?
	BYTE* pAudioData = NULL;		// The pointer to the data read out?

	IMFSample* pSample = NULL;
	IMFMediaBuffer* pBuffer = NULL;


	// Loops through till no more audio to write from
	// or error
	while (true)
	{
		DWORD dwFlags = 0;

		// Read the next sample
		hr = a_pReader->ReadSample(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			0, NULL, &dwFlags, NULL, &pSample);

		if (FAILED(hr)) { break; }

		if (dwFlags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
		{
			printf("Type change - not supported by wave file format.\n");
			break;
		}
		if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			printf("End of input file.\n");
			break;
		}
		if (pSample == NULL)
		{
			printf("No sample.\n");
			continue;
		}


		// Get a pointer to the audio data in the sample.

		hr = pSample->ConvertToContiguousBuffer(&pBuffer);

		if (FAILED(hr)) { break; }

		// Lock in this case does not prevent other threads from calling Lock again. So does not synch threads.
		// Gives the caller(1) access to this (2) amount of data after(3)
		// https://docs.microsoft.com/en-us/windows/win32/api/mfobjects/nf-mfobjects-imfmediabuffer-lock
		hr = pBuffer->Lock(&pAudioData, NULL, &cbBuffer);

		if (FAILED(hr)) { break; }

		// Check if max file size reached

		if (a_cbMaxAudioData - cbAudioData < cbBuffer)
		{
			cbBuffer = a_cbMaxAudioData - cbAudioData;
		}


		// Write data to the output file
		hr = WriteToFile(a_hfile, pAudioData, cbBuffer);

		if (FAILED(hr)) { break; }

		// Unlock...

		hr = pBuffer->Unlock();
		pAudioData = NULL;

		if (FAILED(hr)) { break; }

		cbAudioData += cbBuffer;

		if (cbAudioData >= a_cbMaxAudioData)
		{
			break;
		}
		SafeRelease(&pSample);
		SafeRelease(&pBuffer);

	}


	if (SUCCEEDED(hr))
	{
		printf("Wrote %d bytes of audio data.\n", cbAudioData);

		*pcbDataWritten = cbAudioData;
	}

	if (pAudioData)
	{
		pBuffer->Unlock();
	}

	SafeRelease(&pBuffer);
	SafeRelease(&pSample);


	return hr;
}

//-------------------------------------------------------------------
// Finalize the file header.
// Last step now that data is written.
//-------------------------------------------------------------------

HRESULT SoundConverter::FixChunkSize(
	HANDLE a_hFile,		// OutputFile
	DWORD a_cbHeader,	// Size of fmt chunk
	DWORD a_cbAudioData	// Size of data chunk
)
{
	HRESULT hr = S_OK;

	LARGE_INTEGER ll; // Rather strange union type. May just be a LongLong in this case. Used 

	ll.QuadPart = a_cbHeader - sizeof(DWORD);

	if (0 == SetFilePointerEx(a_hFile, ll, NULL, FILE_BEGIN))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	// Write the data size

	if (SUCCEEDED(hr))
	{
		hr = WriteToFile(a_hFile, &a_cbAudioData, sizeof(a_cbAudioData));
	}

	if (SUCCEEDED(hr))
	{
		ll.QuadPart = sizeof(FOURCC);

		if (0 == SetFilePointerEx(a_hFile, ll, NULL, FILE_BEGIN))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
	}

	if (SUCCEEDED(hr))
	{
		DWORD cbRiffFileSize = a_cbHeader + a_cbAudioData - 8; //  8 = RIFF + WAVE.


		hr = WriteToFile(a_hFile, &cbRiffFileSize, sizeof(cbRiffFileSize));
	}


	return hr;
}

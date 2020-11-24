#define WINVER _WIN32_WINNT_WIN7

// Includes Windows
//#include "PlaySound.h" // - Namespace WINDOWSPLAYSOUNDW
// Includes <mmreg>, <iostream> "wavHeader.h"
// #defines btInMb, fileLocation
//#include "Utility.h"
//// May not be correct.
//#include <WaaSApi.h>
//#include <audiopolicy.h>
//#include <mmdeviceapi.h>


//int main()
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	//// Where the file will be streamed to.
//	//FILE* wavStream;
//	//// Opens the file in read mode. Will probaly crash
//	//// if it doesn't exist.	
//	//	
//	//// Opens the location for reading which can now be accessed from first argument. 
//	//// 0 if succesful. It is locatedint the sound importer 
//	//if (fopen_s(&wavStream, fileLocation, "r") != 0)
//	//{
//	//	std::cout << "File location not found.\nCheck file name is case correct.\n" << std::endl;
//	//	std::cout << "Closing applciation." << std::endl;
//	//	system("pause");
//	//	return 0;
//	//}
//	//else
//	//{
//	//	std::cout << "File " << fileLocation << " succesfully open" << std::endl;
//	//}
//	// File succesfully open.
//	// Create a... buffer? of what the data should look like.
//
//	 FILE* wavStream = GetSoundWhole(fileLocation);
//	
//	 
//
//	// The information in the header.
//	wavHeader* meta = new wavHeader;
//	// 1. Destination, 2. How large is the destination in bytes,
//		// 3. How much to read from the file in bytes. A wav header is always 44
//	// The 'safe' way to read from a file.
//	//fread_s(meta, sizeof(wavHeader), sizeof(wavHeader), 1, wavStream);
//	// The better way to read from a file.
//	fread(meta, sizeof(wavHeader), 1, wavStream);
//	
//	// Note that the 3 argument is the amount of times the data is read.
//	// So the above is doing;
//	// Read from 4th argument (FileLocation)
//	// size of 2nd argument (dataSize)
//	// 3rd argument amount of times (amount of datasize to read)
//	// into 1st argument (data buffer)
//	
//	// Needs the length of data.
//	// Seek past the header.
//	//fseek(wavStream, sizeof(wavHeader), SEEK_CUR);
//	// How large is the file minus the header.
//	// This info could be used to workout if 
//	// the data is streamed or not.
//
//	// This is dumb.
//	int howManyShorts = (meta->h_fileSize - sizeof(wavHeader))/2;
//
//	//std::cout << howManyShorts << std::endl;
//
//	// Place to store the data.
//	short* pcm_data;
//	// Kinda dumb? Also this will take the whole file into memory.
//	pcm_data = new short[howManyShorts];
//	// Read the data into buffer.
//	fread(pcm_data, sizeof(short), howManyShorts, wavStream);
//	
//	PrintFileInfo(meta, howManyShorts);
//
//
//
//	// This should be the byte array.
//	// Needs to be converted into samples so then those samples can be modified
//	// then back to the byte array for sending out.
//	// Note that depending on if the file is big endian(right to left) 
//	// or little endian(left to right) you need to change the next step.
//	// eg 24 bit sample
//	//	Byte[i]		Byte[i+1]	 Byte[i+2]	Little Endian
//	//  00001111	00100111	 00000000	= 9999
//	//	Byte[i]		Byte[i+1]	 Byte[i+2]	Big Endian
//	//  00000000	00100111	 00001111	= 9999
//
//	// Since this demo uses a 16 bit wav file (we got that info from the header)
//	// I wont cover checking this.
//
//	//waveOut()
//	
//
//	
//	// Note that it loads the WHOLE FILE into memory.
//
//
//	// Need to modify file here?
//
//
//	
//
//	// Need to pass the file out 
//
//	system("pause");
//	fclose(wavStream);
//
//	delete[] pcm_data;
//	delete meta;
//	
//	return 0;
//}

// This is a tutorial Decoding a audio file and then outputting 
// a wav file. I'll probably extend this to modify the file that
// is being streamed in.

// I'm using the source reader https://docs.microsoft.com/en-us/windows/win32/medfound/source-reader
// Tutorial https://docs.microsoft.com/en-us/windows/win32/medfound/tutorial--decoding-audio


//#include <mfapi.h>
//#include <mfidl.h>
//#include <mfreadwrite.h>
//#include <stdio.h>
//#include <Mferror.h>

// Link these libs in properties
// mfplat.lib, mfreadwrite.lib, mfuuid.lib



//// Safe release is not included in headers. - https://docs.microsoft.com/en-us/windows/win32/medfound/saferelease
//// Releases COM interface pointers. Alternate is smart pointers.
//template <class T> void SafeRelease(T** ppT)
//{
//	if (*ppT)
//	{
//		(*ppT)->Release();
//		*ppT = NULL;
//	}
//}
//
//
//
//
////-------------------------------------------------------------------
//// Writes a block of data to a file
//// hFile: Handle to the file.
//// p: Pointer to the buffer to write.
//// cb: Size of the buffer, in bytes.
//// Not really neccesary. Called multiple times so more for reuse
//// and to cut down on bloat.
////-------------------------------------------------------------------
//
//HRESULT WriteToFile(HANDLE a_hFile, void* a_p, DWORD a_cb)
//{
//	DWORD cbWritten = 0;
//	HRESULT hr = S_OK;
//	// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
//	BOOL bResult = WriteFile(a_hFile, a_p, a_cb, &cbWritten, NULL);
//
//	if (!bResult)
//	{
//		hr = HRESULT_FROM_WIN32(GetLastError());
//	}
//	return hr;
//}
//
//
////-------------------------------------------------------------------
//// Finalize the file header.
//// Last step now that data is written.
////-------------------------------------------------------------------
//
//HRESULT FixChunkSize(
//	HANDLE a_hFile,		// OutputFile
//	DWORD a_cbHeader,	// Size of fmt chunk
//	DWORD a_cbAudioData	// Size of data chunk
//)
//{
//	HRESULT hr = S_OK;
//
//	LARGE_INTEGER ll; // Rather strange union type. May just be a LongLong in this case. Used 
//	
//	ll.QuadPart = a_cbHeader - sizeof(DWORD);
//
//	if (0 == SetFilePointerEx(a_hFile, ll, NULL, FILE_BEGIN))
//	{
//		hr = HRESULT_FROM_WIN32(GetLastError());
//	}
//
//	// Write the data size
//
//	if (SUCCEEDED(hr))
//	{
//		hr = WriteToFile(a_hFile, &a_cbAudioData, sizeof(a_cbAudioData));
//	}
//
//	if (SUCCEEDED(hr))
//	{
//		ll.QuadPart = sizeof(FOURCC);
//
//		if (0 == SetFilePointerEx(a_hFile, ll, NULL, FILE_BEGIN))
//		{
//			hr = HRESULT_FROM_WIN32(GetLastError());
//		}
//	}
//
//	if (SUCCEEDED(hr))
//	{
//		DWORD cbRiffFileSize = a_cbHeader + a_cbAudioData - 8; //  8 = RIFF + WAVE.
//
//
//		hr = WriteToFile(a_hFile, &cbRiffFileSize, sizeof(cbRiffFileSize));
//	}
//
//
//	return hr;
//}
//
//
////-------------------------------------------------------------------
//// WriteWaveData
////
//// Decodes PCM audio data from the source file and writes it to
//// the WAVE file.
//// If I were to add reverb it would be somewhere in here since data is
//// decoded. Would I need to get ALL the data out then apply the verb?
//// I would need to adjust the maxAudioData. OR I could check the 
//// amplitude of the pcm and then apply the verb.
////-------------------------------------------------------------------
//
//HRESULT WriteWaveData(
//	HANDLE a_hfile,					// Output file
//	IMFSourceReader* a_pReader,		// Source reader
//	DWORD a_cbMaxAudioData,			// Maximum amount of adio data
//	DWORD* pcbDataWritten			// Receives the data written.
//)
//{
//	HRESULT hr = S_OK;
//	DWORD cbAudioData = 0;			// The data read out in bits?
//	DWORD cbBuffer = 0;				// Buffer to write to?
//	BYTE* pAudioData = NULL;		// The pointer to the data read out?
//
//	IMFSample* pSample = NULL;	
//	IMFMediaBuffer* pBuffer = NULL;
//
//
//	// Loops through till no more audio to write from
//	// or error
//	while (true)
//	{
//		DWORD dwFlags = 0;
//
//		// Read the next sample
//		hr = a_pReader->ReadSample(
//			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
//			0, NULL, &dwFlags, NULL, &pSample);
//
//		if (FAILED(hr)) { break; }
//
//		if (dwFlags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
//		{
//			printf("Type change - not supported by wave file format.\n");
//			break;
//		}
//		if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM)
//		{
//			printf("End of input file.\n");
//			break;
//		}
//		if (pSample == NULL)
//		{
//			printf("No sample.\n");
//			continue;
//		}
//
//
//		// Get a pointer to the audio data in the sample.
//
//		hr = pSample->ConvertToContiguousBuffer(&pBuffer);
//
//		if (FAILED(hr)) { break; }
//
//		// Lock in this case does not prevent other threads from calling Lock again. So does not synch threads.
//		// Gives the caller(1) access to this (2) amount of data after(3)
//		// https://docs.microsoft.com/en-us/windows/win32/api/mfobjects/nf-mfobjects-imfmediabuffer-lock
//		hr = pBuffer->Lock(&pAudioData, NULL, &cbBuffer);
//
//		if (FAILED(hr)) { break; }
//
//		// Check if max file size reached
//
//		if (a_cbMaxAudioData - cbAudioData < cbBuffer)
//		{
//			cbBuffer = a_cbMaxAudioData - cbAudioData;
//		}
//
//
//		// Write data to the output file
//		hr = WriteToFile(a_hfile, pAudioData, cbBuffer);
//
//		if (FAILED(hr)) { break; }
//
//		// Unlock...
//
//		hr = pBuffer->Unlock();
//		pAudioData = NULL;
//
//		if (FAILED(hr)) { break; }
//
//		cbAudioData += cbBuffer;
//
//		if (cbAudioData >= a_cbMaxAudioData)
//		{
//			break;
//		}
//		SafeRelease(&pSample);
//		SafeRelease(&pBuffer);
//
//	}
//	
//
//	if (SUCCEEDED(hr))
//	{
//		printf("Wrote %d bytes of audio data.\n", cbAudioData);
//		
//		*pcbDataWritten = cbAudioData;
//	}
//
//	if (pAudioData)
//	{
//		pBuffer->Unlock();
//	}
//
//	SafeRelease(&pBuffer);
//	SafeRelease(&pSample);
//
//
//	return hr;
//}
//
//
////-------------------------------------------------------------------
//// CalculateMaxAudioDataSize
////
//// Calculates how much audio to write to the WAVE file, given the
//// audio format and the maximum duration of the WAVE file.
//// This function also works out how much data it can write to the 
//// output file.
//// Max size of a WAVE is 4GB.
////-------------------------------------------------------------------
//
//DWORD CalculateMaxAudioDataSize(
//IMFMediaType *a_pAudioType,		// Pcm audio format worked out by ConfigureAudioStream
//DWORD a_cbHeader,				// Size of the wav header worked out by WriteWaveHeader
//DWORD a_msecAudioData			// Maximum duration to read in. From Main.
//)
//{
//	UINT cbBlockSize = 0;		// Audio frame size in bytes. Not sure if frame and sample are interchangeable.
//	UINT cbBytesPerSecond = 0;	// Bytes per second.
//
//	// Get the audio block size and number of bytes/second from the audio format.
//	// Note that there are other get attribute functions that return different types.
//	
//	cbBlockSize = MFGetAttributeUINT32(a_pAudioType, MF_MT_AUDIO_BLOCK_ALIGNMENT, 0);
//	cbBytesPerSecond = MFGetAttributeUINT32(a_pAudioType, MF_MT_AUDIO_AVG_BYTES_PER_SECOND, 0);
//
//
//	// Desired size of file.
//	DWORD cbAudioClipSize = (DWORD)MulDiv(cbBytesPerSecond, a_msecAudioData, 1000);
//
//	// Largest file size after header file.
//	DWORD cbMaxSize = MAXDWORD - a_cbHeader;
//
//
//	// Whichever is smaller is the amount that is written to the file.
//	// I feel like this should also throw an error or message
//	// if you hit the max size limit.
//	cbAudioClipSize = min(cbAudioClipSize, cbMaxSize);
//
//	// Rounds the clip size to avoid partial audio frame/sample
//	cbAudioClipSize = (cbAudioClipSize / cbBlockSize) * cbBlockSize;
//
//	return cbAudioClipSize;
//}
//
//
////-------------------------------------------------------------------
//// WriteWaveHeader - Depending on what file type you wanted to write out
//// you would adjust this file to take in the file type.
//// Write the WAVE file header.
//// The details of the file will need to be adjusted later
////-------------------------------------------------------------------
//
//HRESULT WriteWaveHeader(
//	HANDLE a_hFile,				// Output file
//	IMFMediaType* a_pMediaType,	// PCM audio format. Provided by ConfigureAudioStream()
//	DWORD* a_pcbWritten			// Size of the header
//)
//{
//	HRESULT hr = S_OK;
//	UINT32 cbFormat = 0;
//
//	// The wavformat. Need to find the equivalant in other media types.
//	// I think: MPEG1WAVEFORMAT may be the equiv in mp3? - https://en.wikipedia.org/wiki/MP3
//	
//	// Check Wav header for WaveFormat.h or mmeapi.h
//	WAVEFORMATEX* pWav = NULL;
//	
//	*a_pcbWritten = 0;
//
//	// Media Foundation framework background- https://en.wikipedia.org/wiki/Media_Foundation
//	// Convert pcm into the correct structure. 
//	hr = MFCreateWaveFormatExFromMFMediaType(a_pMediaType, &pWav, &cbFormat);
//
//	// The start of the header.
//	if (SUCCEEDED(hr))
//	{
//		DWORD header[] = {
//			FCC('RIFF'),
//			0, //File size not known yet
//			FCC('WAVE'),
//			FCC('fmt'), // FMT chunk start
//			cbFormat	// Should be modified by the above function MFCreateWaveFormatExFromMFMediaType.
//		};
//
//		DWORD dataHeader[] = { FCC('data'), 0 }; // Start of the data chunk. Goes after fmt. Unfilled becuase no data yet
//
//		hr = WriteToFile(a_hFile, header, sizeof(header)); // 
//
//		if (SUCCEEDED(hr))
//		{
//			hr = WriteToFile(a_hFile, dataHeader, sizeof(dataHeader));
//		}
//
//		if (SUCCEEDED(hr))
//		{
//			*a_pcbWritten = sizeof(header) + cbFormat + sizeof(dataHeader); // Total size of header. 
//		}
//
//	}
//
//
//	// Different from the other releases?
//	CoTaskMemFree(pWav);
//
//	return hr;
//}
//
//
////-------------------------------------------------------------------
//// ConfigureAudioStream
//// Selects an audio stream from the source file, and configures the
//// stream to deliver decoded PCM audio. 
////-------------------------------------------------------------------
//
//HRESULT ConfigureAudioStream(
//	IMFSourceReader* a_pReader,		// Pointer to the Source reader.
//	IMFMediaType** ppPCMAuido		// Receives the audio format.
//)
//{
//	// For the difference between complete and partial media types - https://docs.microsoft.com/en-us/windows/win32/medfound/complete-and-partial-media-types
//	IMFMediaType* pUnconmressedAudioType = NULL; // This will be PCM.
//	IMFMediaType* pPartialType = NULL; // A partial type will lack some attributes.
//
//	// Deselect all the other streams.
//	HRESULT hr = a_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, FALSE);
//
//	if (SUCCEEDED(hr))
//	{
//		// Select First audio stream.
//		hr = a_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
//	}
//
//	// Create the partial media type and it's attributes.
//	hr = MFCreateMediaType(&pPartialType);
//	if (SUCCEEDED(hr))
//	{
//		hr = pPartialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio); // Major Type: Audio.
//	}
//
//	if (SUCCEEDED(hr))
//	{
//		hr = pPartialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM); // Sub Type: Pcm
//	}
//	
//	// Now we set this type to the source reader.
//	// I think windows chooses a media type that works best from the information provided.
//	if (SUCCEEDED(hr))
//	{
//		hr = a_pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, pPartialType);
//	}
//	// This should get the media type(which is now a complete type) out of the source reader
//	if (SUCCEEDED(hr))
//	{
//		hr = a_pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pUnconmressedAudioType);
//	}
//
//	// I Feel like this is a safety.
//	// Ensures the 1st audio stream is selected.
//	if (SUCCEEDED(hr))
//	{
//		hr = a_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
//	}
//
//	// Return the format to the caller.
//	if (SUCCEEDED(hr))
//	{
//		*ppPCMAuido = pUnconmressedAudioType;
//		(*ppPCMAuido)->AddRef();
//	}
//
//	SafeRelease(&pUnconmressedAudioType);
//	SafeRelease(&pPartialType);
//
//	return hr;
//}
//
//
////-------------------------------------------------------------------
//// WriteWaveFile - Function
//// Writes a WAVE file by getting audio data from the source reader.
////-------------------------------------------------------------------
//
//HRESULT WriteWaveFile(
//	IMFSourceReader* a_pReader,		// Pointer to Source reader
//	HANDLE a_hfile,					// Handle to the oupt file
//	LONG a_msecAudioData			// Maximum amout of audio to write in msec.
//)
//{
//	// For error checking?
//	HRESULT hr = S_OK;
//
//	// Dword being a "Double word" windows type 32 bit data type. Can contain integers.
//	DWORD cbHeader = 0;
//	DWORD cbAudioData = 0;
//	DWORD cbMaxAudioData = 0;
//
//
//	IMFMediaType* pAudioType = NULL; // What type of data to convert the file being read from into.
//
//	//hr = ConfigureAudioStream(a_pReader, &pAudioType); // What does the in file get converted into.
//	
//	// Write the wav header - Look at wavHeader.h for what the layout is.
//	if (SUCCEEDED(hr))
//	{
//		hr = WriteWaveHeader(a_hfile, pAudioType, &cbHeader);
//	}
//
//	if (SUCCEEDED(hr))
//	{
//		// cb is what?
//		cbMaxAudioData = CalculateMaxAudioDataSize(pAudioType, cbHeader, a_msecAudioData); // How much of the audio to decode in bytes 
//
//		// Decode audio data to the file.
//		hr = WriteWaveData(a_hfile, a_pReader, cbMaxAudioData, &cbAudioData);
//	}
//
//	if (SUCCEEDED(hr))
//	{
//		hr = FixChunkSize(a_hfile, cbHeader, cbAudioData);  // Adjusts the header based on what has occured during the conversion
//	}
//
//	SafeRelease(&pAudioType); // Release pointers.
//	return hr;
//}
#include "SoundConverter.h"

//-------------------------------------------------------------------
// Example Main
// Needs to be rewritten with Main()
//-------------------------------------------------------------------
//static SoundConverter* soundConverter = new SoundConverter();

//int main()
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	//SoundConverter* soundConverter = new SoundConverter();
//
//
//	std::string inputFile;
//	std::string outputFile;
//
//	std::cin >> inputFile;
//	std::cin >> outputFile;
//
//
//	//const WCHAR pSourceFile = "Source";
//	//const WCHAR pTargetFile = "Output";
//	const LONG MAX_AUDIO_DURATION_MSEC = 2000; // If the clip is under this time it will convert the whole thing.
//												// If not if will shorten it.
//	
//	SoundConverter* sound = new SoundConverter();
//
//	if (sound->Convert(inputFile, outputFile, MAX_AUDIO_DURATION_MSEC)) // Need to add conversion from strings into wchars in here
//	{
//		std::cout << "Successful" << std::endl;
//	}
//
//
//
//	delete sound;
//	return 1;

	//// From here.

	//// Error code? could be changed to a bool? Depends what the funcitons return.
	//HRESULT hr = S_OK; // Success code. Handle.

	//IMFSourceReader* pReader = NULL; // The thing that will give me the data.
	//HANDLE hFile = INVALID_HANDLE_VALUE; // Handle for file to write to. For some context about what handles are - https://stackoverflow.com/questions/1303123/what-is-a-handle-in-c

	//// Initialize the COM library?
	//hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	//// Initalize media foundation platform - https://docs.microsoft.com/en-us/windows/win32/medfound/media-foundation-platform-apis
	//if (SUCCEEDED(hr))
	//{
	//	hr = MFStartup(MF_VERSION);
	//}

	//// Create the file reader - Note that you create different kinds of source readers.
	//if (SUCCEEDED(hr))
	//{
	//	hr = MFCreateSourceReaderFromURL(pSourceFile, NULL, &pReader);
	//	if (FAILED(hr))
	//	{
	//		printf("error opening input file: %S\n", pSourceFile); // ,hr third argument?
	//	}
	//}

	//// Open output file. - Example of what create file does https://docs.microsoft.com/en-us/windows/win32/fileio/opening-a-file-for-reading-or-writing
	//if (SUCCEEDED(hr))
	//{
	//	//  Some info about createFile and it's other versions - https://stackoverflow.com/questions/51462048/what-is-the-difference-between-createfile-and-createfilea
	//	hFile = CreateFile(pTargetFile, GENERIC_WRITE, FILE_SHARE_READ, NULL,
	//		CREATE_ALWAYS, 0, NULL);

	//	if (hFile == INVALID_HANDLE_VALUE)
	//	{
	//		hr = HRESULT_FROM_WIN32(GetLastError());
	//		printf("Cannont create output file: %S\n", pTargetFile); // ,hr third argument?
	//	}
	//}

	//if (SUCCEEDED(hr))
	//{
	//	// The function that writes the file. Needs to be added above.
	//	hr = sound->WriteWaveFile(pReader, hFile, MAX_AUDIO_DURATION_MSEC);
	//}

	//if (FAILED(hr))
	//{	// This prints a hex code... not very useful for me
	//	printf("Failed, hr = 0x%X\n", hr);
	//}

	//// Close file?
	//if (hFile != INVALID_HANDLE_VALUE)
	//{
	//	CloseHandle(hFile);
	//}

	////SafeRelease(&pReader);
	//MFShutdown();
	//CoUninitialize();


	//// Heh for some reason I like this...
	//// Really werid to read though.
	//return SUCCEEDED(hr) ? 0 : 1;
	//}




//int wmain(int argc, wchar_t* argv[])
//{															// In	// Out	// Max audio
//	//SoundConverter* soundConverter = new SoundConverter();
//
//	// I expect this terminates program upon corruption?
//	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
//
//	// Need arguments passed in. Otherwise no run.
//	// I can probably do the same while in the program
//	// by saving const char* of input and output places with
//	// error messages and application closes if it can't read to
//	// from destionation.
//	if (argc != 3)
//	{
//		printf("arguments: input_file output_file.wav\n");
//		return 1;
//	}
//	// WCHAR 16 unicode character.
//	const WCHAR* pSourceFile = argv[1];
//	const WCHAR* pTargetFile = argv[2];
//
//	const LONG MAX_AUDIO_DURATION_MSEC = 5000; // The modified sampling value?
//
//
//
//	// From here.
//	
//	// Error code? could be changed to a bool? Depends what the funcitons return.
//	HRESULT hr = S_OK; // Success code. Handle.
//
//	IMFSourceReader* pReader = NULL; // The thing that will give me the data.
//	HANDLE hFile = INVALID_HANDLE_VALUE; // Handle for file to write to. For some context about what handles are - https://stackoverflow.com/questions/1303123/what-is-a-handle-in-c
//	
//	// Initialize the COM library?
//	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
//
//	// Initalize media foundation platform - https://docs.microsoft.com/en-us/windows/win32/medfound/media-foundation-platform-apis
//	if (SUCCEEDED(hr))
//	{
//		hr = MFStartup(MF_VERSION);
//	}
//
//	// Create the file reader - Note that you create different kinds of source readers.
//	if (SUCCEEDED(hr))
//	{									
//		hr = MFCreateSourceReaderFromURL(pSourceFile, NULL, &pReader);
//		if (FAILED(hr))
//		{
//			printf("error opening input file: %S\n", pSourceFile); // ,hr third argument?
//		}
//	}
//
//	// Open output file. - Example of what create file does https://docs.microsoft.com/en-us/windows/win32/fileio/opening-a-file-for-reading-or-writing
//	if (SUCCEEDED(hr))
//	{
//		//  Some info about createFile and it's other versions - https://stackoverflow.com/questions/51462048/what-is-the-difference-between-createfile-and-createfilea
//		hFile = CreateFile(pTargetFile, GENERIC_WRITE, FILE_SHARE_READ, NULL,
//			CREATE_ALWAYS, 0, NULL);
//
//		if (hFile == INVALID_HANDLE_VALUE)
//		{
//			hr = HRESULT_FROM_WIN32(GetLastError());
//			printf("Cannont create output file: %S\n", pTargetFile); // ,hr third argument?
//		}
//	}
//
//	if (SUCCEEDED(hr))
//	{
//		// The function that writes the file. Needs to be added above.
//		hr = WriteWaveFile(pReader, hFile, MAX_AUDIO_DURATION_MSEC);
//	}
//
//	if (FAILED(hr))
//	{	// This prints a hex code... not very useful for me
//		printf("Failed, hr = 0x%X\n", hr);
//	}
//
//	// Close file?
//	if (hFile != INVALID_HANDLE_VALUE)
//	{
//		CloseHandle(hFile);
//	}
//
//	SafeRelease(&pReader);
//	MFShutdown();
//	CoUninitialize();
//
//	
//	// Heh for some reason I like this...
//	// Really werid to read though.
//	return SUCCEEDED(hr) ? 0 : 1;
//
//}

// Things that need to be done. 
// 1. Need to move a bunch of this into an initialize function which takes a input and output file.
// Something like MediaToWavConverter(const char* inputFile, const char* outputFile);
// 2. Different variables to effect the verb unit.
// : Enter values between x and y for z




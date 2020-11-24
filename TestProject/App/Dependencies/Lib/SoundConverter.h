#pragma once

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <stdio.h>
#include <Mferror.h>
#include <string>

// This Decodes media file and then outputs a wav file.
// It will have functionality to add reverb to audio.

// I'm using the source reader https://docs.microsoft.com/en-us/windows/win32/medfound/source-reader
// Tutorial https://docs.microsoft.com/en-us/windows/win32/medfound/tutorial--decoding-audio



class SoundConverter
{

public:

	//SoundConverter(const WCHAR* Input, const WCHAR* output, const long MAX_ADUIO_IN_MS = 5000);
	//SoundConverter();

	// Call this bad boy in main to actually do the conversion.
	// Pass in a long as a time 
	bool Convert(
		std::string Input,
		std::string output,
		const long MAX_ADUIO_IN_MS = 5000
	);

private:
	HRESULT WriteWaveFile(
		IMFSourceReader* a_pReader,		// Pointer to Source reader
		HANDLE a_hfile,					// Handle to the oupt file
		LONG a_msecAudioData			// Maximum amout of audio to write in msec.
		);

	HRESULT ConfigureAudioStream(
		IMFSourceReader* a_pReader,		// Pointer to the Source reader.
		IMFMediaType** ppPCMAuido		// Receives the audio format.
	);

	HRESULT WriteWaveHeader(
		HANDLE a_hFile,					// Output file
		IMFMediaType* a_pMediaType,		// PCM audio format. Provided by ConfigureAudioStream()
		DWORD* a_pcbWritten				// Size of the header
	);

	HRESULT WriteToFile(
		HANDLE a_hFile,					// Output file
		void* a_p,						// Data to write
		DWORD a_cb						// Size of the buffer in bytes
	);

	DWORD CalculateMaxAudioDataSize(
		IMFMediaType* a_pAudioType,		// Pcm audio format worked out by ConfigureAudioStream
		DWORD a_cbHeader,				// Size of the wav header worked out by WriteWaveHeader
		DWORD a_msecAudioData			// Maximum duration to read in. From Main.
	);

	HRESULT WriteWaveData(
		HANDLE a_hfile,					// Output file
		IMFSourceReader* a_pReader,		// Source reader
		DWORD a_cbMaxAudioData,			// Maximum amount of adio data
		DWORD* pcbDataWritten			// Receives the data written.
	);

	HRESULT FixChunkSize(
		HANDLE a_hFile,					// OutputFile
		DWORD a_cbHeader,				// Size of fmt chunk
		DWORD a_cbAudioData				// Size of data chunk
	);

	template <class T> void SafeRelease(T** ppT);

};
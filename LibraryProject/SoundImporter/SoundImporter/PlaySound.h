#pragma once
#include <Windows.h>

// Refer to https://docs.microsoft.com/en-us/previous-versions/dd743680(v=vs.85)
// for documentation.


// Note that I needed to add an aditional library in the linker
// to get this to work.


// Plays a Sound using PlaySoundW.
// Worst version.
namespace WINDOWSPLAYSOUNDW
{
	// This is as simple as it gets...
	// Is not streamed so not ideal.
	// The program waits for sound completion.
	void PlaySoundWINDOWS()
	{
		PlaySoundW(TEXT("1.wav"), NULL, SND_FILENAME);
	}

	void StopSoundWINDOWS()
	{
		// Stops sound.
		PlaySoundW(NULL, 0, 0);
	}
}
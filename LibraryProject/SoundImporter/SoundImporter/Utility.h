#pragma once

// For general purpose functions.

// Has all registered multimedia files?
// Worth a look.
#include <mmreg.h>

// The data within the wav header and the data itself.
// Not at all important and I'm sure microsoft
// has a file with all the types.
#include "wavHeader.h"

#include <iostream>
#include <fstream>



// To check the file is correct. Debug mostly.
#define btInMb 1048576
// For debug purposes file name is up here.
//#define fileLocation ".\\Dependencies\\1.wav"
#define fileLocation "1.wav"



void PrintFileInfo(wavHeader* a_meta, int a_data)
{
	std::cout << "Num Of channels " << a_meta->h_num_channels << "." << std::endl;
	std::cout << "Sample rate " << a_meta->h_sample_rate << "hz." << std::endl;
	std::cout << "Average bytes per second " << a_meta->h_Avg_Bytes_Per_Sec << std::endl;
	std::cout << "Sound file including header is: " << a_meta->h_fileSize << "b or " << a_meta->h_fileSize / btInMb << "mbs.\nIf we divide that by the above average\nwe can"
		<< " estimate the file is " << ((a_data / a_meta->h_Avg_Bytes_Per_Sec) * a_meta->h_num_channels) / 60 << " minutes and "
		<< ((a_data / a_meta->h_Avg_Bytes_Per_Sec) * a_meta->h_num_channels) % 60 << " seconds." << std::endl;
	std::cout << "Every " << a_meta->h_bits_per_sample << " bits will make up 1 sample.\nTherefore the file contains "
		<< (a_meta->h_fileSize - sizeof(wavHeader)) / a_meta->h_bits_per_sample << " indivdual samples." << std::endl;
}

// Seperated the opening of the file to here.
// This should make and return a 'sound' data type.
FILE* GetSoundWhole(const char* a_filelocaiton)
{
	// Wait a sec... this returns something out of scope?
	FILE* wavStream;
	// Opens the file in read mode. Will probaly crash
	// if it doesn't exist.	

	// Opens the location for reading which can now be accessed from first argument. 
	// 0 if succesful. It is locatedint the sound importer 
	if (fopen_s(&wavStream, fileLocation, "r") != 0)
	{
		std::cout << "File location not found.\nCheck file name is case correct.\n" << std::endl;
		std::cout << "Closing applciation." << std::endl;
		system("pause");
		return 0;
	}
	else
	{
		std::cout << "File " << fileLocation << " succesfully open" << std::endl;
	}
	return wavStream;
}


// Mostly a debug function to see how large the file is.
// Could be used after the file is modified to see how much
// larger if any the file has become after modiefied?
// Shouldn't effect size unless more data.
// Note that this infomation is stored in the 1st chunk.
// Already exists in header file so not used.
void GetFileSize(FILE* a_wavStream)
{
	fseek(a_wavStream, 0, SEEK_END);
	// This gets the total file size of the wav.
	int filesize = ftell(a_wavStream);

	fseek(a_wavStream, 0, SEEK_SET);

	std::cout << filesize / btInMb << std::endl;
}
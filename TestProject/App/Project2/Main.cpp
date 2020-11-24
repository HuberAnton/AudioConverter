#include "SoundConverter.h"
#include <iostream>

int main()
{
	
	SoundConverter* soundConverter = new SoundConverter();


	// Pass in the location of the file including the file type 
	// eg C:\Users\..\\InFile.type
	std::string in;
	// Pass in the location of the output file.
	// The file will be converted to a wav but without adding wav to the
	// end of the name you will have to manually open with vlc or your
	// media player of choce
	// eg C:\Users\..\\OutFile.wav
	std::string out;

	// Maximum length of clip in milliseconds
	// If the file exceeds this it will shorten the file.
	// If the file exceeds 4gb it will also stop.

	long time;
	std::cout << "Enter the input file URL:" << std::endl;
	std::cin >> in;
	std::cout << "Enter the output file URL:" << std::endl;
	std::cin >> out;
	std::cout << "Enter the max time of converted audio clip in milliseconds:" << std::endl;
	std::cin >> time;

	// Returns if failed or succeeded with an error code
	 
	soundConverter->Convert(in, out, time);
	system("Pause");
	



	return 0;
}
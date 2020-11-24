#pragma once



// This is the 'standard' way a wav file is stored...
// You can open up a wav file and check through to 
// get infomration about the data. Typically 44 bytes.

struct wavHeader
{
    // RIFF chunk
    char h_chunk_id[4];           // 4 bytes. "RIFF" 
    int h_fileSize;               // 4 bytes. File size // Not including the first 8 bytes.
    char h_format[4];             // 4 bytes. "WAVE"
    // subchunk Format or 'fmt'
    char h_subchunk1_id[4];       // 4 bytes. "fmt "  Start of format chunk. Has a nullterminator at the end which is ignored.
    int h_subchunk1_size;         // 4 bytes. size of the fmt subchunk.
    // Microsoft has a type called WAVEFORMATEX that stores all of this information below until 'data'
    short int h_audio_format;     // 2 bytes. Type of format. Should be 1 for pcm since wav is not compressed?
    short int h_num_channels;     // 2 bytes. Number of channels. eg mono = 1, stereo = 2.
    int h_sample_rate;            // 8 bytes. Hrtz rate. eg 40khz, 44.1khz.
    int h_Avg_Bytes_Per_Sec;      // 8 bytes. Can be worked out with math. (sample rate * BitsPersample * channel) / 1 byte(8)).
    short int h_block_align;      // 2 bytes. 
    short int h_bits_per_sample;  // 2 bytes. bit depth.
    // subchunk 2 Format or 'data'
    char h_subchunk2_id[4];       // 4 bytes. "data" Start of sub chunk.
    int h_subchunk2_size;         // 4 bytes. Size of the data subchunk.
    // short* dataBuffer =        // The data of the file. Seperate from the header but worth noting.
            //new dataBuffer[sizeofDataInBytes / sizeof(short)];       
};


// Loading files directly into memory should be done for
// smaller files.
// Probably need a 'playonce' and 'play' funciton.
// Streaming needs to be done for editing files on the fly.
// Probably need a listener as well.
/*
The idea of a listener is mostly to work out vectors.
From the listener and the source you can work out all the distances
and directions using vector math to then work out 3D audio.
You'd do this by doing the math per frame(?) so the data would
need to be streamed as it would need to be adjusted on the fly.
Play once for non diagetic sounds could be loaded into memory?
Or is that dumb since loading large song files would be a bad idea.
*/
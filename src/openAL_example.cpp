#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

#include <stdint.h>

void
list_audio_devices(const ALCchar *devices)
{
  const ALCchar *device = devices;
  const ALCchar *next = devices + 1;
  
  size_t len = 0;
  //std::cout << devices << std::endl;
  //fprintf(stdout, "Devices list:\n");
  //fprintf(stdout, "----------\n");

  std::cout << "Devices list:\n";
  std::cout << "----------\n";
  
  while( (device) && (*device != '\0') && (next) && (*next != '\0'))
    {
      //fprintf(stdout, "%s\n", device);
      std::cout << device << std::endl;
      len = strlen(device);
      device += (len + 1);
      next += (len + 2);
    }
  
  //fprintf(stdout, "----------\n");
  std::cout << "----------\n";
}

int main(int argc, char ** argv)
{
  
  ALboolean enumeration;

  enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
  if (enumeration == AL_FALSE)
    {
      // enumeration not supported
      std::cout<<"Enumeration not supported."<<std::endl;
    }
  else
    {
      std::cout<<"Enumeration supported!"<<std::endl;
      list_audio_devices( alcGetString(NULL, ALC_DEVICE_SPECIFIER) );
    } 
  
  ALCdevice *device;

  device = alcOpenDevice(NULL);
  if (!device)
    {
      // handle errors 
      std::cout << "Unable to open AL device: " << device << std::endl;
    }

  ALCcontext *context;
  ALCenum error;
  
  context = alcCreateContext(device, NULL);
  if (!alcMakeContextCurrent(context))
    {
      // failed to make context current
      // test for errors here using alGetError();
      error = alGetError();
      if (error != AL_NO_ERROR)
	{
	  std::cout << "Failed to make context current: " << error << std::endl;
	}
    }


  // Source Generation
  ALuint source;

  alGenSources( (ALuint)1, &source);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "alGenSources: " << error << std::endl;
    }
  
  alSourcef(source, AL_PITCH, 1);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "alsourcef pitch: " << error << std::endl;
    }
  
  alSourcef(source, AL_GAIN, 1);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "alsourcef gain: " << error << std::endl;
    }
  
  alSource3f(source, AL_POSITION, 0, 0, 0);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "alsource3f al position: " << error << std::endl;
    }
  
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "alsource3f al velocity: " << error << std::endl;
    }
  
  alSourcei(source, AL_LOOPING, AL_FALSE);
  // check for errros
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "alsourcei al_looping: " << error << std::endl;
    }

  ///////////////////////
  // Buffer generation
  ///////////////////////
  ALuint buffer;
  
  alGenBuffers((ALuint)1, &buffer);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "alGenBuffers: " << error << std::endl;
    }
  
  uint16_t bits_per_sample = 16;
  ALenum format = AL_FORMAT_MONO16;
  char * data;
  ALsizei size = 32000;
  ALsizei freq = 16000;
  ALboolean loop = AL_FALSE;
 
  //char buf[size];
  
  //Now we can finally proceed with loading the raw audio stream into
  //our buffer, this is done like this for alut:
  //alBufferData(buffer, format, data, size, freq);
  // check for errors
  //error = alGetError();
  //if (error != AL_NO_ERROR)
    // {
    //std::cout << "load raw audio stream into buffer: " << error << std::endl;
    // }

  // Load test.wav
  //loadWAVFile("test.wav", &format, data, &size, &freq, &loop);

  /*
  if ((error = alGetError()) != AL_NO_ERROR)
    {
      DisplayALError("alutLoadWAVFile test.wav : ", error);
      alDeleteBuffers(NUM_BUFFERS, g_Buffers);
      return;
    }

  // Copy test.wav data into AL Buffer 0
  alBufferData(g_Buffers[0],format,data,size,freq);
  if ((error = alGetError()) != AL_NO_ERROR)
    {
      DisplayALError("alBufferData buffer 0 : ", error);
      alDeleteBuffers(NUM_BUFFERS, g_Buffers);
      return;
    }

  // Unload test.wav
  unloadWAV(format,data,size,freq);
  if ((error = alGetError()) != AL_NO_ERROR)
    {
      DisplayALError("alutUnloadWAV : ", error);
      alDeleteBuffers(NUM_BUFFERS, g_Buffers);
      return;
    }
  */
  
  ///////////////////////
  // Bind Buffer to Source
  ///////////////////////
  alSourcei(source, AL_BUFFER, buffer);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "bind buffer to source: " << error << std::endl;
    }
  
  alSourcePlay(source);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "play source: " << error << std::endl;
    }

  ALint source_state; 
  
  alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "get source state error: " << error << std::endl;
    }
  
  while (source_state == AL_PLAYING)
    {
      alGetSourcei(source, AL_SOURCE_STATE, &source_state);
      // check for errors
      error = alGetError();
      if (error != AL_NO_ERROR)
	{
	  std::cout << "get source state error: " << error << std::endl;
	}
      
    }
  
  // cleanup context
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  device = alcGetContextsDevice(context);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
  
}

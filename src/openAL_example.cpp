#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

#include <stdint.h>

#include <math.h>
#include <cmath>

//#include <iostream>
#include <fstream>

#define PI 3.14159265

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
  ALenum format = AL_FORMAT_MONO8;
  char * data;
  ALsizei size = 32000; // number of bytes
  ALsizei fs = 16000;
  ALboolean loop = AL_FALSE;
 
  char buf[size];

  double _freq = 1e3;
  double _amp = std::pow(2,bits_per_sample-1) - 1;
  double _offset = _amp;
  uint16_t _y = 0;

  
  std::ofstream outfile;
  outfile.open ("out.txt");

  double f = 1000.0;

  int j = 0;

  for( uint32_t i = 0; i < size/sizeof(uint16_t); i++ )
    {
      _y = (uint16_t)( ( _amp * ( sin( 2.0 * PI * f *( (double)i / (double)fs ) ) ) ) + _offset );
      
      outfile << _y << "\n";

      //buf[j] = 100;
      //buf[j+1] = 100;
      buf[j] =   (uint8_t) ( _y & 0x00FF );
      buf[j+1] = (uint8_t) ( _y >> 8 );

      j+=2;
      
    }

  outfile.close();
  data = &buf[0];
  
  //Now we can finally proceed with loading the raw audio stream into
  //our buffer, this is done like this for alut:
  alBufferData(buffer, format, data, size, fs);
  // check for errors
  error = alGetError();
  if (error != AL_NO_ERROR)
    {
      std::cout << "load raw audio stream into buffer: " << error << std::endl;
    }
  
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
  std::cout<<"Playing!"<<std::endl;
  
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

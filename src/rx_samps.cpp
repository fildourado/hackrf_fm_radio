#include <hackrf.h>

#include <iostream>

//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <getopt.h>
//#include <time.h>

//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <errno.h>


int main(int argc, char ** argv)
{

  hackrf_device* device = NULL;
  int result;
  
  std::cout<<"Starting to receive"<<std::endl;

  result = hackrf_init();
  if( result != HACKRF_SUCCESS )
    {
      std::cout<<"unable to init"<<std::endl;
      return EXIT_FAILURE;
    }
  
  result = hackrf_open(&device);
  if( result != HACKRF_SUCCESS ) {
    std::cout<<"unable to open device"<<std::endl;
    //usage();
    return EXIT_FAILURE;
  }

  // tune to 107.9 MHz
  uint64_t freq = 107900000;
  result = hackrf_set_freq(device, freq);
  if( result != HACKRF_SUCCESS )
    {
      std::cout<<"unable to tune"<<std::endl;
      return EXIT_FAILURE;
    }
  std::cout<<"current freq: "<< freq <<std::endl;



  // start receiving
  result = hackrf_start_rx(device, rx_callback, NULL);
  if( result != HACKRF_SUCCESS ) {
    std::cout<<"unable to register rx callback"<<std::endl;
    return EXIT_FAILURE;
  }

  while( (hackrf_is_streaming(device) == HACKRF_TRUE) )
    {
      
    }
	
  



  
  result = hackrf_close(device);
  if( result != HACKRF_SUCCESS ) 
    {
      std::cout<<"unable to close device"<<std::endl;
      
    }
  else
    {
      std::cout<<"closed device"<<std::endl;
    }
  
  hackrf_exit();
 
 
  return 0;
}

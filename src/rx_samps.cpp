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

/*
int
rx_callback( hackrf_transfer* transfer )
{
  size_t bytes_to_write;
  size_t bytes_written;
  unsigned int i;
  
  if( fd != NULL ) 
    {
      byte_count += transfer->valid_length;
      bytes_to_write = transfer->valid_length;

      if ( limit_num_samples )
	{	  
	  if ( bytes_to_write >= bytes_to_xfer )
	    {
	      bytes_to_write = bytes_to_xfer;
	    }
	  bytes_to_xfer -= bytes_to_write;
	}

      
      if( stream_size > 0 )
	{

	  if ( ( stream_size - 1 + stream_head - stream_tail ) % stream_size < bytes_to_write )
	    {
	      stream_drop++;
	    }
	  else
	    {
	      if( stream_tail + bytes_to_write <= stream_size )
		{
		  memcpy( stream_buf + stream_tail,
			  transfer->buffer,
			  bytes_to_write );
		}
	      else
		{
		  memcpy( stream_buf + stream_tail,
			  transfer->buffer,
			  (stream_size-stream_tail) );
		  
		  memcpy( stream_buf,
			  transfer->buffer+(stream_size-stream_tail),
			  bytes_to_write-(stream_size-stream_tail) );
		}

	      __atomic_store_n( &stream_tail,
				(stream_tail+bytes_to_write)%stream_size,
				__ATOMIC_RELEASE );
	    }

	  return 0;
	}
      else
	{
	  bytes_written = fwrite( transfer->buffer,
				  1,
				  bytes_to_write,
				  fd );
	  
	  if( (bytes_written != bytes_to_write)
	      || (limit_num_samples && (bytes_to_xfer == 0) ) )
	    {
	      return -1;
	    }
	  else
	    {
	      return 0;
	    }
	}
    }
  else
    {
      return -1;
    }
}
*/

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
  //result = hackrf_start_rx( device, rx_callback, NULL );
  
  if( result != HACKRF_SUCCESS )
    {
      std::cout<<"unable to register rx callback"<<std::endl;
      return EXIT_FAILURE;
    }
  
  while( hackrf_is_streaming( device ) == HACKRF_TRUE )
    {
      
    }
	
  //hackrf_stop_rx(hackrf_device* device)
  
  result = hackrf_close(device);

  if( result != HACKRF_SUCCESS ) 
    {
      std::cout << "unable to close device" << std::endl;    
    }
  else
    {
      std::cout << "closed device" << std::endl;
    }
  
  hackrf_exit();
 
 
  return 0;
}

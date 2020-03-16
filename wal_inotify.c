/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

//terminates current process when /tmp/wal is touched
int main()
{
  printf("le");
  char buffer[BUF_LEN];
  //make wal file if not present
  FILE *fptr;
  fptr = fopen("/tmp/wal", "rb+");
  if(fptr == NULL) //if file does not exist, create it
  {
      fptr = fopen("/tmp/wal", "wb");
  }
  fclose(fptr);

  int fd = inotify_init();
  /*checking for error*/
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }
  int wal = inotify_add_watch( fd, "/tmp/wal", IN_DELETE );
  bool reload = false;

  while (!reload) {
     int i = 0;
     int length = read( fd, buffer, sizeof(buffer) ); 

     /*checking for error*/
     if ( length < 0 ) {
       exit(-1);
     }  

     while( i < length) {
	   struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
	   if ( i < length ){
		   if ( event ->mask & IN_DELETE){
			   reload = true;
		   }
	   }
       	i += EVENT_SIZE + event->len;
	}
  }
}

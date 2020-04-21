/*
 *  fb_test.c  -- simple framebuffer test program
 *
 *  build with --
 *     cc fb_test.c -o fb_test
 *  
 *  run with --
 *     ./fb_test
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

int main(int argc, char * argv[])
{
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char * fbp = 0;

  // Open the file for reading and writing
  fbfd = open("/dev/fb1", O_RDWR);
  if (!fbfd) {
    printf("Error: cannot open framebuffer device.\n");
    return(1);
  }
  printf("The framebuffer device was opened successfully.\n");

  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
    printf("Error reading fixed information.\n");
    return(1);
  }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
    printf("Error reading variable information.\n");
  }
  printf("%dx%d, %d bpp\n", vinfo.xres, vinfo.yres, 
         vinfo.bits_per_pixel );

  // map framebuffer to user memory 
  screensize = finfo.smem_len;

  fbp = (char*)mmap(0, 
                    screensize, 
                    PROT_READ | PROT_WRITE, 
                    MAP_SHARED, 
                    fbfd, 0);

  if ((int)fbp == -1) {
    printf("Failed to mmap.\n");
  }
  else for (int i=0; i < 50; i++) {
    // draw...
    // just fill upper half of the screen with something
    memset(fbp, 0xFF, screensize/2);

    // and lower half with something else
    memset(fbp + screensize/2, 0x00+i, screensize/2);
  }

  printf("done");

  // cleanup
  munmap(fbp, screensize);
  close(fbfd);

  return 0;
}


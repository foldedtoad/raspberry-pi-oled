# raspberry-pi-oled
## Raspberry Pi + Adafruit OLED Bonnet Setup and Configuration (and other similar Solomon devices)

This procedure has been tested on a Raspberry Pi Zero W with the Adafruit 128x64 OLED Bonnet.  Since the Raspberry Pi Zero W is currently the lowest capable board in the Raspberry Pi series, this procedure should work for the whole series.  This OLED display communications with the Raspberry Pi via the I2C bus.  The OLED Bonnet's controller is the Solmon SSD1306 chipset.  

**Note:** This procedure outlines the use of a SSD1306 128x64 device, but with a simple addition to the procedure a SSD1306 128x32 device can be configured: alternate, 128x32-based configuration/comments will be inline below.
  
The first operation is to determine if you Raspberry Pi (RaspPi hereafter) has the firmware support needed.  The firmware used was description was Raspbian 4.19.66, but support should be in earlier versions as well.  

From a terminal shell on the Raspberry Pi, use the command 
```
	$ uname -a
```
This should respond with a line which indicates the Raspbian level for your system.
  
From a terminal shell, determine that the OLED driver is present.
```
	$ ls -l /lib/modules/$(uname -r)/kernel/drivers/video/fbdev/ssd1307fb.ko
```
Note that while the driver is named “*ssd1307fb.ko*”, it supports the following Solomon devices:  **ssd1305, ssd1306, ssd1307, ssd1309**.  
  
Next insure the I2C bus driver has been started, and runs at an appropiate baudrate.
```
	$ cd /boot
	$ sudo nano config.txt
```
Add or modify the following lines, then save the file.
```
	dtparam=i2c_arm=on                   # note: may already be set on
	dtparam=i2c_baudrate=1000000         # boost the baudrate to max usable value
```
Start the driver with the following terminal command.
```
	$ sudo modprobe ssd1307fb,inverted
```
or
```
	$ sudo modprobe ssd1307fb,inverted,height=32
```

To insure the drive has started successfully, issue the following terminal command.
```
	$ lsmod | grep ssd1307fb
```
You should see four lines of output, with the top line being the driver instance.  

Next determine if the devicetree support is present, and if so, activate the overlay defining the hardware parameters needed to integrate the OLED controller into the system configuration.  
From terminal shell, determine that the needed overlay file is present.
```
 	$ ls -l /boot/overlays/ssd1306.dtbo
```
The *.dtbo extension indicates the file is a “device-tree binary overlay” file.
If this file is present then you need to manually activate it with the following command
```
	$ sudo dtoverlay ssd1306 inverted
```
At this point you should see that there is a new framebuffer device, */dev/fb1*, by issuing the following terminal command.
```
	$ ls -l /dev/fb*
	crw-rw---- 1 root video 29, 0 Apr   8 15:21 /dev/fb0
	crw-rw---- 1 root video 29, 1 Apr   8 15:21 /dev/fb1
```
The first device listed, */dev/fb0*, is the main video device for the system.  
The second device listed, */dev/fb1*, is the new framebuffer device which provides access to the ssd1306 device.

## Configure the system to start the driver and configure it at boot time
To configure so this whole process is done at boot-time, make the following edits.  
Navigate to the */etc* directory and open the *modules* file.
```
	$ cd /etc
	$ sudo nano modules
```
Go to the bottom of the file, add the following line and save the file.
```
	ssd1307fb
```
Navigate to the */boot* directory and open the *config.txt* file.
```
	$ cd /boot
	$ sudo nano config.txt
```
Go to the bottom of file, add the following lines and save the file.
```
	dtoverlay=ssd1306,inverted              # for 128x64 device
```
or 
```
	dtoverlay=ssd1306,inverted,height=32    # for 128x32 device
```

To enable terminal output to the SSD1306, navigate to */boot/cmdline.txt*, add the following text to the end of the line and save the file.
```
	fbcon=map:01
```
Note that the "01" parameter is rather enigmatic: in this usage the odd-numbered tty's are opened per normal, but even-numbered tty's display on the SSD1306 device. So */dev/tty[2,4,6,8...]* will use the SSD1306.  
The special case is */dev/tty7*, which is the main window GUI shell.

## Reboot the system.
Finally, reboot to allow the changes to take effect.


## Activating a SSD1306-based terminal
To switch to the OLED-based terminal do the following.
```
	$ [ctrl]+[alt]+[F2]
```
This will start */dev/tty2* as a terminal.  

If all works as expected then you should see a blinking cursor on the SSD1306 screen.  

By pressing [enter], you should get a login prompt.  Use whatever login user and password you've set.  
While the default password for Raspberry-Pi's is "raspberry", it's recommended you changed the password to some other that this default.  

To switch back to the GUI-based window, do the following.
```
	$ [ctrl]+[alt]+[F7]
```
Note that the GUI-based window will appear to be non-responsive while you are switched to the SSD1306-based terminal.  

Since the default tty login typically spews out quite a bit of text, it is nice to not have this text displayed.  This is accomplished with the following option.
```
	$ touch ~/.hushlogin
```
This will considerably shorten the time until a useful command line prompt appears.  

## Using alternative fonts
The default terminal font size is too large to be of any practical use in conjunction with the SSD1306.  
The next procedure will add a new, smaller font, which allows more characters per line to be displayed.  
 
Copy the “tom-thumb” font file, *tom-thumb.psf.gz*, into */usr/share/consolefonts/*.
```
	$ sudo cp ~/tom-thumb.psf.gz /usr/share/consolefonts/
```

While you're accessing the SSD1306-based terminal, you can manually change the font to the “tom-thumb” font with the following command.
```
	$ setfont tom-thumb
```
## Example 
Below is an image of a Raspberry Pi Zero W with an Adafruit OLED Bonnet.  
In this example, the "tom-thumb" font is used to edit a file with the nano editor.  
![Adafruit OLED Bonnet + Tom-Thumb font](https://github.com/foldedtoad/raspberry-pi-oled/blob/master/Adafruit_OLED_Bonnet.jpg)


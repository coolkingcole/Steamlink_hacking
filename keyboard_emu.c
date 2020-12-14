#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <iostream>
#include <time.h>
#include <string>

using namespace std;

int singlekeypress(int fd_key_emulator, int key, int mili) {
	// struct member for input events
	struct input_event key_input_event;
	memset(&key_input_event, 0, sizeof(input_event));

	// key press event for 'a'
	key_input_event.type = EV_KEY;
	key_input_event.code = key;
	key_input_event.value = 1;

	// now write to the file descriptor
	write(fd_key_emulator, &key_input_event, sizeof(input_event));

	memset(&key_input_event, 0, sizeof(input_event));
	// EV_SYN for key press event
	key_input_event.type = EV_SYN;
	key_input_event.code = SYN_REPORT;
	key_input_event.value = 0;

	// now write to the file descriptor
	write(fd_key_emulator, &key_input_event, sizeof(input_event));

	memset(&key_input_event, 0, sizeof(input_event));

	// key release event for 'a'
	key_input_event.type = EV_KEY;
	key_input_event.code = key;
	key_input_event.value = 0;

	usleep(mili * 1000);
	// now write to the file descriptor
	write(fd_key_emulator, &key_input_event, sizeof(input_event));

	memset(&key_input_event, 0, sizeof(input_event));
	// EV_SYN for key press event
	key_input_event.type = EV_SYN;
	key_input_event.code = SYN_REPORT;
	key_input_event.value = 0;

	// now write to the file descriptor
	write(fd_key_emulator, &key_input_event, sizeof(input_event));
	return 0;
}

int pressandhold2(int fd_key_emulator, int key1, int key2, int mili) {
	// struct member for input events
	struct input_event key_input_event;
	memset(&key_input_event, 0, sizeof(input_event));

	// key press event for 'a'
	key_input_event.type = EV_KEY;
	key_input_event.code = key1;
	key_input_event.value = 1;
	write(fd_key_emulator, &key_input_event, sizeof(input_event));
	memset(&key_input_event, 0, sizeof(input_event));

	// EV_SYN for key press event
	key_input_event.type = EV_SYN;
	key_input_event.code = SYN_REPORT;
	key_input_event.value = 0;
	write(fd_key_emulator, &key_input_event, sizeof(input_event));
	memset(&key_input_event, 0, sizeof(input_event));

	// key press event for 'a'
	key_input_event.type = EV_KEY;
	key_input_event.code = key2;
	key_input_event.value = 1;
	write(fd_key_emulator, &key_input_event, sizeof(input_event));
	memset(&key_input_event, 0, sizeof(input_event));
	
	// EV_SYN for key press event
	key_input_event.type = EV_SYN;
	key_input_event.code = SYN_REPORT;
	key_input_event.value = 0;
	write(fd_key_emulator, &key_input_event, sizeof(input_event));
	memset(&key_input_event, 0, sizeof(input_event));

	usleep(mili * 1000);

	// key release event for 'a'
	key_input_event.type = EV_KEY;
	key_input_event.code = key1;
	key_input_event.value = 0;
	write(fd_key_emulator, &key_input_event, sizeof(input_event));
	memset(&key_input_event, 0, sizeof(input_event));

	// EV_SYN for key press event
	key_input_event.type = EV_SYN;
	key_input_event.code = SYN_REPORT;
	key_input_event.value = 0;
	write(fd_key_emulator, &key_input_event, sizeof(input_event));

	// key release event for 'a'
	key_input_event.type = EV_KEY;
	key_input_event.code = key2;
	key_input_event.value = 0;
	write(fd_key_emulator, &key_input_event, sizeof(input_event));
	memset(&key_input_event, 0, sizeof(input_event));

	// EV_SYN for key press event
	key_input_event.type = EV_SYN;
	key_input_event.code = SYN_REPORT;
	key_input_event.value = 0;
	write(fd_key_emulator, &key_input_event, sizeof(input_event));

	return 0;
}

/*
 * 
 */
int main(int argc, char** argv) {

	// create uinput file descriptor
	int fd_key_emulator;                                                                    

	// open file descriptor
	fd_key_emulator = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	// uinput_user_dev struct for fake keyboard
	struct uinput_user_dev dev_fake_keyboard;
	memset(&dev_fake_keyboard, 0, sizeof(uinput_user_dev));
	snprintf(dev_fake_keyboard.name, UINPUT_MAX_NAME_SIZE, "kb-emulator");
	dev_fake_keyboard.id.bustype = BUS_USB;
	dev_fake_keyboard.id.vendor = 0x01;
	dev_fake_keyboard.id.product = 0x01;
	dev_fake_keyboard.id.version = 1;



	/**configure the input device to send type of events, inform to subsystem which 
	 * type of input events we are using via ioctl calls. 
	 * UI_SET_EVBIT ioctl request is used to applied on uinput descriptor to enable a type of event.
	 **/
	// enable key press/release event
	ioctl(fd_key_emulator, UI_SET_EVBIT, EV_KEY);

	// enable set of KEY events here
	for (int i=0; i < 256; i++) {
        ioctl(fd_key_emulator, UI_SET_KEYBIT, i);
    }

	// enable synchronization event
	ioctl(fd_key_emulator, UI_SET_EVBIT, EV_SYN);

	// now write the uinput_user_dev structure into uinput file descriptor
	write(fd_key_emulator, &dev_fake_keyboard, sizeof(uinput_user_dev));

	// create the device via an IOCTL call 
	ioctl(fd_key_emulator, UI_DEV_CREATE);
	// now fd_key_emulator represents the end-point file descriptor of the new input device. 
	// add 1 second sleep
	usleep(3000*1000);

	printf("ALT+ENTER\n");
	pressandhold2(fd_key_emulator, KEY_LEFTALT, KEY_ENTER, 	25);	usleep(5000*1000);

	printf("WIN+X\n");
	pressandhold2(fd_key_emulator, KEY_LEFTMETA, KEY_X, 	25);    usleep(1000*1000);

	printf("I\n");
	singlekeypress(fd_key_emulator, KEY_I, 					25);    usleep(1000*1000);
	
	printf("C");
	singlekeypress(fd_key_emulator, KEY_C, 					25); 	usleep(1000*1000);
	printf("A");
	singlekeypress(fd_key_emulator, KEY_A, 					25);    usleep(1000*1000);
	printf("L");
	singlekeypress(fd_key_emulator, KEY_L, 					25);    usleep(1000*1000);
	printf("C");
	singlekeypress(fd_key_emulator, KEY_C,					25); 	usleep(1000*1000);
	printf(".");
	singlekeypress(fd_key_emulator, KEY_DOT, 				25); 	usleep(1000*1000);
	printf("E");
	singlekeypress(fd_key_emulator, KEY_E, 					25); 	usleep(1000*1000);
	printf("X");
	singlekeypress(fd_key_emulator, KEY_X, 					25); 	usleep(1000*1000);
	printf("E");
	singlekeypress(fd_key_emulator, KEY_E, 					25); 	usleep(1000*1000);

	printf("\nEnter\n");
	singlekeypress(fd_key_emulator, KEY_ENTER, 				25);    usleep(1000*1000);

	return 0;
}
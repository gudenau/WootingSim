#include <iostream>

#include <Windows.h>

#include <wooting-rgb-sdk.h>

int main(){
    std::cout << "Hello World!\n";

	if (wooting_rgb_kbd_connected()) {
		std::cout << "Found a Wooting!\n";


		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 21; x++) {
				wooting_rgb_direct_set_key(
					y,
					x,
					255,
					0,
					255
				);
				Sleep(100);
			}
		}
	}
	else {
		std::cout << "Did not find a Wooting\n";
	}
}

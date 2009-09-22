//
// serialbutton.c - A serial port button program
// Written by Ted Burke - 16/9/2009
//

// The following definition is required because SendInput,
// which is used in this program to simulate keystrokes,
// is only supported in certain versions of Windows.
#define _WIN32_WINNT 0x0500

// Round up some of the usual suspects
#include <windows.h>
#include <winuser.h>
#include <stdio.h>

main()
{
	// Handle for serial port
	HANDLE hComm;
	
	// Variables to store serial port status pins
	DWORD modemStat, previousModemStat;
	
	// Create simulated keyboard events, which will be used later on
	INPUT keyDownAndUp[2];
	PINPUT pKeyDown = keyDownAndUp;
	PINPUT pKeyUp = keyDownAndUp+1;
	pKeyDown->type = INPUT_KEYBOARD;
	pKeyDown->ki.wVk = VK_RIGHT; // Righ arrow key
	pKeyDown->ki.wScan = 0; // only necessary for unicode keys or of scan codes specified in flags
	pKeyDown->ki.dwFlags = 0;
	pKeyDown->ki.time = 0;
	pKeyDown->ki.dwExtraInfo = 0;
	pKeyUp->type = INPUT_KEYBOARD;
	pKeyUp->ki.wVk = VK_RIGHT; // Righ arrow key
	pKeyUp->ki.wScan = 0; // only necessary for unicode keys or of scan codes specified in flags
	pKeyUp->ki.dwFlags = KEYEVENTF_KEYUP;
	pKeyUp->ki.time = 0;
	pKeyUp->ki.dwExtraInfo = 0;
	
	// Open serial port
	hComm = CreateFile("COM1", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if (hComm == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error opening serial port");
		exit(1);
	}
	
	// Check serial port status pins
	// MS_CTS_ON=0x0010, MS_DSR_ON=0x0020, MS_RING_ON=0x0040, MS_RLSD_ON=0x0080
	while(1)
	{
		if (!GetCommModemStatus(hComm, &modemStat))
		{
			fprintf(stderr, "Error getting serial port status");
			exit(1);
		}

		//fprintf(stderr, "Comm modem status = %d\n", modemStat);
		
		if ((modemStat & MS_CTS_ON) != (previousModemStat & MS_CTS_ON))
		{
			if (modemStat & MS_CTS_ON)
			{
				// Simulate a "Right Arrow" keystroke (press and release)
				SendInput(2, keyDownAndUp, sizeof(INPUT));
			}
			
			// Short (50ms) pause to debounce switch
			Sleep(50);
		}
		
		// Remember updated status
		previousModemStat = modemStat;
	}
}

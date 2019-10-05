
#include "cpw.h"


UINT DoTestProcedure()
{
	// from "http://xanthium.in/Serial-Port-Programming-using-Win32-API"
	HANDLE 		hComm;	// handle to com port
	LPSTR		szBufRead;		// red buffer
	UINT		nBytes = 0xff;	// how many bytes to read 
	
	hComm = CreateFileW(
					L"\\\\.\\COM1",                	// port name
	                GENERIC_READ | GENERIC_WRITE, 	// Read/Write
	                0,                            	// No Sharing
	                NULL,                         	// No Security
	                OPEN_EXISTING,	// Open existing port only
	                0,            	// Non Overlapped I/O
	                NULL);        	// Null for Comm Devices
	if (INVALID_HANDLE_VALUE==hComm)
	{
		printf("Error in opening serial port\n");		
		return 23;
	}
	else
	{
		printf("opening serial port successful\n");		
	}
	szBufRead = (LPSTR)GlobalAlloc(GPTR, nBytes);
	// 
	ReadDataFromSerialPort(hComm, (LPVOID*)&szBufRead, &nBytes);
	printf("Read from COM1: \"%s\"\n", szBufRead);
	GlobalFree(szBufRead);
	CloseHandle(hComm);							// Closing the Serial Port
	SetDcbParameters(hComm);
  	return 0;
}


UINT SetDcbParameters(HANDLE hComm)
{
	// First you declare a new structure of type DCB and initializes it.
	DCB dcbSerialParams = {0}; // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	// After that retrieve the current settings of the serial port using the GetCommState() function.
	BOOL bStatus = GetCommState(hComm, &dcbSerialParams); 
	if(!bStatus)
	{
		printf("cannot get the COM state\nPort cannot be configured!");
		return 0;
	}
	// and set the values for Baud rate, Byte size, Number of start/Stop bits etc.
	dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	dcbSerialParams.Parity   = NOPARITY;  // Setting Parity = None
	// Now its time to configure the serial port according to the DCB structure using SetCommState() function.
	SetCommState(hComm, &dcbSerialParams);	
	
	// Setting Timeouts
	// Timeouts helps to prevent your program from waiting endlessly till data arrives. It helps the read or 
	// 		write calls to return after a set time period has elapsed.
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant    = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier  = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant   = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
	
	return 0;
}

UINT WriteDataToSerialPort(HANDLE hComm, LPVOID* ppData, LPUINT lpuiBytes)
{
	DWORD dwBytesWritten = 0;
	BOOL bStatus = WriteFile(
					hComm,      			// Handle to the Serial port
                   	*ppData,     			// Data to be written to the port
                   	*lpuiBytes,  			// No of bytes to write
                   	&dwBytesWritten, 				// Bytes written
                   	NULL);
    if(!bStatus)
    {
    	printf("cannot write data to the COM port\n");
    	*ppData = 0;
    	return 32;
	}
	// *lpuiBytes = dNoOfBytesWritten; // don't need
	return 0;
}

UINT ReadDataFromSerialPort(HANDLE hComm, LPVOID* ppData, LPUINT lpuiBytes)
{
	char 	c;						// Temporary character used for reading
	char 	szBuf[0xff];			// Buffer for storing Rxed Data
	BOOL 	bStatus;
	DWORD	dwBytesRead = 0;		// read
	UINT 	i=0;
	LPSTR	sz = (LPSTR)*ppData;	// the "iterator" of the output data pointers
	
	do
	{
		ReadFile(
				hComm, 						// Handle to the Serial port
				&c, 						// Temporary character
				sizeof(c),					// Size of TempChar
				&dwBytesRead,				// Number of bytes read
				NULL);
		memset(sz++, c, 1);					// Store Tempchar into output data buffer
		if(++i>*lpuiBytes)
		{
			*ppData = 0;		// cancel the data reception for this buffer
			break;				// maximal number of bytes read in the ppData...
		}
		wprintf(L"%ls", c);
		 
	} while (dwBytesRead);
	// 
	return 0;
}




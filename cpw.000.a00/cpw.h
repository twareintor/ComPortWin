///////////////////////// cpw.h //////////////////////////////////////////////////

#include<windows.h>
#include<stdio.h>

// from "http://xanthium.in/Serial-Port-Programming-using-Win32-API"

UINT DoTestProcedure();

UINT SetDcbParameters(HANDLE hComm);

UINT WriteDataToSerialPort(HANDLE hComm, LPVOID* ppData, LPUINT lpdwBytes);

UINT ReadDataFromSerialPort(HANDLE hComm, LPVOID* ppData, LPUINT lpdwBytes);


///////////////////////////////////////////////////////////////////////////////////




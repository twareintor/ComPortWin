
#ifndef STDOUT2CON_H_109823748912734891
#define STDOUT2CON_H_109823748912734891

// needed for console output...
#include <windows.h>
#include <io.h>
#include <Fcntl.h>

#include <cstdio>



void SetStdOutToNewConsole();


void SetStdOutToNewConsole()
{
    // allocate a console for this app
    AllocConsole();

    // redirect unbuffered STDOUT to the console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);		// _O_TEXT or _O_WTEXT
    FILE *fp = _fdopen( fileDescriptor, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    // give the console window a nicer title
    SetConsoleTitleW(L"Debug Output");

    // give the console window a bigger buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
    {
        COORD bufferSize;
        bufferSize.X = csbi.dwSize.X;
        bufferSize.Y = 9999;
        SetConsoleScreenBufferSize(consoleHandle, bufferSize);
    }
}



#endif//STDOUT2CON_H_109823748912734891

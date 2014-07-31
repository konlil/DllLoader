//==================================================
// DelayLoadDemo - Matt Pietrek 1998
// Microsoft Systems Journal, December 1998
// FILE: DelayLoadDemo.CPP
//==================================================
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "delayimp.h"

#pragma comment(lib, "COMCTL32.LIB")

int main()
{
    GetTopWindow( 0 );  // Call a USER32 function
    GetTopWindow( 0 );  // Call it a few more times to see that subsequent
    GetTopWindow( 0 );  // calls bypass the /DELAYLOAD generated stubs

    GetDesktopWindow();

    InitCommonControls();
        
    return 0;
}

FARPROC WINAPI MSJCheezyDelayLoadHook(unsigned dliNotify, PDelayLoadInfo pdli )
{
    // Display which type of notification it is
    switch( dliNotify )
    {
        case dliStartProcessing:
            printf( "dliStartProcessing      " );
            break;
            
        case dliNotePreLoadLibrary:
            printf( "dliNotePreLoadLibrary   " );
            break;
            
        case dliNotePreGetProcAddress:
            printf( "dliNotePreGetProcAddress" );
            break;
            
        case dliNoteEndProcessing:
            printf( "dliNoteEndProcessing    " );
            break;          
    }

    // Display the DLL name and the HMODULE
    printf( " %s(%08X) -> ", pdli->szDll, pdli->hmodCur );

    // Display the API ordinal or API name, as appropriate
    if ( pdli->dlp.fImportByName )
        printf( " %s", pdli->dlp.szProcName );
    else
        printf( " ordinal:%u", pdli->dlp.dwOrdinal );

    printf( "\n" );
    
    return 0;   // Make sure __delayLoadHelper doesn't think we did something!
}

//
// Override the standard definition of __pfnDliNotifyHook that's part of
// DELAYHLP.LIB
//
PfnDliHook __pfnDliNotifyHook2 = MSJCheezyDelayLoadHook;


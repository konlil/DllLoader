#include <Windows.h>
#include <delayimp.h>
#include <stdio.h>

extern "C"{
#include "MemoryModule.h"
}

FARPROC WINAPI MSJCheezyDelayLoadHook(unsigned dliNotify, PDelayLoadInfo pdli )
{
	FARPROC rtn = NULL;

    // Display which type of notification it is
    switch( dliNotify )
    {
        case dliStartProcessing:
            OutputDebugStringA( "dliStartProcessing      " );
            break;
            
        case dliNotePreLoadLibrary:
			{
				OutputDebugStringA( "dliNotePreLoadLibrary   " );
				FILE* fp = fopen("d3dx9_43.dll", "rb");
				if (fp)
				{
					fseek(fp, 0, SEEK_END);
					size_t size = ftell(fp);
					unsigned char* data = (unsigned char *)malloc(size);
					fseek(fp, 0, SEEK_SET);
					fread(data, 1, size, fp);
					fclose(fp);

					HMEMORYMODULE handle = NULL;
					handle = MemoryLoadLibrary(data);
					rtn = (FARPROC)handle;
				}
			}
            break;
            
        case dliNotePreGetProcAddress:
			{
				OutputDebugStringA( "dliNotePreGetProcAddress" );
				rtn = MemoryGetProcAddress(pdli->hmodCur, pdli->dlp.szProcName);
			}
            break;
            
        case dliNoteEndProcessing:
            OutputDebugStringA( "dliNoteEndProcessing    " );
            break;          
    }

	char szText[1024];

    // Display the DLL name and the HMODULE
	sprintf_s(szText, " %s(%08X) -> ", pdli->szDll, pdli->hmodCur);
    OutputDebugStringA( szText );

    // Display the API ordinal or API name, as appropriate
    if ( pdli->dlp.fImportByName ){
		sprintf_s(szText, " %s", pdli->dlp.szProcName );
        OutputDebugStringA( szText );
	} else {
		sprintf_s(szText, " ordinal:%u", pdli->dlp.dwOrdinal );
        OutputDebugStringA(szText);
	}

    OutputDebugStringA( "\n" );
    
    return rtn;   // Make sure __delayLoadHelper doesn't think we did something!
}

//
// Override the standard definition of __pfnDliNotifyHook that's part of
// DELAYHLP.LIB
//
PfnDliHook __pfnDliNotifyHook2 = MSJCheezyDelayLoadHook;
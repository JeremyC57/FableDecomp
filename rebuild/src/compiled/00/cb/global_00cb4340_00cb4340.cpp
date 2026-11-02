// GetReadingString(HWND__ *) -- read the active input method reading string.
// Leaf: ?GetReadingString@@YGXPAUHWND__@@@Z
// Flags: /Gr
// Adapted from Microsoft DirectX SDK ImeUi.cpp.
// Copyright (c) Microsoft Corporation. Licensed under the MIT License.
// Changes reconstruct the older native implementation; see NAMING_EVIDENCE.md.
#include "engine/ImeUiInputContext.h"
#include <malloc.h>
#include <excpt.h>
#define TRUE 1
#define FALSE 0
#define MAX_CANDLIST 10
#define EXCEPTION_STACK_OVERFLOW 0xC00000FDL
#define _ImmGetContext ImmGetContext
#define _ImmReleaseContext ImmReleaseContext
#define _IsLeadByte(x) (LeadByteTable[(BYTE)(x)])
#define IsNT() (g_osi.dwPlatformId==2)
#define IMEID_CHT_VER42 0x04020404
#define IMEID_CHT_VER43 0x04030404
#define IMEID_CHT_VER44 0x04040404
#define IMEID_CHT_VER50 0x05000404
#define IMEID_CHT_VER51 0x05010404
#define IMEID_CHT_VER52 0x05020404
#define IMEID_CHS_VER53 0x05030804
#define IMEID_CHS_VER41 0x04010804
#define IMEID_CHS_VER42 0x04020804
extern "C" HIMC __stdcall ImmGetContext(HWND);
extern "C" BOOL __stdcall ImmReleaseContext(HWND,HIMC);
extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(UINT,DWORD,const char*,int,LPWSTR,int);
extern "C" __declspec(dllimport) int __stdcall lstrlenW(const WCHAR*);
extern "C" __declspec(dllimport) LPWSTR __stdcall lstrcatW(LPWSTR,const WCHAR*);
DWORD __stdcall GetImeId(UINT=0);
void __stdcall GetReadingWindowOrientation(DWORD);
extern UINT (__stdcall *_GetReadingString)(HIMC,UINT,LPWSTR,PINT,BOOL*,UINT*);
extern LPINPUTCONTEXT2 (__stdcall *_ImmLockIMC)(HIMC);
extern BOOL (__stdcall *_ImmUnlockIMC)(HIMC);
extern void* (__stdcall *_ImmLockIMCC)(HIMCC);
extern BOOL (__stdcall *_ImmUnlockIMCC)(HIMCC);
extern WCHAR g_szCandidate[10][256];
extern DWORD g_dwCount;
static DWORD g_dwSelection;
extern bool g_bHorizontalReading,g_bReadingWindow;
extern BYTE LeadByteTable[256];
extern UINT g_uCodePage,g_uCandPageSize;
extern int g_iReadingError;
extern WCHAR g_szReadingString[32];
extern OSVERSIONINFOA g_osi;
#define min(a,b) (((a)<(b))?(a):(b))
void __stdcall GetReadingString(HWND hWnd)
{
    DWORD dwId = GetImeId();
    if( !dwId )
    {
        return;
    }

    HIMC himc;
    himc = _ImmGetContext( hWnd );
    if( !himc )
        return;

    DWORD dwlen = 0;
    DWORD dwerr = 0;
    WCHAR* wstr = 0;
    bool unicode = FALSE;
    LPINPUTCONTEXT2 lpIMC = 0;

    if( _GetReadingString )
    {
        BOOL bVertical;
        UINT uMaxUiLen;
        dwlen = _GetReadingString( himc, 0, 0, ( PINT )&dwerr, &bVertical, &uMaxUiLen );
        if( dwlen )
        {
            __try { wstr=(WCHAR*)_alloca(dwlen); }
            __except(GetExceptionCode()==EXCEPTION_STACK_OVERFLOW ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
                ImmReleaseContext(hWnd,himc);return;
            }
            dwlen = _GetReadingString( himc, dwlen, wstr, ( PINT )&dwerr, &bVertical, &uMaxUiLen );
        }

        g_bHorizontalReading = bVertical == 0;
        unicode = true;
    }
    else //	IMEs that doesn't implement Reading String API
    {
        lpIMC = _ImmLockIMC( himc );

        // *** hacking code from Michael Yang ***

        LPBYTE p = 0;

        switch( dwId )
        {

            case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
            case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
            case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP

                p = *( LPBYTE* )( ( LPBYTE )_ImmLockIMCC( lpIMC->hPrivate ) + 24 );
                if( !p ) break;
                dwlen = *( DWORD* )( p + 7 * 4 + 32 * 4 );
                dwerr = *( DWORD* )( p + 8 * 4 + 32 * 4 );
                wstr = ( WCHAR* )( p + 56 );
                unicode = TRUE;
                break;

            case IMEID_CHT_VER50: // 5.0.x.x // WinME

                p = *( LPBYTE* )( ( LPBYTE )_ImmLockIMCC( lpIMC->hPrivate ) + 3 * 4 ); // PCKeyCtrlManager
                if( !p ) break;
                p = *( LPBYTE* )( ( LPBYTE )p + 1 * 4 + 5 * 4 + 4 * 2 ); // = PCReading = &STypingInfo
                if( !p ) break;
                dwlen = *( DWORD* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 + 16 );
                dwerr = *( DWORD* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 + 16 + 1 * 4 );
                wstr = ( WCHAR* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 );
                unicode = FALSE;
                break;

            case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
            case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
            case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)

                p = *( LPBYTE* )( ( LPBYTE )_ImmLockIMCC( lpIMC->hPrivate ) + 4 );   // PCKeyCtrlManager
                if( !p ) break;
                p = *( LPBYTE* )( ( LPBYTE )p + 1 * 4 + 5 * 4 );                       // = PCReading = &STypingInfo
                if( !p ) break;
                dwlen = *( DWORD* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 + 16 * 2 );
                dwerr = *( DWORD* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 + 16 * 2 + 1 * 4 );
                wstr = ( WCHAR* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 );
                unicode = TRUE;
                break;

                // the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
            case IMEID_CHS_VER41:
            {
                int offset;
                offset = ( GetImeId( 1 ) >= 0x00000002 ) ? 8 : 7;

                p = *( LPBYTE* )( ( LPBYTE )_ImmLockIMCC( lpIMC->hPrivate ) + offset * 4 );
                if( !p ) break;
                dwlen = *( DWORD* )( p + 7 * 4 + 16 * 2 * 4 );
                dwerr = *( DWORD* )( p + 8 * 4 + 16 * 2 * 4 );
                dwerr = min(dwerr,dwlen);
                wstr = ( WCHAR* )( p + 6 * 4 + 16 * 2 * 1 );
                unicode = TRUE;
                break;
            }

            case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
            {
                int nTcharSize = IsNT() ? sizeof( WCHAR ) : sizeof( char );
                p = *( LPBYTE* )( ( LPBYTE )_ImmLockIMCC( lpIMC->hPrivate ) + 1 * 4 + 1 * 4 + 6 * 4 ); // = PCReading = &STypintInfo
                if( !p ) break;
                dwlen = *( DWORD* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 + 16 * nTcharSize );
                dwerr = *( DWORD* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 + 16 * nTcharSize + 1 * 4 );
                wstr = ( WCHAR* )( p + 1 * 4 + ( 16 * 2 + 2 * 4 ) + 5 * 4 );
                unicode = IsNT() ? TRUE : FALSE;
            }
        }	// switch

        g_szCandidate[0][0] = 0;
        g_szCandidate[1][0] = 0;
        g_szCandidate[2][0] = 0;
        g_szCandidate[3][0] = 0;
    }
    g_dwCount = dwlen;
    g_dwSelection = ( DWORD )-1; // do not select any char
    if( unicode )
    {
        int i;
        for( i = 0; ( DWORD )i < dwlen; i++ ) // dwlen > 0, if known IME : yutakah
        {
            if( dwerr <= ( DWORD )i && g_dwSelection == ( DWORD )-1 )
            { // select error char
                g_dwSelection = i;
            }
            g_szCandidate[i][0] = wstr[i];
            g_szCandidate[i][1] = 0;
        }
        g_szCandidate[i][0] = 0;
    }
    else
    {
        char* p = ( char* )wstr;
        int i, j;
        for( i = 0, j = 0; ( DWORD )i < dwlen; i++, j++ ) // dwlen > 0, if known IME : yutakah
        {
            if( dwerr <= ( DWORD )i && g_dwSelection == ( DWORD )-1 )
            {
                g_dwSelection = ( DWORD )j;
            }
            MultiByteToWideChar( g_uCodePage, 0, p + i, 1 + ( _IsLeadByte( p[i] ) ? 1 : 0 ),
                g_szCandidate[j], 1 );
            if ( _IsLeadByte( p[i] ) )
            {
                i++;
            }
        }
        g_szCandidate[j][0] = 0;
        g_dwCount = j;
    }
    if( !_GetReadingString )
    {
        _ImmUnlockIMCC( lpIMC->hPrivate );
        _ImmUnlockIMC( himc );
        GetReadingWindowOrientation( dwId );
    }
    _ImmReleaseContext( hWnd, himc );

    g_bReadingWindow = true;
    if( g_bHorizontalReading )
    {
        g_iReadingError = -1;
        g_szReadingString[0] = 0;
        for( UINT i = 0; i < g_dwCount; i++ )
        {
            if( g_dwSelection == ( DWORD )i )
                g_iReadingError = lstrlenW(g_szReadingString);
            LPCTSTR pszTmp = g_szCandidate[i];
            lstrcatW(g_szReadingString,pszTmp);
        }
    }
    g_uCandPageSize = MAX_CANDLIST;
}

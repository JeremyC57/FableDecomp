// ImeUi_RenderUI(bool,bool) -- render input-method composition and candidate windows.
// Leaf: ?ImeUi_RenderUI@@YIX_N0@Z
// Flags: /Gr /GS
// Original Microsoft DirectX SDK ImeUi.cpp; Copyright Microsoft Corporation, MIT license.
// ImeUi_ProcessMessage(HWND,UINT,WPARAM,LPARAM,bool*) -- process IMM composition and candidate events
#include "engine/ImeUi.h"
#include <string.h>
struct HWND__; typedef HWND__* HWND;
struct HIMC__; typedef HIMC__* HIMC;
typedef long LONG; typedef long LPARAM; typedef long LRESULT;
typedef void* HANDLE; typedef UINT WPARAM; typedef unsigned short TCHAR; typedef TCHAR* LPTSTR;
struct tagCANDIDATELIST { DWORD dwSize,dwStyle,dwCount,dwSelection,dwPageStart,dwPageSize,dwOffset[1]; };
typedef tagCANDIDATELIST* LPCANDIDATELIST;
#define COUNTOF(a) (sizeof(a)/sizeof(a[0]))
#define GETLANG() ((unsigned short)((unsigned long)g_hklCurrent & 0xffff))
#define GETPRIMLANG() ((unsigned short)(GETLANG() & 0x3ff))
#define LANG_CHT 0x404
#define LANG_CHS 0x804
#define MAX_CANDLIST 10
#define WM_INPUTLANGCHANGE 0x51
#define WM_IME_SETCONTEXT 0x281
#define WM_IME_STARTCOMPOSITION 0x10d
#define WM_IME_COMPOSITION 0x10f
#define WM_IME_ENDCOMPOSITION 0x10e
#define WM_IME_NOTIFY 0x282
#define WM_IME_CONTROL 0x283
#define WM_KEYDOWN 0x100
#define WM_KEYUP 0x101
#define WM_CHAR 0x102
#define WM_SYSKEYDOWN 0x104
#define WM_SYSKEYUP 0x105
#define VK_BACK 8
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_LEFT 0x25
#define VK_RIGHT 0x27
#define GCS_COMPSTR 8
#define GCS_COMPATTR 0x10
#define GCS_CURSORPOS 0x80
#define GCS_RESULTSTR 0x800
#define IMN_CLOSECANDIDATE 4
#define IMN_OPENCANDIDATE 5
#define IMN_CHANGECANDIDATE 3
#define IMN_SETCONVERSIONMODE 6
#define IMN_SETOPENSTATUS 8
#define IMN_PRIVATE 14
#define IME_CMODE_SOFTKBD 0x80
#define IME_CMODE_SYMBOL 0x400
#define IMC_CLOSESTATUSWINDOW 0x21
#define IMC_OPENSTATUSWINDOW 0x22
#define IMEID_CHT_VER42 0x04020404
#define IMEID_CHT_VER43 0x04030404
#define IMEID_CHT_VER44 0x04040404
#define IMEID_CHT_VER50 0x05000404
#define IMEID_CHT_VER51 0x05010404
#define IMEID_CHT_VER52 0x05020404
#define IMEID_CHT_VER60 0x06000404
#define IMEID_CHS_VER41 0x04010804
#define IMEID_CHS_VER42 0x04020804
#define IMEID_CHS_VER53 0x05030804
extern "C" HIMC __stdcall ImmGetContext(HWND);
extern "C" int __stdcall ImmReleaseContext(HWND,HIMC);
extern "C" HWND __stdcall ImmGetDefaultIMEWnd(HWND);
extern "C" int __stdcall ImmGetConversionStatus(HIMC,DWORD*,DWORD*);
extern "C" int __stdcall ImmSetConversionStatus(HIMC,DWORD,DWORD);
extern "C" __declspec(dllimport) int __stdcall lstrlenW(const unsigned short*);
extern "C" __declspec(dllimport) unsigned short* __stdcall lstrcpyW(unsigned short*,const unsigned short*);
extern "C" __declspec(dllimport) unsigned short* __stdcall lstrcatW(unsigned short*,const unsigned short*);
extern "C" __declspec(dllimport) short __stdcall GetAsyncKeyState(int);
extern "C" __declspec(dllimport) HWND __stdcall GetFocus();
extern "C" __declspec(dllimport) int __stdcall PostMessageA(HWND,UINT,WPARAM,LPARAM);
extern "C" __declspec(dllimport) LRESULT __stdcall SendMessageA(HWND,UINT,WPARAM,LPARAM);
bool g_bInitialized;
static bool g_bCandList;
extern bool g_bReadingWindow,g_bHorizontalReading,g_bVerticalCand,g_bInsertMode,bIsSendingKeyMessage;
extern DWORD g_bDisableImeCompletely,g_dwIMELevel,g_dwIMELevelSaved,g_IMECursorBytes,g_IMECursorChars;
static DWORD g_dwCount,g_dwSelection;
static UINT g_uCandPageSize;
extern int g_iReadingError;
extern HWND g_hwndCurr;
extern HIMC g_himcOrg;
extern HKL g_hklCurrent;
extern TCHAR g_szCompositionString[256],g_szCandidate[10][256],g_szReadingString[32];
extern BYTE g_szCompAttrString[256];
extern LONG (__stdcall* _ImmGetCompositionString)(HIMC,DWORD,void*,DWORD);
extern DWORD (__fastcall* _GetCandidateList)(HIMC,DWORD,LPCANDIDATELIST*);
extern void (__fastcall* _SendCompString)();
extern LRESULT (__stdcall* _SendMessage)(HWND,UINT,WPARAM,LPARAM);
extern void (__fastcall* ImeUiCallback_Free)(void*);
void __fastcall CheckToggleState();
void __fastcall SetImeApi();
void __fastcall ImeUi_FinalizeString(bool);
void __fastcall SetCompStringColor();
DWORD __stdcall GetImeId(UINT uIndex=0);
void __stdcall GetReadingString(HWND hWnd);
#define _ImmGetContext ImmGetContext
#define _ImmReleaseContext ImmReleaseContext
#define _ImmGetConversionStatus ImmGetConversionStatus
#define _ImmSetConversionStatus ImmSetConversionStatus
#define GetCharCount(p) lstrlenW(p)
#define GetCharCountFromBytes(p,n) (n)
inline LRESULT SendKeyMsg(HWND hwnd,UINT msg,WPARAM wp)
{
 bIsSendingKeyMessage=true;
 LRESULT lRc=_SendMessage(hwnd,msg,wp,0);
 bIsSendingKeyMessage=false;
 return lRc;
}
#define SendKeyMsg_DOWN(hwnd,vk) SendKeyMsg(hwnd,WM_KEYDOWN,vk)
#define SendKeyMsg_UP(hwnd,vk) SendKeyMsg(hwnd,WM_KEYUP,vk)
static void SendControlKeys( UINT vk, UINT num )
{
    if( num == 0 )
        return;
    for( UINT i = 0; i < num; i++ )
    {
        SendKeyMsg_DOWN(g_hwndCurr, vk);
    }
    SendKeyMsg_UP(g_hwndCurr, vk);
}
static void CancelCompString( HWND hwnd, bool bUseBackSpace = true, int iNewStrLen = 0 )
{
    if( g_dwIMELevel != 3 )
        return;
    int cc = GetCharCount( g_szCompositionString );
    int i;
    
    SendControlKeys( VK_RIGHT, cc - g_IMECursorChars );

    if( bUseBackSpace || g_bInsertMode )
        iNewStrLen = 0;

    if( iNewStrLen < cc )
    {
        for( i = 0; i < cc - iNewStrLen; i++ )
        {
            SendKeyMsg_DOWN(hwnd, VK_BACK);
            SendKeyMsg( hwnd, WM_CHAR, 8 );	
        }
        SendKeyMsg_UP(hwnd, VK_BACK);
    }
    else
        iNewStrLen = cc;

    SendControlKeys( VK_LEFT, iNewStrLen );
}
#pragma auto_inline(off)
static void InitCompStringData()
{
    g_IMECursorBytes = 0;
    g_IMECursorChars = 0;
    memset( &g_szCompositionString, 0, sizeof( g_szCompositionString ) );
    memset( &g_szCompAttrString, 0, sizeof( g_szCompAttrString ) );
}
#pragma auto_inline(on)

static void ComposeCandidateLine(TCHAR* pszBuffer,int index,LPCTSTR pszCandidate)
{
 TCHAR* psz=pszBuffer;
 *psz++=(TCHAR)('0'+index%10);
 if(g_bVerticalCand) *psz++=' ';
 while(*pszCandidate) *psz++=*pszCandidate++;
 if(!g_bVerticalCand) *psz++=' ';
 *psz=0;
}
static void SetSupportLevel(DWORD dwImeLevel)
{
 if(dwImeLevel<2 || 3<dwImeLevel) return;
 if(GETPRIMLANG()==LANG_KOREAN) dwImeLevel=3;
 g_dwIMELevel=dwImeLevel;
 ImeUi_FinalizeString(false);
 SetCompStringColor();
}

LPARAM __fastcall ImeUi_ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* trapped )
{
    HIMC himc;
    int len;
    static LPARAM lAlt = 0x80000000, lCtrl = 0x80000000, lShift = 0x80000000;

    const bool bInitialized = g_bInitialized;
    *trapped = false;
    if( !bInitialized || g_bDisableImeCompletely )
    {
        return 0;
    }

    switch( uMsg )
    {

        case WM_INPUTLANGCHANGE:
            {
 UINT uLang=GETPRIMLANG();
 CheckToggleState();
 SetImeApi();
 if(uLang!=GETPRIMLANG()) SetSupportLevel(GETPRIMLANG()==LANG_KOREAN?3:g_dwIMELevelSaved);
 HWND hwndImeDef=ImmGetDefaultIMEWnd(g_hwndCurr);
 if(hwndImeDef) {
  SendMessageA(hwndImeDef,WM_IME_CONTROL,IMC_OPENSTATUSWINDOW,0);
  SendMessageA(hwndImeDef,WM_IME_CONTROL,IMC_CLOSESTATUSWINDOW,0);
 }
}
            break;

        case WM_IME_SETCONTEXT:
            lParam = 0;
            break;

        case WM_IME_STARTCOMPOSITION:
            InitCompStringData();
            *trapped = true;
            break;

        case WM_IME_COMPOSITION:
        {
            LONG lRet;
            TCHAR szCompStr[COUNTOF(g_szCompositionString)];

            *trapped = true;
            himc = ImmGetContext( hWnd );
            if( !himc )
            {
                break;
            }

            if( lParam & GCS_RESULTSTR )
            {
                lRet = ( LONG )_ImmGetCompositionString( himc, GCS_RESULTSTR, szCompStr,
                                                         COUNTOF( szCompStr ) ) / sizeof( TCHAR );
                szCompStr[lRet] = 0;
                CancelCompString( g_hwndCurr, false, GetCharCount( szCompStr ) );
                lstrcpyW(g_szCompositionString,szCompStr);
                _SendCompString();
                InitCompStringData();
            }

            if( lParam & GCS_COMPSTR )
            {

                lRet = ( LONG )_ImmGetCompositionString( himc, GCS_COMPSTR, szCompStr,
                                                         COUNTOF( szCompStr ) ) / sizeof( TCHAR );
                szCompStr[lRet] = 0;

                CancelCompString( g_hwndCurr, false, GetCharCount( szCompStr ) );

                lstrcpyW(g_szCompositionString,szCompStr);
                lRet = _ImmGetCompositionString( himc, GCS_COMPATTR, g_szCompAttrString,
                                                 COUNTOF( g_szCompAttrString ) );
                g_szCompAttrString[lRet] = 0;
                
                if( GETLANG() == LANG_CHT && !GetImeId() )
                {
                    int i, chars = (int)lstrlenW( g_szCompositionString ) / ( 3 - sizeof( TCHAR ) );
                    if( chars )
                    {
                        g_dwCount = 4;
                        g_dwSelection = ( DWORD )-1;	

                        for( i = g_dwCount - 1; i >= 0; i-- )
                        {
                            if( i > chars - 1 )
                                g_szCandidate[i][0] = 0;
                            else
                            {
                                g_szCandidate[i][0] = g_szCompositionString[i];
                                g_szCandidate[i][1] = 0;
                            }
                        }
                        g_uCandPageSize = MAX_CANDLIST;
                        memset( g_szCompositionString, 0, 8 );
                        g_bReadingWindow = true;
                        g_bHorizontalReading=true;
                        if( g_bHorizontalReading )
                        {
                            g_iReadingError = -1;
                            g_szReadingString[0] = 0;
                            for( i = 0; i < g_dwCount; i++ )
                            {
                                if( g_dwSelection == ( DWORD )i )
                                    g_iReadingError = (int)lstrlenW( g_szReadingString );
                                LPCTSTR pszTmp = g_szCandidate[i];
                                lstrcatW(g_szReadingString,pszTmp);
                            }
                        }
                    }
                    else
                        g_dwCount = 0;
                }

                g_IMECursorBytes = _ImmGetCompositionString( himc, GCS_CURSORPOS, 0, 0 );
                g_IMECursorChars = GetCharCountFromBytes( g_szCompositionString, g_IMECursorBytes );

                if( g_dwIMELevel == 3 )
                {
                    
                    _SendCompString();
                    
                    len = GetCharCount( g_szCompositionString + g_IMECursorBytes );
                    SendControlKeys( VK_LEFT, len );
                }
            }
            _ImmReleaseContext( hWnd, himc );
        }
            break;

        case WM_IME_ENDCOMPOSITION:
            CancelCompString( g_hwndCurr );
            InitCompStringData();
            break;

        case WM_IME_NOTIFY:
            switch( wParam )
            {
                case IMN_SETCONVERSIONMODE:
                {
                    
                    static bool bNoReentrance = false;
                    if( LANG_CHT == GETLANG() && !bNoReentrance )
                    {
                        bNoReentrance = true;
                        DWORD dwConvMode, dwSentMode;
                        _ImmGetConversionStatus( g_himcOrg, &dwConvMode, &dwSentMode );
                        const DWORD dwFlag = IME_CMODE_SOFTKBD | IME_CMODE_SYMBOL;
                        if( dwConvMode & dwFlag )
                            _ImmSetConversionStatus( g_himcOrg, dwConvMode & ~dwFlag, dwSentMode );
                    }
                    bNoReentrance = false;
                }

                case IMN_SETOPENSTATUS:
                    CheckToggleState();
                    break;

                case IMN_OPENCANDIDATE:
                case IMN_CHANGECANDIDATE:
                    {
                        g_bCandList = true;
                        *trapped = true;
                        himc = _ImmGetContext( hWnd );
                        if( !himc )
                            break;

                        LPCANDIDATELIST lpCandList;
                        DWORD dwIndex, dwBufLen;

                        g_bReadingWindow = false;
                        dwIndex = 0;
                        dwBufLen = _GetCandidateList( himc, dwIndex, &lpCandList );

                        if( dwBufLen )
                        {
                            g_dwSelection = lpCandList->dwSelection;
                            g_dwCount = lpCandList->dwCount;

                            int startOfPage = 0;
                            if( GETLANG() == LANG_CHS && GetImeId() )
                            {

                                const int maxCandChar = 18 * ( 3 - sizeof( TCHAR ) );
                                UINT cChars = 0;
                                UINT i;
                                for( i = 0; i < g_dwCount; i++ )
                                {
                                    UINT uLen = (int)lstrlenW(
                                        ( LPTSTR )( (DWORD)lpCandList + lpCandList->dwOffset[i] ) ) +
                                        ( 3 - sizeof( TCHAR ) );
                                    if( uLen + cChars > maxCandChar )
                                    {
                                        if( i > g_dwSelection )
                                        {
                                            break;
                                        }
                                        startOfPage = i;
                                        cChars = uLen;
                                    }
                                    else
                                    {
                                        cChars += uLen;
                                    }
                                }
                                g_uCandPageSize = i - startOfPage;
                            }
                            else
                            {
                                g_uCandPageSize = (lpCandList->dwPageSize < MAX_CANDLIST ? lpCandList->dwPageSize : MAX_CANDLIST);
                                startOfPage = 
                                    ( g_dwSelection / g_uCandPageSize ) * g_uCandPageSize;
                            }

                            g_dwSelection = ( GETLANG() == LANG_CHS && !GetImeId() ) ? ( DWORD )-1
                                : g_dwSelection - startOfPage;

                            memset( &g_szCandidate, 0, sizeof( g_szCandidate ) );
                            for( UINT i = startOfPage, j = 0;
                                 ( DWORD )i < lpCandList->dwCount && j < g_uCandPageSize;
                                 i++, j++ )
                            {
                                ComposeCandidateLine( g_szCandidate[j], j+1,
                                                      ( LPTSTR )( (DWORD)lpCandList + lpCandList->dwOffset[i] ) );
                            }
                            ImeUiCallback_Free( ( HANDLE )lpCandList );
                            _ImmReleaseContext( hWnd, himc );

                            if( GETPRIMLANG() == LANG_KOREAN ||
                                (GETLANG() == LANG_CHT && !GetImeId()) )
                                g_dwSelection = ( DWORD )-1;
                        }
                        break;
                    }

                case IMN_CLOSECANDIDATE:
                    g_bCandList=false;
                    if(!g_bReadingWindow) {g_dwCount=0; memset(&g_szCandidate,0,sizeof(g_szCandidate));}
                    *trapped = true;
                    break;

                case IMN_PRIVATE:
                {
                    if( !g_bCandList )
                    {
                        GetReadingString( hWnd );
                    }
                    
                    DWORD dwId = GetImeId();
                    switch( dwId )
                    {
                        case IMEID_CHT_VER42:
                        case IMEID_CHT_VER43:
                        case IMEID_CHT_VER44:
                        case IMEID_CHS_VER41:
                        case IMEID_CHS_VER42:
                            if( ( lParam == 1 ) || ( lParam == 2 ) )
                            {
                                *trapped = true;
                            }
                            break;
                        case IMEID_CHT_VER50:
                        case IMEID_CHT_VER51:
                        case IMEID_CHT_VER52:
                        case IMEID_CHT_VER60:
                        case IMEID_CHS_VER53:
                            if( ( lParam == 16 ) || ( lParam == 17 ) || ( lParam == 26 ) || ( lParam == 27 ) ||
                                ( lParam == 28 ) )
                            {
                                *trapped = true;
                            }
                            break;
                    }
                }
                    break;

                default:
                    *trapped = true;
                    break;
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            if( !( lAlt & 0x80000000 ) && wParam != VK_MENU && ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) == 0 )
            {
                PostMessageA( GetFocus(), WM_KEYUP, ( WPARAM )VK_MENU, ( lAlt & 0x01ff0000 ) | 0xC0000001 );
            }
            else if( !( lCtrl & 0x80000000 ) && wParam != VK_CONTROL &&
                     ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) == 0 )
            {
                PostMessageA( GetFocus(), WM_KEYUP, ( WPARAM )VK_CONTROL, ( lCtrl & 0x01ff0000 ) | 0xC0000001 );
            }
            else if( !( lShift & 0x80000000 ) && wParam != VK_SHIFT && ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) == 0 )
            {
                PostMessageA( GetFocus(), WM_KEYUP, ( WPARAM )VK_SHIFT, ( lShift & 0x01ff0000 ) | 0xC0000001 );
            }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            {
                switch( wParam )
                {
                    case VK_MENU:
                        lAlt = lParam;
                        break;
                    case VK_SHIFT:
                        lShift = lParam;
                        break;
                    case VK_CONTROL:
                        lCtrl = lParam;
                        break;
                }
            }
            break;
    }
    return 0;
}

struct tagSIZE {long cx,cy;}; typedef tagSIZE SIZE;
struct _SkinCompStr {DWORD colorInput,colorTargetConv,colorConverted,colorTargetNotConv,colorInputErr;};
extern _SkinCompStr gSkinCompStr;
extern IMEUI_APPEARANCE gSkinIME;
extern CompStringAttribute g_CaretInfo;
extern bool g_bInitialized,g_bImeEnabled,g_bCaretDraw,g_bReadingWindow,g_bHorizontalReading,g_bVerticalCand;
extern DWORD g_dwIMELevel,g_IMECursorBytes,g_IMECursorChars,g_dwCount,g_dwSelection;
extern UINT g_uCaretBlinkLast,g_uCaretBlinkTime,g_screenWidth,g_screenHeight,g_uCandPageSize;
extern int g_iReadingError;
extern HKL g_hklCurrent;
extern TCHAR g_szCompositionString[256],g_szCandidate[10][256],g_szReadingString[32],g_szMultiLineCompString[256];
extern BYTE g_szCompAttrString[256];
extern void (__fastcall* ImeUiCallback_DrawRect)(int,int,int,int,DWORD);
extern "C" __declspec(dllimport) int __stdcall lstrlenW(const unsigned short*);
extern "C" __declspec(dllimport) unsigned short* __stdcall lstrcpyW(unsigned short*,const unsigned short*);
extern "C" __declspec(dllimport) DWORD __stdcall GetTickCount();
void __fastcall DrawImeIndicator();
#define COUNTOF(a) (sizeof(a)/sizeof(a[0]))
#define IMEID_LANG(x) ((x)&0xffff)
#define LANG_CHS 0x804
#define ATTR_INPUT 0
#define ATTR_TARGET_CONVERTED 1
#define ATTR_CONVERTED 2
#define ATTR_TARGET_NOTCONVERTED 3
#define ATTR_INPUT_ERROR 4
#define MAX_CANDLIST 10
#define POSITION_UNINITIALIZED 0
#define TEXT(x) L##x
static void DrawCaret(DWORD x,DWORD y,DWORD height)
{
 if(g_bCaretDraw) ImeUiCallback_DrawRect(x,y+gSkinIME.caretYMargin,x+gSkinIME.caretWidth,
 y+height-gSkinIME.caretYMargin,g_CaretInfo.colorComp);
}
void __fastcall ImeUi_RenderUI(bool bDrawCompAttr,bool bDrawOtherUi)
{
 if(!g_bInitialized || !g_bImeEnabled || !g_CaretInfo.pFont) return;
 if(!bDrawCompAttr && !bDrawOtherUi) return;
 if(g_dwIMELevel==2) {if(!bDrawOtherUi) return;}
 if(bDrawOtherUi) DrawImeIndicator();

    UINT uCurrentTime = GetTickCount();
    if( uCurrentTime - g_uCaretBlinkLast > g_uCaretBlinkTime )
    {
        g_uCaretBlinkLast = uCurrentTime;
        g_bCaretDraw = !g_bCaretDraw;
    }

    int i = 0;

    g_CaretInfo.pFont->SetColor( g_CaretInfo.colorComp );

    DWORD uDummy;

    int len = (int)lstrlenW( g_szCompositionString );

    DWORD bgX = g_CaretInfo.caretX;
    DWORD bgY = g_CaretInfo.caretY;
    DWORD candX = POSITION_UNINITIALIZED;
    DWORD candY = 0;
    LPTSTR pszMlcs = g_szMultiLineCompString;

    DWORD wCompChar = 0;
    DWORD hCompChar;
    g_CaretInfo.pFont->GetTextExtent( TEXT( " " ), &uDummy, &hCompChar );
    if( g_dwIMELevel == 3 && g_IMECursorBytes && g_szCompositionString[0] )
    {
        
        TCHAR temp = g_szCompositionString[g_IMECursorBytes];
        g_szCompositionString[g_IMECursorBytes] = 0;
        g_CaretInfo.pFont->GetTextExtent( g_szCompositionString, &wCompChar, &hCompChar );
        g_szCompositionString[g_IMECursorBytes] = temp;
        bgX -= wCompChar;
    }

    bool saveCandPos = false;
    DWORD cType = 1;
    LPTSTR pszCurrentCompLine = g_szCompositionString;
    DWORD dwCompLineStart = bgX;
    DWORD bgXnext = bgX;

    if( GETPRIMLANG() != LANG_KOREAN || g_bCaretDraw )	
        for( i = 0; i < len; i += cType )
        {
            DWORD bgColor = 0x00000000;
            TCHAR szChar[3];
            szChar[0] = g_szCompositionString[i];
            szChar[1] = szChar[2] = 0;
            bgX = bgXnext;
            TCHAR cSave = g_szCompositionString[i + cType];
            g_szCompositionString[i + cType] = 0;
            g_CaretInfo.pFont->GetTextExtent( pszCurrentCompLine, &bgXnext, &hCompChar );
            g_szCompositionString[i + cType] = cSave;
            bgXnext += dwCompLineStart;
            wCompChar = bgXnext - bgX;

            switch( g_szCompAttrString[i] )
            {
                case ATTR_INPUT:
                    bgColor = gSkinCompStr.colorInput;
                    break;
                case ATTR_TARGET_CONVERTED:
                    bgColor = gSkinCompStr.colorTargetConv;
                    if( IMEID_LANG( GetImeId() ) != LANG_CHS )
                        saveCandPos = true;
                    break;
                case ATTR_CONVERTED:
                    bgColor = gSkinCompStr.colorConverted;
                    break;
                case ATTR_TARGET_NOTCONVERTED:

                    bgColor = gSkinCompStr.colorTargetNotConv;
                    break;
                case ATTR_INPUT_ERROR:
                    bgColor = gSkinCompStr.colorInputErr;
                    break;
                default:
                    
                    break;
            }

            if( g_dwIMELevel == 3 )
            {
                if( bDrawCompAttr && ( LONG )bgX >= g_CaretInfo.margins.left && ( LONG )bgX <= g_CaretInfo.margins.right )
                {
                    ImeUiCallback_DrawRect(bgX,bgY,bgX+wCompChar,bgY+hCompChar,bgColor);
                }
            }
            else if( g_dwIMELevel == 2 )
            {

                bool bWrite = (pszMlcs - g_szMultiLineCompString <
                    (unsigned int)(COUNTOF(g_szMultiLineCompString) - 5 * (3 - sizeof(TCHAR))));

                if( ( LONG )( bgX + wCompChar ) >= g_CaretInfo.margins.right )
                {
                    bgX = dwCompLineStart = bgXnext = g_CaretInfo.margins.left;
                    bgY = bgY + hCompChar;
                    pszCurrentCompLine = g_szCompositionString + i;
                    if( bWrite )
                    {
                        if( pszMlcs == g_szMultiLineCompString || pszMlcs[-1] == 0 ) 
                            *pszMlcs++ = ' ';	
                        *pszMlcs++ = 0;
                    }
                }
                    ImeUiCallback_DrawRect( bgX, bgY, bgX + wCompChar, bgY + hCompChar, bgColor );
                if( bWrite )
                {
                    *pszMlcs++ = g_szCompositionString[i];
                }
                if( ( DWORD )i == g_IMECursorBytes )
                {
                    DrawCaret(bgX,bgY,hCompChar);
                }
            }
            if( ( saveCandPos && candX == POSITION_UNINITIALIZED ) ||
                ( IMEID_LANG( GetImeId() ) == LANG_CHS && (int)(i / ( 3 - sizeof( TCHAR ) )) == ( int )g_IMECursorChars ) )
            {
                candX = bgX;
                candY = bgY;
            }
            saveCandPos = false;
        }

    bgX = bgXnext;
    if( g_dwIMELevel == 2 )
    {
        
        if( len != 0 && ( DWORD )i == g_IMECursorBytes )
        {
            DrawCaret(bgX,bgY,hCompChar);
        }

        *pszMlcs++ = 0;
        *pszMlcs++ = 0;
        DWORD x, y;
        x = g_CaretInfo.caretX;
        y = g_CaretInfo.caretY;
        pszMlcs = g_szMultiLineCompString;
        while (*pszMlcs &&
            pszMlcs - g_szMultiLineCompString < (unsigned int)(COUNTOF(g_szMultiLineCompString)))
        {
            g_CaretInfo.pFont->SetPosition(x, y);
            g_CaretInfo.pFont->DrawText(pszMlcs);
            pszMlcs += lstrlenW(pszMlcs) + 1;
            x = g_CaretInfo.margins.left;
            y += hCompChar;
        }
    }

 if(!bDrawOtherUi) return;
 {
    
    if( !g_dwCount || g_szCandidate[0][0] == 0 )
    {
        return;
    }

    if(candX==0) {candX=g_CaretInfo.caretX;candY=g_CaretInfo.caretY;}

    SIZE largest;
    largest.cx = largest.cy = 0;

    DWORD uDigitWidth;
    DWORD uSpaceWidth;
    g_CaretInfo.pFont->GetTextExtent(TEXT("0"),&uDigitWidth,(DWORD*)&largest.cy);
    g_CaretInfo.pFont->GetTextExtent(TEXT(" "),&uSpaceWidth,(DWORD*)&largest.cy);
    DWORD dwMarginX = ( uSpaceWidth + 1 ) / 2;
    DWORD adwCandWidth[ MAX_CANDLIST ];

    DWORD dwCandWidth = 0;
    if( g_bReadingWindow && g_bHorizontalReading )
        g_CaretInfo.pFont->GetTextExtent( g_szReadingString, ( DWORD* )&largest.cx, ( DWORD* )&largest.cy );
    else
    {
        for( int i = 0; g_szCandidate[i][0] && i < g_uCandPageSize; i++ )
        {
            DWORD tx,ty;
            g_CaretInfo.pFont->GetTextExtent(g_szCandidate[i],&tx,&ty);

            if( ( signed )tx > largest.cx )
                largest.cx = tx;
            if( ( signed )ty > largest.cy )
                largest.cy = ty;
            adwCandWidth[ i ] = tx;
            dwCandWidth += tx;
        }
    }

    DWORD slotsUsed;
    if( g_bReadingWindow && g_dwCount < g_uCandPageSize )
        slotsUsed = g_dwCount;
    else
        slotsUsed = g_uCandPageSize;

    DWORD dwCandHeight;
    if( g_bVerticalCand && !( g_bReadingWindow && g_bHorizontalReading ) )
        dwCandHeight = slotsUsed * largest.cy + 2;
    else
        dwCandHeight = largest.cy + 2;
    if( candY + hCompChar + dwCandHeight > g_screenHeight )
        candY -= dwCandHeight;
    else
        candY += hCompChar;
    if( ( int )candY < 0 )
        candY = 0;

    if( !g_bReadingWindow && IMEID_LANG( GetImeId() ) == LANG_CHS )
        dwCandWidth += dwMarginX * ( slotsUsed );
    else if( g_bReadingWindow && g_bHorizontalReading )
        dwCandWidth = largest.cx + 2 + dwMarginX * 2;
    else if( g_bVerticalCand || g_bReadingWindow )
        dwCandWidth = largest.cx + 2 + dwMarginX * 2;
    else
        dwCandWidth = slotsUsed * ( largest.cx + 1 ) + 1;
    if( candX + dwCandWidth > g_screenWidth )
        candX = g_screenWidth - dwCandWidth;
    if( ( int )candX < 0 )
        candX = 0;

    int saveRight;
    int left = candX;
    int top = candY;
    int right = candX + dwCandWidth;
    int bottom = candY + dwCandHeight;
    ImeUiCallback_DrawRect( left, top, right, bottom, gSkinIME.candColorBorder );
    left++;
    top++;
    right--;
    bottom--;
    if( g_bReadingWindow || IMEID_LANG( GetImeId() ) == LANG_CHS )
    {
        ImeUiCallback_DrawRect( left, top, right, bottom, gSkinIME.candColorBase );
    }
    else if( g_bVerticalCand )
    {
        saveRight=right;
        if( !g_bReadingWindow )
        {
            right=left+dwMarginX+uDigitWidth+uSpaceWidth/2-1;
            ImeUiCallback_DrawRect(left,top,right,bottom,gSkinIME.candColorBase);
            left=right+1;
            right=saveRight;
            ImeUiCallback_DrawRect(left,top,right,bottom,gSkinIME.candColorBase);
        }
    }
    else
    {
        for( i = 0; ( DWORD )i < slotsUsed; i++ )
        {
            ImeUiCallback_DrawRect( left, top, left + largest.cx, bottom, gSkinIME.candColorBase );
            left += largest.cx + 1;
        }
    }

    candX++;
    candY++;
    if( g_bReadingWindow && g_bHorizontalReading )
    {
        int iStart = -1, iEnd = -1, iDummy;
        candX += dwMarginX;

        TCHAR szTemp[COUNTOF( g_szReadingString ) ];
        if( g_iReadingError >= 0 )
        {
            lstrcpyW(szTemp,g_szReadingString);
            LPTSTR psz = szTemp + g_iReadingError;
            psz++;
            *psz = 0;
            g_CaretInfo.pFont->GetTextExtent( szTemp, ( DWORD* )&iEnd, ( DWORD* )&iDummy );
            TCHAR cSave = szTemp[ g_iReadingError ];
            szTemp[g_iReadingError] = 0;
            g_CaretInfo.pFont->GetTextExtent( szTemp, ( DWORD* )&iStart, ( DWORD* )&iDummy );
            szTemp[g_iReadingError] = cSave;
            ImeUiCallback_DrawRect( candX + iStart, candY, candX + iEnd, candY + largest.cy,
                                        gSkinIME.candColorBorder );
        }

        g_CaretInfo.pFont->SetPosition( candX, candY );
        g_CaretInfo.pFont->SetColor( g_CaretInfo.colorCand );
        g_CaretInfo.pFont->DrawText( g_szReadingString );

        if( iStart >= 0 )
        {
            g_CaretInfo.pFont->SetPosition( candX + iStart, candY );
                g_CaretInfo.pFont->SetColor( 0xff000000 + ( ~( ( 0x00ffffff ) & g_CaretInfo.colorCand ) ) );
            g_CaretInfo.pFont->DrawText( szTemp + g_iReadingError );
        }
    }
    else
    {
        for( i = 0; i < ( int )g_uCandPageSize && ( DWORD )i < g_dwCount; i++ )
        {
            if( g_dwSelection == ( DWORD )i )
            {
                    g_CaretInfo.pFont->SetColor( 0xff000000 + ( ~( ( 0x00ffffff ) & g_CaretInfo.colorCand ) ) );

                {
                    if( g_bReadingWindow || g_bVerticalCand )
                        ImeUiCallback_DrawRect( candX, candY + i * largest.cy,
                                                candX - 1 + dwCandWidth, candY + ( i + 1 ) * largest.cy,
                                                gSkinIME.candColorBorder );
                    else
                        ImeUiCallback_DrawRect( candX, candY,
                                                candX + adwCandWidth[i], candY + largest.cy,
                                                gSkinIME.candColorBorder );
                }
            }
            else
                g_CaretInfo.pFont->SetColor( g_CaretInfo.colorCand );
            if( g_szCandidate[i][0] != 0 )
            {
                if(g_bReadingWindow || g_bVerticalCand)
                    g_CaretInfo.pFont->SetPosition(dwMarginX+candX,candY+i*largest.cy);
                else
                    g_CaretInfo.pFont->SetPosition(uSpaceWidth/2+candX,candY);
                g_CaretInfo.pFont->DrawText(g_szCandidate[i]);
            }
            if( !g_bReadingWindow && !g_bVerticalCand )
            {
                if( IMEID_LANG( GetImeId() ) == LANG_CHS )
                    candX += adwCandWidth[i] + dwMarginX;
                else
                    candX += largest.cx + 1;
            }
        }
    }

 }
}

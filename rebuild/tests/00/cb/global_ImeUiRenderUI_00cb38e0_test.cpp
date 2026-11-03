#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#endif
static const unsigned short* FixtureText(const char* s){static const unsigned short space[]={32,0},zero[]={48,0};assert(s[0]==32||s[0]==48);return s[0]==32?space:zero;}
#ifndef FABLE_RETAIL_IMEUI_SDK_H
#define FABLE_RETAIL_IMEUI_SDK_H
// Layout: original ImeUi.h and Ego_r IMEUI_APPEARANCE64 / IMEUI_VERTEX32.
typedef unsigned int DWORD;
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef const unsigned short* LPCTSTR;
struct tagRECT { int left,top,right,bottom; };
typedef tagRECT RECT;
struct HKL__;
typedef HKL__* HKL;
class CImeUiFont_Base
{
public:
    virtual void    SetHeight( UINT uHeight )
    {
        (void)uHeight;
    }; // for backward compatibility
    virtual void    SetColor( DWORD color ) = 0;
    virtual void    SetPosition( int x, int y ) = 0;
    virtual void    GetTextExtent( LPCTSTR szText, DWORD* puWidth, DWORD* puHeight ) = 0;
    virtual void    DrawText( LPCTSTR pszText ) = 0;
};

typedef struct
{
    // symbol (Henkan-kyu)
    DWORD symbolColor;
    DWORD symbolColorOff;
    DWORD symbolColorText;
    BYTE symbolHeight;
    BYTE symbolTranslucence;
    BYTE symbolPlacement;
    CImeUiFont_Base* symbolFont;

    // candidate list
    DWORD candColorBase;
    DWORD candColorBorder;
    DWORD candColorText;

    // composition string
    DWORD compColorInput;
    DWORD compColorTargetConv;
    DWORD compColorConverted;
    DWORD compColorTargetNotConv;
    DWORD compColorInputErr;
    BYTE compTranslucence;
    DWORD compColorText;

    // caret
    BYTE caretWidth;
    BYTE caretYMargin;
}               IMEUI_APPEARANCE;

typedef struct	// D3DTLVERTEX compatible
{
    float sx;
    float sy;
    float sz;
    float rhw;
    DWORD color;
    DWORD specular;
    float tu;
    float tv;
}               IMEUI_VERTEX;


// Original ImeUi.cpp private layout, native g_CaretInfo +0x14 is margins.
struct CompStringAttribute {
 UINT caretX,caretY;CImeUiFont_Base* pFont;DWORD colorComp,colorCand;RECT margins;
};
#define IMEUI_STATE_OFF 0
#define IMEUI_STATE_ON 1
#define LANG_KOREAN 0x12
#endif

// 00cb38e0 ImeUi_RenderUI
// Leaf: ?ImeUi_RenderUI@@YIX_N0@Z
// Flags: /Gr /GS
// Original Microsoft DirectX SDK ImeUi.cpp; Copyright Microsoft Corporation, MIT license.
// ImeUi_ProcessMessage(HWND,UINT,WPARAM,LPARAM,bool*) -- process IMM composition and candidate events
struct HWND__; typedef HWND__* HWND;
struct HIMC__; typedef HIMC__* HIMC;
typedef int LONG; typedef int LPARAM; typedef int LRESULT;
typedef void* HANDLE; typedef UINT WPARAM; typedef unsigned short TCHAR; typedef TCHAR* LPTSTR;
struct tagCANDIDATELIST { DWORD dwSize,dwStyle,dwCount,dwSelection,dwPageStart,dwPageSize,dwOffset[1]; };
typedef tagCANDIDATELIST* LPCANDIDATELIST;
#define COUNTOF(a) (sizeof(a)/sizeof(a[0]))
#define GETLANG() ((unsigned short)((size_t)g_hklCurrent & 0xffff))
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
extern "C" HIMC __stdcall FixtureImmGetContext(HWND);
extern "C" int __stdcall FixtureImmReleaseContext(HWND,HIMC);
extern "C" HWND __stdcall FixtureImmGetDefaultIMEWnd(HWND);
extern "C" int __stdcall FixtureImmGetConversionStatus(HIMC,DWORD*,DWORD*);
extern "C" int __stdcall FixtureImmSetConversionStatus(HIMC,DWORD,DWORD);
extern "C"  int __stdcall FixturelstrlenW(const unsigned short*);
extern "C"  unsigned short* __stdcall FixturelstrcpyW(unsigned short*,const unsigned short*);
extern "C"  unsigned short* __stdcall FixturelstrcatW(unsigned short*,const unsigned short*);
extern "C"  short __stdcall FixtureGetAsyncKeyState(int);
extern "C"  HWND __stdcall FixtureGetFocus();
extern "C"  int __stdcall FixturePostMessageA(HWND,UINT,WPARAM,LPARAM);
extern "C"  LRESULT __stdcall FixtureSendMessageA(HWND,UINT,WPARAM,LPARAM);
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
#define _ImmGetContext FixtureImmGetContext
#define _ImmReleaseContext FixtureImmReleaseContext
#define _ImmGetConversionStatus FixtureImmGetConversionStatus
#define _ImmSetConversionStatus FixtureImmSetConversionStatus
#define GetCharCount(p) FixturelstrlenW(p)
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
 HWND hwndImeDef=FixtureImmGetDefaultIMEWnd(g_hwndCurr);
 if(hwndImeDef) {
  FixtureSendMessageA(hwndImeDef,WM_IME_CONTROL,IMC_OPENSTATUSWINDOW,0);
  FixtureSendMessageA(hwndImeDef,WM_IME_CONTROL,IMC_CLOSESTATUSWINDOW,0);
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
            himc = FixtureImmGetContext( hWnd );
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
                FixturelstrcpyW(g_szCompositionString,szCompStr);
                _SendCompString();
                InitCompStringData();
            }

            if( lParam & GCS_COMPSTR )
            {

                lRet = ( LONG )_ImmGetCompositionString( himc, GCS_COMPSTR, szCompStr,
                                                         COUNTOF( szCompStr ) ) / sizeof( TCHAR );
                szCompStr[lRet] = 0;

                CancelCompString( g_hwndCurr, false, GetCharCount( szCompStr ) );

                FixturelstrcpyW(g_szCompositionString,szCompStr);
                lRet = _ImmGetCompositionString( himc, GCS_COMPATTR, g_szCompAttrString,
                                                 COUNTOF( g_szCompAttrString ) );
                g_szCompAttrString[lRet] = 0;
                
                if( GETLANG() == LANG_CHT && !GetImeId() )
                {
                    int i, chars = (int)FixturelstrlenW( g_szCompositionString ) / ( 3 - sizeof( TCHAR ) );
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
                                    g_iReadingError = (int)FixturelstrlenW( g_szReadingString );
                                LPCTSTR pszTmp = g_szCandidate[i];
                                FixturelstrcatW(g_szReadingString,pszTmp);
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
                                    UINT uLen = (int)FixturelstrlenW(
                                        ( LPTSTR )( (size_t)lpCandList + lpCandList->dwOffset[i] ) ) +
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
                                                      ( LPTSTR )( (size_t)lpCandList + lpCandList->dwOffset[i] ) );
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
            if( !( lAlt & 0x80000000 ) && wParam != VK_MENU && ( FixtureGetAsyncKeyState( VK_MENU ) & 0x8000 ) == 0 )
            {
                FixturePostMessageA( FixtureGetFocus(), WM_KEYUP, ( WPARAM )VK_MENU, ( lAlt & 0x01ff0000 ) | 0xC0000001 );
            }
            else if( !( lCtrl & 0x80000000 ) && wParam != VK_CONTROL &&
                     ( FixtureGetAsyncKeyState( VK_CONTROL ) & 0x8000 ) == 0 )
            {
                FixturePostMessageA( FixtureGetFocus(), WM_KEYUP, ( WPARAM )VK_CONTROL, ( lCtrl & 0x01ff0000 ) | 0xC0000001 );
            }
            else if( !( lShift & 0x80000000 ) && wParam != VK_SHIFT && ( FixtureGetAsyncKeyState( VK_SHIFT ) & 0x8000 ) == 0 )
            {
                FixturePostMessageA( FixtureGetFocus(), WM_KEYUP, ( WPARAM )VK_SHIFT, ( lShift & 0x01ff0000 ) | 0xC0000001 );
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

struct tagSIZE {int cx,cy;}; typedef tagSIZE SIZE;
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
extern "C"  int __stdcall FixturelstrlenW(const unsigned short*);
extern "C"  unsigned short* __stdcall FixturelstrcpyW(unsigned short*,const unsigned short*);
extern "C"  DWORD __stdcall FixtureGetTickCount();
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
#define TEXT(x) FixtureText(x)
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

    UINT uCurrentTime = FixtureGetTickCount();
    if( uCurrentTime - g_uCaretBlinkLast > g_uCaretBlinkTime )
    {
        g_uCaretBlinkLast = uCurrentTime;
        g_bCaretDraw = !g_bCaretDraw;
    }

    int i = 0;

    g_CaretInfo.pFont->SetColor( g_CaretInfo.colorComp );

    DWORD uDummy;

    int len = (int)FixturelstrlenW( g_szCompositionString );

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
            pszMlcs += FixturelstrlenW(pszMlcs) + 1;
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
            FixturelstrcpyW(szTemp,g_szReadingString);
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


bool g_bReadingWindow,g_bHorizontalReading,g_bVerticalCand,g_bInsertMode,bIsSendingKeyMessage;
DWORD g_bDisableImeCompletely,g_dwIMELevel,g_dwIMELevelSaved,g_IMECursorBytes,g_IMECursorChars;
int g_iReadingError;HWND g_hwndCurr;HIMC g_himcOrg;HKL g_hklCurrent;
TCHAR g_szCompositionString[256],g_szCandidate[10][256],g_szReadingString[32];BYTE g_szCompAttrString[256];
static HWND window=(HWND)(size_t)0x100,imeWindow=(HWND)(size_t)0x200;
static HIMC context=(HIMC)(size_t)0x300;
static bool noContext,noCandidate,showImeWindow,keysDown[256];
static unsigned getContextCalls,releaseCalls,toggleCalls,setApiCalls,finalizeCalls,colorCalls,readingCalls,freeCalls,sendCompCalls,convertCalls,setConvertCalls,postedCalls,sentCalls,sysSendCalls,compositionCalls;
static DWORD idValue,newLanguage,conversion,sentence,conversionSet,sentenceSet;
static unsigned short compText[256],resultText[256],sentComp[256];static BYTE compAttr[256];static unsigned cursor;
struct Event {HWND hwnd;UINT message;WPARAM wp;LPARAM lp;};static Event sent[100],posted[100],sysSent[100];
struct CandidateBlock {DWORD size,style,count,selection,pageStart,pageSize,offset[12];TCHAR text[12][32];};static CandidateBlock candidate;
extern "C" int __stdcall FixturelstrlenW(const unsigned short* p){int n=0;while(*p++)++n;return n;}
extern "C" unsigned short* __stdcall FixturelstrcpyW(unsigned short* p,const unsigned short* q){unsigned short* start=p;while((*p++=*q++)!=0){}return start;}
extern "C" unsigned short* __stdcall FixturelstrcatW(unsigned short* p,const unsigned short* q){FixturelstrcpyW(p+FixturelstrlenW(p),q);return p;}
extern "C" HIMC __stdcall FixtureImmGetContext(HWND h){assert(h==window);++getContextCalls;return noContext?0:context;}
extern "C" int __stdcall FixtureImmReleaseContext(HWND h,HIMC c){assert(h==window&&c==context);++releaseCalls;return 1;}
extern "C" HWND __stdcall FixtureImmGetDefaultIMEWnd(HWND h){assert(h==window);return showImeWindow?imeWindow:0;}
extern "C" int __stdcall FixtureImmGetConversionStatus(HIMC c,DWORD* a,DWORD* b){assert(c==context);++convertCalls;*a=conversion;*b=sentence;return 1;}
extern "C" int __stdcall FixtureImmSetConversionStatus(HIMC c,DWORD a,DWORD b){assert(c==context);++setConvertCalls;conversionSet=a;sentenceSet=b;return 1;}
extern "C" short __stdcall FixtureGetAsyncKeyState(int key){assert(key>=0&&key<256);return keysDown[key]?(short)0x8000:0;}
extern "C" HWND __stdcall FixtureGetFocus(){return window;}
static void Record(Event* list,unsigned& count,HWND h,UINT m,WPARAM w,LPARAM l){assert(count<100);Event& e=list[count++];e.hwnd=h;e.message=m;e.wp=w;e.lp=l;}
extern "C" int __stdcall FixturePostMessageA(HWND h,UINT m,WPARAM w,LPARAM l){Record(posted,postedCalls,h,m,w,l);return 1;}
extern "C" LRESULT __stdcall FixtureSendMessageA(HWND h,UINT m,WPARAM w,LPARAM l){Record(sysSent,sysSendCalls,h,m,w,l);return 0;}
static LRESULT __stdcall FixtureSend(HWND h,UINT m,WPARAM w,LPARAM l){assert(bIsSendingKeyMessage);Record(sent,sentCalls,h,m,w,l);return 0;}
LRESULT (__stdcall* _SendMessage)(HWND,UINT,WPARAM,LPARAM)=FixtureSend;
void __fastcall CheckToggleState(){++toggleCalls;}
void __fastcall SetImeApi(){++setApiCalls;if(newLanguage)g_hklCurrent=(HKL)(size_t)newLanguage;}
void __fastcall ImeUi_FinalizeString(bool value){assert(!value);++finalizeCalls;}
void __fastcall SetCompStringColor(){++colorCalls;}
DWORD __stdcall GetImeId(UINT index){assert(index==0);return idValue;}
void __stdcall GetReadingString(HWND h){assert(h==window);++readingCalls;}
static void __fastcall FixtureSendComposition(){++sendCompCalls;FixturelstrcpyW(sentComp,g_szCompositionString);}
void (__fastcall* _SendCompString)()=FixtureSendComposition;
static void __fastcall FixtureFree(void* p){assert(p==&candidate);++freeCalls;}
void (__fastcall* ImeUiCallback_Free)(void*)=FixtureFree;
static DWORD __fastcall FixtureCandidate(HIMC c,DWORD index,LPCANDIDATELIST* out){assert(c==context&&index==0);*out=(LPCANDIDATELIST)&candidate;return noCandidate?0:sizeof(candidate);}
DWORD (__fastcall* _GetCandidateList)(HIMC,DWORD,LPCANDIDATELIST*)=FixtureCandidate;
static LONG __stdcall FixtureComposition(HIMC c,DWORD kind,void* out,DWORD capacity){assert(c==context);++compositionCalls;if(kind==GCS_CURSORPOS){assert(out==0&&capacity==0);return cursor;}if(kind==GCS_COMPATTR){unsigned n=FixturelstrlenW(compText);assert(capacity>=n);memcpy(out,compAttr,n);return n;}const TCHAR* p=kind==GCS_COMPSTR?compText:resultText;assert(kind==GCS_COMPSTR||kind==GCS_RESULTSTR);unsigned n=FixturelstrlenW(p);assert(capacity>=n*2);memcpy(out,p,n*2);return n*2;}
LONG (__stdcall* _ImmGetCompositionString)(HIMC,DWORD,void*,DWORD)=FixtureComposition;
static void SetText(TCHAR* out,const char* text){do{*out++=(TCHAR)*text;}while(*text++);}
static bool TextIs(const TCHAR* p,const char* q){while(*p&&*q){if(*p++!=(unsigned char)*q++)return false;}return *p==0&&*q==0;}
static bool Call(UINT msg,WPARAM wp,LPARAM lp){bool trapped=true;assert(ImeUi_ProcessMessage(window,msg,wp,lp,&trapped)==0);return trapped;}
static void MessageReset(){
 memset(g_szCompositionString,0,sizeof(g_szCompositionString));memset(g_szCompAttrString,0,sizeof(g_szCompAttrString));memset(g_szCandidate,0,sizeof(g_szCandidate));memset(g_szReadingString,0,sizeof(g_szReadingString));memset(compText,0,sizeof(compText));memset(resultText,0,sizeof(resultText));memset(compAttr,0,sizeof(compAttr));memset(sentComp,0,sizeof(sentComp));memset(keysDown,0,sizeof(keysDown));
 g_bInitialized=true;g_bDisableImeCompletely=0;g_bCandList=g_bReadingWindow=g_bHorizontalReading=g_bInsertMode=bIsSendingKeyMessage=false;g_bVerticalCand=true;g_dwIMELevel=g_dwIMELevelSaved=2;g_IMECursorBytes=g_IMECursorChars=g_dwCount=g_dwSelection=0;g_uCandPageSize=3;g_iReadingError=-1;g_hwndCurr=window;g_himcOrg=context;g_hklCurrent=(HKL)(size_t)0x409;
 noContext=noCandidate=false;showImeWindow=true;idValue=1;newLanguage=0;conversion=0;sentence=0x12;cursor=0;
 Call(WM_KEYDOWN,VK_MENU,(LPARAM)0x80000000);Call(WM_KEYDOWN,VK_CONTROL,(LPARAM)0x80000000);Call(WM_KEYDOWN,VK_SHIFT,(LPARAM)0x80000000);
 getContextCalls=releaseCalls=toggleCalls=setApiCalls=finalizeCalls=colorCalls=readingCalls=freeCalls=sendCompCalls=convertCalls=setConvertCalls=postedCalls=sentCalls=sysSendCalls=compositionCalls=0;conversionSet=sentenceSet=0;
 memset(&candidate,0,sizeof(candidate));candidate.size=sizeof(candidate);candidate.count=6;candidate.pageSize=3;
 for(unsigned i=0;i<12;++i){candidate.offset[i]=(DWORD)((char*)candidate.text[i]-(char*)&candidate);candidate.text[i][0]=(TCHAR)('A'+i);candidate.text[i][1]=(TCHAR)('a'+i);}
}
int FixtureMessageTests(){unsigned cases=0;
#ifdef _MSC_VER
 assert(sizeof(DWORD)==4&&sizeof(tagCANDIDATELIST)==28);
#endif
 for(unsigned flags=0;flags<4;++flags){MessageReset();g_bInitialized=(flags&1)!=0;g_bDisableImeCompletely=(flags&2)!=0;SetText(g_szCompositionString,"old");bool tr=Call(WM_IME_STARTCOMPOSITION,0,0);assert(tr==(flags==1));assert(TextIs(g_szCompositionString,flags==1?"":"old"));++cases;}
 for(unsigned msg=0;msg<3;++msg){MessageReset();const UINT messages[]={0x999,WM_IME_SETCONTEXT,WM_IME_CONTROL};assert(!Call(messages[msg],7,0x123));assert(getContextCalls==0&&sendCompCalls==0);++cases;}
 for(unsigned k=0;k<2;++k)for(unsigned cursorValue=0;cursorValue<4;++cursorValue){MessageReset();g_dwIMELevel=2+k;g_IMECursorBytes=g_IMECursorChars=cursorValue;SetText(g_szCompositionString,"old");memset(g_szCompAttrString,1,sizeof(g_szCompAttrString));assert(Call(WM_IME_STARTCOMPOSITION,0,0));assert(TextIs(g_szCompositionString,"")&&g_IMECursorBytes==0&&g_IMECursorChars==0&&g_szCompAttrString[255]==0);++cases;}
 for(unsigned level=2;level<=3;++level)for(unsigned flags=0;flags<4;++flags)for(unsigned cp=0;cp<4;++cp){MessageReset();g_dwIMELevel=level;SetText(compText,"abc");SetText(resultText,"RESULT");for(unsigned i=0;i<3;++i)compAttr[i]=(BYTE)(i+1);cursor=cp;LPARAM bits=(flags&1?GCS_RESULTSTR:0)|(flags&2?GCS_COMPSTR:0);assert(Call(WM_IME_COMPOSITION,0,bits));assert(getContextCalls==1&&releaseCalls==1);assert(sendCompCalls==((flags&1)?1u:0u)+((flags&2)&&level==3?1u:0u));assert(TextIs(g_szCompositionString,(flags&2)?"abc":""));assert(g_IMECursorChars==((flags&2)?cp:0));assert(g_IMECursorBytes==g_IMECursorChars);if(flags&2)assert(g_szCompAttrString[0]==1&&g_szCompAttrString[2]==3&&g_szCompAttrString[3]==0);assert(!bIsSendingKeyMessage);if(level==2)assert(sentCalls==0);++cases;}
 for(unsigned flags=0;flags<4;++flags){MessageReset();noContext=true;SetText(g_szCompositionString,"old");assert(Call(WM_IME_COMPOSITION,0,(flags&1?GCS_RESULTSTR:0)|(flags&2?GCS_COMPSTR:0)));assert(getContextCalls==1&&releaseCalls==0&&compositionCalls==0&&TextIs(g_szCompositionString,"old"));++cases;}
 for(unsigned chars=0;chars<=4;++chars){MessageReset();g_hklCurrent=(HKL)(size_t)LANG_CHT;idValue=0;for(unsigned i=0;i<chars;++i)compText[i]=(TCHAR)('a'+i);assert(Call(WM_IME_COMPOSITION,0,GCS_COMPSTR));assert(g_dwCount==(chars?4u:0u));if(chars){assert(g_dwSelection==(DWORD)-1&&g_uCandPageSize==10&&g_bReadingWindow&&g_bHorizontalReading&&g_iReadingError==-1);assert(FixturelstrlenW(g_szReadingString)==(int)chars);for(unsigned i=0;i<4;++i)assert(g_szCandidate[i][0]==(i<chars?'a'+i:0));assert(g_szCompositionString[0]==0);}++cases;}
 for(unsigned level=2;level<=3;++level)for(unsigned position=0;position<4;++position)for(unsigned insertion=0;insertion<2;++insertion){MessageReset();g_dwIMELevel=level;g_bInsertMode=insertion!=0;SetText(g_szCompositionString,"abc");g_IMECursorChars=g_IMECursorBytes=position;assert(!Call(WM_IME_ENDCOMPOSITION,0,0));assert(TextIs(g_szCompositionString,"")&&g_IMECursorChars==0&&g_szCompAttrString[255]==0);if(level==2)assert(sentCalls==0);else{unsigned back=0,chars=0,right=0;for(unsigned i=0;i<sentCalls;++i){if(sent[i].message==WM_KEYDOWN&&sent[i].wp==VK_BACK)++back;if(sent[i].message==WM_CHAR)++chars;if(sent[i].message==WM_KEYDOWN&&sent[i].wp==VK_RIGHT)++right;assert(sent[i].lp==0);}assert(back==3&&chars==3&&right==3-position);}++cases;}
 for(unsigned language=0;language<2;++language)for(unsigned bits=0;bits<4;++bits){MessageReset();if(language)g_hklCurrent=(HKL)(size_t)LANG_CHT;conversion=0x20|(bits&1?IME_CMODE_SYMBOL:0)|(bits&2?IME_CMODE_SOFTKBD:0);assert(!Call(WM_IME_NOTIFY,IMN_SETCONVERSIONMODE,0));assert(toggleCalls==1&&convertCalls==language&&setConvertCalls==(language&&bits?1u:0u));if(setConvertCalls)assert(conversionSet==0x20&&sentenceSet==sentence);++cases;}
 MessageReset();assert(!Call(WM_IME_NOTIFY,IMN_SETOPENSTATUS,0)&&toggleCalls==1);++cases;
 for(unsigned notified=0;notified<2;++notified)for(unsigned orientation=0;orientation<2;++orientation)for(unsigned selection=0;selection<6;++selection)for(unsigned language=0;language<4;++language){MessageReset();g_bVerticalCand=orientation!=0;candidate.selection=selection;g_hklCurrent=(HKL)(size_t)(language==1?LANG_KOREAN:language==2?LANG_CHT:language==3?LANG_CHS:0x409);idValue=language>=2?0:1;assert(Call(WM_IME_NOTIFY,notified?IMN_OPENCANDIDATE:IMN_CHANGECANDIDATE,0));assert(g_bCandList&&!g_bReadingWindow&&g_dwCount==6&&g_uCandPageSize==3&&getContextCalls==1&&releaseCalls==1&&freeCalls==1);assert(g_dwSelection==(language?(DWORD)-1:selection%3));unsigned page=selection/3*3;for(unsigned j=0;j<3;++j){assert(g_szCandidate[j][0]=='1'+j);assert(g_szCandidate[j][orientation?2:1]=='A'+page+j);assert(g_szCandidate[j][orientation?1:3]==' ');assert(g_szCandidate[j][4]==0);}assert(g_szCandidate[3][0]==0);++cases;}
 for(unsigned selection=0;selection<6;++selection){MessageReset();g_hklCurrent=(HKL)(size_t)LANG_CHS;candidate.selection=selection;for(unsigned i=0;i<6;++i)SetText(candidate.text[i],"abcde");assert(Call(WM_IME_NOTIFY,IMN_OPENCANDIDATE,0));assert(g_uCandPageSize==3&&g_dwSelection==selection%3);assert(TextIs(g_szCandidate[0],"1 abcde"));++cases;}
 for(unsigned failure=0;failure<2;++failure){MessageReset();noContext=failure==0;noCandidate=failure==1;assert(Call(WM_IME_NOTIFY,IMN_OPENCANDIDATE,0));assert(g_bCandList&&getContextCalls==1&&freeCalls==0&&releaseCalls==0&&g_dwCount==0);++cases;}
 for(unsigned read=0;read<2;++read){MessageReset();g_bReadingWindow=read!=0;g_bCandList=true;g_dwCount=4;SetText(g_szCandidate[0],"abc");assert(Call(WM_IME_NOTIFY,IMN_CLOSECANDIDATE,0));assert(!g_bCandList&&g_dwCount==(read?4u:0u)&&TextIs(g_szCandidate[0],read?"abc":""));++cases;}
 const DWORD ids[]={IMEID_CHT_VER42,IMEID_CHT_VER43,IMEID_CHT_VER44,IMEID_CHS_VER41,IMEID_CHS_VER42,IMEID_CHT_VER50,IMEID_CHT_VER51,IMEID_CHT_VER52,IMEID_CHT_VER60,IMEID_CHS_VER53,0x999};const LPARAM messages[]={0,1,2,3,15,16,17,18,25,26,27,28,29};
 for(unsigned id=0;id<11;++id)for(unsigned m=0;m<13;++m)for(unsigned visible=0;visible<2;++visible){MessageReset();idValue=ids[id];g_bCandList=visible!=0;LPARAM lp=messages[m];bool expected=id<5?(lp==1||lp==2):id<10?(lp==16||lp==17||lp==26||lp==27||lp==28):false;assert(Call(WM_IME_NOTIFY,IMN_PRIVATE,lp)==expected);assert(readingCalls==(visible?0u:1u));++cases;}
 for(unsigned n=0;n<3;++n){MessageReset();const WPARAM notification[]={0,9,99};assert(Call(WM_IME_NOTIFY,notification[n],0));++cases;}
 for(unsigned lang=0;lang<3;++lang)for(unsigned show=0;show<2;++show){MessageReset();showImeWindow=show!=0;newLanguage=lang==0?0x409:lang==1?LANG_KOREAN:LANG_CHT;g_dwIMELevelSaved=2;assert(!Call(WM_INPUTLANGCHANGE,0,0));assert(toggleCalls==1&&setApiCalls==1&&finalizeCalls==(lang?1u:0u)&&colorCalls==finalizeCalls);assert(g_dwIMELevel==(lang==1?3u:2u));assert(sysSendCalls==show*2);if(show)assert(sysSent[0].wp==IMC_OPENSTATUSWINDOW&&sysSent[1].wp==IMC_CLOSESTATUSWINDOW&&sysSent[0].hwnd==imeWindow);++cases;}
 const UINT keys[]={VK_MENU,VK_CONTROL,VK_SHIFT};
 for(unsigned key=0;key<3;++key)for(unsigned down=0;down<2;++down)for(unsigned system=0;system<2;++system){MessageReset();assert(!Call(WM_KEYDOWN,keys[key],0x002b0001));keysDown[keys[key]]=down!=0;assert(!Call(system?WM_SYSKEYUP:WM_KEYUP,'Q',0));if(postedCalls!=(down?0u:1u)){fprintf(stderr,"key=%u down=%u system=%u posted=%u wp=%u\n",key,down,system,postedCalls,posted[0].wp);}assert(postedCalls==(down?0u:1u));if(!down)assert(posted[0].wp==keys[key]&&posted[0].message==WM_KEYUP&&(unsigned)posted[0].lp==0xc02b0001u);++cases;}
 assert(cases==494);return 0;
}


_SkinCompStr gSkinCompStr;
IMEUI_APPEARANCE gSkinIME;
CompStringAttribute g_CaretInfo;
bool g_bImeEnabled,g_bCaretDraw;

UINT g_uCaretBlinkLast,g_uCaretBlinkTime,g_screenWidth,g_screenHeight;

TCHAR g_szMultiLineCompString[256];

static unsigned nowTime,indicatorCalls,textCount,rectCount;
struct FixtureRect {int x,y,right,bottom;DWORD color;};static FixtureRect rects[100];
struct FixtureTextCall {int x,y;DWORD color;TCHAR text[260];};static FixtureTextCall texts[100];
extern "C" DWORD __stdcall FixtureGetTickCount(){return nowTime;}
void __fastcall DrawImeIndicator(){++indicatorCalls;}
static void __fastcall FixtureDrawRect(int x,int y,int right,int bottom,DWORD color){assert(rectCount<100);FixtureRect& r=rects[rectCount++];r.x=x;r.y=y;r.right=right;r.bottom=bottom;r.color=color;}
void (__fastcall* ImeUiCallback_DrawRect)(int,int,int,int,DWORD)=FixtureDrawRect;
class FixtureFont:public CImeUiFont_Base {
public:int x,y;DWORD color;
 void SetColor(DWORD c){color=c;}
 void SetPosition(int a,int b){x=a;y=b;}
 void GetTextExtent(LPCTSTR s,DWORD* w,DWORD* h){*w=8*FixturelstrlenW(s);*h=16;}
 void DrawText(LPCTSTR s){assert(textCount<100);FixtureTextCall& t=texts[textCount++];t.x=x;t.y=y;t.color=color;FixturelstrcpyW(t.text,s);}
};static FixtureFont font;
static void RenderReset(){
 memset(&gSkinIME,0,sizeof(gSkinIME));memset(&g_CaretInfo,0,sizeof(g_CaretInfo));
 memset(g_szCompositionString,0,sizeof(g_szCompositionString));memset(g_szCompAttrString,0,sizeof(g_szCompAttrString));memset(g_szMultiLineCompString,0,sizeof(g_szMultiLineCompString));memset(g_szCandidate,0,sizeof(g_szCandidate));memset(g_szReadingString,0,sizeof(g_szReadingString));
 g_bInitialized=g_bImeEnabled=g_bCaretDraw=true;g_bReadingWindow=g_bHorizontalReading=false;g_bVerticalCand=true;
 g_dwIMELevel=2;g_IMECursorBytes=g_IMECursorChars=1;g_dwCount=0;g_dwSelection=0;g_uCandPageSize=3;g_iReadingError=-1;
 g_uCaretBlinkLast=nowTime=100;g_uCaretBlinkTime=500;g_hklCurrent=(HKL)(size_t)0x409;idValue=0;
 g_CaretInfo.caretX=20;g_CaretInfo.caretY=30;g_CaretInfo.colorComp=0x112233;g_CaretInfo.colorCand=0x123456;g_CaretInfo.pFont=&font;
 g_CaretInfo.margins.left=10;g_CaretInfo.margins.right=100;g_CaretInfo.margins.top=0;g_CaretInfo.margins.bottom=120;
 gSkinIME.caretYMargin=1;gSkinIME.caretWidth=2;gSkinIME.candColorBase=0xffffffff;gSkinIME.candColorBorder=0xff000000;
 gSkinCompStr.colorInput=101;gSkinCompStr.colorTargetConv=102;gSkinCompStr.colorConverted=103;gSkinCompStr.colorTargetNotConv=104;gSkinCompStr.colorInputErr=105;
 g_screenWidth=640;g_screenHeight=480;indicatorCalls=textCount=rectCount=0;memset(rects,0,sizeof(rects));memset(texts,0,sizeof(texts));font.x=font.y=0;font.color=0;
}
static DWORD AttrColor(unsigned n){return n<=4?101+n:0;}
int FixtureRenderTests(){
#ifdef _MSC_VER
 assert(sizeof(IMEUI_APPEARANCE)==64&&sizeof(CompStringAttribute)==36);
#endif
 unsigned cases=0;
 for(unsigned mode=0;mode<8;++mode){RenderReset();g_bInitialized=(mode&1)!=0;g_bImeEnabled=(mode&2)!=0;if(!(mode&4))g_CaretInfo.pFont=0;ImeUi_RenderUI(true,true);assert(indicatorCalls==(mode==7?1u:0u));assert(textCount==0&&rectCount==0);++cases;}
 for(unsigned level=2;level<=3;++level)for(unsigned flags=0;flags<4;++flags)for(unsigned cursor=0;cursor<=3;++cursor)for(unsigned attr=0;attr<6;++attr){
 RenderReset();g_dwIMELevel=level;g_IMECursorBytes=g_IMECursorChars=cursor;g_szCompositionString[0]='A';g_szCompositionString[1]='B';g_szCompositionString[2]='C';for(unsigned k=0;k<3;++k)g_szCompAttrString[k]=(BYTE)attr;
 bool drawAttr=(flags&1)!=0,other=(flags&2)!=0;ImeUi_RenderUI(drawAttr,other);bool active=flags!=0&&(level!=2||other);
 assert(indicatorCalls==(active&&other?1u:0u));
 if(!active){assert(rectCount==0&&textCount==0);}
 else if(level==2){assert(textCount==1&&FixturelstrlenW(texts[0].text)==3&&texts[0].text[0]=='A'&&texts[0].text[2]=='C');assert(texts[0].x==20&&texts[0].y==30);assert(rectCount==4);unsigned normal=0,caret=0;for(unsigned j=0;j<rectCount;++j){const FixtureRect& r=rects[j];if(r.color==g_CaretInfo.colorComp){++caret;assert(r.x==20+(int)cursor*8&&r.y==31&&r.right==r.x+2&&r.bottom==45);}else{assert(r.color==AttrColor(attr)&&r.x==20+(int)normal*8&&r.y==30&&r.right==r.x+8&&r.bottom==46);++normal;}}assert(normal==3&&caret==1);}
 else {assert(textCount==0);unsigned expected=drawAttr?(cursor<=1?3:cursor==2?2:1):0;assert(rectCount==expected);for(unsigned j=0;j<rectCount;++j){const FixtureRect& r=rects[j];assert(r.x>=10&&r.x<=100&&r.y==30&&r.right-r.x==8&&r.bottom==46&&r.color==AttrColor(attr));}}
 assert(g_szCompositionString[0]=='A'&&g_szCompositionString[1]=='B'&&g_szCompositionString[2]=='C'&&g_szCompositionString[3]==0);++cases;
 }
 for(unsigned vertical=0;vertical<2;++vertical)for(unsigned reading=0;reading<3;++reading)for(unsigned selected=0;selected<4;++selected)for(unsigned nearEdge=0;nearEdge<2;++nearEdge){
 RenderReset();g_dwCount=3;g_uCandPageSize=3;g_dwSelection=selected;g_IMECursorBytes=0;g_bVerticalCand=vertical!=0;g_bReadingWindow=reading!=0;g_bHorizontalReading=reading==2;
 for(unsigned j=0;j<3;++j){g_szCandidate[j][0]=(TCHAR)('1'+j);g_szCandidate[j][1]=' ';g_szCandidate[j][2]=(TCHAR)('X'+j);}
 if(reading==2){g_szReadingString[0]='R';g_szReadingString[1]='S';g_szReadingString[2]='T';g_iReadingError=selected<3?(int)selected:-1;}
 if(nearEdge){g_CaretInfo.caretX=620;g_CaretInfo.caretY=470;}
 ImeUi_RenderUI(false,true);assert(indicatorCalls==1);assert(textCount==(reading==2?(selected<3?2u:1u):3u));assert(rectCount>=2);assert(rects[0].x>=0&&rects[0].right<=640&&rects[0].bottom<=480&&rects[0].y>=0);for(unsigned j=0;j<textCount;++j)assert(texts[j].x>=0&&texts[j].y>=0&&FixturelstrlenW(texts[j].text)>0);++cases;
 }
 for(unsigned wrap=0;wrap<3;++wrap){RenderReset();g_CaretInfo.margins.right=28+wrap*8;g_IMECursorBytes=2;g_szCompositionString[0]='A';g_szCompositionString[1]='B';g_szCompositionString[2]='C';ImeUi_RenderUI(true,true);assert(rectCount==4);assert(textCount>=1);assert(g_szCompositionString[0]=='A'&&g_szCompositionString[1]=='B'&&g_szCompositionString[2]=='C');++cases;}
 RenderReset();nowTime=601;ImeUi_RenderUI(true,true);assert(g_bCaretDraw==false&&g_uCaretBlinkLast==601);++cases;
 assert(cases==252);return 0;
}

int main(){FixtureMessageTests();FixtureRenderTests();puts("00cb38e0_TEST PASS");return 0;}
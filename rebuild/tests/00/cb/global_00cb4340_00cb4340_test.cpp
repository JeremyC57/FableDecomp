#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
// Test-only import substitutions avoid colliding with the CRT import library.
#define GetTickCount TestGetTickCount
#define MultiByteToWideChar TestMultiByteToWideChar
#define lstrlenW TestStringLength
#define lstrcatW TestStringConcat
#ifndef _MSC_VER
#include <alloca.h>
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
#define __try try
#define __except(x) catch(...)
#define _alloca alloca
#define GetExceptionCode() 0
#define EXCEPTION_EXECUTE_HANDLER 1
#define EXCEPTION_CONTINUE_SEARCH 0
#endif
static int stage;
static void check(bool yes) {if(!yes){fprintf(stderr,"stage %d failed\n",stage);exit(1);}}
#ifndef FABLE_RETAIL_IMEUI_SDK_H
#define FABLE_RETAIL_IMEUI_SDK_H
// Layout: original ImeUi.h and Ego_r IMEUI_APPEARANCE64 / IMEUI_VERTEX32.
#ifdef _MSC_VER
typedef unsigned long DWORD;
#else
typedef unsigned int DWORD;
#endif
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef const unsigned short* LPCTSTR;
struct tagRECT { long left,top,right,bottom; };
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

#ifndef FABLE_RETAIL_IMEUI_INPUT_H
#define FABLE_RETAIL_IMEUI_INPUT_H

typedef int BOOL;
typedef unsigned short WCHAR;
typedef WCHAR* LPWSTR;
typedef unsigned char* LPBYTE;
typedef int* PINT;
struct HWND__;typedef HWND__* HWND;
struct HIMC__;typedef HIMC__* HIMC;
struct HIMCC__;typedef HIMCC__* HIMCC;
struct tagPOINT { long x,y; };typedef tagPOINT POINT;
struct tagLOGFONTA {
 long lfHeight,lfWidth,lfEscapement,lfOrientation,lfWeight;
 BYTE lfItalic,lfUnderline,lfStrikeOut,lfCharSet,lfOutPrecision,lfClipPrecision,lfQuality,lfPitchAndFamily;
 char lfFaceName[32];
};typedef tagLOGFONTA LOGFONTA;
struct tagLOGFONTW {
 long lfHeight,lfWidth,lfEscapement,lfOrientation,lfWeight;
 BYTE lfItalic,lfUnderline,lfStrikeOut,lfCharSet,lfOutPrecision,lfClipPrecision,lfQuality,lfPitchAndFamily;
 WCHAR lfFaceName[32];
};typedef tagLOGFONTW LOGFONTW;
struct tagCOMPOSITIONFORM { DWORD dwStyle;POINT ptCurrentPos;RECT rcArea; };typedef tagCOMPOSITIONFORM COMPOSITIONFORM;
struct tagCANDIDATEFORM { DWORD dwIndex,dwStyle;POINT ptCurrentPos;RECT rcArea; };typedef tagCANDIDATEFORM CANDIDATEFORM;
struct tagINPUTCONTEXT2 {
 HWND hWnd;BOOL fOpen;POINT ptStatusWndPos,ptSoftKbdPos;DWORD fdwConversion,fdwSentence;
 union { LOGFONTA A;LOGFONTW W; } lfFont;
 COMPOSITIONFORM cfCompForm;CANDIDATEFORM cfCandForm[4];
 HIMCC hCompStr,hCandInfo,hGuideLine,hPrivate;DWORD dwNumMsgBuf;HIMCC hMsgBuf;DWORD fdwInit,dwReserve[3];
};typedef tagINPUTCONTEXT2 INPUTCONTEXT2;typedef INPUTCONTEXT2* LPINPUTCONTEXT2;
struct _OSVERSIONINFOA { DWORD dwOSVersionInfoSize,dwMajorVersion,dwMinorVersion,dwBuildNumber,dwPlatformId;char szCSDVersion[128]; };
typedef _OSVERSIONINFOA OSVERSIONINFOA;
#endif

// GetReadingString(HWND__ *) -- read the active input method reading string.
// Leaf: ?GetReadingString@@YGXPAUHWND__@@@Z
// Flags: /Gr
// Adapted from Microsoft DirectX SDK ImeUi.cpp.
// Copyright (c) Microsoft Corporation. Licensed under the MIT License.
// Changes reconstruct the older native implementation; see NAMING_EVIDENCE.md.
#ifdef _MSC_VER
#include <malloc.h>
#endif
#ifdef _MSC_VER
#include <excpt.h>
#endif
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
extern "C"  int __stdcall MultiByteToWideChar(UINT,DWORD,const char*,int,LPWSTR,int);
extern "C"  int __stdcall lstrlenW(const WCHAR*);
extern "C"  LPWSTR __stdcall lstrcatW(LPWSTR,const WCHAR*);
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


WCHAR g_szCandidate[10][256];DWORD g_dwCount;bool g_bHorizontalReading,g_bReadingWindow;
BYTE LeadByteTable[256];UINT g_uCodePage,g_uCandPageSize;int g_iReadingError;WCHAR g_szReadingString[32];OSVERSIONINFOA g_osi;
static DWORD imeId,version;static UINT apiLength;static DWORD apiError;static BOOL apiVertical;
static HIMC context;static INPUTCONTEXT2 inputContext;static unsigned char privateBlock[512],entryBlock[512],readingBlock[512];
static int getCalls,releaseCalls,lockCalls,unlockCalls,orientCalls,apiCalls,conversionCalls;
DWORD __stdcall GetImeId(UINT n){return n?version:imeId;}
void __stdcall GetReadingWindowOrientation(DWORD n){check(n==imeId);++orientCalls;}
extern "C" HIMC __stdcall ImmGetContext(HWND){++getCalls;return context;}
extern "C" BOOL __stdcall ImmReleaseContext(HWND,HIMC c){check(c==context);++releaseCalls;return 1;}
static UINT __stdcall ReadAPI(HIMC c,UINT n,LPWSTR p,PINT e,BOOL* vertical,UINT* maximum){
 check(c==context);++apiCalls;*e=(int)apiError;*vertical=apiVertical;*maximum=8;
 if(p){check(n==apiLength);for(UINT i=0;i<n;++i)p[i]=(WCHAR)(65+i);}return apiLength;
}
static LPINPUTCONTEXT2 __stdcall LockIMC(HIMC c){check(c==context);++lockCalls;return &inputContext;}
static BOOL __stdcall UnlockIMC(HIMC c){check(c==context);++unlockCalls;return 1;}
static void* __stdcall LockIMCC(HIMCC c){check(c==inputContext.hPrivate);++lockCalls;return privateBlock;}
static BOOL __stdcall UnlockIMCC(HIMCC c){check(c==inputContext.hPrivate);++unlockCalls;return 1;}
UINT (__stdcall *_GetReadingString)(HIMC,UINT,LPWSTR,PINT,BOOL*,UINT*);
LPINPUTCONTEXT2 (__stdcall *_ImmLockIMC)(HIMC)=LockIMC;
BOOL (__stdcall *_ImmUnlockIMC)(HIMC)=UnlockIMC;
void* (__stdcall *_ImmLockIMCC)(HIMCC)=LockIMCC;
BOOL (__stdcall *_ImmUnlockIMCC)(HIMCC)=UnlockIMCC;
extern "C" int __stdcall MultiByteToWideChar(UINT cp,DWORD flags,const char* p,int count,LPWSTR out,int capacity){
 check(cp==932&&flags==0&&capacity==1&&(count==1||count==2));++conversionCalls;
 *out=count==1?(unsigned char)p[0]:(WCHAR)(0x8000+(unsigned char)p[1]);return 1;
}
extern "C" int __stdcall lstrlenW(const WCHAR* p){int n=0;while(p[n])++n;return n;}
extern "C" LPWSTR __stdcall lstrcatW(LPWSTR p,const WCHAR* q){LPWSTR out=p;while(*p)++p;while((*p++=*q++)!=0){}return out;}
static void setword(unsigned char* p,int off,DWORD value){memcpy(p+off,&value,4);}
static void setpointer(unsigned char* p,int off,unsigned char* q){memcpy(p+off,&q,sizeof(q));}
static void reset(){
 memset(g_szCandidate,0,sizeof(g_szCandidate));memset(&inputContext,0,sizeof(inputContext));memset(privateBlock,0,sizeof(privateBlock));memset(entryBlock,0,sizeof(entryBlock));memset(readingBlock,0,sizeof(readingBlock));
 memset(&g_osi,0,sizeof(g_osi));memset(LeadByteTable,0,sizeof(LeadByteTable));memset(g_szReadingString,0,sizeof(g_szReadingString));
 imeId=IMEID_CHT_VER42;version=2;apiLength=3;apiError=1;apiVertical=0;context=(HIMC)1;inputContext.hPrivate=(HIMCC)2;_GetReadingString=0;
 g_bReadingWindow=false;g_bHorizontalReading=true;g_dwCount=77;g_dwSelection=88;g_iReadingError=99;g_uCandPageSize=3;g_uCodePage=932;
 getCalls=releaseCalls=lockCalls=unlockCalls=orientCalls=apiCalls=conversionCalls=0;g_osi.dwPlatformId=2;
}
static void expectABC(DWORD selected){check(g_dwCount==3&&g_dwSelection==selected&&g_szCandidate[0][0]==65&&g_szCandidate[1][0]==66&&g_szCandidate[2][0]==67&&g_szCandidate[3][0]==0&&g_bReadingWindow&&g_uCandPageSize==10&&releaseCalls==1);}
static void wideData(unsigned char* data,int off,int lenOff,int errOff){WCHAR a[3]={65,66,67};memcpy(data+off,a,sizeof(a));setword(data,lenOff,3);setword(data,errOff,1);}
int main(){
#ifdef _MSC_VER
 check(offsetof(INPUTCONTEXT2,hPrivate)==0x124&&sizeof(OSVERSIONINFOA)==148);
#endif
 stage=1;reset();imeId=0;GetReadingString((HWND)1);check(getCalls==0&&releaseCalls==0&&g_dwCount==77);
 stage=2;reset();context=0;GetReadingString((HWND)1);check(getCalls==1&&releaseCalls==0&&g_dwCount==77);
 stage=3;reset();_GetReadingString=ReadAPI;GetReadingString((HWND)1);expectABC(1);check(apiCalls==2&&lockCalls==0&&unlockCalls==0&&orientCalls==0&&g_iReadingError==1&&g_szReadingString[3]==0);
 stage=4;reset();_GetReadingString=ReadAPI;apiLength=0;GetReadingString((HWND)1);check(g_dwCount==0&&g_dwSelection==(DWORD)-1&&apiCalls==1&&g_szReadingString[0]==0&&g_iReadingError==-1);
 stage=5;reset();_GetReadingString=ReadAPI;apiVertical=1;apiError=9;GetReadingString((HWND)1);expectABC((DWORD)-1);check(!g_bHorizontalReading&&g_iReadingError==99);
 const DWORD ids[]={IMEID_CHT_VER42,IMEID_CHT_VER43,IMEID_CHT_VER44,IMEID_CHT_VER51,IMEID_CHT_VER52,IMEID_CHS_VER53};
 for(int n=0;n<6;++n){stage=6+n;reset();imeId=ids[n];
  if(n<3){setpointer(privateBlock,24,readingBlock);wideData(readingBlock,56,156,160);}
  else{setpointer(privateBlock,4,entryBlock);setpointer(entryBlock,24,readingBlock);wideData(readingBlock,64,96,100);}
  GetReadingString((HWND)1);expectABC(1);check(lockCalls==2&&unlockCalls==2&&orientCalls==1&&conversionCalls==0&&g_iReadingError==1);
 }
 for(int n=0;n<2;++n){stage=12+n;reset();imeId=IMEID_CHS_VER41;version=n?2:1;setpointer(privateBlock,n?32:28,readingBlock);wideData(readingBlock,56,156,160);setword(readingBlock,160,9);GetReadingString((HWND)1);expectABC((DWORD)-1);}
 stage=14;reset();imeId=IMEID_CHT_VER50;setpointer(privateBlock,12,entryBlock);setpointer(entryBlock,32,readingBlock);readingBlock[64]=65;readingBlock[65]=(unsigned char)0x81;readingBlock[66]=64;setword(readingBlock,80,3);setword(readingBlock,84,1);LeadByteTable[0x81]=1;GetReadingString((HWND)1);check(g_dwCount==2&&g_dwSelection==1&&conversionCalls==2&&g_szCandidate[0][0]==65&&g_szCandidate[1][0]==0x8040&&g_iReadingError==1);
 for(int n=0;n<2;++n){stage=15+n;reset();imeId=IMEID_CHS_VER42;g_osi.dwPlatformId=n?2:1;setpointer(privateBlock,32,readingBlock);
  if(n)wideData(readingBlock,64,96,100);else{readingBlock[64]=65;readingBlock[65]=66;readingBlock[66]=67;setword(readingBlock,80,3);setword(readingBlock,84,1);}
  GetReadingString((HWND)1);expectABC(1);check(conversionCalls==(n?0:3));
 }
 stage=17;reset();imeId=0x12345678;GetReadingString((HWND)1);check(g_dwCount==0&&lockCalls==1&&unlockCalls==2&&orientCalls==1&&g_bReadingWindow);
 stage=18;reset();GetReadingString((HWND)1);check(g_dwCount==0&&lockCalls==2&&unlockCalls==2&&g_szCandidate[0][0]==0);
 stage=19;reset();imeId=IMEID_CHT_VER51;setpointer(privateBlock,4,entryBlock);GetReadingString((HWND)1);check(g_dwCount==0&&unlockCalls==2);
 puts("00cb4340_TEST PASS");return 0;
}
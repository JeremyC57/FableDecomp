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

// DrawImeIndicator(void) -- draw the input method status indicator.
// Leaf: ?DrawImeIndicator@@YIXXZ
// Flags: /Gr
// Adapted from Microsoft DirectX SDK ImeUi.cpp.
// Copyright (c) Microsoft Corporation. Licensed under the MIT License.
// Changes reconstruct the older native implementation; see NAMING_EVIDENCE.md.
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
extern "C"  DWORD __stdcall GetTickCount();
#pragma intrinsic(sin,cos,memset)
extern DWORD g_dwState;
extern IMEUI_APPEARANCE gSkinIME;
extern CompStringAttribute g_CaretInfo;
extern UINT g_screenWidth,g_screenHeight;
extern DWORD swirl;
extern double lastSwirl;
extern HKL g_hklCurrent;
extern LPCTSTR g_pszIndicatior;
extern LPCTSTR g_aszIndicator[];
extern void (__fastcall *ImeUiCallback_DrawFans)(const IMEUI_VERTEX*,UINT);
#define GETPRIMLANG() ((unsigned short)((unsigned long)g_hklCurrent & 0x3ff))
static void DrawImeIndicator()
{
    bool bOn = g_dwState != IMEUI_STATE_OFF;

    IMEUI_VERTEX PieData[16];
    float SizeOfPie = ( float )gSkinIME.symbolHeight;

    memset( PieData, 0, sizeof( PieData ) );

    switch( gSkinIME.symbolPlacement )
    {
        case 0: // vertical centering IME indicator
        {
            if( SizeOfPie + g_CaretInfo.margins.right + 4 > g_screenWidth )
            {
                PieData[0].sx = ( -SizeOfPie / 2 ) + g_CaretInfo.margins.left - 4;
                PieData[0].sy = (float)g_CaretInfo.margins.bottom - gSkinIME.symbolHeight/2;
            }
            else
            {
                PieData[0].sx = -( SizeOfPie / 2 ) + g_CaretInfo.margins.right + gSkinIME.symbolHeight + 4;
                PieData[0].sy = (float)g_CaretInfo.margins.bottom - gSkinIME.symbolHeight/2;
            }
            break;
        }
        case 1: // upperleft
            PieData[0].sx = 4 + ( SizeOfPie / 2 );
            PieData[0].sy = 4 + ( SizeOfPie / 2 );
            break;
        case 2: // upperright
            PieData[0].sx = g_screenWidth - ( 4 + ( SizeOfPie / 2 ) );
            PieData[0].sy = 4 + ( SizeOfPie / 2 );
            break;
        case 3: // lowerright
            PieData[0].sx = g_screenWidth - ( 4 + ( SizeOfPie / 2 ) );
            PieData[0].sy = g_screenHeight - ( 4 + ( SizeOfPie / 2 ) );
            break;
        case 4: // lowerleft
            PieData[0].sx = 4 + ( SizeOfPie / 2 );
            PieData[0].sy = g_screenHeight - ( 4 + ( SizeOfPie / 2 ) );
            break;
    }
    PieData[0].rhw = 1.0f;
    if( bOn )
    {
        if( GetTickCount() - lastSwirl > 250 )
        {
            swirl++;
            lastSwirl = GetTickCount();
            if( swirl > 13 )
                swirl = 0;
        }
    }
    else
        swirl = 0;
    for( int t1 = 1; t1 < 16; t1++ )
    {
        float radian = 2.0f * 3.1415926f * ( t1 - 1 + ( DWORD(bOn) * swirl ) ) / 14.0f;
        PieData[t1].sx = ( float )( PieData[0].sx + (float)cos( radian ) * SizeOfPie / 2.0f );
        PieData[t1].sy = ( float )( PieData[0].sy + (float)sin( radian ) * SizeOfPie / 2.0f );
        PieData[t1].rhw = 1.0f;
    }

    PieData[0].color = 0xffffff + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
    if( !gSkinIME.symbolColor && bOn )
    {
        {
            PieData[1].color = 0xff0000 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[2].color = 0xff3000 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[3].color = 0xff6000 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[4].color = 0xff9000 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[5].color = 0xffC000 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[6].color = 0xffff00 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[7].color = 0xC0ff00 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[8].color = 0x90ff00 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[9].color = 0x60ff00 + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[10].color = 0x30c0ff + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[11].color = 0x00a0ff + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[12].color = 0x3090ff + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[13].color = 0x6060ff + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[14].color = 0x9030ff + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
            PieData[15].color = 0xc000ff + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
        }
    }
    else
    {
        DWORD dwColor = bOn ? gSkinIME.symbolColor : gSkinIME.symbolColorOff;
        for( int t1 = 1; t1 < 16; t1++ )
        {
            PieData[t1].color = dwColor + ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 );
        }
    }

    ImeUiCallback_DrawFans( PieData, 16 );

    float fHeight = gSkinIME.symbolHeight * 0.625f;

    // fix for Ent Gen #120 - reduce the height of character when Korean IME is on
    if( GETPRIMLANG() == LANG_KOREAN && bOn )
    {
        fHeight *= 0.8f;
    }

    if( gSkinIME.symbolFont )
    {

        // GOS deals height in points that is 1/72nd inch and assumes display device is 96dpi.
        fHeight = fHeight * (96.0f / 72.0f);
        gSkinIME.symbolFont->SetHeight( ( UINT )fHeight );
        gSkinIME.symbolFont->SetColor( ( ( ( DWORD )gSkinIME.symbolTranslucence ) << 24 ) | gSkinIME.symbolColorText );

        //
        // draw the proper symbol over the fan
        //
        DWORD w, h;
        LPCTSTR cszSymbol = ( g_dwState == IMEUI_STATE_ON ) ? g_pszIndicatior : g_aszIndicator[0];

        gSkinIME.symbolFont->GetTextExtent( cszSymbol, &w, &h );
        gSkinIME.symbolFont->SetPosition( ( int )( PieData[0].sx ) - w / 2, ( int )( PieData[0].sy ) - h / 2 );
        gSkinIME.symbolFont->DrawText( cszSymbol );

    }
}


extern bool g_bInitialized,g_bImeEnabled;
extern DWORD g_dwIMELevel;
void DrawCompositionString(bool);
void DrawCandidateList();
void ImeUi_RenderUI(bool bDrawCompAttr,bool bDrawOtherUi) {
 if(!g_bInitialized || !g_bImeEnabled || !g_CaretInfo.pFont)return;
 if(!bDrawCompAttr && !bDrawOtherUi)return;
 if(g_dwIMELevel==2) { if(!bDrawOtherUi)return; }
 if(bDrawOtherUi)DrawImeIndicator();
 DrawCompositionString(bDrawCompAttr);
 if(bDrawOtherUi)DrawCandidateList();
}

DWORD g_dwState;IMEUI_APPEARANCE gSkinIME;CompStringAttribute g_CaretInfo;
UINT g_screenWidth,g_screenHeight;DWORD swirl;double lastSwirl;HKL g_hklCurrent;
static const unsigned short offText[]={65,0},onText[]={0x3042,0};
LPCTSTR g_pszIndicatior=onText;LPCTSTR g_aszIndicator[]={offText};
bool g_bInitialized,g_bImeEnabled;DWORD g_dwIMELevel;
void DrawCompositionString(bool){} void DrawCandidateList(){}
static DWORD tick;static int tickCalls,fanCalls;
extern "C" DWORD __stdcall GetTickCount(){++tickCalls;return tick;}
static IMEUI_VERTEX fan[16];
static void __fastcall CaptureFan(const IMEUI_VERTEX* a,UINT n){check(n==16);++fanCalls;memcpy(fan,a,sizeof(fan));}
void (__fastcall *ImeUiCallback_DrawFans)(const IMEUI_VERTEX*,UINT)=CaptureFan;
class TestFont:public CImeUiFont_Base {
public: UINT height;DWORD color;int x,y,calls;LPCTSTR text;
 TestFont():height(0),color(0),x(0),y(0),calls(0),text(0){}
 void SetHeight(UINT n){height=n;++calls;}
 void SetColor(DWORD c){color=c;++calls;}
 void SetPosition(int a,int b){x=a;y=b;++calls;}
 void GetTextExtent(LPCTSTR s,DWORD* w,DWORD* h){text=s;*w=8;*h=12;++calls;}
 void DrawText(LPCTSTR s){check(s==text);++calls;}
};
static void reset(){memset(&gSkinIME,0,sizeof(gSkinIME));memset(&g_CaretInfo,0,sizeof(g_CaretInfo));
 gSkinIME.symbolHeight=24;gSkinIME.symbolTranslucence=128;gSkinIME.symbolColorOff=0x404040;
 gSkinIME.symbolColorText=0x123456;g_screenWidth=640;g_screenHeight=480;
 g_CaretInfo.margins.left=100;g_CaretInfo.margins.right=200;g_CaretInfo.margins.bottom=80;
 g_dwState=0;swirl=9;lastSwirl=0;tick=0;tickCalls=fanCalls=0;g_hklCurrent=0;
}
static void geometry(float x,float y){
 check(fanCalls==1&&fan[0].sx==x&&fan[0].sy==y&&fan[0].rhw==1.0f);
 for(int n=0;n<16;++n){check(fan[n].sz==0&&fan[n].specular==0&&fan[n].tu==0&&fan[n].tv==0&&fan[n].rhw==1.0f);}
 for(int n=1;n<16;++n){double dx=fan[n].sx-x,dy=fan[n].sy-y;check(fabs(dx*dx+dy*dy-144.0)<0.125);}
}
int main(){
#ifdef _MSC_VER
 check(sizeof(IMEUI_VERTEX)==32&&sizeof(IMEUI_APPEARANCE)==64&&offsetof(CompStringAttribute,margins)==20);
#endif
 const float xs[]={216,16,624,624,16,0};const float ys[]={68,16,16,464,464,0};
 for(int n=0;n<6;++n){stage=n+1;reset();gSkinIME.symbolPlacement=(BYTE)n;DrawImeIndicator();geometry(xs[n],ys[n]);
  check(swirl==0&&tickCalls==0&&fan[0].color==0x80ffffffU);for(int k=1;k<16;++k)check(fan[k].color==0x80404040U);}
 stage=7;reset();g_screenWidth=220;DrawImeIndicator();geometry(84,68);
 stage=8;reset();g_dwState=1;swirl=0;tick=250;DrawImeIndicator();check(swirl==0&&tickCalls==1&&lastSwirl==0);
 const DWORD colors[]={0xff0000,0xff3000,0xff6000,0xff9000,0xffc000,0xffff00,0xc0ff00,0x90ff00,0x60ff00,0x30c0ff,0x00a0ff,0x3090ff,0x6060ff,0x9030ff,0xc000ff};
 for(int k=1;k<16;++k)check(fan[k].color==(0x80000000U|colors[k-1]));
 stage=9;reset();g_dwState=1;swirl=12;tick=251;DrawImeIndicator();check(swirl==13&&lastSwirl==251&&tickCalls==2);
 stage=10;reset();g_dwState=1;swirl=13;tick=256;DrawImeIndicator();check(swirl==0&&lastSwirl==256&&tickCalls==2);
 stage=11;reset();g_dwState=2;gSkinIME.symbolColor=0x13579b;DrawImeIndicator();for(int k=1;k<16;++k)check(fan[k].color==0x8013579bU);
 stage=12;reset();TestFont f;gSkinIME.symbolFont=&f;DrawImeIndicator();check(f.height==20&&f.color==0x80123456U&&f.calls==5&&f.text==offText&&f.x==212&&f.y==62);
 stage=13;reset();TestFont k;gSkinIME.symbolFont=&k;g_dwState=1;g_hklCurrent=(HKL)0x412;DrawImeIndicator();check(k.height==16&&k.text==onText&&k.calls==5);
 stage=14;reset();TestFont otherFont;gSkinIME.symbolFont=&otherFont;g_dwState=2;g_hklCurrent=(HKL)0x411;DrawImeIndicator();check(otherFont.height==20&&otherFont.text==offText);
 puts("00cb2f80_TEST PASS");return 0;
}
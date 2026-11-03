// DrawImeIndicator(void) -- draw the input method status indicator.
// Leaf: ?DrawImeIndicator@@YIXXZ
// Flags: /Gr
// Adapted from Microsoft DirectX SDK ImeUi.cpp.
// Copyright (c) Microsoft Corporation. Licensed under the MIT License.
// Changes reconstruct the older native implementation; see NAMING_EVIDENCE.md.
#include "engine/ImeUi.h"
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
extern "C" void* __cdecl memset(void*,int,unsigned int);
extern "C" __declspec(dllimport) DWORD __stdcall GetTickCount();
#pragma intrinsic(sin,cos,memset)
extern DWORD g_dwState;
extern IMEUI_APPEARANCE gSkinIME;
extern CompStringAttribute g_CaretInfo;
extern UINT g_screenWidth,g_screenHeight;
extern DWORD swirl;
extern double lastSwirl;
static HKL g_hklCurrent; // ImeUi.cpp file static: SetImeApi 0x00CB36E0 differs 178v179 with it extern (Codex pass 12 landing 2026-11-03)
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
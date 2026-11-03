#ifndef FABLE_RETAIL_IMEUI_INPUT_H
#define FABLE_RETAIL_IMEUI_INPUT_H
#include "engine/ImeUi.h"
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

#ifndef FABLE_ENGINE_ERRORDIALOG_H
#define FABLE_ENGINE_ERRORDIALOG_H
#include "engine/ConfigDetection.h"
// Win32 SDK public types and prototypes, corroborated by original PDB types
// and the PE import-directory entries used by DisplayErrorDialog.
typedef unsigned long DWORD;typedef unsigned int UINT;typedef int BOOL;
typedef unsigned short WORD,ATOM;typedef unsigned char BYTE;typedef long LPARAM,LRESULT;
typedef unsigned int WPARAM;typedef unsigned long REGSAM;
struct HWND__;typedef HWND__* HWND;
struct HINSTANCE__;typedef HINSTANCE__* HINSTANCE;
struct HKEY__;typedef HKEY__* HKEY;
struct HICON__;typedef HICON__* HICON;typedef HICON HCURSOR;
struct HBRUSH__;typedef HBRUSH__* HBRUSH;
struct HMENU__;typedef HMENU__* HMENU;
struct _SECURITY_ATTRIBUTES;
typedef LRESULT (__stdcall *WNDPROC)(HWND,UINT,WPARAM,LPARAM);
typedef int (__stdcall *DLGPROC)(HWND,UINT,WPARAM,LPARAM);
struct tagWNDCLASSEXA {
 UINT cbSize,style;WNDPROC lpfnWndProc;int cbClsExtra,cbWndExtra;HINSTANCE hInstance;
 HICON hIcon;HCURSOR hCursor;HBRUSH hbrBackground;const char* lpszMenuName;
 const char* lpszClassName;HICON hIconSm;
};typedef tagWNDCLASSEXA WNDCLASSEXA;
extern "C" {
__declspec(dllimport) int __cdecl wsprintfA(char*,const char*,...);
__declspec(dllimport) char* __stdcall lstrcpyA(char*,const char*);
__declspec(dllimport) char* __stdcall lstrcatA(char*,const char*);
__declspec(dllimport) int __stdcall lstrlenA(const char*);
__declspec(dllimport) int __stdcall LoadStringA(HINSTANCE,UINT,char*,int);
__declspec(dllimport) long __stdcall RegOpenKeyExA(HKEY,const char*,DWORD,REGSAM,HKEY*);
__declspec(dllimport) long __stdcall RegQueryValueExA(HKEY,const char*,DWORD*,DWORD*,BYTE*,DWORD*);
__declspec(dllimport) long __stdcall RegCloseKey(HKEY);
__declspec(dllimport) long __stdcall RegCreateKeyExA(HKEY,const char*,DWORD,char*,DWORD,REGSAM,_SECURITY_ATTRIBUTES*,HKEY*,DWORD*);
__declspec(dllimport) long __stdcall RegSetValueExA(HKEY,const char*,DWORD,DWORD,const BYTE*,DWORD);
__declspec(dllimport) HICON __stdcall LoadIconA(HINSTANCE,const char*);
__declspec(dllimport) HCURSOR __stdcall LoadCursorA(HINSTANCE,const char*);
__declspec(dllimport) LRESULT __stdcall DefWindowProcA(HWND,UINT,WPARAM,LPARAM);
__declspec(dllimport) ATOM __stdcall RegisterClassExA(const WNDCLASSEXA*);
__declspec(dllimport) HWND __stdcall CreateWindowExA(DWORD,const char*,const char*,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,void*);
__declspec(dllimport) BOOL __stdcall ShowWindow(HWND,int);
__declspec(dllimport) int __stdcall ShowCursor(BOOL);
__declspec(dllimport) BOOL __stdcall DestroyWindow(HWND);
__declspec(dllimport) BOOL __stdcall UnregisterClassA(const char*,HINSTANCE);
}
// Layout: exact PDB struct keys and fields, native table stride12 and sentinel.
struct ERRORARRAYSTRUCT { int nErrorCode,nErrorStringID,nErrorURLStringID; };
struct window_data { HWND hWnd;HINSTANCE hInstance; };
extern ERRORARRAYSTRUCT ErrorArray[44];
extern char ErrorMessage[1024],ErrorMessageTitle[128],ErrorMessageURL[512];
extern int GlobalForceExit;
extern unsigned int DontShowAgain;
extern unsigned short gApplicationLanguage;
extern ConfigDetection* pConfig;
extern window_data window_globals;
extern "C" HINSTANCE hStrings;
extern "C" char* AppendText;
extern "C" int optionSafe;
int __fastcall RetrieveStringByLanguage(HINSTANCE,unsigned long,char*,unsigned long,unsigned short);
extern "C" int __fastcall RetrieveString(HINSTANCE,unsigned long,char*,unsigned long);
extern "C" int __fastcall LocalizedDialogBox(HINSTANCE,const char*,HWND,DLGPROC);
#ifdef FABLE_RETAIL_ERROR_DIALOG_PROC_PASS30
#include "engine/Win32Types.h"

// Windows SDK callback messages, result codes and dialog API declarations.
#define WM_CLOSE 0x0010
#define WM_INITDIALOG 0x0110
#define WM_COMMAND 0x0111
#define IDCANCEL 2
#define SW_SHOWNORMAL 1
#define LOWORD(value) ((WORD)(DWORD)(value))
extern "C" {
__declspec(dllimport) UINT __stdcall IsDlgButtonChecked(HWND,int);
__declspec(dllimport) BOOL __stdcall EndDialog(HWND,int);
__declspec(dllimport) HINSTANCE __stdcall ShellExecuteA(HWND,const char*,const char*,const char*,const char*,int);
__declspec(dllimport) BOOL __stdcall GetWindowRect(HWND,RECT*);
__declspec(dllimport) HWND __stdcall GetDesktopWindow();
__declspec(dllimport) BOOL __stdcall MoveWindow(HWND,int,int,int,int,BOOL);
__declspec(dllimport) BOOL __stdcall SetWindowTextA(HWND,const char*);
__declspec(dllimport) BOOL __stdcall SetDlgItemTextA(HWND,int,const char*);
__declspec(dllimport) HWND __stdcall GetDlgItem(HWND,int);
__declspec(dllimport) BOOL __stdcall EnableWindow(HWND,BOOL);
}
int __fastcall ConvertStaticToHyperlink(HWND);
#endif
int __stdcall ErrorDialogProc(HWND,UINT,WPARAM,LPARAM);
extern "C" void __fastcall ClearDirtyExitFlag();
extern "C" int __stdcall DisplayErrorDialog(int*,int);
#endif

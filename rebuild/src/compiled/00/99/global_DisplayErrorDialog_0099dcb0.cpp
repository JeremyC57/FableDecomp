// DisplayErrorDialog(int* Error, int ForceExit) -- localize and display the startup configuration errors, preserving a remembered choice
// Leaf: _DisplayErrorDialog@8
// Flags: /GS
// Retail: Fable.exe @ 0x0099DCB0, 1455 bytes (EXTENT FIX: the catalog row says 1454; byte 1455 at 0x0099E25E is the int3 VC7.1 emits after
// the no-return exit() call, 0x0099E25F is the one alignment byte before the next function at 0x0099E260).
// Names: Ego_r 0x00AEB890 / FableWin 0x02F6C0A0 `_DisplayErrorDialog@8`; callees RetrieveStringByLanguage / @RetrieveString@16 /
// @LocalizedDialogBox@16 / ?ErrorDialogProc@@YGHPAUHWND__@@IIJ@Z / @ClearDirtyExitFlag@0 and the two ?n*FromErrorCode@@YIHH@Z helpers
// (inlined here) are in both PDB name sets in this order. Data symbols (gApplicationLanguage, hStrings, pConfig, ErrorArray, ...) are the
// Codex pass 11 raw-PDB reading (one source): see NOTES.md.
// Shape: the error loop is `while (nErrorIndex < 10 && Error[nErrorIndex])`; with it VC7.1 keeps the URL RetrieveString call out of line
// and expands the loop and title calls, exactly as retail (the `for` + `break` form inlines all three, 1520 bytes).
#include "engine/ErrorDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <excpt.h>
#pragma intrinsic(memset)
int __fastcall nErrorStringFromErrorCode(int nError)
{
 int index=0;
 while(ErrorArray[index].nErrorCode!=-1){
  if(ErrorArray[index].nErrorCode==nError)break;
  ++index;
 }
 return ErrorArray[index].nErrorStringID;
}
int __fastcall nErrorUrlStringFromErrorCode(int nError)
{
 int index=0;
 while(ErrorArray[index].nErrorCode!=-1){
  if(ErrorArray[index].nErrorCode==nError)break;
  ++index;
 }
 return ErrorArray[index].nErrorURLStringID;
}
extern "C" int __fastcall RetrieveString(HINSTANCE hModule,unsigned long dwStringID,char* pBuffer,unsigned long dwBufferSize)
{
 int result=RetrieveStringByLanguage(hModule,dwStringID,pBuffer,dwBufferSize,gApplicationLanguage);
 if(result)return result;
 if(gApplicationLanguage!=0x409){
  result=RetrieveStringByLanguage(hModule,dwStringID,pBuffer,dwBufferSize,0x409);
  if(result)return result;
 }
 return LoadStringA(hModule,dwStringID,pBuffer,dwBufferSize);
}
extern "C" int __stdcall DisplayErrorDialog(int* Error,int ForceExit)
{
 int url=-1;
 if(Error[0]==-1){
  wsprintfA(ErrorMessage,"%d",Error[0]);
  lstrcpyA(ErrorMessageURL,"readme.rtf");
 }else{
  int nErrorIndex=0;
  while(nErrorIndex<10&&Error[nErrorIndex]){
   char szCurrentError[1024];
   url=nErrorUrlStringFromErrorCode(Error[nErrorIndex]);
   int id=nErrorStringFromErrorCode(Error[nErrorIndex]);
   if(!RetrieveString(hStrings,id,szCurrentError,sizeof(szCurrentError)))
    sprintf(szCurrentError,"Missing error string %d",id);
   if(nErrorIndex>0){
    lstrcatA(ErrorMessage,"\r\n\r\n");
    lstrcatA(ErrorMessage,szCurrentError);
   }else lstrcpyA(ErrorMessage,szCurrentError);
   ++nErrorIndex;
  }
  if(!RetrieveString(hStrings,url,ErrorMessageURL,sizeof(ErrorMessageURL)))
   sprintf(ErrorMessageURL,"readme.rtf");
 }
 if(!RetrieveString(hStrings,ForceExit?228:227,ErrorMessageTitle,sizeof(ErrorMessageTitle)))
  sprintf(ErrorMessageTitle,"Fable The Lost Chapters - Error");
 GlobalForceExit=ForceExit;
 if(AppendText){
  strcat(ErrorMessage," (");strcat(ErrorMessage,AppendText);strcat(ErrorMessage,")");AppendText=0;
 }
 if(!ForceExit){
  char Data[16];char Buffer[256];HKEY key;DWORD DataSize;
  sprintf(Buffer,"%s %s (0x%04x):%d",pConfig->VendorName,pConfig->DeviceName,pConfig->DeviceID,Error[0]);
  RegOpenKeyExA((HKEY)0x80000001,"Software\\Microsoft\\Microsoft Games\\Fable TLC",0,0x20019,&key);
  DataSize=sizeof(Data);
  Data[0]=0;
  RegQueryValueExA(key,Buffer,0,0,(BYTE*)Data,&DataSize);RegCloseKey(key);
  if(Data[0]=='y'){optionSafe=Data[1]-'0';return optionSafe;}
 }
 HWND hWnd=window_globals.hWnd;
 if(!hWnd){
  WNDCLASSEXA wcex;memset(&wcex,0,sizeof(wcex));wcex.cbSize=sizeof(wcex);
  wcex.lpfnWndProc=DefWindowProcA;wcex.hInstance=window_globals.hInstance;
  wcex.hIcon=LoadIconA(window_globals.hInstance,(const char*)107);
  wcex.hCursor=LoadCursorA(0,(const char*)32512);
  wcex.lpszClassName="Fable - The Lost Chapters";
  RegisterClassExA(&wcex);
  hWnd=CreateWindowExA(0,"Fable - The Lost Chapters","Fable - The Lost Chapters",0x80000000,0x80000000,0x80000000,0x80000000,0x80000000,0,0,window_globals.hInstance,0);
  ShowWindow(hWnd,5);
 }
 ShowCursor(1);
 int result=LocalizedDialogBox(hStrings,(const char*)202,hWnd,ErrorDialogProc);
 ShowCursor(0);
 if(ForceExit||result==2){
  ClearDirtyExitFlag();
  __try {} __except(1) {}
  exit(1);
 }
 if(window_globals.hWnd)ShowWindow(window_globals.hWnd,5);
 else{DestroyWindow(hWnd);UnregisterClassA("SAMPLEAPP",window_globals.hInstance);}
 if(DontShowAgain){
  char Data[16];char Buffer[256];HKEY key;
  sprintf(Buffer,"%s %s (0x%04x):%d",pConfig->VendorName,pConfig->DeviceName,pConfig->DeviceID,Error[0]);
  RegCreateKeyExA((HKEY)0x80000001,"Software\\Microsoft\\Microsoft Games\\Fable TLC",0,0,0,0x20006,0,&key,0);
  Data[0]='y';Data[1]=(char)result+'0';Data[2]=0;
  RegSetValueExA(key,Buffer,0,1,(BYTE*)Data,lstrlenA(Data)+1);RegCloseKey(key);
  if(Data[0]){optionSafe=Data[1]-'0';return optionSafe;}
 }
 if(result)optionSafe=1;
 return result;
}
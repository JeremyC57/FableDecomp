#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __try if(true)
#define __except(x) else
#endif
#define __declspec(x)
static jmp_buf exit_jump;
static int exit_seen,exit_code;
static void test_exit(int code){exit_seen=1;exit_code=code;longjmp(exit_jump,1);}
#define exit test_exit



#ifndef FABLE_ENGINE_ERRORDIALOG_H
#define FABLE_ENGINE_ERRORDIALOG_H
#ifndef FABLE_ENGINE_CONFIGDETECTION_H
#define FABLE_ENGINE_CONFIGDETECTION_H


struct _GUID { unsigned int Data1;unsigned short Data2,Data3;unsigned char Data4[8]; };
struct SOUND_DEVICE {
public:
    char name[64];
    _GUID Guid;
    unsigned int DriverVersionLowPart;
    unsigned int DriverVersionHighPart;
    unsigned int VendorID;
    unsigned int DeviceID;
    unsigned int SubSysID;
    unsigned int Revision;
};
struct CONFIGTABLE {
public:
    const char *Property[50];
};
class ConfigDetection {
public:
    unsigned int cf_ForceShader;
    unsigned int cf_MaximumResolution;
    unsigned int cf_LinearTextureAddressing;
    unsigned int cf_LinearTextureAddressingZoom;
    unsigned int cf_LinearTextureAddressingSun;
    unsigned int cf_UseFixedFunction;
    unsigned int cf_DisableDriverManagement;
    unsigned int cf_DisableSpecular;
    unsigned int cf_PrototypeCard;
    unsigned int cf_UnsupportedVideoCard;
    unsigned int cf_UnsupportedAudioCard;
    unsigned int cf_DisableRenderTargets;
    unsigned int cf_DisableAlphaRenderTargets;
    unsigned int cf_UseAlternateConvolveMask;
    unsigned int cf_MinMaxBlendOpIsBroken;
    unsigned int cf_OldVideoDriver;
    unsigned int cf_EnableStopStart;
    unsigned int cf_HeadRelativeSpeech;
    unsigned int cf_OldSoundDriver;
    unsigned int cf_DisableBuffering;
    unsigned int cf_InvalidDriver;
    unsigned int cf_InvalidSoundDriver;
    unsigned int cf_SafeMode;
    unsigned int cf_UseAnisotropicFilter;
    float cf_DecalZBiasValue;
    float cf_DecalSlopeZBiasValue;
    float cf_TransparentDecalZBiasValue;
    float cf_TransparentDecalSlopeZBiasValue;
    unsigned int cf_UseAlternateRenderTargetClear;
    unsigned int cf_UseAlternateOutlineEffect;
    unsigned int cf_DisableStretchRect;
    unsigned int cf_UseAlternateSkyPixelShader;
    unsigned int cf_DisableDisplacementEffect;
    unsigned int cf_DisableReflectiveWater;
    unsigned int cf_DisableFilteredScreenGUIBackground;
    unsigned int cf_DisableTextureTransforms;
    unsigned int cf_MaxEffectsDetail;
    unsigned int cf_MaxShadowDetail;
    unsigned int cf_MaxMeshDetail;
    unsigned int cf_MaxTextureDetail;
    unsigned int cf_DefaultEffectsDetail;
    unsigned int cf_DefaultShadowDetail;
    unsigned int cf_DefaultMeshDetail;
    unsigned int cf_DefaultTextureDetail;
    unsigned int cf_MaxAntiAliasLevel;
    unsigned int cf_HardStop;
    unsigned int cf_GenericFlag0;
    unsigned int cf_GenericFlag1;
    unsigned int cf_GenericFlag2;
    char * VendorName;
    char * DeviceName;
    unsigned int DeviceID;
    unsigned int VendorID;
    unsigned int DriverVersionLowPart;
    unsigned int DriverVersionHighPart;
    unsigned int m_SysMemory;
    unsigned int m_CpuSpeed;
    unsigned int m_VideoMemory;
    char * AppendText;
    int optionNoVideo;
    int optionSafe;
    SOUND_DEVICE Sound_devices[10];
    unsigned int Sound_currentDevice;
    unsigned int Sound_maxDevices;
    CONFIGTABLE ConfigTable[50];
    unsigned int req_CPUSpeed;
    unsigned int req_Memory;
    unsigned int req_VideoMemory;
    unsigned int req_DirectXVersion;
    unsigned int req_DirectXRevision;
    unsigned int req_DiskSpace;
private:
    bool m_ErrorOccurred;
    char * m_ErrorString;
};
#endif



typedef unsigned int DWORD;typedef unsigned int UINT;typedef int BOOL;
typedef unsigned short WORD,ATOM;typedef unsigned char BYTE;typedef int LPARAM,LRESULT;
typedef unsigned int WPARAM;typedef unsigned int REGSAM;
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
__declspec(dllimport) int __stdcall RegOpenKeyExA(HKEY,const char*,DWORD,REGSAM,HKEY*);
__declspec(dllimport) int __stdcall RegQueryValueExA(HKEY,const char*,DWORD*,DWORD*,BYTE*,DWORD*);
__declspec(dllimport) int __stdcall RegCloseKey(HKEY);
__declspec(dllimport) int __stdcall RegCreateKeyExA(HKEY,const char*,DWORD,char*,DWORD,REGSAM,_SECURITY_ATTRIBUTES*,HKEY*,DWORD*);
__declspec(dllimport) int __stdcall RegSetValueExA(HKEY,const char*,DWORD,DWORD,const BYTE*,DWORD);
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
int __fastcall RetrieveStringByLanguage(HINSTANCE,unsigned int,char*,unsigned int,unsigned short);
extern "C" int __fastcall RetrieveString(HINSTANCE,unsigned int,char*,unsigned int);
extern "C" int __fastcall LocalizedDialogBox(HINSTANCE,const char*,HWND,DLGPROC);
int __stdcall ErrorDialogProc(HWND,UINT,WPARAM,LPARAM);
extern "C" void __fastcall ClearDirtyExitFlag();
extern "C" int __stdcall DisplayErrorDialog(int*,int);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
extern "C" int __fastcall RetrieveString(HINSTANCE hModule,unsigned int dwStringID,char* pBuffer,unsigned int dwBufferSize)
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

#undef exit
ERRORARRAYSTRUCT ErrorArray[44];
char ErrorMessage[1024],ErrorMessageTitle[128],ErrorMessageURL[512];
int GlobalForceExit;
unsigned int DontShowAgain;
unsigned short gApplicationLanguage;
ConfigDetection* pConfig;
window_data window_globals;
extern "C" {HINSTANCE hStrings;char* AppendText;int optionSafe;}
static int mode,remembered,dialog_result,opened,queried,created_key,saved,closed_key;
static int created_window,destroyed_window,unregistered,shown,cursor_balance,dialog_calls,cleared_dirty;
static char registry_name[256],registry_data[16];
static unsigned int lang_calls[2],load_calls;
static const char* resource(unsigned int id){
 switch(id){case 111:return "error-A";case 112:return "error-B";case 113:return "error-C";
 case 211:return "url-A";case 212:return "url-B";case 213:return "url-C";
 case 227:case 228:return "caption";default:return 0;}
}
int __fastcall RetrieveStringByLanguage(HINSTANCE module,unsigned int id,char* out,unsigned int length,unsigned short language){
 assert(module==hStrings&&length>0);++lang_calls[language==0x409?1:0];
 if(mode==3||mode==2||(mode==1&&language!=0x409))return 0;
 const char* text=resource(id);if(!text)return 0;assert(strlen(text)<length);strcpy(out,text);return(int)strlen(text);
}
extern "C" int __stdcall LoadStringA(HINSTANCE module,UINT id,char* out,int length){
 assert(module==hStrings);++load_calls;if(mode!=2)return 0;
 const char* text=resource(id);if(!text)return 0;assert((int)strlen(text)<length);strcpy(out,text);return(int)strlen(text);
}
extern "C" int __cdecl wsprintfA(char* out,const char* format,...){
 assert(strcmp(format,"%d")==0);strcpy(out,"-1");return 2;
}
extern "C" char* __stdcall lstrcpyA(char* dst,const char* src){return strcpy(dst,src);}
extern "C" char* __stdcall lstrcatA(char* dst,const char* src){return strcat(dst,src);}
extern "C" int __stdcall lstrlenA(const char* src){return(int)strlen(src);}
extern "C" int __stdcall RegOpenKeyExA(HKEY key,const char* path,DWORD zero,REGSAM access,HKEY* out){
 assert(key==(HKEY)0x80000001&&zero==0&&access==0x20019);
 assert(strcmp(path,"Software\\Microsoft\\Microsoft Games\\Fable TLC")==0);*out=(HKEY)0x40;++opened;return 0;
}
extern "C" int __stdcall RegQueryValueExA(HKEY key,const char* value,DWORD* reserved,DWORD* type,BYTE* data,DWORD* length){
 assert(key==(HKEY)0x40&&!reserved&&!type&&*length==16);strcpy(registry_name,value);++queried;
 if(remembered>=0){data[0]='y';data[1]=(BYTE)('0'+remembered);data[2]=0;*length=3;}
 return remembered>=0?0:2;
}
extern "C" int __stdcall RegCloseKey(HKEY key){assert(key==(HKEY)0x40||key==(HKEY)0x44);++closed_key;return 0;}
extern "C" int __stdcall RegCreateKeyExA(HKEY key,const char* path,DWORD reserved,char* cls,DWORD options,REGSAM access,_SECURITY_ATTRIBUTES* sec,HKEY* out,DWORD* disposition){
 assert(key==(HKEY)0x80000001&&!reserved&&!cls&&!options&&access==0x20006&&!sec&&!disposition);
 assert(strcmp(path,"Software\\Microsoft\\Microsoft Games\\Fable TLC")==0);*out=(HKEY)0x44;++created_key;return 0;
}
extern "C" int __stdcall RegSetValueExA(HKEY key,const char* value,DWORD reserved,DWORD type,const BYTE* data,DWORD length){
 assert(key==(HKEY)0x44&&!reserved&&type==1&&length==3);strcpy(registry_name,value);memcpy(registry_data,data,length);++saved;return 0;
}
extern "C" HICON __stdcall LoadIconA(HINSTANCE instance,const char* id){assert(instance==window_globals.hInstance&&id==(const char*)107);return(HICON)0x60;}
extern "C" HCURSOR __stdcall LoadCursorA(HINSTANCE instance,const char* id){assert(!instance&&id==(const char*)32512);return(HCURSOR)0x64;}
extern "C" LRESULT __stdcall DefWindowProcA(HWND,UINT,WPARAM,LPARAM){abort();}
extern "C" ATOM __stdcall RegisterClassExA(const WNDCLASSEXA* value){
 assert(value->cbSize==sizeof(*value)&&value->style==0&&value->lpfnWndProc==DefWindowProcA);
 assert(!value->cbClsExtra&&!value->cbWndExtra&&value->hInstance==window_globals.hInstance);
 assert(value->hIcon==(HICON)0x60&&value->hCursor==(HCURSOR)0x64&&!value->hbrBackground&&!value->lpszMenuName&&!value->hIconSm);
 assert(strcmp(value->lpszClassName,"Fable - The Lost Chapters")==0);return 1;
}
extern "C" HWND __stdcall CreateWindowExA(DWORD ex,const char* cls,const char* title,DWORD style,int x,int y,int w,int h,HWND parent,HMENU menu,HINSTANCE instance,void* param){
 assert(!ex&&style==0x80000000&&x==(int)0x80000000&&y==x&&w==x&&h==x&&!parent&&!menu&&!param);
 assert(instance==window_globals.hInstance&&strcmp(cls,title)==0&&strcmp(cls,"Fable - The Lost Chapters")==0);++created_window;return(HWND)0x80;
}
extern "C" BOOL __stdcall ShowWindow(HWND window,int cmd){assert((window==(HWND)0x80||window==window_globals.hWnd)&&cmd==5);++shown;return 1;}
extern "C" int __stdcall ShowCursor(BOOL show){cursor_balance+=show?1:-1;assert(cursor_balance>=0);return cursor_balance;}
extern "C" BOOL __stdcall DestroyWindow(HWND window){assert(window==(HWND)0x80);++destroyed_window;return 1;}
extern "C" BOOL __stdcall UnregisterClassA(const char* name,HINSTANCE instance){assert(strcmp(name,"SAMPLEAPP")==0&&instance==window_globals.hInstance);++unregistered;return 1;}
int __stdcall ErrorDialogProc(HWND,UINT,WPARAM,LPARAM){abort();}
extern "C" int __fastcall LocalizedDialogBox(HINSTANCE instance,const char* id,HWND window,DLGPROC proc){
 assert(instance==hStrings&&id==(const char*)202&&proc==ErrorDialogProc);
 assert(window==(window_globals.hWnd?window_globals.hWnd:(HWND)0x80));++dialog_calls;return dialog_result;
}
extern "C" void __fastcall ClearDirtyExitFlag(){++cleared_dirty;}
static int run_case(int* errors,int force){
 if(setjmp(exit_jump))return -100;
 return DisplayErrorDialog(errors,force);
}
int main(){
 ConfigDetection config;memset(&config,0,sizeof(config));config.VendorName=(char*)"vendor";config.DeviceName=(char*)"device";config.DeviceID=0x1234;pConfig=&config;
 for(unsigned int i=0;i<44;++i){ErrorArray[i].nErrorCode=-1;ErrorArray[i].nErrorStringID=999;ErrorArray[i].nErrorURLStringID=999;}
 for(unsigned int i=0;i<3;++i){ErrorArray[i].nErrorCode=10+i;ErrorArray[i].nErrorStringID=111+i;ErrorArray[i].nErrorURLStringID=211+i;}
 unsigned int cases=0;
 for(mode=0;mode<4;++mode)for(int language=0;language<2;++language)for(int shape=0;shape<4;++shape)for(int extra=0;extra<2;++extra)for(int scenario=0;scenario<8;++scenario){
  int errors[11];memset(errors,0,sizeof(errors));int count=shape==0?0:shape==1?1:shape==2?3:10;
  if(shape==0)errors[0]=-1;else for(int j=0;j<count;++j)errors[j]=10+j%3;
  int force=scenario==4;remembered=scenario==6?0:scenario==7?3:-1;
  dialog_result=scenario==0?0:scenario==2?3:scenario==5?2:1;
  DontShowAgain=(scenario==1||scenario==2||scenario==6)?1:0;
  bool existing=scenario==0||scenario==1||scenario==4||scenario==6;
  window_globals.hWnd=existing?(HWND)0x84:0;window_globals.hInstance=(HINSTANCE)0x20;hStrings=(HINSTANCE)0x24;
  gApplicationLanguage=language?0x409:0x407;AppendText=extra?(char*)"detail":0;optionSafe=0x7f;
  memset(ErrorMessage,0xcc,sizeof(ErrorMessage));memset(ErrorMessageTitle,0xcc,sizeof(ErrorMessageTitle));memset(ErrorMessageURL,0xcc,sizeof(ErrorMessageURL));
  opened=queried=created_key=saved=closed_key=created_window=destroyed_window=unregistered=shown=cursor_balance=dialog_calls=cleared_dirty=exit_seen=exit_code=0;
  lang_calls[0]=lang_calls[1]=load_calls=0;registry_name[0]=registry_data[0]=0;
  int got=run_case(errors,force);bool early=!force&&remembered>=0;bool terminated=!early&&(force||dialog_result==2);
  assert(GlobalForceExit==force&&AppendText==0&&cursor_balance==0);
  char expected[1024];expected[0]=0;
  if(shape==0)strcpy(expected,"-1");else for(int j=0;j<count;++j){
   if(j)strcat(expected,"\r\n\r\n");char part[64];if(mode==3)sprintf(part,"Missing error string %d",111+j%3);else strcpy(part,resource(111+j%3));strcat(expected,part);
  }
  if(extra)strcat(expected," (detail)");assert(strcmp(ErrorMessage,expected)==0);
  if(strcmp(ErrorMessageTitle,mode==3?"Fable The Lost Chapters - Error":"caption")!=0)fprintf(stderr,"case %d %d %d %d %d title=%s\n",mode,language,shape,extra,scenario,ErrorMessageTitle);
  assert(strcmp(ErrorMessageTitle,mode==3?"Fable The Lost Chapters - Error":"caption")==0);
  const char* url=(shape==0||mode==3)?"readme.rtf":resource(211+(count-1)%3);assert(strcmp(ErrorMessageURL,url)==0);
  assert(opened==(!force)&&queried==(!force));
  if(!force){char key[256];sprintf(key,"vendor device (0x1234):%d",errors[0]);assert(strcmp(registry_name,key)==0);}
  if(early){assert(got==remembered&&optionSafe==remembered&&!dialog_calls&&!created_window&&!exit_seen&&!saved);}
  else{
   assert(dialog_calls==1&&created_window==(!existing));
   if(terminated){assert(got==-100&&exit_seen&&exit_code==1&&cleared_dirty==1&&!destroyed_window&&!saved);}
   else{
    assert(!exit_seen&&got==dialog_result&&destroyed_window==(!existing)&&unregistered==(!existing));
    if(DontShowAgain){assert(created_key==1&&saved==1&&registry_data[0]=='y'&&registry_data[1]=='0'+dialog_result&&registry_data[2]==0&&optionSafe==dialog_result);}
    else assert(optionSafe==(dialog_result?1:0x7f));
   }
  }
  assert(closed_key==opened+created_key);++cases;
 }
 assert(cases==512);puts("0099dcb0_TEST PASS");return 0;
}
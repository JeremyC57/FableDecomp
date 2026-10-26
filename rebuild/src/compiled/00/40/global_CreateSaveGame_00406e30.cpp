// 00406e30 CreateSaveGame
// Flags: /O2 /Oy /W3
// Leaf: CreateSaveGame
#include <wchar.h>
extern "C" int __cdecl _wchdir(const wchar_t*);
class CCharString;
class CWideString{ public:void*PStringData;CWideString(const wchar_t*);CWideString(const CWideString&);~CWideString();operator const wchar_t*();};
// ABI: GetValidProfileName 0x00406350 forwards EDX to the CWideString copy ctor 0x0099b720; callers pass profile+0x8 wrapper.
CWideString __fastcall GetValidProfileName(const CWideString&);
namespace NResourceDirectoryNames{CWideString __fastcall GetSaveDir();}
CWideString __fastcall operator+(const CWideString&,const CWideString&);CWideString __fastcall operator+(const CWideString&,const wchar_t*);
class CAFile{ public:static bool __fastcall PathExists(const CWideString&);};
class CDriveManager{ public:long CreateDirectoryW(const CWideString&);};
struct CDriveManagerPointer{CDriveManager*Object;long*Count;CDriveManager*operator->()const{return Object;}};extern CDriveManagerPointer g_pDriveManager;
long __fastcall CreateSaveGame(const CCharString&,const CWideString&profile,long mode,long unused,wchar_t*out,long capacity){
 CWideString valid=GetValidProfileName(profile);CWideString path=NResourceDirectoryNames::GetSaveDir()+valid+L"\\";
 wcsncpy(out,path,capacity-1);out[capacity-1]=0;
 if(mode==3){bool missing;{CWideString local(out);missing=!CAFile::PathExists(local);}if(missing)return 3;}
 else{bool failed;{CWideString local(out);failed=g_pDriveManager->CreateDirectoryW(local)!=1;}if(failed)return 15;_wchdir(L"..");}
 return 0;
}

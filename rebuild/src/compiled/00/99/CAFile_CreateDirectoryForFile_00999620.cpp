// 00999620 CAFile::CreateDirectoryForFile
// Flags: /O2 /Oy /W3 /Gr /GS
// Leaf: CreateDirectoryForFile
#include <stdlib.h>
#include <new>
namespace ExternalRetry_00999620 {
// Layout: 99b220 reads begin/end pointers;99b510 decrements refs+0xc, frees begin then scalar deletes Buffer.
struct CWideStringData {unsigned short* text,*last,*limit;int refs;};
extern int WideInstances; // 013bca20
// Layout: wide string one dword; drive[3],dir/name/ext[256]; target frame0x620 confirmed by author006 full-span parity.
struct CWideString {CWideStringData* data;CWideStringData* AllocStringData(const unsigned short*,int);__declspec(noinline) CWideString();__declspec(noinline) CWideString(const unsigned short*);__declspec(noinline) CWideString(const CWideString&);__declspec(noinline) ~CWideString();__declspec(noinline) CWideString& operator=(const unsigned short*);operator const unsigned short*();};
__declspec(noinline) CWideString::CWideString():data(0){++WideInstances;}
__declspec(noinline) CWideString::CWideString(const CWideString&x):data(0){++WideInstances;if(this!=&x&&x.data){data=x.data;++data->refs;}}
__declspec(noinline) CWideString::CWideString(const unsigned short*p):data(0){++WideInstances;if(p&&*p)data=AllocStringData(p,-1);}
__declspec(noinline) CWideString::~CWideString(){if(data){if(--data->refs<=0){if(data->text)free(data->text);delete data;}data=0;}--WideInstances;}
__declspec(noinline) CWideString& CWideString::operator=(const unsigned short*p){if(data){if(--data->refs<=0){if(data->text)free(data->text);delete data;}data=0;}if(p&&*p)data=AllocStringData(p,-1);return *this;}
// Names: bound donor11606 GetCurrentWorkingDirectory=997f80; donor18663 ExpandPath=998c90.
// AllocStringData=99b3c0; conversion operator=99af00; CAFile::PathExists=999230; CreateDirectoryW=9994a0.
CWideString __fastcall GetCurrentWorkingDirectory();CWideString __fastcall ExpandPath(const CWideString&,const CWideString&);CWideString __fastcall MakePath(const CWideString&,const CWideString&,const CWideString&,const CWideString&);
void __cdecl _wsplitpath(const unsigned short*,unsigned short*,unsigned short*,unsigned short*,unsigned short*);
// Layout: 00997780 calls Text on all four inputs then CRT _wmakepath into MAX_PATH260-word buffer (cookie+0x208) before wide construction.
void __cdecl _wmakepath(unsigned short*,const unsigned short*,const unsigned short*,const unsigned short*,const unsigned short*);
__declspec(noinline) CWideString __fastcall MakePath(const CWideString& d,const CWideString& p,const CWideString& n,const CWideString& e){unsigned short buffer[260];_wmakepath(buffer,static_cast<const unsigned short*>(const_cast<CWideString&>(d)),static_cast<const unsigned short*>(const_cast<CWideString&>(p)),static_cast<const unsigned short*>(const_cast<CWideString&>(n)),static_cast<const unsigned short*>(const_cast<CWideString&>(e)));return CWideString(buffer);}
__forceinline CWideString MakeParentDirectory(const CWideString& drive,const CWideString& dir){CWideString extension(L""),name(L"");return MakePath(drive,dir,name,extension);}
struct CAFile{static bool __fastcall PathExists(const CWideString&);static void __fastcall CreateDirectoryW(const CWideString&);static void __fastcall CreateDirectoryForFile(const CWideString&);};
void __fastcall CAFile::CreateDirectoryForFile(const CWideString& input){CWideString path=ExpandPath(input,GetCurrentWorkingDirectory());if(!PathExists(path)){CWideString drive,dir;{unsigned short d[3],folder[256],name[256],ext[256];_wsplitpath(static_cast<const unsigned short*>(path),d,folder,name,ext);drive=d;dir=folder;}CWideString parent=MakeParentDirectory(drive,dir);if(!PathExists(parent))CreateDirectoryW(parent);}}
}
// 009994a0 CAFile::CreateDirectoryW
// Flags: /O2 /Oy /W3 /Gr /GS
// Leaf: CreateDirectoryW
#include <stdlib.h>
namespace AFileSupport {
struct CWideStringData {unsigned short* text,*last,*limit;int refs;};
extern int WideInstances; // 013bca20
// Layout: wide string one dword; drive[3],dir/name/ext[256],component[262] (retail frame: path+0, drive+4, cwd temp/d+8, component+0x10, ext+0x21c, dir+0x41c, name+0x61c); retail target frame0x820.
struct CWideString {CWideStringData* data;CWideStringData* AllocStringData(const unsigned short*,int);__declspec(noinline) CWideString();__declspec(noinline) CWideString(const unsigned short*);__declspec(noinline) CWideString(const CWideString&);__declspec(noinline) ~CWideString();__declspec(noinline) CWideString& operator=(const unsigned short*); operator const unsigned short*();__declspec(noinline) operator const unsigned short*()const{static const unsigned short empty[]={0};return data?data->text:empty;}};
__declspec(noinline) CWideString::CWideString():data(0){++WideInstances;}
__declspec(noinline) CWideString::CWideString(const CWideString&x):data(0){++WideInstances;if(this!=&x&&x.data){data=x.data;++data->refs;}}
__declspec(noinline) CWideString::CWideString(const unsigned short*p):data(0){++WideInstances;if(p&&*p)data=AllocStringData(p,-1);}
__declspec(noinline) CWideString::~CWideString(){if(data){if(--data->refs<=0){if(data->text)free(data->text);delete data;}data=0;}--WideInstances;}
__declspec(noinline) CWideString& CWideString::operator=(const unsigned short*p){if(data){if(--data->refs<=0){if(data->text)free(data->text);delete data;}data=0;}if(p&&*p)data=AllocStringData(p,-1);return *this;}
__declspec(noinline) void __cdecl _wsplitpath(const unsigned short* path,unsigned short* drive,unsigned short* dir,unsigned short* name,unsigned short* ext){if(path[0]&&path[1]==58){if(drive){drive[0]=path[0];drive[1]=58;drive[2]=0;}path+=2;}else if(drive)*drive=0;const unsigned short*lastSlash=0,*dot=0,*p=path;for(;*p;++p){if(*p==92||*p==47)lastSlash=p+1;else if(*p==46)dot=p;}if(lastSlash){if(dir){unsigned n=(unsigned)(lastSlash-path);if(n>255)n=255;for(unsigned i=0;i<n;++i)dir[i]=path[i];dir[n]=0;}path=lastSlash;}else if(dir)*dir=0;if(dot&&dot>=path){if(name){unsigned n=(unsigned)(dot-path);if(n>255)n=255;for(unsigned i=0;i<n;++i)name[i]=path[i];name[n]=0;}if(ext){unsigned n=(unsigned)(p-dot);if(n>255)n=255;for(unsigned i=0;i<n;++i)ext[i]=dot[i];ext[n]=0;}}else{if(name){unsigned n=(unsigned)(p-path);if(n>255)n=255;for(unsigned i=0;i<n;++i)name[i]=path[i];name[n]=0;}if(ext)*ext=0;}}
CWideString __fastcall GetCurrentWorkingDirectory();CWideString __fastcall ExpandPath(const CWideString&,const CWideString&);
int __cdecl _getdrive();int __cdecl _chdrive(int);int __cdecl _wchdir(const unsigned short*);int __cdecl _wmkdir(const unsigned short*);
namespace NFile{__declspec(noinline) bool __fastcall GetItemFromPath(const unsigned short* text,unsigned long index,unsigned short* out){out[0]=0;int count=0,at=0;for(;text[at];++at)if(text[at]==92&&++count>index)break;if(!text[at])return 0;if(at==0||text[at-1]==58){out[0]=92;out[1]=0;return 1;}int first=at-1;for(;first!=-1;--first)if(text[first]==92||text[first]==58||first==0){++first;break;}if(first<0)first=0;int i=0;for(;first+i<at;++i)out[i]=text[first+i];out[i]=0;return 1;}}
// Layout: CWideString CWideStringData has first/end/capacity pointers and refs+12;99b220 computes length from span.
// Shape: drive is a function-scope local after path (its slot +4 precedes the inner-block arrays); the non-const
//   operator const unsigned short*() is only declared (retail 0x0099AF00 is out of TU; an in-TU body changes the frame).
// Nonconst/const wide-character conversions preserve99af00(path) and99af10(drive) identities.
// Three256-word split buffers, MAX_PATH260 component and _MAX_DRIVE3;997880 emits only backslash-terminated components.
namespace NFile{__forceinline long __fastcall ConvertDriveLetterToNumber(wchar_t c){if(c>='a'&&c<='z')return c-96;if(c>='A'&&c<='Z')return c-64;return 0;}}
__forceinline CWideString ResolveExpandPath(const CWideString& input){return ExpandPath(input,GetCurrentWorkingDirectory());}
}
// CAFile is global in retail; AFileSupport only isolates the in-TU string, split and CRT-shaped helper models.
class CAFile{ public:static void __fastcall CreateDirectoryW(const AFileSupport::CWideString&);};
void __fastcall CAFile::CreateDirectoryW(const AFileSupport::CWideString& input){AFileSupport::CWideString path=AFileSupport::ResolveExpandPath(input);AFileSupport::CWideString drive;{unsigned short d[3],dir[256],ext[256],name[256],part[262];AFileSupport::_wsplitpath(static_cast<const unsigned short*>(path),d,dir,name,ext);drive=d;const unsigned short& c=static_cast<const unsigned short*>(static_cast<const AFileSupport::CWideString&>(drive))[0];if(c){unsigned short n=(unsigned short)AFileSupport::NFile::ConvertDriveLetterToNumber((wchar_t)c);int previous=AFileSupport::_getdrive();AFileSupport::_chdrive(n);if(n){int index=0;while(AFileSupport::NFile::GetItemFromPath(static_cast<const unsigned short*>(path),index,part)==1){if(AFileSupport::_wchdir(part)){if(AFileSupport::_wmkdir(part)==-1)break;if(AFileSupport::_wchdir(part))break;}++index;}}if(previous!=-1)AFileSupport::_chdrive(previous);}}}

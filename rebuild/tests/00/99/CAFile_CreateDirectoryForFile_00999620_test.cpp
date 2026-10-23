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
#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace ExternalRetry_00999620 {
int WideInstances=0;static int allocations,creates,joins,currents,absolutes,checks;static int mode;static char created[300];
static unsigned Len(const unsigned short* p){unsigned n=0;while(p[n])++n;return n;}
CWideStringData* AllocateWide(const unsigned short* p){unsigned n=Len(p);CWideStringData*d=new CWideStringData;d->text=(unsigned short*)malloc((n+1)*2);for(unsigned i=0;i<=n;++i)d->text[i]=p[i];d->last=d->text+n;d->limit=d->last+1;d->refs=1;++allocations;return d;}
CWideStringData* AllocateNarrow(const char* p){unsigned short t[300];unsigned i=0;while(p[i]){t[i]=(unsigned char)p[i];++i;}t[i]=0;return AllocateWide(t);}
static const unsigned short cwd[]={67,58,92,98,97,115,101,92,0};
const unsigned short* CurrentText(){++currents;return cwd;}
const unsigned short* AbsoluteText(const unsigned short* p,const unsigned short* b){assert(Len(b)==8);++absolutes;static unsigned short out[300];unsigned i=0;if(p[1]!=58){for(;b[i];++i)out[i]=b[i];}unsigned j=0;do{out[i++]=p[j];}while(p[j++]);return out;}
const unsigned short* JoinText(const unsigned short* d,const unsigned short* p,const unsigned short* n,const unsigned short* e){assert(!n[0]&&!e[0]);++joins;static unsigned short out[600];unsigned i=0;for(unsigned j=0;d[j];++j)out[i++]=d[j];for(unsigned k=0;p[k];++k)out[i++]=p[k];out[i]=0;return out;}
bool DirectoryExists(const unsigned short* p){assert(p[0]);++checks;return mode==0||(mode==1&&checks%2==0);}
void MakeDirectory(const unsigned short* p){++creates;unsigned i=0;do{created[i]=(char)p[i];}while(p[i++]);}

CWideStringData* CWideString::AllocStringData(const unsigned short*p,int n){assert(n==-1);return AllocateWide(p);}

CWideString::operator const unsigned short*(){static unsigned short empty[]={0};return data?data->text:empty;}
bool __fastcall CAFile::PathExists(const CWideString& path){return DirectoryExists(path.data->text);}
void __fastcall CAFile::CreateDirectoryW(const CWideString& path){MakeDirectory(path.data->text);}
// Fixture environment supplies a fixed cached current directory and restricted ordinary drive paths.
CWideString __fastcall GetCurrentWorkingDirectory(){CWideString r;r=CurrentText();return CWideString(r);}
CWideString __fastcall ExpandPath(const CWideString&p,const CWideString&b){CWideString r;r=AbsoluteText(p.data?p.data->text:cwd,b.data?b.data->text:cwd);return CWideString(r);}
void __cdecl _wmakepath(unsigned short* out,const unsigned short*d,const unsigned short*p,const unsigned short*n,const unsigned short*e){const unsigned short* text=JoinText(d,p,n,e);unsigned i=0;do{out[i]=text[i];}while(text[i++]);}
void __cdecl _wsplitpath(const unsigned short*text,unsigned short*d,unsigned short*p,unsigned short*n,unsigned short*e){unsigned start=0;if(text[1]==58){d[0]=text[0];d[1]=58;d[2]=0;start=2;}else d[0]=0;unsigned last=start;for(unsigned i=start;text[i];++i)if(text[i]==92)last=i+1;unsigned j=0;for(;start+j<last;++j)p[j]=text[start+j];p[j]=0;n[0]=e[0]=0;}

}
int main(){using namespace ExternalRetry_00999620;{CWideString input(L"sub\\file.bin");CAFile::CreateDirectoryForFile(input);assert(!creates&&currents==1&&absolutes==1&&WideInstances==1);mode=1;checks=0;CAFile::CreateDirectoryForFile(input);assert(!creates&&joins==1&&checks==2&&WideInstances==1);mode=2;checks=0;CAFile::CreateDirectoryForFile(input);assert(creates==1&&joins==2&&!strcmp(created,"C:\\base\\sub\\")&&WideInstances==1);CWideString absolute(L"D:\\dir\\x");CAFile::CreateDirectoryForFile(absolute);assert(creates==2&&!strcmp(created,"D:\\dir\\"));{CWideString copy(input);assert(copy.data==input.data&&input.data->refs==2);unsigned short changed[]={88,0};input=changed;assert(copy.data!=input.data&&copy.data->refs==1&&input.data->refs==1&&static_cast<const unsigned short*>(copy)[0]==115&&static_cast<const unsigned short*>(input)[0]==88);}assert(currents==4&&absolutes==4&&WideInstances==2);}assert(WideInstances==0);puts("00999620_TEST PASS");return 0;}
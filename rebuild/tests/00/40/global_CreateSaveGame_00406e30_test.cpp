// 00406e30 CreateSaveGame
// Flags: /O2 /Oy /W3
// Leaf: CreateSaveGame
#include <wchar.h>
extern "C" int __cdecl _wchdir(const wchar_t*);
class CCharString;
class CWideString{ public:void*PStringData;CWideString(const wchar_t*);CWideString(const CWideString&);~CWideString();operator const wchar_t*() const;};
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


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
// CWideString data: text at0x0, reference count at0x0c (0099b720/0099b510).
struct WideData{wchar_t*text;unsigned length,capacity;int refs;};
class CCharString{ public:int unused;};static CCharString none;static int live,code,created,changed;static bool exists;static const CWideString*expectedProfile;
CWideString::CWideString(const wchar_t*s):PStringData(0){if(s&&*s){WideData*d=new WideData;d->length=wcslen(s);d->capacity=d->length+1;d->text=(wchar_t*)malloc(d->capacity*sizeof(wchar_t));wcscpy(d->text,s);d->refs=1;PStringData=d;++live;}}
CWideString::CWideString(const CWideString&w):PStringData(w.PStringData){if(PStringData)++((WideData*)PStringData)->refs;}
CWideString::~CWideString(){if(PStringData){WideData*d=(WideData*)PStringData;assert(d->refs>0);if(!--d->refs){free(d->text);delete d;--live;}PStringData=0;}}
CWideString::operator const wchar_t*() const{return PStringData?((WideData*)PStringData)->text:L"";}
CWideString __fastcall GetValidProfileName(const CWideString&s){assert(&s==expectedProfile);CWideString shared(s);assert(shared.PStringData==s.PStringData);wchar_t text[128];wcscpy(text,shared);bool different=false;for(unsigned i=0;text[i];++i)if(wcschr(L"\\/:*?\"<>|",text[i])){text[i]=L'_';different=true;}if(different)return CWideString(text);return CWideString(shared);}
namespace NResourceDirectoryNames{CWideString __fastcall GetSaveDir(){return CWideString(L"root/");}}
CWideString __fastcall operator+(const CWideString&a,const CWideString&b){wchar_t text[256];wcscpy(text,const_cast<CWideString&>(a));wcscat(text,const_cast<CWideString&>(b));return CWideString(text);}
CWideString __fastcall operator+(const CWideString&a,const wchar_t*b){wchar_t text[256];wcscpy(text,const_cast<CWideString&>(a));wcscat(text,b);return CWideString(text);}
bool __fastcall CAFile::PathExists(const CWideString&w){assert((const wchar_t*)const_cast<CWideString&>(w)!=0);return exists;}
long CDriveManager::CreateDirectoryW(const CWideString&w){assert((const wchar_t*)const_cast<CWideString&>(w)!=0);++created;return code;}
CDriveManager drive;CDriveManagerPointer g_pDriveManager={&drive,0};
extern "C" int __cdecl _wchdir(const wchar_t*p){assert(!wcscmp(p,L".."));++changed;return 0;}
int main(){assert(sizeof(CWideString)==4&&offsetof(WideData,refs)==0x0c);
 {CWideString profile(L"user");expectedProfile=&profile;WideData*data=(WideData*)profile.PStringData;assert(data->refs==1);
  {CWideString copy(profile);assert(copy.PStringData==profile.PStringData&&data->refs==2);{CWideString another(copy);assert(data->refs==3&&((const wchar_t*)another)==((const wchar_t*)profile));}assert(data->refs==2);}assert(data->refs==1);
  wchar_t out[64];exists=false;assert(CreateSaveGame(none,profile,3,0,out,64)==3&&!wcscmp(out,L"root/user\\"));assert(live==1&&data->refs==1);
  exists=true;assert(CreateSaveGame(none,profile,3,0,out,64)==0);code=0;assert(CreateSaveGame(none,profile,1,0,out,64)==15&&changed==0);code=1;assert(CreateSaveGame(none,profile,1,0,out,64)==0&&changed==1&&created==2);
  assert(CreateSaveGame(none,profile,3,0,out,1)==0&&out[0]==0);assert(CreateSaveGame(none,profile,3,0,out,5)==0&&!wcscmp(out,L"root"));assert(live==1&&data->refs==1&&!wcscmp(((const wchar_t*)profile),L"user"));
 }
 assert(live==0);
 {CWideString profile(L"a/b:c");expectedProfile=&profile;wchar_t out[64];assert(CreateSaveGame(none,profile,3,0,out,64)==0&&!wcscmp(out,L"root/a_b_c\\"));assert(!wcscmp(((const wchar_t*)profile),L"a/b:c")&&((WideData*)profile.PStringData)->refs==1);}
 {CWideString empty(L"");expectedProfile=&empty;wchar_t out[64];assert(!empty.PStringData);assert(CreateSaveGame(none,empty,3,0,out,64)==0&&!wcscmp(out,L"root/\\"));}
 assert(live==0);puts("00406e30_TEST PASS");
}
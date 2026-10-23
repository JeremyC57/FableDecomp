// 00406e30 Row00406e30::Create
// Flags: /O2 /Oy /W3 
// Leaf: Create
#include <wchar.h>
extern "C" int __cdecl _wchdir(const wchar_t*);
namespace Row00406e30 {
struct Wide{void*p;Wide(const wchar_t*);Wide(const Wide&);~Wide();const wchar_t*Get()const;};
// ABI: 0x00406350 forwards EDX to Wide copy ctor 0x0099b720; callers pass profile+0x8 wrapper.
Wide __fastcall Valid(const Wide&);Wide __fastcall SaveDir();Wide __fastcall Join(const Wide&,const Wide&);Wide __fastcall Append(const Wide&,const wchar_t*);bool __fastcall Exists(const Wide&);
struct Drive{int Create(const Wide&);};extern Drive*Manager;
int __fastcall Create(void*,const Wide&profile,int mode,int unused,wchar_t*out,int capacity){
 Wide valid=Valid(profile);Wide path=Append(Join(SaveDir(),valid),L"\\");
 wcsncpy(out,path.Get(),capacity-1);out[capacity-1]=0;
 if(mode==3){bool missing;{Wide local(out);missing=!Exists(local);}if(missing)return 3;}
 else{bool failed;{Wide local(out);failed=Manager->Create(local)!=1;}if(failed)return 15;_wchdir(L"..");}
 return 0;
}
}

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
namespace Row00406e30 {
// Wide data: text at0x0, reference count at0x0c (0099b720/0099b510).
struct WideData{wchar_t*text;unsigned length,capacity;int refs;};
static int live,code,created,changed;static bool exists;static const Wide*expectedProfile;
Wide::Wide(const wchar_t*s):p(0){if(s&&*s){WideData*d=new WideData;d->length=wcslen(s);d->capacity=d->length+1;d->text=(wchar_t*)malloc(d->capacity*sizeof(wchar_t));wcscpy(d->text,s);d->refs=1;p=d;++live;}}
Wide::Wide(const Wide&w):p(w.p){if(p)++((WideData*)p)->refs;}
Wide::~Wide(){if(p){WideData*d=(WideData*)p;assert(d->refs>0);if(!--d->refs){free(d->text);delete d;--live;}p=0;}}
const wchar_t*Wide::Get()const{return p?((WideData*)p)->text:L"";}
Wide __fastcall Valid(const Wide&s){assert(&s==expectedProfile);Wide shared(s);assert(shared.p==s.p);wchar_t text[128];wcscpy(text,shared.Get());bool different=false;for(unsigned i=0;text[i];++i)if(wcschr(L"\\/:*?\"<>|",text[i])){text[i]=L'_';different=true;}if(different)return Wide(text);return Wide(shared);}
Wide __fastcall SaveDir(){return Wide(L"root/");}
Wide __fastcall Join(const Wide&a,const Wide&b){wchar_t text[256];wcscpy(text,a.Get());wcscat(text,b.Get());return Wide(text);}
Wide __fastcall Append(const Wide&a,const wchar_t*b){wchar_t text[256];wcscpy(text,a.Get());wcscat(text,b);return Wide(text);}
bool __fastcall Exists(const Wide&w){assert(w.Get()!=0);return exists;}
int Drive::Create(const Wide&w){assert(w.Get()!=0);++created;return code;}
Drive drive;Drive*Manager=&drive;
}
extern "C" int __cdecl _wchdir(const wchar_t*p){assert(!wcscmp(p,L".."));++Row00406e30::changed;return 0;}
int main(){using namespace Row00406e30;assert(sizeof(Wide)==4&&offsetof(WideData,refs)==0x0c);
 {Wide profile(L"user");expectedProfile=&profile;WideData*data=(WideData*)profile.p;assert(data->refs==1);
  {Wide copy(profile);assert(copy.p==profile.p&&data->refs==2);{Wide another(copy);assert(data->refs==3&&another.Get()==profile.Get());}assert(data->refs==2);}assert(data->refs==1);
  wchar_t out[64];exists=false;assert(Create(0,profile,3,0,out,64)==3&&!wcscmp(out,L"root/user\\"));assert(live==1&&data->refs==1);
  exists=true;assert(Create(0,profile,3,0,out,64)==0);code=0;assert(Create(0,profile,1,0,out,64)==15&&changed==0);code=1;assert(Create(0,profile,1,0,out,64)==0&&changed==1&&created==2);
  assert(Create(0,profile,3,0,out,1)==0&&out[0]==0);assert(Create(0,profile,3,0,out,5)==0&&!wcscmp(out,L"root"));assert(live==1&&data->refs==1&&!wcscmp(profile.Get(),L"user"));
 }
 assert(live==0);
 {Wide profile(L"a/b:c");expectedProfile=&profile;wchar_t out[64];assert(Create(0,profile,3,0,out,64)==0&&!wcscmp(out,L"root/a_b_c\\"));assert(!wcscmp(profile.Get(),L"a/b:c")&&((WideData*)profile.p)->refs==1);}
 {Wide empty(L"");expectedProfile=&empty;wchar_t out[64];assert(!empty.p);assert(Create(0,empty,3,0,out,64)==0&&!wcscmp(out,L"root/\\"));}
 assert(live==0);puts("00406e30_TEST PASS");
}
// CEnvironment::BuildThemes() -- build theme resources from the full lighting-table path.
// Retail006b3350 165bytes. String calls0099b6b0/e480/b8d0/b510/9110/b7d0/b720; lookup00885f50, palette00888a60, delete[]00bfeb1c.
#include <stddef.h>
#include <new>
#ifdef _MSC_VER
#include "engine/CEnvironment.h"
#include "fable_string.h"
#define CEnvironmentDef CEnvironmentDefDonor
#include "engine/_quarantine/CEnvironmentDef.h"
#undef CEnvironmentDef
#else
#define __fastcall
// Host declarations mirror fable_string.h while avoiding its mandatory 32-bit ABI assertions.
class CWideString { public:
 void* Data;
 CWideString(const wchar_t*); //0099b6b0
 CWideString(const CWideString&); //0099b720
 ~CWideString(); //0099b510
 const CWideString& operator+=(const CWideString&); //0099b8d0
 CWideString& operator=(const CWideString&); //0099b7d0
};
class CCharString { public: void* Data; CWideString ToWideString() const; }; //0099e480
#endif
class CAFile { public: static CWideString __fastcall ConvertPathToFullPath(const CWideString&); }; //00999110 (LTCG result ecx,source edx)
// Donor CEnvironmentDef ColourLookupTexture is+40; native+3c reflects a four-byte base-layout difference.
class CEnvironmentDef { public: unsigned char beforeLookup[0x3c]; CCharString ColourLookupTexture; };
// Generated _quarantine/CGameDefinitionManager.h is unusable: sizeof1 contradicts its void* vptr. Native EnvironmentDef is audited+d8.
class CGameDefinitionManager { public: unsigned char beforeEnvironment[0xd8]; CEnvironmentDef* EnvironmentDef; };
// DDK <new> omits the ordinary array-delete declaration; declare it before the inline destructor.
void operator delete[](void*) throw();
namespace NEnvironmentInternal {
struct CEnvironmentLookupTexture {
 long Width,Height,Pitch; unsigned char* Data;
 CEnvironmentLookupTexture(CWideString); //00885f50
 ~CEnvironmentLookupTexture() { delete[] Data; }
};
class CEnvironmentThemeResourcePallette { public:
 void Build(const CGameDefinitionManager&,const CEnvironmentDef&,const CEnvironmentLookupTexture&); //00888a60
};
}
// Donor CEnvironment::ThemeSet is +1a7c, native +1954 due to smaller native theme game-state/STL layouts.
struct CEnvironmentNativeView {
 void* vtable; CGameDefinitionManager* DefinitionManager;
 unsigned char priorState[0x1954-8]; NEnvironmentInternal::CEnvironmentThemeResourcePallette ThemeSet;
 void BuildThemes();
};
void CEnvironmentNativeView::BuildThemes() {
 CEnvironmentDef* def=DefinitionManager->EnvironmentDef;
 CWideString path(L"data\\LightingTable\\");
 path+=def->ColourLookupTexture.ToWideString();
 path=CAFile::ConvertPathToFullPath(path);
 NEnvironmentInternal::CEnvironmentLookupTexture texture(path);
 ThemeSet.Build(*DefinitionManager,*def,texture);
}

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
static unsigned char* textureAllocation;
static int textureFrees,nullArrayDeletes;
static bool emptyTexture;
void* operator new[](size_t n) {return malloc(n?n:1);}
void operator delete[](void* p) throw() {
 if(!p) ++nullArrayDeletes;
 if(p && p==textureAllocation) {++textureFrees;textureAllocation=0;}
 free(p);
}
#ifndef _MSC_VER
class CWideStringData { public:wchar_t* text;};
#endif
static int live,failed,built;
static CEnvironmentNativeView* active;
static CGameDefinitionManager *initialManager,*nextManager;
static CEnvironmentDef* expectedDef;
static CWideStringData* Get(const CWideString* p) {CWideStringData* d;memcpy(&d,p,sizeof(d));return d;}
static void Set(CWideString* p,CWideStringData* d) {memcpy(p,&d,sizeof(d));}
static CWideStringData* New(const wchar_t* t) {
 CWideStringData* d=new CWideStringData;d->text=new wchar_t[wcslen(t)+1];wcscpy(d->text,t);++live;return d;
}
CWideString::CWideString(const wchar_t* t) {Set(this,New(t));}
CWideString::CWideString(const CWideString& x) {Set(this,New(Get(&x)->text));}
CWideString::~CWideString() {CWideStringData* d=Get(this);delete[] d->text;delete d;--live;}
CWideString& CWideString::operator=(const CWideString& x) {
 CWideStringData* d=New(Get(&x)->text);CWideStringData* old=Get(this);delete[] old->text;delete old;--live;Set(this,d);return *this;
}
const CWideString& CWideString::operator+=(const CWideString& x) {
 wchar_t t[256];wcscpy(t,Get(this)->text);wcscat(t,Get(&x)->text);CWideString n(t);*this=n;return *this;
}
#ifdef _MSC_VER
CCharString::CCharString() {}
CCharString::~CCharString() {}
#endif
CWideString CCharString::ToWideString() const {
 if(this!=&expectedDef->ColourLookupTexture) failed=1;
 active->DefinitionManager=nextManager;
 return CWideString(L"Day.bmp");
}
CWideString __fastcall CAFile::ConvertPathToFullPath(const CWideString& p) {
 if(wcscmp(Get(&p)->text,L"data\\LightingTable\\Day.bmp")) failed=1;
 wchar_t t[256];wcscpy(t,L"FULL:");wcscat(t,Get(&p)->text);return CWideString(t);
}
NEnvironmentInternal::CEnvironmentLookupTexture::CEnvironmentLookupTexture(CWideString path) {
 if(wcscmp(Get(&path)->text,L"FULL:data\\LightingTable\\Day.bmp")) failed=1;
 Width=2;Height=2;Pitch=2;Data=emptyTexture?0:new unsigned char[4];textureAllocation=Data;if(Data) {Data[0]=17;Data[3]=91;}
}
void NEnvironmentInternal::CEnvironmentThemeResourcePallette::Build(const CGameDefinitionManager& m,const CEnvironmentDef& d,const CEnvironmentLookupTexture& t) {
 ++built;if(this!=&active->ThemeSet || &m!=nextManager || &d!=expectedDef || t.Width!=2 || t.Height!=2 || t.Pitch!=2 || (emptyTexture?t.Data!=0:(!t.Data || t.Data[0]!=17 || t.Data[3]!=91))) failed=1;
}
int main() {
 CEnvironmentNativeView x;CGameDefinitionManager m,n;CEnvironmentDef d,e;
 active=&x;initialManager=&m;nextManager=&n;expectedDef=&d;m.EnvironmentDef=&d;n.EnvironmentDef=&e;
 if(sizeof(void*)==4 && (offsetof(CEnvironmentNativeView,ThemeSet)!=0x1954 || offsetof(CEnvironmentDef,ColourLookupTexture)!=0x3c || offsetof(CGameDefinitionManager,EnvironmentDef)!=0xd8)) return 3;
 for(int i=0;i<2;++i) {emptyTexture=(i==1);x.DefinitionManager=&m;x.BuildThemes();if(live || failed || textureAllocation || textureFrees!=1) {printf("debug i=%d live=%d failed=%d alloc=%p frees=%d null=%d\n",i,live,failed,textureAllocation,textureFrees,nullArrayDeletes);return 1;}}
#ifdef _MSC_VER
 if(nullArrayDeletes!=1) return 4;
#endif
 if(built!=2) return 2;
 puts("006b3350_TEST PASS");return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif

// CMapInfo::operator=(const CMapInfo&) -- memberwise world-map entry assignment.
// Retail00513BF0 length202; strings99EFB0; block destructionBFE9BC.
// No CMapInfo/C2DBoxI/CCPPointerInfo header exists, including _quarantine.
#pragma once

#include <stddef.h>

#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif


class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#pragma pack(pop)

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    long GetLength() const;
    const CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();
    CWideStringData* AllocStringData(
        const wchar_t* text,
        long length);

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

private:
    void UnassignString();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

void operator delete(void*) throw(); //00BFE9BC scalar CRT delete
class C2DBoxI { public: long TLX,TLY,BRX,BRY; };
class CCPPointerInfo { public: unsigned long RefCount; void(FABLE_FASTCALL* DeleteFunc)(void*); void* Data; };
template<class T> class CCountedPointer { public:
 T* Data; CCPPointerInfo* Info;
 CCountedPointer& operator=(const CCountedPointer& o){
  T* data=o.Data; CCPPointerInfo* info=o.Info;
  if(Info!=info){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=data;Info=info;if(Info)++Info->RefCount;}
  return *this;
 }
};
class CMap;
class CMapInfo { public:
 C2DBoxI Area; CCountedPointer<CMap> PMap; CCharString LevelName,LevelScriptName;
 unsigned long MapUID; bool Used,Placed,Activated,Loading,LoadedOnPlayerProximity;
 CCharString SerialiseScript;
#ifdef _MSC_VER
 unsigned __int64 LevStamp,TngStamp;
#else
 unsigned long long LevStamp,TngStamp;
#endif
 bool IsSea;
 CMapInfo& operator=(const CMapInfo&);
};
CMapInfo& CMapInfo::operator=(const CMapInfo& o){
 Area=o.Area;PMap=o.PMap;LevelName=o.LevelName;LevelScriptName=o.LevelScriptName;
 MapUID=o.MapUID;Used=o.Used;Placed=o.Placed;Activated=o.Activated;Loading=o.Loading;
 LoadedOnPlayerProximity=o.LoadedOnPlayerProximity;SerialiseScript=o.SerialiseScript;
 LevStamp=o.LevStamp;TngStamp=o.TngStamp;IsSea=o.IsSea;return *this;
}


#include <stdio.h>
#include <stdlib.h>
static int assigns,callbacks,blockDeletes;
static void* watched;
void operator delete(void* p) throw(){if(p==watched)++blockDeletes;free(p);}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* s,long):storage_((CCharStringData*)s){}
CCharString::~CCharString(){}
const CCharString& CCharString::operator=(const CCharString& s){++assigns;storage_=s.storage_;return *this;}
CCharString::operator const char*()const{return (const char*)storage_;}
class CMap { public: int id; };
static CMap* expected;
static CCPPointerInfo* dying;
void FABLE_FASTCALL Dispose(void* p){if(p!=expected||dying->RefCount!=0)abort();++callbacks;}
static void setup(CMapInfo& x,int n){
 x.Area.TLX=n;x.Area.TLY=n+1;x.Area.BRX=n+2;x.Area.BRY=n+3;
 x.PMap.Data=0;x.PMap.Info=0;x.MapUID=n+4;
 x.Used=true;x.Placed=false;x.Activated=true;x.Loading=false;x.LoadedOnPlayerProximity=true;
 x.LevelName=CCharString("level",-1);x.LevelScriptName=CCharString("script",-1);x.SerialiseScript=CCharString("serial",-1);
 x.LevStamp=123456789;x.TngStamp=987654321;x.IsSea=true;
}
static int equal(const CMapInfo& a,const CMapInfo& b){return
 a.Area.TLX==b.Area.TLX&&a.Area.TLY==b.Area.TLY&&a.Area.BRX==b.Area.BRX&&a.Area.BRY==b.Area.BRY&&
 a.PMap.Data==b.PMap.Data&&a.PMap.Info==b.PMap.Info&&
 (const char*)a.LevelName==(const char*)b.LevelName&&(const char*)a.LevelScriptName==(const char*)b.LevelScriptName&&
 (const char*)a.SerialiseScript==(const char*)b.SerialiseScript&&a.MapUID==b.MapUID&&a.Used==b.Used&&a.Placed==b.Placed&&
 a.Activated==b.Activated&&a.Loading==b.Loading&&a.LoadedOnPlayerProximity==b.LoadedOnPlayerProximity&&
 a.LevStamp==b.LevStamp&&a.TngStamp==b.TngStamp&&a.IsSea==b.IsSea;}
#define CHECK(x) do{if(!(x)){printf("00513bf0_TEST FAIL line%d\n",__LINE__);return 1;}}while(0)
int main(){
#ifdef _MSC_VER
 CHECK(sizeof(CMapInfo)==72);CHECK(offsetof(CMapInfo,SerialiseScript)==44);CHECK(offsetof(CMapInfo,IsSea)==64);
#endif
 CMapInfo a,b;setup(a,1);setup(b,10);CMap oldMap,newMap;
 CCPPointerInfo* oldInfo=new CCPPointerInfo;oldInfo->RefCount=1;oldInfo->DeleteFunc=Dispose;oldInfo->Data=&oldMap;
 CCPPointerInfo newInfo;newInfo.RefCount=1;newInfo.DeleteFunc=Dispose;newInfo.Data=&newMap;
 a.PMap.Data=&oldMap;a.PMap.Info=oldInfo;b.PMap.Data=&newMap;b.PMap.Info=&newInfo;
 watched=dying=oldInfo;expected=&oldMap;assigns=0;
 CHECK(&(a=b)==&a);CHECK(equal(a,b));CHECK(assigns==3);CHECK(callbacks==1&&blockDeletes==1&&newInfo.RefCount==2);
 assigns=0;CHECK(&(a=a)==&a);CHECK(newInfo.RefCount==2&&assigns==3&&callbacks==1);
 b.PMap.Data=&oldMap;assigns=0;a=b;CHECK(a.PMap.Data==&newMap);CHECK(newInfo.RefCount==2&&assigns==3);
 b.PMap.Info=0;b.PMap.Data=0;a=b;CHECK(equal(a,b));CHECK(newInfo.RefCount==1&&callbacks==1);
 a=b;CHECK(equal(a,b));CHECK(callbacks==1);
 b.PMap.Info=&newInfo;b.PMap.Data=&newMap;a=b;CHECK(equal(a,b)&&newInfo.RefCount==2);
 puts("00513bf0_TEST PASS");return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// SymbolMapSort::LinearInsert -- insert a by-value three-string symbol record into its sorted prefix
// Leaf: LinearInsert
// Retail009E6FA0 length185. Propagated AddGossip label is wrong: fastcall(first,last,value,tag), ret20.
// Native string copy0099EC30,assign0099EFB0,dtor0099EAE0; comparator009E6530; move_backward009E6410; unguarded_insert009E66F0.
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
    CWideString& operator=(const CWideString& other);
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
    CCharString& operator=(const CCharString& other);
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

// Actual string header omits the native copy constructor and comparator. Field-free API view retains its genuine storage.
struct SymbolString:CCharString{SymbolString(const SymbolString&);SymbolString& operator=(const SymbolString&);bool operator<(const SymbolString&)const;};
struct SymbolInfo{SymbolString GroupName;SymbolString EntryName;long Val;};
struct SymbolRecord{SymbolString Name;SymbolInfo Info;};
struct NonscalarTag{NonscalarTag(){}};
SymbolRecord* __fastcall MoveBackward(SymbolRecord*,SymbolRecord*,SymbolRecord*,NonscalarTag&,int); //009E6410
void __fastcall UnguardedInsert(SymbolRecord*,SymbolRecord,NonscalarTag); //009E66F0
void __fastcall LinearInsert(SymbolRecord* first,SymbolRecord* last,SymbolRecord value,NonscalarTag tag){
 if(value.Name<first->Name){MoveBackward(first,last,last+1,tag,0);*first=value;}
 else UnguardedInsert(last,value,tag);
}

static int live=0,copies=0,moves=0,inserts=0;
static CCharStringData* data(const CCharString& s){CCharStringData* d;std::memcpy(&d,&s,sizeof(d));return d;}
static void store(CCharString& s,CCharStringData* d){std::memcpy(&s,&d,sizeof(d));}
CCharString::CCharString(){store(*this,0);++live;}
CCharString::~CCharString(){CCharStringData*d=data(*this);if(d&&!--d->owners){std::free(d->text);std::free(d);}--live;}
SymbolString::SymbolString(const SymbolString& s):CCharString(){CCharStringData*d=data(s);store(*this,d);if(d)++d->owners;++copies;}
SymbolString& SymbolString::operator=(const SymbolString& s){if(this!=&s){CCharStringData*d=data(s);if(d)++d->owners;CCharStringData*old=data(*this);if(old&&!--old->owners){std::free(old->text);std::free(old);}store(*this,d);}return *this;}
bool SymbolString::operator<(const SymbolString& s)const{CCharStringData*a=data(*this),*b=data(s);if(a==b)return false;if(!a)return true;if(!b)return false;fable_u32 la=a->unknown04,lb=b->unknown04;if(la!=lb)return la<lb;for(fable_u32 offset=0;offset<la;offset+=4){fable_u32 av,bv;std::memcpy(&av,a->text+offset,4);std::memcpy(&bv,b->text+offset,4);if(av<bv)return true;if(av>bv)return false;}return false;}
SymbolRecord* __fastcall MoveBackward(SymbolRecord*f,SymbolRecord*l,SymbolRecord*r,NonscalarTag&,int n){if(n)std::abort();++moves;while(f!=l)*--r=*--l;return r;}
void __fastcall UnguardedInsert(SymbolRecord*l,SymbolRecord v,NonscalarTag){++inserts;SymbolRecord*prev=l-1;while(v.Name<prev->Name){*l=*prev;l=prev;--prev;}*l=v;}
static SymbolString str(const char*t){CCharStringData*d=(CCharStringData*)std::malloc(sizeof(CCharStringData));fable_u32 len=(fable_u32)std::strlen(t);fable_u32 capacity=(len+4)&~3u;d->text=(char*)std::malloc(capacity);std::memset(d->text,0,capacity);std::memcpy(d->text,t,len);d->unknown04=len;d->unknown08=capacity;d->flags0C=1;d->owners=1; // construct by copy from actual base storage
 CCharString base;store(base,d);return *reinterpret_cast<SymbolString*>(&base);}
static SymbolRecord rec(const char*n,long k){SymbolRecord r={str(n),{str("def"),str("value"),k}};return r;}
int main(){bool ok=true;{SymbolRecord a[5]={rec("b",2),rec("d",4),rec("z",26),rec("q",17),rec("q",17)};
 LinearInsert(a,a+2,rec("a",1),NonscalarTag());ok=ok&&a[0].Info.Val==1&&a[1].Info.Val==2&&a[2].Info.Val==4&&moves==1&&inserts==0;
 LinearInsert(a,a+3,rec("c",3),NonscalarTag());ok=ok&&a[0].Info.Val==1&&a[1].Info.Val==2&&a[2].Info.Val==3&&a[3].Info.Val==4&&inserts==1;
 LinearInsert(a,a+4,rec("c",30),NonscalarTag());ok=ok&&a[2].Info.Val==3&&a[3].Info.Val==30&&a[4].Info.Val==4&&inserts==2;
 ok=ok&&std::strcmp(data(a[3].Info.GroupName)->text,"def")==0&&std::strcmp(data(a[3].Info.EntryName)->text,"value")==0&&copies>0;}
 {SymbolRecord a[3]={rec("ba",11),rec("zz",33),rec("qq",44)};SymbolString az=str("az"),ba=str("ba");ok=ok&&!(az<ba)&&(ba<az);LinearInsert(a,a+2,rec("az",22),NonscalarTag());ok=ok&&a[0].Info.Val==11&&a[1].Info.Val==22&&a[2].Info.Val==33;}
 // A five-byte name forces a second padded dword comparison.
 {SymbolString left=str("aaaaz"),right=str("aaaab"),shorter=str("zzzz");ok=ok&&(right<left)&&!(left<right)&&(shorter<left);}
 ok=ok&&live==0;std::printf("009e6fa0_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;}
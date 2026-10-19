#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <wchar.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __int64 long long
#endif
// CConsoleInputMap::InsertNode(Node*, Node*, const Value&, long) -- insert a string/count-pointer pair into a red-black tree
// Retail 0x009EF750, 259 bytes. Corrected auto-label CCacheInfo: native CConsole::AddInput 0x009EC5E0 calls unique insert 0x009EFB80
// at 0x009EC66D on this+0x40; it supplies lowercased input->Name(+4) and counted CConsoleInputBase*.
// 0x009EFB80 calls this helper at 0x009EFBF6/0x009EFC4F. No generated owner headers exist.
// Native node: colour+0, parent+4, left+8, right+C, CCharString+10, counted data+14, counted info+18.
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


struct CWideStringData
{
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
struct CCharStringData
{
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

#include <new>
#include <stdlib.h>
struct CStringCopy : CCharString {
    CStringCopy();
    CStringCopy(const CStringCopy& source); // 0x0099EC30
    CCharStringData* Data() const { return *reinterpret_cast<CCharStringData* const*>(this); }
};
struct CStringOrder : CCharStringData { bool Less(const CCharStringData* other) const; }; // 0x00429950
struct CountBlock { long References; void (FABLE_FASTCALL* Destroy)(void*); void* Object; };
struct Value {
    CStringCopy Key; void* Data; CountBlock* Info;
    Value() : Key(), Data(0), Info(0) {}
    Value(const Value& v) : Key(v.Key), Data(v.Data), Info(v.Info) { if (Info) ++Info->References; }
    ~Value() { if (Info && --Info->References==0) { Info->Destroy(Info->Object); operator delete(Info); } }
};
struct Node { unsigned long Colour; Node* Parent; Node* Left; Node* Right; Value Pair; };
struct Iterator { Node* Pointer; explicit Iterator(Node* p) : Pointer(p) {} };
void Rebalance(Node* node, Node*& root); // 0x0042971D
struct CConsoleInputMap {
    Node* Header; unsigned long Count;
    Iterator InsertNode(Node* before, Node* parent, const Value& value, long forceRight);
    static Node* MakeNode(const Value& v) { Node* n = static_cast<Node*>(malloc(sizeof(Node))); new (&n->Pair) Value(v); return n; }
};
static bool Less(const CStringCopy& a, const CStringCopy& b) {
    CCharStringData* y=b.Data(); CCharStringData* x=a.Data();
    if (y==x) return false;
    if (!x) return true;
    if (!y) return false;
    return static_cast<CStringOrder*>(x)->Less(y);
}
Iterator CConsoleInputMap::InsertNode(Node* before, Node* parent, const Value& value, long forceRight) {
    Node* n;
    if (parent == Header || (!forceRight && (before != 0 || Less(value.Key, parent->Pair.Key)))) {
        n = MakeNode(value); parent->Left=n;
        if (parent==Header) { Header->Parent=n; Header->Right=n; }
        else if (parent==Header->Left) Header->Left=n;
    } else {
        n = MakeNode(value); parent->Right=n;
        if (parent==Header->Right) Header->Right=n;
    }
    n->Parent=parent; n->Left=0; n->Right=0;
    Rebalance(n, Header->Parent); ++Count;
    return Iterator(n);
}

CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const char* text, long len) : storage_(new CCharStringData) {
    if (len<0) len=(long)strlen(text);
    storage_->text=new char[len+1]; memcpy(storage_->text,text,len); storage_->text[len]=0;
    storage_->unknown04=(fable_u32)len; storage_->unknown08=(fable_u32)len+1; storage_->flags0C=0; storage_->owners=1;
}
CCharString::~CCharString() { if(storage_ && --storage_->owners==0) { delete[] storage_->text; delete storage_; } }
CCharString& CCharString::operator=(const CCharString& x) {
    if(x.storage_) ++x.storage_->owners;
    if(storage_ && --storage_->owners==0) { delete[] storage_->text; delete storage_; }
    storage_=x.storage_; return *this;
}
CStringCopy::CStringCopy() : CCharString() {}
CStringCopy::CStringCopy(const CStringCopy& x) : CCharString() { CCharString::operator=(x); }
bool CStringOrder::Less(const CCharStringData* x) const { const signed char* a=(const signed char*)text;const signed char* b=(const signed char*)x->text;while(*a==*b&&*a){++a;++b;}return *a<*b; }
static int destroyedObjects;
static void FABLE_FASTCALL DestroyCountedObject(void* p) { if(p) ++destroyedObjects; }
static Node* observedNode; static Node** observedRoot; static int rebalanceCalls;
void Rebalance(Node* n, Node*& root) { observedNode=n; observedRoot=&root; ++rebalanceCalls; }
static void Seed(Value& v, const char* name, CountBlock* info, void* data) {
    if(name) v.Key.CCharString::operator=(CCharString(name,-1));
    v.Data=data; v.Info=info;
}
static int Check(int condition) { return condition ? 0 : 1; }
int main() {
    int bad=0; int payload=123;
    CountBlock info={2,DestroyCountedObject,&payload};
    Value val; Seed(val,"middle",&info,&payload);
    Node header; header.Parent=0; header.Left=&header; header.Right=&header;
    CConsoleInputMap map; map.Header=&header; map.Count=0;
    Node* n=map.InsertNode(0,&header,val,0).Pointer;
    bad+=Check(n==header.Parent && n==header.Left && n==header.Right && n->Parent==&header && !n->Left && !n->Right);
    bad+=Check(n->Pair.Data==&payload && n->Pair.Info==&info && info.References==3 && val.Key.Data()->owners==2);
    bad+=Check(map.Count==1 && rebalanceCalls==1 && observedNode==n && observedRoot==&header.Parent);
    Value lo; Seed(lo,"alpha",0,0);
    Node* l=map.InsertNode(0,n,lo,0).Pointer;
    bad+=Check(n->Left==l && header.Left==l && l->Parent==n && map.Count==2 && !l->Pair.Info);
    Value hi; Seed(hi,"zulu",0,0);
    Node* r=map.InsertNode(0,n,hi,0).Pointer;
    bad+=Check(n->Right==r && header.Right==r && r->Parent==n && map.Count==3);
    Value empty;
    Node* e=map.InsertNode(0,l,empty,0).Pointer;
    bad+=Check(l->Left==e && header.Left==e && !e->Pair.Key.Data());
    // A nonzero full-word forced-right flag takes priority over the ordering and before hints.
    Node* forced=map.InsertNode(l,e,lo,256).Pointer;
    bad+=Check(e->Right==forced && !e->Left && header.Left==e);
    Node* hint=map.InsertNode(e,r,lo,0).Pointer;
    bad+=Check(r->Left==hint && map.Count==6 && rebalanceCalls==6);
    // Equal backing strings choose right, while a null incoming key sorts before a non-null key.
    Node* equal=map.InsertNode(0,forced,lo,0).Pointer;
    bad+=Check(forced->Right==equal && map.Count==7);
    Node* list[7]={n,l,r,e,forced,hint,equal};
    for(int i=0;i<7;++i) { list[i]->Pair.~Value(); free(list[i]); }
    bad+=Check(val.Key.Data()->owners==1 && lo.Key.Data()->owners==1 && hi.Key.Data()->owners==1);
    bad+=Check(info.References==2 && destroyedObjects==0);
    {
        Value finalValue;CountBlock* finalInfo=new CountBlock;
        finalInfo->References=1;finalInfo->Destroy=DestroyCountedObject;finalInfo->Object=&payload;
        Seed(finalValue,"final",finalInfo,&payload);
        Node finalHeader;finalHeader.Parent=0;finalHeader.Left=finalHeader.Right=&finalHeader;
        CConsoleInputMap finalMap;finalMap.Header=&finalHeader;finalMap.Count=0;
        Node* finalNode=finalMap.InsertNode(0,&finalHeader,finalValue,0).Pointer;
        bad+=Check(finalInfo->References==2);
        finalNode->Pair.~Value();free(finalNode);
        bad+=Check(finalInfo->References==1 && destroyedObjects==0);
    }
    bad+=Check(destroyedObjects==1);
    {
      Value normal,high;Seed(normal,"alpha",0,0);char name[]={char(0x80),0};Seed(high,name,0,0);
      Node head;head.Parent=0;head.Left=head.Right=&head;CConsoleInputMap check;check.Header=&head;check.Count=0;
      Node* first=check.InsertNode(0,&head,normal,0).Pointer;Node* second=check.InsertNode(0,first,high,0).Pointer;
      bad+=Check(first->Left==second&&head.Left==second);second->Pair.~Value();free(second);first->Pair.~Value();free(first);
    }
    if(bad) { printf("009ef750_TEST FAIL %d\n",bad); return 1; }
    printf("009ef750_TEST PASS\n"); return 0;
}
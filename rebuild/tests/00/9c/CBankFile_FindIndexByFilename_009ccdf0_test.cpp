// Host-only declaration adapter mirrors fable_string.h; target uses its actual header.
#ifndef _MSC_VER
#include <stddef.h>
#define FABLE_WIDE_STRING_HOST_TYPES
#define FABLE_FASTCALL
#define __cdecl
typedef unsigned int fable_u32;
typedef int fable_i32;
typedef unsigned char fable_u8;



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

#endif
// CBankFile::FindIndexByFilename(const CCharString&, long) -- find a normalized basename, optionally restricting its entry type.
// Retail: 0x009CCDF0, 242 bytes. Leaf: FindIndexByFilename
// CBankFile.h donor arrays/maps are 16 bytes versus retail12. RuntimeData+20,
// PackedDataType+58 and FilenameIndices+c4 therefore require this retail model.
// Shared fable_string.h supplies CCharString; ToLower is missing and declared in a
// method-only view of the same one-pointer object. Calls ConvertPathToFilename009CBE10,
// ToLower0099ED10, destructor0099EAE0, upper_bound009D2B50, lower_bound0042AD34,
// packed lookup00A629F0. Node color/parent/left/right/key/value at0/4/8/c/10/14.
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
struct CCharStringLowerView { void* PStringData; CCharString ToLower() const; }; //0099ED10
inline CCharString ToLower(const CCharString& s) { return reinterpret_cast<const CCharStringLowerView&>(s).ToLower(); }
struct CFilenameNode {
    long Color; CFilenameNode* Parent; CFilenameNode* Left; CFilenameNode* Right;
    CCharString Key; unsigned long Value;
};
struct CFilenameMap {
    CFilenameNode* Head; unsigned long Count; unsigned char Compare;
    CFilenameNode* UpperBound(const CCharString&); //009D2B50
    CFilenameNode* LowerBound(const CCharString&); //0042AD34
};
inline CFilenameNode* NextFilenameNode(CFilenameNode* p) {
    if (p->Right) { p=p->Right; while(p->Left) p=p->Left; }
    else { CFilenameNode* parent=p->Parent; while(p==parent->Right) {p=parent;parent=parent->Parent;} if(p->Right!=parent) p=parent; }
    return p;
}
struct CRuntimeData { unsigned long DataOffset,DataSize; unsigned char DataType; bool Valid; };
template<class T> class CArray { public: T* First;T* Last;T* Limit; };
struct CPackedUIntArray {
    unsigned long* PackedInts;unsigned long Size;unsigned char Bits;unsigned long Bias;
    unsigned long operator[](unsigned long) const; //00A629F0
};
class CBankFile { public:
    void* VTable;unsigned long Size;
    CArray<CCharString> Symbols; CArray<unsigned long> Checksums;
    CArray<CRuntimeData> RuntimeData; CArray<void*> UpdateData;
    CPackedUIntArray PackedDataOffset,PackedDataSize,PackedDataType,PackedValid;
    unsigned char _otherBankState[0x4c];
    CFilenameMap FilenameIndices;
    CCharString ConvertPathToFilename(const CCharString&) const; //009CBE10
    unsigned long GetDataType(unsigned long i) const { if(PackedDataType.Size) return PackedDataType[i-1];return RuntimeData.First[i].DataType; }
    unsigned long FindIndexByFilename(const CCharString&,long);
};
unsigned long CBankFile::FindIndexByFilename(const CCharString& path,long type)
{
    CCharString name=reinterpret_cast<const CCharStringLowerView&>(static_cast<const CCharString&>(ConvertPathToFilename(path))).ToLower();
    CFilenameNode* end=FilenameIndices.UpperBound(name);
    CFilenameNode* p=FilenameIndices.LowerBound(name);
    for(;p!=end;p=NextFilenameNode(p)) {
        unsigned long index=p->Value;
        if(type==-1 || GetDataType(index)==(unsigned long)type) return index;
    }
    return 0;
}

#include <stdio.h>
#include <string.h>
static CBankFile* activeBank;
static const CCharString* expectedPath;
static CFilenameNode *rangeBegin,*rangeEnd;
static int live,baseline,invalid,convertCalls,lowerTextCalls,upperCalls,lowerCalls,packedCalls;
static unsigned long packed[7];
static const char* expectedName;
CCharString::CCharString() { storage_=new CCharStringData;storage_->text=new char[1];storage_->text[0]=0;++live; }
CCharString::CCharString(const char* s,long n) {
    if(n!=-1)++invalid;storage_=new CCharStringData;storage_->text=new char[strlen(s)+1];strcpy(storage_->text,s);++live;
}
CCharString::~CCharString() {delete[] storage_->text;delete storage_;--live;}
CCharString::operator const char*() const {return storage_->text;}
CCharString CCharStringLowerView::ToLower() const {
    ++lowerTextCalls;if(convertCalls!=1)++invalid;
    const char* text=(const char*)*reinterpret_cast<const CCharString*>(this);char buffer[256];unsigned long i=0;
    for(;text[i] && i<255;++i)buffer[i]=(text[i]>='A' && text[i]<='Z')?(char)(text[i]+32):text[i];buffer[i]=0;
    return CCharString(buffer,-1);
}
CCharString CBankFile::ConvertPathToFilename(const CCharString& path) const {
    ++convertCalls;if(this!=activeBank || &path!=expectedPath || lowerTextCalls)++invalid;
    const char* s=path;const char* base=s;for(;*s;++s)if(*s=='/' || *s=='\\')base=s+1;
    return CCharString(base,-1);
}
CFilenameNode* CFilenameMap::UpperBound(const CCharString& name) {
    ++upperCalls;if(this!=&activeBank->FilenameIndices || lowerTextCalls!=1 || lowerCalls || strcmp(name,expectedName) || live!=baseline+1)++invalid;
    return rangeEnd;
}
CFilenameNode* CFilenameMap::LowerBound(const CCharString& name) {
    ++lowerCalls;if(this!=&activeBank->FilenameIndices || upperCalls!=1 || strcmp(name,expectedName) || live!=baseline+1)++invalid;
    return rangeBegin;
}
unsigned long CPackedUIntArray::operator[](unsigned long i) const {
    ++packedCalls;if(this!=&activeBank->PackedDataType || i>=7) {++invalid;return 0;}return packed[i];
}
static int run(CBankFile& bank,const CCharString& path,long type,unsigned long want,int reads) {
    activeBank=&bank;expectedPath=&path;baseline=live;invalid=convertCalls=lowerTextCalls=upperCalls=lowerCalls=packedCalls=0;
    unsigned long got=bank.FindIndexByFilename(path,type);
    return got==want && invalid==0 && convertCalls==1 && lowerTextCalls==1 && upperCalls==1 && lowerCalls==1 && packedCalls==reads && live==baseline;
}
int main() {
    int ok=1;
    { CBankFile bank; CFilenameNode nodes[7],head;CRuntimeData runtime[8];CCharString path("Folder/Sub/HeRo.BbB",-1);
      for(int i=0;i<7;++i) { nodes[i].Color=0;nodes[i].Parent=nodes[i].Left=nodes[i].Right=0;nodes[i].Value=i+1;runtime[i+1].DataType=(unsigned char)(21+i);packed[i]=31+i; }
      nodes[3].Left=&nodes[1];nodes[3].Right=&nodes[5];nodes[3].Parent=&head;
      nodes[1].Parent=nodes[5].Parent=&nodes[3];nodes[1].Left=&nodes[0];nodes[1].Right=&nodes[2];
      nodes[5].Left=&nodes[4];nodes[5].Right=&nodes[6];nodes[0].Parent=nodes[2].Parent=&nodes[1];nodes[4].Parent=nodes[6].Parent=&nodes[5];
      head.Parent=&nodes[3];head.Left=&nodes[0];head.Right=&nodes[6];
      bank.FilenameIndices.Head=&head;bank.RuntimeData.First=runtime;bank.RuntimeData.Last=runtime+8;bank.RuntimeData.Limit=runtime+8;
      rangeBegin=&nodes[0];rangeEnd=&head;expectedName="hero.bbb";bank.PackedDataType.Size=0;
      ok=run(bank,path,-1,1,0) && run(bank,path,21,1,0) && run(bank,path,27,7,0) && run(bank,path,99,0,0);
      bank.PackedDataType.Size=7;
      ok=ok && run(bank,path,-1,1,0) && run(bank,path,31,1,1) && run(bank,path,37,7,7) && run(bank,path,99,0,7);
      rangeBegin=&nodes[2];rangeEnd=&nodes[5];ok=ok && run(bank,path,35,5,3) && run(bank,path,36,0,3);
      rangeBegin=rangeEnd;ok=ok && run(bank,path,-1,0,0);
      CCharString windows("C:\\dir\\HeRo.BbB",-1);rangeBegin=&nodes[0];rangeEnd=&head;
      ok=ok && run(bank,windows,-1,1,0);
    }
    ok=ok && live==0;printf("009ccdf0_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;
}
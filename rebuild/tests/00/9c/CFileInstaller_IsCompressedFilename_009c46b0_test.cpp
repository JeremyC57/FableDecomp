#ifndef _MSC_VER
#define __fastcall
#endif
#define FABLE_FASTCALL __fastcall
typedef unsigned int fable_u32;typedef int fable_i32;typedef unsigned char fable_u8;
// CFileInstaller::IsCompressedFilename(const CWideString&,CWideString&) -- replace recognized compressed extensions.
// Leaf: IsCompressedFilename
// Retail009C46B0,240bytes. Donor CFileInstaller CompressedFileTypes+b4 -> native+94 (smaller STL).
// Calls CAFile::SplitPath997620, wide ctors99AED0/dtors99B510, conversion99B2C0,
// lower99ED10, treeFind9AB5D0, char destructor99EAE0, charToWide99E480,widePlus99BE70,assign99B7D0.
// Missing conversion/lower declarations are field-free ABI method views over actual shared storage.
#define CCharString CCharStringCore
#define CWideString CWideStringCore


#include <stddef.h>



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

#undef CWideString
#undef CCharString
class CWideString;
class CCharString:public CCharStringCore { public:CCharString();CCharString(const char*,long);~CCharString();CCharString ToLower() const;CWideString ToWideString() const;};
class CWideString:public CWideStringCore { public:CWideString();~CWideString();CCharString ConvertToCharString() const;CWideString& operator=(const CWideString&);};
CWideString __fastcall operator+(const CWideString&,const CWideString&);
struct CExtensionNode {unsigned char TreeLinks[0x10];CCharString first,second;};
struct CExtensionTree { CExtensionNode* Head;CExtensionNode* Find(const CCharString&); };
struct CExtensionMap:CExtensionTree { CExtensionNode* find(const CCharString& key){return Find(key);} };
class CAFile { public:static void __fastcall SplitPath(const CWideString&,CWideString*,CWideString*,CWideString*,CWideString*);};
class CFileInstaller { public:unsigned char EarlierState[0x94];CExtensionMap CompressedFileTypes;bool IsCompressedFilename(const CWideString&,CWideString&);};
bool CFileInstaller::IsCompressedFilename(const CWideString& filename,CWideString& output){
 bool result;
 { CWideString stem,extension;
 CAFile::SplitPath(filename,0,0,&stem,&extension);
 CExtensionNode* found=CompressedFileTypes.find(extension.ConvertToCharString().ToLower());
 if(found!=CompressedFileTypes.Head){
 output=stem+found->second.ToWideString();result=true;
 }else{output=stem+extension;result=false;}
 }return result;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
static int outstanding=0,badOwnership=0,findSeen=0,charReleases=0;
static char* chars(const char* t){char* p=(char*)malloc(strlen(t)+1);strcpy(p,t);return p;}
static wchar_t* wide(const wchar_t* t){wchar_t* p=(wchar_t*)malloc((wcslen(t)+1)*sizeof(wchar_t));wcscpy(p,t);return p;}
static void drop(CWideStringData*& p){if(p){if(p->owners<=0)badOwnership=1;if(--p->owners==0){free(p->text);free(p);--outstanding;}p=0;}}
static void drop(CCharStringData*& p){if(p){if(p->owners<=0)badOwnership=1;if(--p->owners==0){free(p->text);free(p);--outstanding;}p=0;++charReleases;}}
CWideStringCore::CWideStringCore():storage_(0){}
CWideStringCore::CWideStringCore(const CWideStringCore&r):storage_(r.storage_){if(storage_)++storage_->owners;}
CWideStringCore::~CWideStringCore(){drop(storage_);}
CWideStringCore::operator const wchar_t*()const{return storage_?storage_->text:L"";}
const CWideStringCore& CWideStringCore::operator=(const wchar_t*t){drop(storage_);storage_=(CWideStringData*)malloc(sizeof(CWideStringData));storage_->text=wide(t);storage_->owners=1;++outstanding;return *this;}
CWideStringCore& CWideStringCore::operator=(const CWideStringCore&r){if(this!=&r){drop(storage_);storage_=r.storage_;if(storage_)++storage_->owners;}return *this;}
CCharStringCore::CCharStringCore():storage_(0){}
CCharStringCore::CCharStringCore(const char*t,long):storage_(0){operator=(t);}
CCharStringCore::~CCharStringCore(){drop(storage_);}
CCharStringCore::operator const char*()const{return storage_?storage_->text:"";}
const CCharStringCore& CCharStringCore::operator=(const char*t){drop(storage_);storage_=(CCharStringData*)malloc(sizeof(CCharStringData));storage_->text=chars(t);storage_->owners=1;++outstanding;return *this;}
CWideString::CWideString(){} CWideString::~CWideString(){} CCharString::CCharString(){} CCharString::CCharString(const char*t,long n):CCharStringCore(t,n){} CCharString::~CCharString(){}
CCharString CWideString::ConvertToCharString()const{const wchar_t* p=*this;char b[256];int i=0;while(p[i]){b[i]=(char)p[i];++i;}b[i]=0;return CCharString(b,-1);}
CCharString CCharString::ToLower()const{char b[256];strcpy(b,(const char*)*this);for(char*p=b;*p;++p)*p=(char)tolower((unsigned char)*p);return CCharString(b,-1);}
CWideString CCharString::ToWideString()const{const char*p=*this;wchar_t b[256];int i=0;while(p[i]){b[i]=(unsigned char)p[i];++i;}b[i]=0;CWideString w;w.CWideStringCore::operator=(b);return w;}
CWideString& CWideString::operator=(const CWideString&r){CWideStringCore::operator=(r);return *this;}
CWideString __fastcall operator+(const CWideString&a,const CWideString&b){wchar_t text[512];wcscpy(text,(const wchar_t*)a);wcscat(text,(const wchar_t*)b);CWideString r;r.CWideStringCore::operator=(text);return r;}
void __fastcall CAFile::SplitPath(const CWideString& f,CWideString* drive,CWideString* dir,CWideString* name,CWideString* ext){if(drive||dir)exit(20);wchar_t p[256];wcscpy(p,(const wchar_t*)f);wchar_t* dot=wcsrchr(p,L'.');if(dot){ext->CWideStringCore::operator=(dot);*dot=0;}else ext->CWideStringCore::operator=(L"");name->CWideStringCore::operator=(p);}
static CExtensionNode* node;
CExtensionNode* CExtensionTree::Find(const CCharString& key){if(strcmp((const char*)key,".cmp")==0){++findSeen;return node;}return Head;}
int main(){
 {CFileInstaller i;CExtensionNode sentinel,mapped;node=&mapped;i.CompressedFileTypes.Head=&sentinel;mapped.second.CCharStringCore::operator=(".txt");
 CWideString f,o;f.CWideStringCore::operator=(L"folder/name.CMP");if(!i.IsCompressedFilename(f,o)||wcscmp((const wchar_t*)o,L"folder/name.txt"))return 1;
 if(charReleases<2)return 2;
 f.CWideStringCore::operator=(L"name.unknown");if(i.IsCompressedFilename(f,o)||wcscmp((const wchar_t*)o,L"name.unknown"))return 3;
 f.CWideStringCore::operator=(L"plain");if(i.IsCompressedFilename(f,o)||wcscmp((const wchar_t*)o,L"plain"))return 4;
 f.CWideStringCore::operator=(L".CMP");if(!i.IsCompressedFilename(f,o)||wcscmp((const wchar_t*)o,L".txt")||findSeen!=2)return 5;
 }
 { CFileInstaller i; CExtensionNode sentinel,mapped; node=&mapped; i.CompressedFileTypes.Head=&sentinel;
 mapped.second.CCharStringCore::operator=(".txt"); CWideString f; f.CWideStringCore::operator=(L"same.CMP");
 if(!i.IsCompressedFilename(f,f)||wcscmp((const wchar_t*)f,L"same.txt"))return 7;
 f.CWideStringCore::operator=(L"same.UNK"); if(i.IsCompressedFilename(f,f)||wcscmp((const wchar_t*)f,L"same.UNK"))return 8;
 }
 if(outstanding||badOwnership)return 6;
 puts("009c46b0_TEST PASS");return 0;}
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

// CHeroLogBook::AddObjectiveToQuest(ulong,ulong,const CCharString&) -- PDB00487960
// Leaf: AddObjectiveToQuest
#define CCharString CCharStringDonor
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

#undef CCharString
#include <string.h>
#include <new>
// Actual header lacks explicit copy/append/Sprintf methods; native string remains one pointer.
struct BasicString {char*Text;bool operator==(const char*)const;}; //004115A0/PDB0040C910 CBasicString<char>::operator==
extern const char EmptyString[]; //0122D70E
struct CCharString {CCharStringData*Data;CCharString();CCharString(const CCharString&);~CCharString();const CCharString&operator=(const char*);const CCharString&operator+=(const CCharString&);void __cdecl Sprintf(const char*,...);__forceinline bool operator==(const char*t)const{if(!Data)return strcmp("",t)==0;return *reinterpret_cast<const BasicString*>(Data)==t;}}; //Sprintf member __cdecl(PDB00AFB180), not thiscall/free function
struct Objective {unsigned long ID;CCharString Description;bool Completed;};
struct Allocator {};
struct ObjectiveVector {ObjectiveVector();Objective*First,*Last,*End;void InsertOverflow(Objective*,const Objective&,const Allocator&,unsigned long,bool); //0047C030 native five-stack-argument vector growth helper
 __forceinline void push_back(const Objective&o){if(Last!=End){if(Last)new(Last)Objective(o);++Last;}else{Allocator a;InsertOverflow(Last,o,a,1,true);}}
};
struct Quest {unsigned long ID;CCharString Description;long Status;ObjectiveVector Objectives;}; //native24 vs donor28 due vector12/16
struct QuestVector {Quest*First,*Last,*End;};
struct CHeroLogBook {CHeroLogBook();QuestVector GivenQuests;bool AddObjectiveToQuest(unsigned long,unsigned long,const CCharString&);}; //actual default PDB01ACA8D0; only accessed prefix represented
bool CHeroLogBook::AddObjectiveToQuest(unsigned long questID,unsigned long objectiveID,const CCharString& description){
 Objective objective;objective.Completed=false;objective.ID=objectiveID;
 if(!description.Data){if(strcmp("","")==0)goto empty_description;}
 else if(*reinterpret_cast<const BasicString*>(description.Data)=="")goto empty_description;
 objective.Description.Sprintf("%d-",objectiveID);objective.Description+=description;goto find_quest;
 empty_description:objective.Description="This quest objective has no description!";
 find_quest:
 unsigned long i=0;unsigned long n=GivenQuests.Last-GivenQuests.First;
 for(;i<n;++i)if(GivenQuests.First[i].ID==questID){GivenQuests.First[i].Objectives.push_back(objective);break;}
 return true;
}


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
static int LiveData,Copies,Destroyed,Growth,Formatted,Appended,EqualityCalls;static bool Wrong;
const char EmptyString[]={0};
static void Release(CCharStringData*p){if(p&&!--p->owners){free(p->text);free(p);--LiveData;}}
static CCharStringData* Make(const char*t){CCharStringData*p=static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));p->text=static_cast<char*>(malloc(strlen(t)+1));strcpy(p->text,t);p->owners=1;++LiveData;return p;}
CCharString::CCharString():Data(0){}
CCharString::CCharString(const CCharString&o):Data(o.Data){++Copies;if(Data)++Data->owners;}
CCharString::~CCharString(){++Destroyed;Release(Data);}
const CCharString& CCharString::operator=(const char*t){CCharStringData*p=Make(t);Release(Data);Data=p;return *this;}
const CCharString& CCharString::operator+=(const CCharString&o){++Appended;char b[256];sprintf(b,"%s%s",Data?Data->text:"",o.Data?o.Data->text:"");return *this=b;}
void __cdecl CCharString::Sprintf(const char*format,...){++Formatted;if(strcmp(format,"%d-"))Wrong=true;va_list a;va_start(a,format);unsigned long id=va_arg(a,unsigned long);va_end(a);fable_i32 signedID=(id&0x80000000UL)?(fable_i32)(-1-(long)(0xffffffffUL-(id&0xffffffffUL))):(fable_i32)id;char b[64];sprintf(b,"%d-",(int)signedID);*this=b;}
bool BasicString::operator==(const char*t)const{++EqualityCalls;return strcmp(Text,t)==0;}
ObjectiveVector::ObjectiveVector():First(0),Last(0),End(0){}
void ObjectiveVector::InsertOverflow(Objective*position,const Objective&o,const Allocator&,unsigned long count,bool atEnd){++Growth;if(position!=Last||count!=1||!atEnd)Wrong=true;unsigned n=First?static_cast<unsigned>(Last-First):0;Objective*items=static_cast<Objective*>(::operator new((n+2)*sizeof(Objective)));for(unsigned i=0;i<n;++i){new(items+i)Objective(First[i]);First[i].~Objective();}::operator delete(First);new(items+n)Objective(o);First=items;Last=items+n+1;End=items+n+2;}
CHeroLogBook::CHeroLogBook(){GivenQuests.First=GivenQuests.Last=GivenQuests.End=0;}
static void Clean(ObjectiveVector&v){for(Objective*p=v.First;p!=v.Last;++p)p->~Objective();::operator delete(v.First);v.First=v.Last=v.End=0;}
#define CHECK(x) do{if(!(x)){printf("0047a8e0_TEST FAIL %d\n",__LINE__);return 1;}}while(0)
int main(){const unsigned long ids[]={17UL,0x80000000UL,0xffffffffUL};for(int idCase=0;idCase<3;++idCase)for(int text=0;text<3;++text)for(int capacity=0;capacity<2;++capacity){CHeroLogBook book;Quest quests[3];quests[0].ID=10;quests[1].ID=77;quests[2].ID=77;book.GivenQuests.First=quests;book.GivenQuests.Last=book.GivenQuests.End=quests+3;ObjectiveVector&v=quests[1].Objectives;if(capacity){v.First=static_cast<Objective*>(::operator new(2*sizeof(Objective)));v.Last=v.First;v.End=v.First+2;}CCharString description;if(text)description=text==1?"":"find the key";Copies=Destroyed=Growth=Formatted=Appended=EqualityCalls=0;Wrong=false;CHECK(book.AddObjectiveToQuest(77,ids[idCase],description));CHECK(!Wrong&&v.Last-v.First==1&&quests[0].Objectives.First==0&&quests[2].Objectives.First==0);CHECK(v.First->ID==ids[idCase]&&!v.First->Completed&&v.First->Description.Data->owners==1);CHECK(strcmp(v.First->Description.Data->text,text<2?"This quest objective has no description!":(idCase==0?"17-find the key":(idCase==1?"-2147483648-find the key":"-1-find the key")))==0);CHECK(Copies==1&&Destroyed==1&&Growth==!capacity&&Formatted==(text==2)&&Appended==(text==2)&&EqualityCalls==(text!=0));Clean(v);}
CHECK(LiveData==0);{CHeroLogBook book;Quest quests[1];quests[0].ID=10;book.GivenQuests.First=quests;book.GivenQuests.Last=book.GivenQuests.End=quests+1;CCharString desc;desc="missing quest";CHECK(book.AddObjectiveToQuest(99,3,desc));CHECK(!quests[0].Objectives.First);book.GivenQuests.Last=book.GivenQuests.First;CHECK(book.AddObjectiveToQuest(10,3,desc));CHECK(!quests[0].Objectives.First);}CHECK(LiveData==0);printf("0047a8e0_TEST PASS\n");return 0;}
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCharString)==4);
FABLE_STATIC_ASSERT(sizeof(Objective)==12);
FABLE_STATIC_ASSERT(sizeof(Quest)==24);
FABLE_STATIC_ASSERT(sizeof(ObjectiveVector)==12);
#endif
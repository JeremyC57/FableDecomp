#include <stddef.h>
#define CCharString CCharString_HeaderEvidence
#define CCharStringData CCharStringData_HeaderEvidence
#define CWideString CWideString_HeaderEvidence
#define CWideStringData CWideStringData_HeaderEvidence
#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __thiscall
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


#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_static_assert_, __LINE__)[(expression) ? 1 : -1]

#pragma once



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

#undef CCharString
#undef CCharStringData
#undef CWideString
#undef CWideStringData
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
extern const char g_EmptyString[];
#pragma pack(push,1)
template<class T>class CBasicString{public:T* PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;};class CCharStringData{public:CBasicString<char> Data;long NoRefs;};
#pragma pack(pop)
class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();CCharString& operator=(const CCharString&);operator const char*()const{if(!PStringData)return g_EmptyString;return PStringData->Data.PData;}};

CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
class CDefString { public: long TablePos; };
class CIVCountedPointeeBase{public:CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;void AddRef()const{++IVRefCount;}long GetRefCount()const{return (long)IVRefCount;}long Release()const{long result=(long)--IVRefCount;if(result==0)Delete();return result;}};
class CResourceList;class CResource:public CIVCountedPointeeBase{public:CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;unsigned long ResourceSize,LastUsedFrame;CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}virtual ~CResource();};class CDefPointeeBase:public CResource{public:virtual void Delete()const;};enum ENullDef{ENullDef_DescriptiveNull=0};
class CFailedAllocationHandler{public:virtual ~CFailedAllocationHandler();};class CResourceList:public CFailedAllocationHandler{public:CResource Head;unsigned long ResourceCount;long AllocatedMemory,MaximumMemory;unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;};class CDefinitionManager:public CResourceList{public:CDefString GetDefNameFromGlobalIndex(long)const;};
class CGameDefinitionManager:public CDefinitionManager {};
class CDefStringTable { public: CCharString GetString(long) const; }; extern CDefStringTable g_DefStringTable;
class CRGBColour { public: unsigned char B,G,R,A; void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; } };
class C3DVector { public: float X,Y,Z; };
class CEditEngineDrawInfo { public: C3DVector Pos; float Radius; CCharString Fullname; CRGBColour Colour; long PlayerOwner; float EffectRadius; bool UsesEffectRadius; float AngleXY; };
class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};struct CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;protected:CBaseObject();virtual ~CBaseObject();};namespace std{struct _List_node_base;struct _List_iterator_base{_List_node_base* _M_node;};template<class T>struct _Nonconst_traits{};template<class T,class Traits>struct _List_iterator:public _List_iterator_base{};}
class CClass:public CBaseObject{public:std::_List_iterator<CClass*,std::_Nonconst_traits<CClass*> > ClassListPos,TypeListPos;unsigned char Type;};class CThing:public CClass{public:unsigned char _unmodeled14[0x74-0x14];CCharString ScriptName;unsigned char _unmodeled78[0x8c-0x78];unsigned short DefGlobalIndex;const CGameDefinitionManager& PeekDefinitionManager()const;};

class CThingPhysical:public CThing { public: virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const; };
class CThingGameObject:public CThingPhysical {};
class CThingPhysicalSwitch:public CThingGameObject { public: virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const; };
bool CThingPhysicalSwitch::DrawGetEditDrawInfo(float time,CEditEngineDrawInfo* info) const {
 if(CThingPhysical::DrawGetEditDrawInfo(time,info)) {
  { const CCharString definitionName=g_DefStringTable.GetString(PeekDefinitionManager().GetDefNameFromGlobalIndex(DefGlobalIndex).TablePos); info->Fullname=CCharString("")+definitionName+" - "+(const char*)ScriptName; }
  info->Colour.Set(255,255,0,255); return true;
 } return false;
}
const char g_EmptyString[]=""; CDefStringTable g_DefStringTable; static CGameDefinitionManager manager; static const CThing* expectedThis; static CEditEngineDrawInfo* expectedInfo; static bool baseResult; static int bad,calls; static CCharStringData blocks[64]; static char text[64][128]; static int used;
CCharString::CCharString(const char* value,long length) { if(length<0) length=(long)strlen(value); PStringData=&blocks[used]; PStringData->NoRefs=1;PStringData->Data.StringLength=(unsigned long)length;PStringData->Data.DataLength=(unsigned long)length+1;PStringData->Data.UseFastExtend=false; PStringData->Data.PData=text[used++]; memcpy(PStringData->Data.PData,value,length); PStringData->Data.PData[length]=0; }
CCharString::~CCharString() {if(PStringData)--PStringData->NoRefs;}CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString&other):PStringData(other.PStringData){if(PStringData)++PStringData->NoRefs;}
CCharString& CCharString::operator=(const CCharString& other){if(PStringData!=other.PStringData){if(PStringData)--PStringData->NoRefs;PStringData=other.PStringData;if(PStringData)++PStringData->NoRefs;}return *this;}
CCharString __fastcall operator+(const CCharString& a,const CCharString& b) { char buffer[128]; strcpy(buffer,(const char*)a); strcat(buffer,(const char*)b); return CCharString(buffer); }
CCharString __fastcall operator+(const CCharString& a,const char* b) { char buffer[128]; strcpy(buffer,(const char*)a); strcat(buffer,b); return CCharString(buffer); }
bool CThingPhysical::DrawGetEditDrawInfo(float time,CEditEngineDrawInfo* info) const { if(this!=expectedThis || info!=expectedInfo || time!=3.5f || calls++) bad=1; return baseResult; }
const CGameDefinitionManager& CThing::PeekDefinitionManager() const { if(this!=expectedThis || calls++!=1) bad=1; return manager; }
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index) const { if(this!=&manager || index!=319 || calls++!=2) bad=1; CDefString value; value.TablePos=71; return value; }
CCharString CDefStringTable::GetString(long pos) const { if(this!=&g_DefStringTable || pos!=71 || calls++!=3) bad=1; return CCharString("ObjectDef"); }
int main() { CThingPhysicalSwitch object; CEditEngineDrawInfo info; expectedThis=&object; expectedInfo=&info; object.DefGlobalIndex=319; object.ScriptName=CCharString("ScriptOne"); baseResult=true; if(!object.DrawGetEditDrawInfo(3.5f,&info) || bad || calls!=4 || strcmp((const char*)info.Fullname,"ObjectDef - ScriptOne") || info.Colour.B || info.Colour.G!=255 || info.Colour.R!=255 || info.Colour.A!=255) goto fail;
 calls=0; object.ScriptName.PStringData=0; if(!object.DrawGetEditDrawInfo(3.5f,&info) || bad || calls!=4 || strcmp((const char*)info.Fullname,"ObjectDef - ")) goto fail;
 calls=0; baseResult=false; if(object.DrawGetEditDrawInfo(3.5f,&info) || bad || calls!=1) goto fail;
 puts("0083d710_TEST PASS"); return 0; fail: puts("0083d710_TEST FAIL"); return 1; }

CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}CBaseObject::CBaseObject(){IntelligentPointer=0;}CBaseObject::~CBaseObject(){}void CIVCountedPointeeBase::Delete()const{}void CDefPointeeBase::Delete()const{}CResource::~CResource(){}CFailedAllocationHandler::~CFailedAllocationHandler(){}

// CThingVillage::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const -- build the editor name and yellow colour after physical base info succeeds.
// Leaf: ?DrawGetEditDrawInfo@CThingVillage@@UBE_NMPAVCEditEngineDrawInfo@@@Z
// Identity: proven; RTTI own vtable0x012451BC slot25 and both PDB names virtual const DrawGetEditDrawInfo(float,CEditEngineDrawInfo*); own data ScriptName+0x74,DefGlobalIndex+0x8C and literal separator.
// Retry model correction: actual original CThing::PeekDefinitionManager() const QBE at004c7990, not nonexistent GetDefinitionManager; own body loads global manager then+20 and exact PDB Peek name. Correct CBaseObject8/CClass20 with intelligent pointer and original iterator fields; actual ScriptName116/DefGlobalIndex140. Full packed CBasicString<char>13/CCharStringData17 replaces false flat data pointer, real non-POD string API including public copy. Shared fable_string.h retained unchanged under aliases because its narrow API/layout prefix is incomplete for these original methods. True CDefinitionManager:CResourceList:CFailedAllocationHandler/CResource/CIV prefix, inactive manager/Thing suffixes omitted without whole-size claims.
// Layout: included generated Physical/GameObject headers flatten the polymorphic base chain and omit named base fields; CThing header is quarantined. Local minimal named member view uses proved retail offsets, and actual RTTI inheritance with matching class keys. Draw-info has no shared header; string header alias remains data evidence.
#define CCharString CCharString_HeaderEvidence
#define CCharStringData CCharStringData_HeaderEvidence
#define CWideString CWideString_HeaderEvidence
#define CWideStringData CWideStringData_HeaderEvidence
#include "fable_string.h"
#undef CCharString
#undef CCharStringData
#undef CWideString
#undef CWideStringData
#define CThingPhysical CThingPhysical_HeaderLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObject_HeaderLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
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
class CThingVillage:public CThingGameObject { public: virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const; };
bool CThingVillage::DrawGetEditDrawInfo(float time,CEditEngineDrawInfo* info) const {
 if(CThingPhysical::DrawGetEditDrawInfo(time,info)) {
  { const CCharString definitionName=g_DefStringTable.GetString(PeekDefinitionManager().GetDefNameFromGlobalIndex(DefGlobalIndex).TablePos); info->Fullname=CCharString("")+definitionName+" - "+(const char*)ScriptName; }
  info->Colour.Set(128,255,255,255); return true;
 } return false;
}

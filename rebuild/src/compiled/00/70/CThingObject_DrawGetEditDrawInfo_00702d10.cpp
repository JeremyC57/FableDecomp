// CThingObject::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const -- after the physical base fills the editor info, name the object "<def name> - <script name>" and colour it yellow.
// Leaf: ?DrawGetEditDrawInfo@CThingObject@@UBE_NMPAVCEditEngineDrawInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702D10, 240 bytes, thiscall `ret 8`.
// Identity: rtti-vtables.tsv CThingObject vtable 0x01264DCC slot 25 = 0x00702D10 (the slot the landed byte twin 0x0083D710 holds in CThingPhysicalSwitch's vtable); both PDB name sets hold ?DrawGetEditDrawInfo@CThingObject@@UBE_NMPAVCEditEngineDrawInfo@@@Z. Retail bytes equal the twin's modulo call displacements: same base call CThingPhysical::DrawGetEditDrawInfo 0x006A4E90 (CThingGameObject does not override it), PeekDefinitionManager 0x004C7990, the same pooled " - " literal and yellow colour. Catalog label CThingObject::vfunc_25.
// Shape: the landed twin 0x0083D710 unchanged except the class (CThingObject : CThingGameObject, class key V): the CDefString name reaches operator+ through the inline `operator CCharString() const`.
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
class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();CCharString const& operator=(CCharString const&a0);operator const char*()const{if(!PStringData)return g_EmptyString;return PStringData->Data.PData;}};

CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
class CDefStringTable; class CDefString { public: long TablePos; operator CCharString() const; };
class CIVCountedPointeeBase{public:CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;void AddRef()const{++IVRefCount;}long GetRefCount()const{return (long)IVRefCount;}long Release()const{long result=(long)--IVRefCount;if(result==0)Delete();return result;}};
class CResourceList;class CResource:public CIVCountedPointeeBase{public:CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;unsigned long ResourceSize,LastUsedFrame;CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}virtual ~CResource();};class CDefPointeeBase:public CResource{public:virtual void Delete()const;};enum ENullDef{ENullDef_DescriptiveNull=0};
class CFailedAllocationHandler{public:virtual ~CFailedAllocationHandler();};class CResourceList:public CFailedAllocationHandler{public:CResource Head;unsigned long ResourceCount;long AllocatedMemory,MaximumMemory;unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;};class CDefinitionManager:public CResourceList{public:CDefString GetDefNameFromGlobalIndex(long)const;};
class CGameDefinitionManager:public CDefinitionManager {};
class CDefStringTable { public: CCharString GetString(long) const; }; extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)
inline CDefString::operator CCharString() const { return GDefStringTable.GetString(TablePos); }
class CRGBColour { public: unsigned char B,G,R,A; void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; } };
#include "engine/C3DVector.h"
class CEditEngineDrawInfo { public: C3DVector Pos; float Radius; CCharString Fullname; CRGBColour Colour; long PlayerOwner; float EffectRadius; bool UsesEffectRadius; float AngleXY; };
class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};struct CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;protected:CBaseObject();virtual ~CBaseObject();};namespace std{struct _List_node_base;struct _List_iterator_base{_List_node_base* _M_node;};template<class T>struct _Nonconst_traits{};template<class T,class Traits>struct _List_iterator:public _List_iterator_base{};}
class CClass:public CBaseObject{public:std::_List_iterator<CClass*,std::_Nonconst_traits<CClass*> > ClassListPos,TypeListPos;unsigned char Type;};class CThing:public CClass{public:unsigned char _unmodeled14[0x74-0x14];CCharString ScriptName;unsigned char _unmodeled78[0x8c-0x78];unsigned short DefGlobalIndex;const CGameDefinitionManager& PeekDefinitionManager()const;};

class CThingPhysical:public CThing { public: virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const; };
class CThingGameObject:public CThingPhysical {};
class CThingObject:public CThingGameObject { public: virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const; };
bool CThingObject::DrawGetEditDrawInfo(float time,CEditEngineDrawInfo* info) const {
 if(CThingPhysical::DrawGetEditDrawInfo(time,info)) {
  info->Fullname=CCharString("")+PeekDefinitionManager().GetDefNameFromGlobalIndex(DefGlobalIndex)+" - "+(const char*)ScriptName;
  info->Colour.Set(255,255,0,255); return true;
 } return false;
}
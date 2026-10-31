// 0090bbf0 CAIStateGroup_MinionWorkInInn::IsRunnable()
// Codex parked-50 best attempt: DIFFER(100v100); residue: 100/100 bytes; Creature and OUT-slot scratch registers differ, and final definition-delete virtual dispatch uses a different scratch register.
// Leaf: ?IsRunnable@CAIStateGroup_MinionWorkInInn@@UAE_NXZ
// Flags:
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define CThingPhysical CThingPhysicalLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreatureLayout
#include "engine/CThingAICreature.h"
#undef CThingAICreature
#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CSuspendableProcessMicrothread.h"
#define CSimBuildingDef CSimBuildingDefLayout
#include "engine/_quarantine/CSimBuildingDef.h"
#undef CSimBuildingDef
class CBaseObject;
class CBaseIntelligentPointer {public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;private:unsigned char unobservedPointerState[4];};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {};
#pragma pack(push,1)
class CClass:public CBaseObject {public:unsigned char unobservedClassLists[8];unsigned char Type;};
#pragma pack(pop)
enum ETCInterfaceType {};
namespace std {
template<unsigned _Nw>struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned _S_whichword(unsigned p){return p/32;}
    static unsigned _S_whichbit(unsigned p){return p%32;}
    static _WordT _S_maskbit(unsigned p){return _WordT(1)<<_S_whichbit(p);}
    _WordT _M_getword(unsigned p)const{return _M_w[_S_whichword(p)];}
};
template<unsigned _Nb>class bitset:public _Base_bitset<(_Nb+31)/32> {
public:
    bool _Unchecked_test(unsigned p)const{return (this->_M_getword(p)&this->_S_maskbit(p))!=0;}
    bool operator[](unsigned p)const{return _Unchecked_test(p);}
};
}
class CCharStringData;
class CCharString {public:CCharStringData*PStringData;CCharString(const char*,long=-1);~CCharString();};
class CTCBase;class CTCParamsBase;
#pragma pack(push,1)
class CThing:public CClass {
protected:
    unsigned char unobservedThingPrefix[0x20-sizeof(CClass)];
    std::bitset<274> UsingComponents;
    unsigned char unobservedThingSuffix[sizeof(CThingLayout)-0x44];
public:
    bool HasTC(ETCInterfaceType type)const{return UsingComponents[(unsigned)type];}
    CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);
};
class CThingPhysical:public CThing {public:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {public:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {public:unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingBuilding;
class CThingAICreature:public CThingCreatureBase {
private:
    unsigned char unobservedAIPrefix[0x170-sizeof(CThingCreatureBase)];
    CIntelligentPointer<CThingBuilding> WorkBuilding;
    unsigned char unobservedAISuffix[sizeof(CThingAICreatureLayout)-0x178];
public: CThingBuilding* GetWorkBuilding() const;
};
// Layout: original raw PDB microthread base0..13, Component reference14, Creature reference18.
class CMainGameComponent;
class CAIBrain:public CSuspendableProcessMicrothread {
 const CMainGameComponent& Component;
 CThingAICreature& Creature;
 unsigned char unobservedBrainSuffix[sizeof(CAIBrainLayout)-28];
public:CThingAICreature& GetCreature()const{return Creature;}
};
class CAIStateGroupBase:public CBaseClass {protected:CAIBrain*PBrain;private:unsigned char unobservedGroupState[sizeof(CAIStateGroupBaseLayout)-8];};
class CSimBuildingDef:public CParentDefClassBase {public:unsigned char unobservedSimDefState[0x3c-sizeof(CParentDefClassBase)];long Property;};
class CThingBuilding:public CThingGameObject {public:CDefPointer<const CSimBuildingDef>PeekPSimDefinition()const;};
class CAIStateGroup_MinionWorkInInn:public CAIStateGroupBase {public:virtual bool IsRunnable();};
#pragma pack(pop)
inline CThingBuilding* CThingAICreature::GetWorkBuilding()const{return static_cast<CThingBuilding*>(WorkBuilding.GetPItem());}
bool CAIStateGroup_MinionWorkInInn::IsRunnable()
{
    CThingBuilding*work=PBrain->GetCreature().GetWorkBuilding();
    if(!work)return false;
    if(!PBrain->GetCreature().HasTC((ETCInterfaceType)0x23))return false;
    CDefPointer<const CSimBuildingDef>definition=work->PeekPSimDefinition();
    if(definition.Object)return (definition->Property&(1UL<<5))!=0;
    return false;
}

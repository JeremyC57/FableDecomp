// CTCIdleScheduler::RegisterIdleStateGroup(CAIStateGroupBase* group, EIdleStateGroup type) -- register an idle state group under its
// type with a random starting weight: below the global default period, or below the thing def's idle-scheduler period for the type.
// Leaf: ?RegisterIdleStateGroup@CTCIdleScheduler@@QAEXPAVCAIStateGroupBase@@W4EIdleStateGroup@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00801920, 297 bytes (`ret 8`).
// Naming: Ego_r.pdb / FableWin.pdb ?RegisterIdleStateGroup@CTCIdleScheduler@@QAEXPAVCAIStateGroupBase@@W4EIdleStateGroup@@@Z.
// Callees: map<EIdleStateGroup,CAIStateGroupBase*>::operator[] 0x00802110, map<EIdleStateGroup,long>::operator[] 0x00802190 (twice),
//   CTCBase::GetPWorldSeed 0x00686DA0 (twice), GFROR13 0x00497890 (GFRandom expanded at its first use) and GFRandom 0x004CDAE0 (called
//   at its second), CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C,
//   CDefClassBase::GetSubDef<CIdleSchedulerDef> 0x00801670, the def map's const find 0x008014F0 (twice), the defs' release (slot 1).
// Global: the default period 0x013BAF1C (its only reference is here; the name is descriptive).
// Shape: both def pointers are declared before the thing pointer is named, so the def index is read again for the lookup
//   (`cmp word ptr [eax+8Ch],bp` ... `movzx edx,word ptr [eax+8Ch]`); GetDef / GetSubDef and the tree's const find are header bodies
//   the TU sees (called out of line): the thing def lives in ebp after the lookup and its frame word is reused by both find results,
//   the sub def sits in the dead `group` argument slot. The period is looked up twice (the test, then the read inside the GFRandom
//   argument, which stays in the frame until after GetPWorldSeed).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/GFRandom.h"
#include "engine/CTCIdleScheduler.h"
// No shared header: Ego_r.pdb CIdleSchedulerDef (RTTI .?AVCIdleSchedulerDef@@ : CParentDefClassBase), its one member the
// map<EIdleStateGroup, long> Period (retail +0x3C); the landed GetSubDef<CIdleSchedulerDef> 0x00801670 row's model plus the member.
class CIdleSchedulerDef : public CParentDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CIdleSchedulerDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::map<EIdleStateGroup,long> Period;            // +0x3c
};
extern unsigned long GDefaultIdlePeriod;              // 0x013BAF1C (descriptive: read only here)
void CTCIdleScheduler::RegisterIdleStateGroup(CAIStateGroupBase* group,EIdleStateGroup type)
{
    IdleStates[type]=group;
    IdleStatesWeight[type]=GFRandom(GDefaultIdlePeriod,GetPWorldSeed());
    if(ParentThing->GetDefGlobalIndex()!=0){
        CDefPointer<const CIdleSchedulerDef> idle_def;
        CDefPointer<const CThingBaseDef> def;
        const CThing* thing=ParentThing;
        GetDefinitionManager().GetDef(thing->GetDefGlobalIndex(),&def);
        if(def->GetSubDef(&idle_def)){
            const std::map<EIdleStateGroup,long>& period=idle_def->Period;
            if(period.find(type)!=period.end()){
                IdleStatesWeight[type]=GFRandom(period.find(type)->second,GetPWorldSeed());
            }
        }
    }
}
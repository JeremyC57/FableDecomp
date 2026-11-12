// CAIStateGroup_MinionListenToStory::StandUp() -- when the minion's current creature mode is 10 (sitting on the floor), try a CCreatureAction_GetUpFromSitting with the ST_STAND_UP_FROM_FLOOR animation: hand a clone to the brain's output if the creature is free to perform it, then remove mode 10 either way
// Leaf: ?StandUp@CAIStateGroup_MinionListenToStory@@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Object->Data, Block->Info are the header CCountedPointer's (same offsets); the local CCountBlock (= CCPPointerInfo) members Count->RefCount, Deleter->DeleteFunc, Object->Data are the header CCPPointerInfo's (same offsets).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00910570, 320 bytes (incl. int3 padding), thiscall, plain `ret`. FableWin.pdb
//   `?StandUp@CAIStateGroup_MinionListenToStory@@IAEXXZ`; OnReplace (vtable 0x01289180 slot 10, 0x009106B0, landed) is
//   the tail call `jmp 0x00910570`. The catalog's "CTCLightningOrb_Shutdown" is a propagated label.
// Callees: CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of +0x24),
//   CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (catalog "CPixelShader::CountReferences"), the mode's slot 1
//   GetType, CBrainOutput() 0x00835620, CCombatActionBase(creature, long, const CSoundPair&) 0x00858030,
//   CCombatActionBase::SetCombatAnimation 0x008574D0, CThingCreatureBase::IsFreeToPerformAction 0x00662210, the
//   action's slot 11 Clone, CCountedPointer<CCreatureActionBase>::Reset 0x00668D90, CAIBrain::SetOutput 0x0088B8F0,
//   CTCCreatureModeManager::RemoveMode 0x0073ABA0, ~CCombatActionBase 0x0062E900, ~CBaseIntelligentPointer 0x00A01C10,
//   operator delete 0x00BFE9BC (the counted pointer's block).
// Layout: as the landed NCreatureMode::GetUpFromSitting 0x0085FF10 and CAIStateGroup_UsesSeat::OnReplace 0x009387F0:
//   CCreatureAction_GetUpFromSitting (vtable 0x01276C2C, 0x11C bytes) = CCombatActionBase 0x114 + mode type + bool;
//   CBrainOutput 0x48 bytes (Types +0, PAction +0x20, PTarget +0x40) with an inline destructor here.
// Shape: the mode-manager pointer is set only under the presence bit (a miss reads the key's slot); the brain output is
//   constructed before the action; the action uses the inline 2-argument constructor (0 / false, then
//   SetCombatAnimation); the clone goes through an inline CBrainOutput helper taking `const CCreatureActionBase&`.
// Name fix (agentNF6 2026-11-10): CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 is ?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ (Ego_r.pdb / FableWin.pdb; engine/CTCCreatureModeManager.h): const, returns a reference to the nested CTCCreatureModeManager::CModeBase.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
namespace { extern CCharString GET_UP_FROM_FLOOR_ANIM_NAME; }                  // 0x013BB670 "ST_STAND_UP_FROM_FLOOR"

class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
public:
    bool IsFreeToPerformAction(const CCreatureActionBase&) const;   // 0x00662210
};

class CSoundPair {
public:
    long First;
    long Second;
    CSoundPair() : First(0), Second(0) { }
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CCreatureActionBase* Clone() const;             // slot 11 (+0x2c)
};

class CCombatActionBase : public CCreatureActionBase {      // retail sizeof 0x114
public:
    unsigned char _pad04[0x114 - 0x04];
    CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                                   // 0x0062E900
    void SetCombatAnimation(const CCharString& name);       // 0x008574D0
};

class CCreatureAction_GetUpFromSitting : public CCombatActionBase {   // retail sizeof 0x11c, vtable 0x01276C2C
public:
    long SitModeType;                                       // +0x114
    bool Flag;                                              // +0x118
    FABLE_FORCEINLINE CCreatureAction_GetUpFromSitting(CThingCreatureBase& creature, const CCharString& anim)
        : CCombatActionBase(creature, 0x32, CSoundPair()), SitModeType(0), Flag(false)
    {
        SetCombatAnimation(anim);
    }
    virtual CCreatureActionBase* Clone() const;
};



class CTCCreatureModeManager { public:
    class CModeBase {
public:
    virtual ~CModeBase();
    virtual unsigned long GetType() const;                  // slot 1
};                 // CTCCreatureModeManager::CModeBase (Ego_r.pdb / FableWin.pdb nested class)                              // interface type 0x31
public:
    CTCCreatureModeManager::CModeBase& GetCurrentMode() const;                      // 0x0073A6E0
    void RemoveMode(unsigned long mode);                    // 0x0073ABA0
};

#include "engine/CCountedPointer.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CBrainOutput {                                        // 0x48 bytes
public:
    unsigned long Types;                                    // +0x00
    long          LookFlags;                                // +0x04
    unsigned char NavigationInput[0x18];                    // +0x08
    CCountedPointer<CCreatureActionBase> PAction;           // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;                        // +0x40
    CBrainOutput();                                         // 0x00835620
    void SetAsPerformAction(const CCreatureActionBase& action)   // inline (descriptive name: sets type 8 + the clone)
    {
        Types |= 8;
        PAction.Reset(action.Clone());
    }
};

class CAIBrain {
public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                          // +0x18
    void SetOutput(const CBrainOutput& output);             // 0x0088B8F0
};

class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                       // +0x04
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
};

class CAIStateGroup_MinionListenToStory : public CAIStateGroupBase {
public:
    void StandUp();
};

void CAIStateGroup_MinionListenToStory::StandUp()
{
    CTCCreatureModeManager* modes;
    GetCreature()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
    if (modes->GetCurrentMode().GetType() == 10) {
        CBrainOutput output;
        CCreatureAction_GetUpFromSitting action(*GetCreature(), GET_UP_FROM_FLOOR_ANIM_NAME);
        if (GetCreature()->IsFreeToPerformAction(action)) {
            output.SetAsPerformAction(action);
            PBrain->SetOutput(output);
        }
        modes->RemoveMode(10);
    }
}
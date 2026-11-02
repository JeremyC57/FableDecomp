// NCreatureMode::CModeUnderground::OnDeactivate() -- clear the AI creature's ImmuneToAttack flag (set by OnActivate), the stand-mode OnDeactivate, then restore the saved targeting flags
// Leaf: ?OnDeactivate@CModeUnderground@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6710, length 38 bytes, thiscall, tail jump.
// Identity: the only reference is slot 6 of vtable 0x01267E38 (RTTI NCreatureMode::CModeUnderground; slot 5 is the landed OnActivate
//   0x008D66E0, slot 3 the landed FrameUpdate 0x008D9410). Ego_r + FableWin ?OnDeactivate@CModeUnderground@NCreatureMode@@UAEXXZ
//   (donor order OnActivate / OnDeactivate in Ego_r; FableWin emits the inline ?SetAsImmuneToAttack@CThingAICreature@@QAEX_N@Z COMDAT
//   right after OnActivate, the mirror store of this row). RevertOldTargetFlags' landed header names this row as its only caller.
// Callees: CModeStandBase::OnDeactivate @ 0x00861D00 (Ego_r ?OnDeactivate@CModeStandBase@@UAEXXZ), NCreatureMode::RevertOldTargetFlags
//   (CThing&, ETargetingType) @ 0x0085F880 (Ego_r __fastcall ?RevertOldTargetFlags@NCreatureMode@@YIXAAVCThing@@W4ETargetingType@@@Z; tail jump).
// Layout: CModeBase ParentThing +4; CThing ThingTypeFlags +0x6c (0x10 tested: the AI-creature class bit, as OnActivate 0x008D66E0);
//   CThingAICreature flag byte +0x1c8 (Ego_r +0x1f8), ImmuneToAttack bit 6 (landed SetHasInformation 0x00830EB0); CModeUnderground
//   saved targeting flags +0x74 (written by OnActivate from GetOldTargetFlagsAndReplaceWith; descriptive member name).
// Model: CThing : CClass : CBaseObject on engine/CBaseObject.h (retail opt-in) + engine/CClass.h (layout-rename include).
// Shape: the flag clear happens before the base call; the saved flags are loaded into edx before the thing into ecx (right-to-left).
enum ETargetingType { };

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // CBaseObject : CBaseClass (IntelligentPointer +4)
#define CClass CClassFields                             // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

class CClass : public CBaseObject { public:            // RTTI CClass : CBaseObject; fields as engine/CClass.h
    unsigned char ClassListPos[0x4];                    // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                     // +0x0c list<CClass*>::iterator
    unsigned char Type;                                 // +0x10
    unsigned long GetType() const { return Type; }      // FableWin.pdb ?GetType@CClass@@QBEKXZ (inline)
};

class CThing : public CClass { public:                  // RTTI CThing : CClass
    unsigned char _pad14[0x6c - 0x14];
    unsigned long ThingTypeFlags;                       // +0x6c
};

class CThingAICreature : public CThing { public:
    unsigned char _pad70[0x1c8 - 0x70];
    bool PerformingVillageTaskFlag : 1;                 // +0x1c8 bit 0
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;                            // bit 6
    bool EnableCreatureAutoPlacing : 1;
    void SetAsImmuneToAttack(bool immune) { ImmuneToAttack = immune; }   // FableWin ?SetAsImmuneToAttack@CThingAICreature@@QAEX_N@Z (inline)
};

namespace NCreatureMode {
void __fastcall RevertOldTargetFlags(CThing& thing, ETargetingType flags);   // 0x0085F880
}

class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        virtual ~CModeBase();                           // slot 0
        virtual void v01();
        virtual void v02();
        virtual void v03();
        virtual void v04();
        virtual void OnActivate();                      // slot 5
        virtual void OnDeactivate();                    // slot 6
        CThing* ParentThing;                            // +0x04
    };
};

class CModeStandBase : public CTCCreatureModeManager::CModeBase {
public:
    virtual void OnActivate();                          // 0x008621A0
    virtual void OnDeactivate();                        // 0x00861D00
    unsigned char _pad08[0x68 - 0x08];
};

namespace NCreatureMode {
class CModeUnderground : public CModeStandBase {        // vtable 0x01267E38
public:
    virtual void OnActivate();                          // slot 5: 0x008D66E0
    virtual void OnDeactivate();                        // slot 6: this row
    unsigned char  _pad68[0x74 - 0x68];
    ETargetingType OldTargetFlags;                      // +0x74
};

void CModeUnderground::OnDeactivate()
{
    if (ParentThing->ThingTypeFlags & 0x10)
        static_cast<CThingAICreature*>(ParentThing)->SetAsImmuneToAttack(false);
    CModeStandBase::OnDeactivate();
    RevertOldTargetFlags(*ParentThing, OldTargetFlags);
}
}
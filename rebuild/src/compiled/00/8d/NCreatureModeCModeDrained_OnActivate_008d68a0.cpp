// NCreatureMode::CModeDrained::OnActivate() -- entering the drained mode: an AI creature has its AI suspended, a player creature's player enters player mode 31 (not exclusively); then the stand-mode OnActivate
// Leaf: ?OnActivate@CModeDrained@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D68A0, 58 bytes (int3 padding to 0x008D68E0), virtual, both exits tail-jump to the base OnActivate.
// Identity: the only reference is slot 5 (OnActivate in the creature-mode family; slot 6 OnDeactivate) of vtable 0x01268234 (RTTI
//   NCreatureMode::CModeDrained). Ego_r.pdb + FableWin.pdb ?OnActivate@CModeDrained@NCreatureMode@@UAEXXZ; donor order OnActivate /
//   OnDeactivate / FrameUpdate (both PDBs). The catalog row named the method without its class namespace.
// Callees: CThingAICreature::SuspendAI(bool, void*) 0x00833A10 (both PDBs), CThing::GetPlayer() 0x004C7A30 (both PDBs
//   ?GetPlayer@CThing@@QAEAAVCPlayer@@XZ, returns CPlayer&), CPlayer::AddMode(EPlayerMode, bool) 0x006345C0 (both PDBs), CModeStandBase::OnActivate
//   0x008621A0 (Ego_r + FableWin ?OnActivate@CModeStandBase@@UAEXXZ, global namespace; tail jump).
// Shape: CClass::Type read once into al (1 = AI creature, 2 = player creature; the landed CModeDragonFireBeam::OnDeactivate 0x008DBB00 tests
//   the same byte); VC7.1 duplicates the base tail jump into the first arm. Both calls push their arguments before the object (SuspendAI
//   on ParentThing in ecx, AddMode on the GetPlayer result). The suspender is null here (OnDeactivate 0x008DA7A0, slot 6, resumes with SuspendAI(false, 0));
//   the EPlayerMode enumerator names are in no name set, so 31 is written as a cast.
// Model: as the landed CModeDragonFireBeam::OnDeactivate 0x008DBB00 and CCreatureAction_ActivateZTarget::PerformAction 0x008424A0:
//   engine/CBaseObject.h (retail opt-in) + engine/CClass.h (layout-rename include); CTCCreatureModeManager::CModeBase ParentThing +4.
//   engine/CModeStandBase.h and engine/CPlayer.h are flattened generated layouts (vptr as data, no member functions), so the mode chain
//   and the one CPlayer member are declared here.
enum EPlayerMode { };

class CPlayer {
public:
    void AddMode(EPlayerMode mode, bool exclusive);     // 0x006345C0 ?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // CBaseObject : CBaseClass (Validate slot 1, IntelligentPointer +4)
#define CClass CClassFields                             // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

class CClass : public CBaseObject { public:            // RTTI CClass : CBaseObject; fields as engine/CClass.h
    unsigned char ClassListPos[0x4];                    // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                     // +0x0c list<CClass*>::iterator
    unsigned char Type;                                 // +0x10
    unsigned long GetType() const { return Type; }      // FableWin.pdb ?GetType@CClass@@QBEKXZ (inline)
};

class CThing : public CClass {                         // RTTI CThing : CClass
public:
    CPlayer& GetPlayer();                               // 0x004C7A30 ?GetPlayer@CThing@@QAEAAVCPlayer@@XZ
};

class CThingAICreature : public CThing {               // CThingCreatureBase .. CThing chain, offset 0
public:
    void SuspendAI(bool suspend, void* suspender);      // 0x00833A10 ?SuspendAI@CThingAICreature@@QAEX_NPAX@Z
};

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
};

namespace NCreatureMode {
class CModeDrained : public CModeStandBase {            // vtable 0x01268234
public:
    virtual void OnActivate();                          // slot 5: this row
};

void CModeDrained::OnActivate()
{
    if (ParentThing->GetType() == 1)
        static_cast<CThingAICreature*>(ParentThing)->SuspendAI(true, 0);
    else if (ParentThing->GetType() == 2)
        ParentThing->GetPlayer().AddMode((EPlayerMode)31, false);
    CModeStandBase::OnActivate();
}
}
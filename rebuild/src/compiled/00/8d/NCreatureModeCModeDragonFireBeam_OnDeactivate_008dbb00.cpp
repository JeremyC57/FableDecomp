// NCreatureMode::CModeDragonFireBeam::OnDeactivate() -- kill the beam effect, resume the dragon's AI (undo OnActivate's SuspendAI), then the stand-mode OnDeactivate
// Leaf: ?OnDeactivate@CModeDragonFireBeam@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DBB00, length 33 bytes, thiscall `ret` by tail jump.
// Identity: the only reference is slot 6 of vtable 0x01285900 (RTTI NCreatureMode::CModeDragonFireBeam; slot 6 = OnDeactivate in the
//   mode family, slot 5 = OnActivate). Ego_r + FableWin ?OnDeactivate@CModeDragonFireBeam@NCreatureMode@@UAEXXZ, listed beside
//   KillBeamEffect in both PDBs; its first call is the landed private KillBeamEffect 0x008DBA90 (whose only caller is this row).
// Callees: CModeDragonFireBeam::KillBeamEffect @ 0x008DBA90 (private `AAE`), CThingAICreature::SuspendAI(bool, void*) @ 0x00833A10,
//   CModeStandBase::OnDeactivate @ 0x00861D00 (Ego_r ?OnDeactivate@CModeStandBase@@UAEXXZ, global namespace; tail jump).
// Model: as the landed CModeGuildmasterPowerUpHeal::OnDeactivate 0x008D69A0, on engine/CBaseObject.h (retail opt-in) + engine/CClass.h
//   (layout-rename include; CClass::Type byte +0x10, type 1 = AI creature;
//   CModeBase ParentThing +4). engine/CModeStandBase.h is flattened (vptr as data), so the mode chain is modelled locally.
class CThing;

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

class CThing : public CClass {
};

class CThingAICreature : public CThing {               // CThingCreatureBase .. CThing chain, offset 0
public:
    void SuspendAI(bool suspend, void* suspender);      // 0x00833A10
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
    virtual void OnDeactivate();                        // 0x00861D00
};

namespace NCreatureMode {
class CModeDragonFireBeam : public CModeStandBase {     // vtable 0x01285900
public:
    virtual void OnActivate();                          // slot 5
    virtual void OnDeactivate();                        // slot 6: this row
private:
    void KillBeamEffect();                              // 0x008DBA90
};

void CModeDragonFireBeam::OnDeactivate()
{
    KillBeamEffect();
    if (ParentThing->GetType() == 1)
        static_cast<CThingAICreature*>(ParentThing)->SuspendAI(false, this);
    CModeStandBase::OnDeactivate();
}
}
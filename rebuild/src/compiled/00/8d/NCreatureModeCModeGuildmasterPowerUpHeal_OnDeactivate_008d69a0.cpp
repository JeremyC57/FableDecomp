// NCreatureMode::CModeGuildmasterPowerUpHeal::OnDeactivate() -- resume the creature's AI (undo OnActivate's SuspendAI), then the stand-mode OnDeactivate
// Leaf: ?OnDeactivate@CModeGuildmasterPowerUpHeal@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D69A0, 28 bytes, thiscall: `if (ParentThing->Type == 1)
//   ParentThing->SuspendAI(false, this)` (`cmp byte ptr [ecx+10h],1 ; push esi ; push 0 ; call`), then
//   `jmp CModeStandBase::OnDeactivate` 0x00861D00.
// Identity: the only reference is slot 6 of vtable 0x012685BC, whose RTTI is NCreatureMode::CModeGuildmasterPowerUpHeal
//   (rtti-vtables.tsv; bases CModeStandBase : CTCCreatureModeManager::CModeBase); slot 6 is OnDeactivate in the mode family
//   (CModeDragonRearUp's slot 6 is the landed twin 0x008D6A10; base slot = CModeBase::OnDeactivate). The same vtable's slot 5
//   is the landed CModeGuildmasterPowerUpHeal::OnActivate 0x008DDD70, which opens with the mirror image
//   (`ParentThing->SuspendAI(true, this)` under the same Type test). Ego_r.pdb
//   ?OnDeactivate@CModeGuildmasterPowerUpHeal@NCreatureMode@@UAEXXZ (donor 0x00817D20) beside ?OnActivate@... (0x0081E4A0).
//   The manifest "CModeDragonRearUp::OnDeactivate" is the twin's name.
// Callees: CThingAICreature::SuspendAI(bool, void*) 0x00833A10 (landed; Ego_r ?SuspendAI@CThingAICreature@@QAEX_NPAX@Z),
//   CModeStandBase::OnDeactivate 0x00861D00 (Ego_r ?OnDeactivate@CModeStandBase@@UAEXXZ, global namespace).
// Model: CThing : CClass (rtti_bases.tsv), CClass::Type the byte at +0x10 (engine/CClass.h; FableWin.pdb inline
//   ?GetType@CClass@@QBEKXZ); type 1 is the AI-creature class (the enum's name is unproven, so a literal). CThingAICreature
//   derives from CThing at offset 0. engine/CModeStandBase.h is flattened (vptr as data), so the mode chain is modelled
//   locally with the Ego_r CModeBase members (ParentThing +4); header debt in NOTES.
class CThing;

class CClass {
public:
    virtual void v00();
    unsigned long GetType() const { return Type; }      // FableWin.pdb ?GetType@CClass@@QBEKXZ (inline)
    unsigned char _base_0x04[0xc];                      // +0x04 (CBaseObject IntelligentPointer, ClassListPos, TypeListPos)
    unsigned char Type;                                 // +0x10
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
class CModeGuildmasterPowerUpHeal : public CModeStandBase {   // vtable 0x012685BC
public:
    virtual void OnActivate();                          // slot 5: 0x008DDD70
    virtual void OnDeactivate();                        // slot 6: this row
};

void CModeGuildmasterPowerUpHeal::OnDeactivate()
{
    if (ParentThing->GetType() == 1)
        static_cast<CThingAICreature*>(ParentThing)->SuspendAI(false, this);
    CModeStandBase::OnDeactivate();
}
}
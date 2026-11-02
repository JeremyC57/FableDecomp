// CThingDeadCreature::WorldUpdateBulletTime() -- refresh the body's graphic appearance when it has component 0x83, then the base bullet-time update
// Leaf: ?WorldUpdateBulletTime@CThingDeadCreature@@UAEXXZ
// Naming wave 31 (family srtf): 0x004C7590 is ?SetAsRenderedThisFrame@CThing@@QAEXK@Z (Ego_r.pdb block order at 0x004C9150 + FableWin.pdb; was the
//   invented ?SetGraphicAppearanceChanged@CThing@@QAEXK@Z)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836240, 24 bytes, thiscall: `test byte [esi+30h],8` (bit 0x83 of the UsingComponents
//   bitset at +0x20) -> `push 2 ; call 0x004C7590`, then `jmp CThing::WorldUpdateBulletTime` 0x004C8BA0 (qualified base call).
// Identity (proven): the only reference is slot 20 of the CThingDeadCreature vtable 0x01272B04 (0x01272B54); slot 20 of the CThing
//   vtable 0x01239ABC is 0x004C8BA0 = CThing::WorldUpdateBulletTime (its own body tests interface 0x46 = bit 70, `[edi+28h] & 40h`),
//   and CThingAICreature / CThingPlayerCreature slot 20 hold their overrides. Ego_r.pdb name set:
//   ?WorldUpdateBulletTime@CThingDeadCreature@@UAEXXZ (donor 0x00829BD0, 0x20 before ?WorldUpdate@CThingDeadCreature@@UAE_NXZ
//   0x00829BF0) = retail 0x00836240 / landed CThingDeadCreature::WorldUpdate 0x00836260, same 0x20 spacing.
//   The catalog label "CEnginePrimitiveManager2DAnimatedMeshes::RenderSubPrimitive" is a propagated guess.
// Callees: CThing::SetGraphicAppearanceChanged(unsigned long) 0x004C7590 (the landed row's own spelling, descriptive: no PDB name;
//   `if (PTCGraphicAppearance) PTCGraphicAppearance->SetAsChanged(flags)`), CThing::WorldUpdateBulletTime 0x004C8BA0 (virtual:
//   Ego_r ?WorldUpdateBulletTime@CThing@@UAEXXZ; the binding QAEXXZ is wrong, NAME_FIXES).
// Shape: the same `HasTC(0x83)` bitset test + appearance call (reason 2) as the landed WorldUpdate 0x00836260 next door.
// Layout (retail): CThing UsingComponents bitset<274> at +0x20 (nine dwords); local model (engine/CThing.h carries the Ego_r
//   offsets, 0x10 high past the containers), as the landed 0x00836260.
class CThing {
public:
    virtual ~CThing();
    virtual void WorldUpdateBulletTime();               // slot 20: 0x004C8BA0
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20 UsingComponents (bitset<274>)
    void SetAsRenderedThisFrame(unsigned long reasons); // 0x004C7590
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};

class CThingGameObject : public CThing {
};

class CThingDeadCreature : public CThingGameObject {    // vtable 0x01272B04
public:
    virtual void WorldUpdateBulletTime();               // slot 20: this row
};

void CThingDeadCreature::WorldUpdateBulletTime()
{
    if (HasTC(0x83))
        SetAsRenderedThisFrame(2);
    CThingGameObject::WorldUpdateBulletTime();
}
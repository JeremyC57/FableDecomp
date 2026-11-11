// NEntityEvents::CEventAttackedWithMeleeWeaponWithoutHittingAnything::GetType() const -- returns (EEventType)0x1e
// Leaf: ?GetType@CEventAttackedWithMeleeWeaponWithoutHittingAnything@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085A430, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventAttackedWithMeleeWeaponWithoutHittingAnything's vtable 0x01276484 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventAttackedWithMeleeWeaponWithoutHittingAnything or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventAttackedWithMeleeWeaponWithoutHittingAnything@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventAttackedWithMeleeWeaponWithoutHittingAnything {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventAttackedWithMeleeWeaponWithoutHittingAnything::GetType() const
{
    return (EEventType)0x1e;
}
}

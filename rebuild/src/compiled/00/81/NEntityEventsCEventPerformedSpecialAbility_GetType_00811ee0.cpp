// NEntityEvents::CEventPerformedSpecialAbility::GetType() const -- returns (EEventType)0x1a
// Leaf: ?GetType@CEventPerformedSpecialAbility@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00811EE0, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventPerformedSpecialAbility's vtable 0x012723C0 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventPerformedSpecialAbility or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventPerformedSpecialAbility@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventPerformedSpecialAbility {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventPerformedSpecialAbility::GetType() const
{
    return (EEventType)0x1a;
}
}

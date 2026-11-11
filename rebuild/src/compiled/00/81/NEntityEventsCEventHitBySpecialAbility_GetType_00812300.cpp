// NEntityEvents::CEventHitBySpecialAbility::GetType() const -- returns (EEventType)0x1b
// Leaf: ?GetType@CEventHitBySpecialAbility@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00812300, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventHitBySpecialAbility's vtable 0x012723F0 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventHitBySpecialAbility or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventHitBySpecialAbility@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventHitBySpecialAbility {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventHitBySpecialAbility::GetType() const
{
    return (EEventType)0x1b;
}
}

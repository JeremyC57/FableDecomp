// NEntityEvents::CEventHitBySpecialAbility::IsFlourish() const -- returns false
// Leaf: ?IsFlourish@CEventHitBySpecialAbility@NEntityEvents@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00812310, thiscall, `ret`.
// Identity: slot 5 of NEntityEvents::CEventHitBySpecialAbility's vtable 0x012723F0 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventHitBySpecialAbility or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts IsFlourish(void) at this slot; Ego_r/FableWin name set: ?IsFlourish@CEventHitBySpecialAbility@NEntityEvents@@UBE_NXZ; `ret` matches the signature.
namespace NEntityEvents {
class CEventHitBySpecialAbility {
public:
    virtual bool IsFlourish() const;
};
bool CEventHitBySpecialAbility::IsFlourish() const
{
    return false;
}
}

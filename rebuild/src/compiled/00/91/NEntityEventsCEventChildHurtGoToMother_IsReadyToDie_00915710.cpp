// NEntityEvents::CEventChildHurtGoToMother::IsReadyToDie() const -- returns false
// Leaf: ?IsReadyToDie@CEventChildHurtGoToMother@NEntityEvents@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00915710, thiscall, `ret`.
// Identity: slot 4 of NEntityEvents::CEventChildHurtGoToMother's vtable 0x0128FA80 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventChildHurtGoToMother or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts IsReadyToDie(void) at this slot; Ego_r/FableWin name set: ?IsReadyToDie@CEventChildHurtGoToMother@NEntityEvents@@UBE_NXZ; `ret` matches the signature.
namespace NEntityEvents {
class CEventChildHurtGoToMother {
public:
    virtual bool IsReadyToDie() const;
};
bool CEventChildHurtGoToMother::IsReadyToDie() const
{
    return false;
}
}

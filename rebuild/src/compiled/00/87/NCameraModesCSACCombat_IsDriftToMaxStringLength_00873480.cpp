// NCameraModes::CSACCombat::IsDriftToMaxStringLength() const -- returns true
// Leaf: ?IsDriftToMaxStringLength@CSACCombat@NCameraModes@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873480, thiscall, `ret`.
// Identity: slot 31 of NCameraModes::CSACCombat's vtable 0x012775B4 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CSACCombat or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsDriftToMaxStringLength(void) at this slot; Ego_r/FableWin name set: ?IsDriftToMaxStringLength@CSACCombat@NCameraModes@@MBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CSACCombat {
public:
protected:
    virtual bool IsDriftToMaxStringLength() const;
};
bool CSACCombat::IsDriftToMaxStringLength() const
{
    return true;
}
}

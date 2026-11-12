// NCameraModes::CPCMouseLook::IsShortenOnOcclusion() const -- returns true
// Leaf: ?IsShortenOnOcclusion@CPCMouseLook@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00874E60, thiscall, `ret`.
// Identity: slot 6 of NCameraModes::CPCMouseLook's vtable 0x01277784 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CPCMouseLook or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@CPCMouseLook@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CPCMouseLook {
public:
    virtual bool IsShortenOnOcclusion() const;
};
bool CPCMouseLook::IsShortenOnOcclusion() const
{
    return true;
}
}

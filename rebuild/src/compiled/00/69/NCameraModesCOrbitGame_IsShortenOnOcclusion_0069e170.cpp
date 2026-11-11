// NCameraModes::COrbitGame::IsShortenOnOcclusion() const -- returns false
// Leaf: ?IsShortenOnOcclusion@COrbitGame@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E170, thiscall, `ret`.
// Identity: slot 6 of NCameraModes::COrbitGame's vtable 0x0125C6DC (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::COrbitGame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@COrbitGame@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class COrbitGame {
public:
    virtual bool IsShortenOnOcclusion() const;
};
bool COrbitGame::IsShortenOnOcclusion() const
{
    return false;
}
}

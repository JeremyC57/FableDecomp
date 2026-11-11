// NCreatureMode::CModeSitTavernStool::GetType() const -- returns 0xb
// Leaf: ?GetType@CModeSitTavernStool@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094B5B0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSitTavernStool's vtable 0x01292FF8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSitTavernStool or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSitTavernStool@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSitTavernStool {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSitTavernStool::GetType() const
{
    return 0xb;
}
}

// NCreatureMode::CModeSitBench::GetType() const -- returns 9
// Leaf: ?GetType@CModeSitBench@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007400C0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSitBench's vtable 0x01267BFC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSitBench or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSitBench@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSitBench {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSitBench::GetType() const
{
    return 9;
}
}

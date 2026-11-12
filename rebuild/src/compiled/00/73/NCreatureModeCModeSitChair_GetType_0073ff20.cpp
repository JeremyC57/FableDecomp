// NCreatureMode::CModeSitChair::GetType() const -- returns 8
// Leaf: ?GetType@CModeSitChair@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FF20, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSitChair's vtable 0x01267B74 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSitChair or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSitChair@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSitChair {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSitChair::GetType() const
{
    return 8;
}
}

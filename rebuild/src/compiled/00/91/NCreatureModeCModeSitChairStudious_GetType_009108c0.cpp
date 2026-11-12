// NCreatureMode::CModeSitChairStudious::GetType() const -- returns 8
// Leaf: ?GetType@CModeSitChairStudious@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009108C0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSitChairStudious's vtable 0x0128F590 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSitChairStudious or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSitChairStudious@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSitChairStudious {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSitChairStudious::GetType() const
{
    return 8;
}
}

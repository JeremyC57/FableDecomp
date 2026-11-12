// NCreatureMode::CModeCurious::GetType() const -- returns 0x17
// Leaf: ?GetType@CModeCurious@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F600, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCurious's vtable 0x01267890 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCurious or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCurious@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCurious {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCurious::GetType() const
{
    return 0x17;
}
}

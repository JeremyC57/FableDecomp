// NCreatureMode::CModeCheering::GetType() const -- returns 0x12
// Leaf: ?GetType@CModeCheering@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F390, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCheering's vtable 0x012677CC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCheering or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCheering@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCheering {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCheering::GetType() const
{
    return 0x12;
}
}

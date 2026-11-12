// NCreatureMode::CModeBully::GetType() const -- returns 0x18
// Leaf: ?GetType@CModeBully@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F6D0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeBully's vtable 0x012678CC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeBully or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeBully@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeBully {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeBully::GetType() const
{
    return 0x18;
}
}

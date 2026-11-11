// NCreatureMode::CModeHeckling::GetType() const -- returns 0x13
// Leaf: ?GetType@CModeHeckling@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F460, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeHeckling's vtable 0x0126780C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeHeckling or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeHeckling@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeHeckling {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeHeckling::GetType() const
{
    return 0x13;
}
}

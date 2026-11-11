// NCreatureMode::CModeCreeping::GetType() const -- returns 0x11
// Leaf: ?GetType@CModeCreeping@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F2C0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCreeping's vtable 0x0126778C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCreeping or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCreeping@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCreeping {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCreeping::GetType() const
{
    return 0x11;
}
}

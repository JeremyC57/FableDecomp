// NCreatureMode::CModeIntangible::GetType() const -- returns 0x51
// Leaf: ?GetType@CModeIntangible@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741EA0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeIntangible's vtable 0x01268514 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeIntangible or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeIntangible@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeIntangible {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeIntangible::GetType() const
{
    return 0x51;
}
}

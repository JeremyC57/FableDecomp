// NCreatureMode::CModeDrunk::GetType() const -- returns 0x2c
// Leaf: ?GetType@CModeDrunk@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741190, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDrunk's vtable 0x0126816C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDrunk or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDrunk@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDrunk {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDrunk::GetType() const
{
    return 0x2c;
}
}

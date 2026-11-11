// NCreatureMode::CModeWalkInAllDirections::GetType() const -- returns 0x1c
// Leaf: ?GetType@CModeWalkInAllDirections@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743E10, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeWalkInAllDirections's vtable 0x012687AC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeWalkInAllDirections or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeWalkInAllDirections@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeWalkInAllDirections {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeWalkInAllDirections::GetType() const
{
    return 0x1c;
}
}

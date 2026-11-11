// NCreatureMode::CModeBasic::GetType() const -- returns 0x58
// Leaf: ?GetType@CModeBasic@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073EBB0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeBasic's vtable 0x012675BC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeBasic or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeBasic@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeBasic {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeBasic::GetType() const
{
    return 0x58;
}
}

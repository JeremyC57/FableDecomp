// NCreatureMode::CModeBlock::GetType() const -- returns 0xc
// Leaf: ?GetType@CModeBlock@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740130, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeBlock's vtable 0x01267C3C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeBlock or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeBlock@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeBlock {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeBlock::GetType() const
{
    return 0xc;
}
}

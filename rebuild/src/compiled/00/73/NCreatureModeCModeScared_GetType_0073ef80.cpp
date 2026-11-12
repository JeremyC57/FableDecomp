// NCreatureMode::CModeScared::GetType() const -- returns 0xf
// Leaf: ?GetType@CModeScared@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073EF80, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeScared's vtable 0x012676A4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeScared or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeScared@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeScared {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeScared::GetType() const
{
    return 0xf;
}
}

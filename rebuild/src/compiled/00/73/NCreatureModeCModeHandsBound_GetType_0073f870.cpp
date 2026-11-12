// NCreatureMode::CModeHandsBound::GetType() const -- returns 0x2e
// Leaf: ?GetType@CModeHandsBound@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F870, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeHandsBound's vtable 0x01267944 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeHandsBound or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeHandsBound@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeHandsBound {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeHandsBound::GetType() const
{
    return 0x2e;
}
}

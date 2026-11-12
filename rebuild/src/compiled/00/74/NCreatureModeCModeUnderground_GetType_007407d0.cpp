// NCreatureMode::CModeUnderground::GetType() const -- returns 0x1e
// Leaf: ?GetType@CModeUnderground@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007407D0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeUnderground's vtable 0x01267E38 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeUnderground or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeUnderground@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeUnderground {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeUnderground::GetType() const
{
    return 0x1e;
}
}

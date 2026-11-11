// NCreatureMode::CModeScripted::GetType() const -- returns 0x57
// Leaf: ?GetType@CModeScripted@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073EEB0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeScripted's vtable 0x01267670 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeScripted or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeScripted@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeScripted {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeScripted::GetType() const
{
    return 0x57;
}
}

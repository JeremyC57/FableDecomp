// NCreatureMode::CModeOutraged::GetType() const -- returns 0x10
// Leaf: ?GetType@CModeOutraged@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F050, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeOutraged's vtable 0x012676E0 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeOutraged or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeOutraged@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeOutraged {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeOutraged::GetType() const
{
    return 0x10;
}
}

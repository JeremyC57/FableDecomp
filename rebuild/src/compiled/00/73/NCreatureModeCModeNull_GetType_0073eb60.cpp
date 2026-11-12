// NCreatureMode::CModeNull::GetType() const -- returns 0
// Leaf: ?GetType@CModeNull@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073EB60, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeNull's vtable 0x01267588 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeNull or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeNull@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeNull {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeNull::GetType() const
{
    return 0;
}
}

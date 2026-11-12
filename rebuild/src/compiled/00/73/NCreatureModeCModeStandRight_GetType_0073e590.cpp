// NCreatureMode::CModeStandRight::GetType() const -- returns 1
// Leaf: ?GetType@CModeStandRight@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E590, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeStandRight's vtable 0x01267534 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandRight or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeStandRight@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandRight {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeStandRight::GetType() const
{
    return 1;
}
}

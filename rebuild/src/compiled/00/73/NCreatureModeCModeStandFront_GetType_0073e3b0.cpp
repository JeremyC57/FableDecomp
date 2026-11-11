// NCreatureMode::CModeStandFront::GetType() const -- returns 1
// Leaf: ?GetType@CModeStandFront@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E3B0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeStandFront's vtable 0x012674AC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandFront or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeStandFront@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandFront {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeStandFront::GetType() const
{
    return 1;
}
}

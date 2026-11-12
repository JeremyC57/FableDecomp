// NCreatureMode::CModeRollAround::GetType() const -- returns 0x46
// Leaf: ?GetType@CModeRollAround@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741540, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeRollAround's vtable 0x01268270 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeRollAround or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeRollAround@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeRollAround {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeRollAround::GetType() const
{
    return 0x46;
}
}

// NCreatureMode::CModeChargeForcePush::GetType() const -- returns 0x5b
// Leaf: ?GetType@CModeChargeForcePush@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007417B0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeChargeForcePush's vtable 0x01268328 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeChargeForcePush or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeChargeForcePush@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeChargeForcePush {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeChargeForcePush::GetType() const
{
    return 0x5b;
}
}

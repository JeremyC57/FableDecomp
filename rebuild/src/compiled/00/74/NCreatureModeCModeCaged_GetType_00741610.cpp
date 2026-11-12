// NCreatureMode::CModeCaged::GetType() const -- returns 0x56
// Leaf: ?GetType@CModeCaged@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741610, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCaged's vtable 0x012682B0 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCaged or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCaged@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCaged {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCaged::GetType() const
{
    return 0x56;
}
}

// NCreatureMode::CModeSleepBed::GetType() const -- returns 0xd
// Leaf: ?GetType@CModeSleepBed@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084B800, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSleepBed's vtable 0x01275760 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSleepBed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSleepBed@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSleepBed {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSleepBed::GetType() const
{
    return 0xd;
}
}

// NCreatureMode::CModeCook::GetType() const -- returns 0xe
// Leaf: ?GetType@CModeCook@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F7A0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCook's vtable 0x01267908 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCook or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCook@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCook {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCook::GetType() const
{
    return 0xe;
}
}

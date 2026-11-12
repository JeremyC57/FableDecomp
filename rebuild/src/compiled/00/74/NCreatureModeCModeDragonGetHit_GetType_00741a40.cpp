// NCreatureMode::CModeDragonGetHit::GetType() const -- returns 0x4f
// Leaf: ?GetType@CModeDragonGetHit@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741A40, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDragonGetHit's vtable 0x012683E4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDragonGetHit or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDragonGetHit@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDragonGetHit {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDragonGetHit::GetType() const
{
    return 0x4f;
}
}

// NCreatureMode::CModeOutOfActionBase::GetParticleEffectIndex() const -- returns -1
// Leaf: ?GetParticleEffectIndex@CModeOutOfActionBase@NCreatureMode@@EBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073DA20, thiscall, `ret`.
// Identity: slot 13 of NCreatureMode::CModeOutOfActionBase's vtable 0x0126743C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeOutOfActionBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetParticleEffectIndex(void) at this slot; Ego_r/FableWin name set: ?GetParticleEffectIndex@CModeOutOfActionBase@NCreatureMode@@EBEJXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeOutOfActionBase {
public:
private:
    virtual long GetParticleEffectIndex() const;
};
long CModeOutOfActionBase::GetParticleEffectIndex() const
{
    return -1;
}
}

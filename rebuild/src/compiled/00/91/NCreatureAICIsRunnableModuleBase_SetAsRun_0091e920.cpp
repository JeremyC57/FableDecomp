// NCreatureAI::CIsRunnableModuleBase::SetAsRun() -- does nothing
// Leaf: ?SetAsRun@CIsRunnableModuleBase@NCreatureAI@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091E920, thiscall, `ret`.
// Identity: slot 1 of NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon's vtable 0x0128FFD4 (defined here: the slot holds this body in 2 vtable(s), all of NCreatureAI::CIsRunnableModuleBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts SetAsRun(void) at this slot; Ego_r/FableWin name set: ?SetAsRun@CIsRunnableModuleBase@NCreatureAI@@UAEXXZ; `ret` matches the signature.
namespace NCreatureAI {
class CIsRunnableModuleBase {
public:
    virtual void SetAsRun();
};
void CIsRunnableModuleBase::SetAsRun()
{
}
}

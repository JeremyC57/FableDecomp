// NCreatureMode::CModeOutOfActionBase::LeafInit() -- does nothing
// Leaf: ?LeafInit@CModeOutOfActionBase@NCreatureMode@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073DA10, thiscall, `ret`.
// Identity: slot 12 of NCreatureMode::CModeDazed's vtable 0x01267CC4 (defined here: the slot holds this body in 2 vtable(s), all of NCreatureMode::CModeOutOfActionBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts LeafInit(void) at this slot; Ego_r/FableWin name set: ?LeafInit@CModeOutOfActionBase@NCreatureMode@@EAEXXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeOutOfActionBase {
public:
private:
    virtual void LeafInit();
};
void CModeOutOfActionBase::LeafInit()
{
}
}

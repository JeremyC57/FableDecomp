// NParticleEngine::CPSCBase::TransferUpdateParametersToDialog(CSymbolMap const &) -- does nothing
// Leaf: ?TransferUpdateParametersToDialog@CPSCBase@NParticleEngine@@UAEXABVCSymbolMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF29F0, thiscall, `ret 4`.
// Identity: slot 11 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (defined here: the slot holds this body in 12 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts TransferUpdateParametersToDialog(CSymbolMap const &) at this slot; Ego_r/FableWin name set: ?TransferUpdateParametersToDialog@CPSCBase@NParticleEngine@@UAEXABVCSymbolMap@@@Z; `ret` matches the signature.
class CSymbolMap;
namespace NParticleEngine {
class CPSCBase {
public:
    virtual void TransferUpdateParametersToDialog(CSymbolMap const &);
};
void CPSCBase::TransferUpdateParametersToDialog(CSymbolMap const &)
{
}
}

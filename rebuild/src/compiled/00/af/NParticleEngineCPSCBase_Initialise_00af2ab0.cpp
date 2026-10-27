// NParticleEngine::CPSCBase::Initialise(CIEngine*, CParticleSystem&, const CParticleUpdateParameters<unsigned long,unsigned long>&, CEnginePrimitiveHandle&) -- default component initialise: mark the component initialised and report it
// Leaf: ?Initialise@CPSCBase@NParticleEngine@@UAE_NPAVCIEngine@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2AB0, 15 bytes, `ret 10h`.
// Identity: slot 28 (+0x70) of the CPSCBase vtable 0x0129F3F4, inherited unchanged by four component vtables
//   (absolute refs 0x012A009C / 0x012A03E4 / 0x012A051C / 0x012A0694); landed models of CParticleSystem (0x00AF40B0) call
//   slot 28 as Initialise(engine, system, &UpdateParameters, handle). Ego_r.pdb
//   `?Initialise@CPSCBase@NParticleEngine@@UAE_NPAVCIEngine@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z`.
// Layout: Ego_r.pdb CPSCBase (12 bytes): vptr +0 (CParticleUpdateParamCallback<ulong,ulong> base), ParticleSystem +4,
//   bool bit fields at +8: UpdateStarted (bit 0), Initialised (bit 1). The set-then-read of the bit field gives
//   `or al,2 ; mov [ecx+8],al ; shr al,1 ; and al,1`.
class CIEngine;
class CEnginePrimitiveHandle;

namespace NParticleEngine {

class CParticleSystem;
template <class A, class B> class CParticleUpdateParameters;

class CPSCBase {
public:
    CParticleSystem* ParticleSystem;                       // +0x04
    bool UpdateStarted : 1;                                // +0x08 bit 0
    bool Initialised : 1;                                  // +0x08 bit 1
    virtual bool Initialise(CIEngine* engine, CParticleSystem& system,
                            const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                            CEnginePrimitiveHandle& handle);                      // slot 28 (+0x70)
};

bool CPSCBase::Initialise(CIEngine*, CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&,
                          CEnginePrimitiveHandle&)
{
    Initialised = true;
    return Initialised;
}

}   // namespace NParticleEngine
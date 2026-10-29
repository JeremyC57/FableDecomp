// NParticleEngine::CPSCBase::CPSCBase() -- CBase base, the UpdateStarted/Initialised bits cleared, own vtable, no owning particle system yet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2620, length 29 bytes.
// Leaf: ??0CPSCBase@NParticleEngine@@QAE@XZ
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x0129F3F4 = NParticleEngine::CPSCBase (RTTI .?AVCPSCBase@NParticleEngine@@,
// rtti-vtables.tsv); Ego_r/FableWin.pdb ??0CPSCBase@NParticleEngine@@QAE@XZ. The catalog name lacked the namespace.
// Layout (Ego_r.pdb, 12 bytes): ParticleSystem +4, the UpdateStarted/Initialised bool bit-fields in the
// byte at +8. Shape: both bit clears fold into `and byte ptr [esi+8],0FCh` ahead of the vtable store,
// the pointer initialiser is an immediate zero store after it (same constructor shape as
// CDefClassBase::CDefClassBase 0x0042D9E0).
namespace NParticleEngine { class CParticleSystem; }

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                                // 0x0099A2F0
};

namespace NParticleEngine {
class CPSCBase : public CBaseClassNonCopyable {             // vtable 0x0129F3F4
public:
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
    CPSCBase();
};

CPSCBase::CPSCBase()
    : ParticleSystem(0)
{
    UpdateStarted = false;
    Initialised = false;
}
}
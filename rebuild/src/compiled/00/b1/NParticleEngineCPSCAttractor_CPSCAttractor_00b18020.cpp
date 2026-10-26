// NParticleEngine::CPSCAttractor::CPSCAttractor() -- CPSCBase base, enabled / not param-positioned, linear fall-off 0, unit radius and force, empty user-point array
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B18020, length 53 bytes.
// Leaf: ??0CPSCAttractor
// Callees: NParticleEngine::CPSCBase::CPSCBase @ 0x00AF2620. Vtable 0x012A04AC = NParticleEngine::CPSCAttractor.
// Layout (Ego_r.pdb): AttractorEnabled / AttractorUseParamPosition bits at +0xc, AttractorInfluenceFallOff
// +0x10, AttractorPositionParam +0x14 and AttractorPositionParamName +0x18 (left unset),
// AttractorInfluenceRadius +0x1c, AttractorInfluenceForce +0x20, AttractorUserPointsArray +0x24
// (CArray<C3DVector>: three pointers in retail).
// Shape: the bit-fields are mem-initialisers (a body assignment sinks the read-modify-write to the
// end); with the fall-off zero they are scheduled before the vtable store, the two 1.0f floats
// (`mov ecx,3F800000h`) and the array's inline constructor follow it.
struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2F0
};

namespace NParticleEngine {
class CParticleSystem;

class CPSCBase : public CBase { public:                   // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
    CPSCBase();                             // 0x00AF2620
};

struct C3DVector { float X, Y, Z; };

struct C3DVectorArray {                     // retail CArray<C3DVector> (12 bytes)
    C3DVector* m_pData;
    C3DVector* m_pLast;
    C3DVector* m_pEnd;
    C3DVectorArray() : m_pData(0), m_pLast(0), m_pEnd(0) { }
};

class CPSCAttractor : public CPSCBase { public:           // vtable 0x012A04AC
    bool           AttractorEnabled : 1;            // +0x0c bit 0
    bool           AttractorUseParamPosition : 1;   // +0x0c bit 1
    long           AttractorInfluenceFallOff;       // +0x10
    unsigned long  AttractorPositionParam;          // +0x14
    unsigned long  AttractorPositionParamName;      // +0x18
    float          AttractorInfluenceRadius;        // +0x1c
    float          AttractorInfluenceForce;         // +0x20
    C3DVectorArray AttractorUserPointsArray;        // +0x24
    CPSCAttractor();
};

CPSCAttractor::CPSCAttractor()
    : AttractorEnabled(true)
    , AttractorUseParamPosition(false)
    , AttractorInfluenceFallOff(0)
    , AttractorInfluenceRadius(1.0f)
    , AttractorInfluenceForce(1.0f)
{
}
}
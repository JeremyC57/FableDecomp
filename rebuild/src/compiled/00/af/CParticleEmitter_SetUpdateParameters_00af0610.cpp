// CParticleEmitter::SetUpdateParameters(const CParticleUpdateParameters<unsigned long, unsigned long>&) -- UpdateParameters = parameters: the inline CParticleUpdateParameters assignment (VectorParams and UpdateParams vector maps: vector operator= + Dirty flag)
// Leaf: SetUpdateParameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF0610, length 57 bytes (`ret 4`, no result).
// Callees: vector<pair<unsigned long, C3DVector> >::operator= @ 0x006E2000,
//          vector<pair<unsigned long, CParticleUpdateParamDesc> >::operator= @ 0x006E22F0.
// Callers: CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11C0 / 0x006E154B (on the emitter).
// Layout: UpdateParameters at +0x14 in the retail emitter (its maps at +0x18 and +0x28): the
// generated engine/CParticleEmitter.h says +0x18 because the PDB ParticleSystems vector is 16 bytes
// (retail 12, +0x08..+0x14), so the emitter is modelled locally. The parameter class is the one of
// 0x006E2570 (its out-of-line operator=), here inlined.
// Shape: the inline CParticleUpdateParameters::operator= with both inline CVectorMap assignments;
// the last map's destination is formed with `add esi,28h` (`this` is dead after it) and the first
// map's Dirty store is scheduled after the second call's argument push.

namespace NParticleEngine {

struct SVectorParam {                                                     // pair<unsigned long, C3DVector>
    unsigned long m_key;
    float         m_value[3];
};

struct SUpdateParam {                                                     // pair<unsigned long, CParticleUpdateParamDesc>
    unsigned long m_key;
    unsigned long m_desc[3];
};

struct CVectorParamVector {                                               // retail std::vector (12 bytes)
    SVectorParam* m_first;
    SVectorParam* m_last;
    SVectorParam* m_end;
    CVectorParamVector& operator=(const CVectorParamVector& other);       // 0x006E2000
};

struct CUpdateParamVector {                                               // retail std::vector (12 bytes)
    SUpdateParam* m_first;
    SUpdateParam* m_last;
    SUpdateParam* m_end;
    CUpdateParamVector& operator=(const CUpdateParamVector& other);       // 0x006E22F0
};

struct CCompareVector {
};

struct CCompareParamDesc {
};

struct CVectorParamMap {                                                  // CVectorMap<unsigned long, C3DVector, ...>
    CVectorParamVector Vector;                                            // +0x00
    CCompareVector     Compare;                                           // +0x0c
    bool               Dirty;                                             // +0x0d
    CVectorParamMap& operator=(const CVectorParamMap& other)
    {
        Vector = other.Vector;
        Dirty = other.Dirty;
        return *this;
    }
};

struct CUpdateParamMap {                                                  // CVectorMap<unsigned long, CParticleUpdateParamDesc, ...>
    CUpdateParamVector Vector;                                            // +0x00
    CCompareParamDesc  Compare;                                           // +0x0c
    bool               Dirty;                                             // +0x0d
    CUpdateParamMap& operator=(const CUpdateParamMap& other)
    {
        Vector = other.Vector;
        Dirty = other.Dirty;
        return *this;
    }
};

struct CParticleUpdateParameters {                                        // <unsigned long, unsigned long>
    virtual void v00();
    CVectorParamMap VectorParams;                                         // +0x04
    CUpdateParamMap UpdateParams;                                         // +0x14
    CParticleUpdateParameters& operator=(const CParticleUpdateParameters& other)
    {
        VectorParams = other.VectorParams;
        UpdateParams = other.UpdateParams;
        return *this;
    }
};

} // namespace NParticleEngine

struct CParticleEmitter {
    virtual void v00();
    unsigned char _base4[4];                                              // +0x04
    unsigned char ParticleSystems[0xc];                                   // +0x08 vector<CCountedPointer<CParticleSystem> > (12 bytes retail)
    NParticleEngine::CParticleUpdateParameters UpdateParameters;          // +0x14
    void SetUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);
};

void CParticleEmitter::SetUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters)
{
    UpdateParameters = parameters;
}
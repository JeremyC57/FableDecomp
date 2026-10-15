// NParticleEngine::CParticleUpdateParameters::operator=(const CParticleUpdateParameters&) -- assignment of CParticleUpdateParameters<unsigned long, unsigned long>: the VectorParams and UpdateParams vector maps (each: vector operator= + Dirty flag); returns *this
// Leaf: ??4CParticleUpdateParameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2570, length 59 bytes (`ret 4`, eax = this).
// Callees: vector<pair<unsigned long, C3DVector> >::operator= @ 0x006E2000 (VectorParams),
//          vector<pair<unsigned long, CParticleUpdateParamDesc> >::operator= @ 0x006E22F0 (UpdateParams).
// Identity: the manifest label "CParticleEmitter::SetUpdateParameters" is propagated. The only caller
// (CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0) runs it on `[esi+3Ch] + 28h`, an
// embedded CParticleUpdateParameters, and the body returns `this` -- the assignment operator. The
// real CParticleEmitter::SetUpdateParameters is 0x00AF0610 (this assignment inlined at +0x14).
// Layout: PDB CParticleUpdateParameters<unsigned long,unsigned long> = vptr/pad +0, VectorParams
// (CVectorMap) +4, UpdateParams +24; with the 12-byte retail vector each CVectorMap is 0x10 bytes
// (vector, empty comparator +0xc, bool Dirty +0xd), so UpdateParams is at +0x14. The class template
// is modelled as a plain class (its two arguments are both unsigned long).
// Shape: each CVectorMap::operator= is inline: the source and destination map addresses are formed
// in edi/ebx, the vector assigned out of line, the Dirty byte copied through the map pointers; the
// empty comparator copies nothing; +0 (the vptr) is not assigned.

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
    CParticleUpdateParameters& operator=(const CParticleUpdateParameters& other);
};

CParticleUpdateParameters& CParticleUpdateParameters::operator=(const CParticleUpdateParameters& other)
{
    VectorParams = other.VectorParams;
    UpdateParams = other.UpdateParams;
    return *this;
}

} // namespace NParticleEngine

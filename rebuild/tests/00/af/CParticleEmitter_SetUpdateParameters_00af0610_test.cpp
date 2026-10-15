// Behaviour test for CParticleEmitter::SetUpdateParameters (0x00AF0610): the emitter's embedded
// parameter block takes both vector maps (vector + Dirty flag) of the argument.
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

namespace NParticleEngine {

struct SVectorParam { unsigned long m_key; float m_value[3]; };
struct SUpdateParam { unsigned long m_key; unsigned long m_desc[3]; };

struct CVectorParamVector {
    SVectorParam* m_first; SVectorParam* m_last; SVectorParam* m_end;
    CVectorParamVector& operator=(const CVectorParamVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 1; return *this; }
};

struct CUpdateParamVector {
    SUpdateParam* m_first; SUpdateParam* m_last; SUpdateParam* m_end;
    CUpdateParamVector& operator=(const CUpdateParamVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 2; return *this; }
};

struct CCompareVector {};
struct CCompareParamDesc {};

struct CVectorParamMap {
    CVectorParamVector Vector;
    CCompareVector     Compare;
    bool               Dirty;
    CVectorParamMap& operator=(const CVectorParamMap& other) { Vector = other.Vector; Dirty = other.Dirty; return *this; }
};

struct CUpdateParamMap {
    CUpdateParamVector Vector;
    CCompareParamDesc  Compare;
    bool               Dirty;
    CUpdateParamMap& operator=(const CUpdateParamMap& other) { Vector = other.Vector; Dirty = other.Dirty; return *this; }
};

struct CParticleUpdateParameters {
    virtual void v00() {}
    CVectorParamMap VectorParams;
    CUpdateParamMap UpdateParams;
    CParticleUpdateParameters& operator=(const CParticleUpdateParameters& other)
    {
        VectorParams = other.VectorParams;
        UpdateParams = other.UpdateParams;
        return *this;
    }
};

} // namespace NParticleEngine

struct CParticleEmitter {
    virtual void v00() {}
    unsigned char _base4[4];
    unsigned char ParticleSystems[0xc];
    NParticleEngine::CParticleUpdateParameters UpdateParameters;
    void SetUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);
};

void CParticleEmitter::SetUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters)
{
    UpdateParameters = parameters;
}

int main()
{
    static NParticleEngine::SVectorParam vp[2];
    static NParticleEngine::SUpdateParam up[3];
    CParticleEmitter e;
    NParticleEngine::CParticleUpdateParameters& a = e.UpdateParameters;
    NParticleEngine::CParticleUpdateParameters b;
    a.VectorParams.Vector.m_first = a.VectorParams.Vector.m_last = a.VectorParams.Vector.m_end = 0;
    a.UpdateParams.Vector.m_first = a.UpdateParams.Vector.m_last = a.UpdateParams.Vector.m_end = 0;
    a.VectorParams.Dirty = false; a.UpdateParams.Dirty = true;
    b.VectorParams.Vector.m_first = vp; b.VectorParams.Vector.m_last = vp + 2; b.VectorParams.Vector.m_end = vp + 2;
    b.UpdateParams.Vector.m_first = up; b.UpdateParams.Vector.m_last = up + 3; b.UpdateParams.Vector.m_end = up + 3;
    b.VectorParams.Dirty = true; b.UpdateParams.Dirty = false;
    e.SetUpdateParameters(b);
    if (g_calls != 2 || g_order[0] != 1 || g_order[1] != 2 || a.VectorParams.Vector.m_last != vp + 2
        || a.UpdateParams.Vector.m_end != up + 3 || !a.VectorParams.Dirty || a.UpdateParams.Dirty) {
        std::printf("00af0610_TEST FAIL\n");
        return 1;
    }
    std::printf("00af0610_TEST PASS\n");
    return 0;
}
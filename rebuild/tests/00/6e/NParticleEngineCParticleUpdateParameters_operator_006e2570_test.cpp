// Behaviour test for CParticleUpdateParameters::operator= (0x006E2570): both vector maps assigned
// (vector + Dirty flag), the result is *this.
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
    CParticleUpdateParameters& operator=(const CParticleUpdateParameters& other);
};

CParticleUpdateParameters& CParticleUpdateParameters::operator=(const CParticleUpdateParameters& other)
{
    VectorParams = other.VectorParams;
    UpdateParams = other.UpdateParams;
    return *this;
}

} // namespace NParticleEngine

int main()
{
    static NParticleEngine::SVectorParam vp[2];
    static NParticleEngine::SUpdateParam up[3];
    NParticleEngine::CParticleUpdateParameters a, b;
    a.VectorParams.Vector.m_first = a.VectorParams.Vector.m_last = a.VectorParams.Vector.m_end = 0;
    a.UpdateParams.Vector.m_first = a.UpdateParams.Vector.m_last = a.UpdateParams.Vector.m_end = 0;
    a.VectorParams.Dirty = false; a.UpdateParams.Dirty = true;
    b.VectorParams.Vector.m_first = vp; b.VectorParams.Vector.m_last = vp + 2; b.VectorParams.Vector.m_end = vp + 2;
    b.UpdateParams.Vector.m_first = up; b.UpdateParams.Vector.m_last = up + 3; b.UpdateParams.Vector.m_end = up + 3;
    b.VectorParams.Dirty = true; b.UpdateParams.Dirty = false;
    NParticleEngine::CParticleUpdateParameters& r = (a = b);
    if (&r != &a || g_calls != 2 || g_order[0] != 1 || g_order[1] != 2 || a.VectorParams.Vector.m_last != vp + 2
        || a.UpdateParams.Vector.m_end != up + 3 || !a.VectorParams.Dirty || a.UpdateParams.Dirty) {
        std::printf("006e2570_TEST FAIL\n");
        return 1;
    }
    std::printf("006e2570_TEST PASS\n");
    return 0;
}
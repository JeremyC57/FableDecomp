#include <cstdio>
#include <cstring>
#include <new>

// NParticleEngine::CPSCAttractor::CPSCAttractor(): CPSCBase base, enabled and not param-positioned,
// fall-off 0, radius and force 1.0, an empty user-point array; the position params are left alone.
static int g_pscBaseCtors = 0;
struct CBase { virtual ~CBase() { } };
namespace NParticleEngine {
class CParticleSystem;
class CPSCBase : public CBase { public:
    CParticleSystem* ParticleSystem;
    bool             UpdateStarted : 1;
    bool             Initialised : 1;
    CPSCBase() : ParticleSystem(0) { UpdateStarted = false; Initialised = false; ++g_pscBaseCtors; }
};
struct C3DVector { float X, Y, Z; };
struct C3DVectorArray {
    C3DVector* m_pData;
    C3DVector* m_pLast;
    C3DVector* m_pEnd;
    C3DVectorArray() : m_pData(0), m_pLast(0), m_pEnd(0) { }
};
class CPSCAttractor : public CPSCBase { public:
    bool           AttractorEnabled : 1;
    bool           AttractorUseParamPosition : 1;
    long           AttractorInfluenceFallOff;
    unsigned long  AttractorPositionParam;
    unsigned long  AttractorPositionParamName;
    float          AttractorInfluenceRadius;
    float          AttractorInfluenceForce;
    C3DVectorArray AttractorUserPointsArray;
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

int main()
{
    using namespace NParticleEngine;
    static unsigned char storage[sizeof(CPSCAttractor) + 16];
    std::memset(storage, 0x5A, sizeof(storage));
    CPSCAttractor* a = new (storage) CPSCAttractor();
    unsigned long untouched;
    std::memset(&untouched, 0x5A, sizeof(untouched));
    if (g_pscBaseCtors != 1 || !a->AttractorEnabled || a->AttractorUseParamPosition || a->AttractorInfluenceFallOff != 0
        || a->AttractorInfluenceRadius != 1.0f || a->AttractorInfluenceForce != 1.0f || a->AttractorUserPointsArray.m_pData != 0
        || a->AttractorUserPointsArray.m_pLast != 0 || a->AttractorUserPointsArray.m_pEnd != 0
        || a->AttractorPositionParam != untouched || a->ParticleSystem != 0) {
        std::printf("00b18020_TEST FAIL\n");
        return 1;
    }
    std::printf("00b18020_TEST PASS\n");
    return 0;
}
#include <cstdio>
class CEngine { public: int id; };
class CEnginePrimitiveManager { public:
    CEngine* m_pEngine;
    CEnginePrimitiveManager(CEngine* engine) : m_pEngine(engine) { }
    virtual ~CEnginePrimitiveManager() { }
};
class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager { public:
    bool ClipEffectToWeatherMask, m_enabled, WriteZBuffer, DisplayBoundingBox, DisplayBoundingSphere;
    CEnginePrimitiveManagerParticleGroup(CEngine* engine)
        : CEnginePrimitiveManager(engine), ClipEffectToWeatherMask(false), WriteZBuffer(false), DisplayBoundingBox(false), DisplayBoundingSphere(false)
    {
        m_enabled = true;
    }
};
int main()
{
    CEngine e;
    CEnginePrimitiveManagerParticleGroup g(&e);
    if (g.m_pEngine != &e || g.ClipEffectToWeatherMask || !g.m_enabled || g.WriteZBuffer || g.DisplayBoundingBox || g.DisplayBoundingSphere) { std::printf("00bbbf40_TEST FAIL\n"); return 1; }
    std::printf("00bbbf40_TEST PASS\n");
    return 0;
}
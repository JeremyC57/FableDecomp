#include <cstdio>
class CEngineGraphic { public: long m_words[4]; };
class CDegradableInfo { public:
    float          HealthPercentage;
    CEngineGraphic EngineGraphic;
    long           SmashParticleEmitter;
    bool           BlocksNavigation;
};
class CDegradableDef { public:
    class CIsHealth1GTHealth2 { public:
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; }
    };
};
void __unguarded_linear_insert(CDegradableInfo* last, CDegradableInfo value, CDegradableDef::CIsHealth1GTHealth2 comp)
{
    CDegradableInfo* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}
int main()
{
    CDegradableInfo r[4];
    float h[4] = { 1.0f, 0.75f, 0.25f, 0.5f };
    int i;
    for (i = 0; i < 4; ++i) { r[i].HealthPercentage = h[i]; r[i].SmashParticleEmitter = i; r[i].BlocksNavigation = (i == 3); }
    CDegradableInfo v = r[3];
    CDegradableDef::CIsHealth1GTHealth2 gt;
    __unguarded_linear_insert(r + 3, v, gt);
    float want[4] = { 1.0f, 0.75f, 0.5f, 0.25f };
    long tag[4] = { 0, 1, 3, 2 };
    for (i = 0; i < 4; ++i) if (r[i].HealthPercentage != want[i] || r[i].SmashParticleEmitter != tag[i]) { std::printf("0067e550_TEST FAIL\n"); return 1; }
    if (!r[2].BlocksNavigation || r[3].BlocksNavigation) { std::printf("0067e550_TEST FAIL\n"); return 1; }
    std::printf("0067e550_TEST PASS\n");
    return 0;
}
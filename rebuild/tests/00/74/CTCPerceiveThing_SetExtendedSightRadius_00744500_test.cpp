#include <cstdio>
static float g_fMinusOne = -1.0f;
class CPerceivedThingDef { public: float SightRadius; float ExtendedSightRadius; };
class CTCPerceiveThing { public:
    const CPerceivedThingDef* PDef_Object;
    float SightRadius;
    float ExtendedSightRadius;
    void SetExtendedSightRadius(float radius)
    {
        if (radius != g_fMinusOne) {
            ExtendedSightRadius = radius;
            return;
        }
        ExtendedSightRadius = PDef_Object->ExtendedSightRadius;
        if (ExtendedSightRadius < SightRadius)
            ExtendedSightRadius = SightRadius;
    }
};
int main()
{
    CPerceivedThingDef d; d.SightRadius = 10.0f; d.ExtendedSightRadius = 20.0f;
    CTCPerceiveThing p; p.PDef_Object = &d; p.SightRadius = 15.0f; p.ExtendedSightRadius = 0.0f;
    p.SetExtendedSightRadius(5.0f); if (p.ExtendedSightRadius != 5.0f) { std::printf("00744500_TEST FAIL\n"); return 1; }
    p.SetExtendedSightRadius(-1.0f); if (p.ExtendedSightRadius != 20.0f) { std::printf("00744500_TEST FAIL\n"); return 1; }
    p.SightRadius = 30.0f; p.SetExtendedSightRadius(-1.0f); if (p.ExtendedSightRadius != 30.0f) { std::printf("00744500_TEST FAIL\n"); return 1; }
    std::printf("00744500_TEST PASS\n");
    return 0;
}
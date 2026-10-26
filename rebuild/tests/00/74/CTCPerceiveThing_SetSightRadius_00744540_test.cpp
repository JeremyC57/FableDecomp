#include <cstdio>
static float g_fMinusOne = -1.0f;
class CPerceivedThingDef { public: float SightRadius; float ExtendedSightRadius; };
class CTCPerceiveThing { public:
    const CPerceivedThingDef* PDef_Object;
    float SightRadius;
    float ExtendedSightRadius;
    void SetSightRadius(float radius)
    {
        if (radius != g_fMinusOne) {
            SightRadius = radius;
            return;
        }
        SightRadius = PDef_Object->SightRadius;
        if (SightRadius > ExtendedSightRadius)
            ExtendedSightRadius = SightRadius;
    }
};
int main()
{
    CPerceivedThingDef d; d.SightRadius = 12.0f; d.ExtendedSightRadius = 0.0f;
    CTCPerceiveThing p; p.PDef_Object = &d; p.SightRadius = 0.0f; p.ExtendedSightRadius = 20.0f;
    p.SetSightRadius(3.0f); if (p.SightRadius != 3.0f || p.ExtendedSightRadius != 20.0f) { std::printf("00744540_TEST FAIL\n"); return 1; }
    p.SetSightRadius(-1.0f); if (p.SightRadius != 12.0f || p.ExtendedSightRadius != 20.0f) { std::printf("00744540_TEST FAIL\n"); return 1; }
    p.ExtendedSightRadius = 5.0f; p.SetSightRadius(-1.0f); if (p.SightRadius != 12.0f || p.ExtendedSightRadius != 12.0f) { std::printf("00744540_TEST FAIL\n"); return 1; }
    std::printf("00744540_TEST PASS\n");
    return 0;
}
#include <cstdio>
static const float g_zero = 0.0f;
struct CThingPlacement { float angle; virtual float GetAngleXY() const { return angle; } virtual ~CThingPlacement() { } };
struct CThing { CThingPlacement* m_pPlacement; };
struct CTCWeatherVane {
    CThing* ParentThing; float InitialOffsetXY;
    void OnInitialActivate()
    {
        CThingPlacement* placement = ParentThing->m_pPlacement;
        if (placement) InitialOffsetXY = placement->GetAngleXY() + 0.5f;
        else InitialOffsetXY = g_zero + 0.5f;
    }
};
int main()
{
    CThingPlacement p; p.angle = 1.25f; CThing t; t.m_pPlacement = &p; CTCWeatherVane v; v.ParentThing = &t; v.InitialOffsetXY = -1.0f;
    v.OnInitialActivate(); if (v.InitialOffsetXY != 1.75f) { std::printf("00732d00_TEST FAIL\n"); return 1; }
    t.m_pPlacement = 0; v.OnInitialActivate(); if (v.InitialOffsetXY != 0.5f) { std::printf("00732d00_TEST FAIL\n"); return 1; }
    std::printf("00732d00_TEST PASS\n");
    return 0;
}

#include <cstdio>
// Behaviour test for CThingHolySiteDef::Copy (0x00464498): base copy + Graphic + PlayerStart.
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
struct CThingBaseDef {
    long m_base;
    void CopyBase(const CThingBaseDef* src) { m_base = src->m_base; }
};
struct CThingHolySiteDef : CThingBaseDef {
    CEngineGraphic Graphic; bool PlayerStart; long m_untouched;
    void Copy(const CThingBaseDef* src)
    {
        const CThingHolySiteDef* s = static_cast<const CThingHolySiteDef*>(src);
        CThingBaseDef::CopyBase(src);
        Graphic = s->Graphic;
        PlayerStart = s->PlayerStart;
    }
};
int main()
{
    CThingHolySiteDef a, b;
    a.m_base = 5; a.Graphic.BankIndex = 9; a.Graphic.AnimStep = 1.5f; a.Graphic.RenderSizeX = 2.0f; a.Graphic.AdditiveAlpha = true; a.Graphic.Type = 3; a.PlayerStart = true; a.m_untouched = 1;
    b.m_base = 0; b.Graphic.BankIndex = 0; b.Graphic.AnimStep = 0; b.Graphic.RenderSizeX = 0; b.Graphic.AdditiveAlpha = false; b.Graphic.Type = 0; b.PlayerStart = false; b.m_untouched = 2;
    b.Copy(&a);
    int fails = 0;
    if (b.m_base != 5 || b.Graphic.BankIndex != 9 || b.Graphic.AnimStep != 1.5f || b.Graphic.RenderSizeX != 2.0f) ++fails;
    if (!b.Graphic.AdditiveAlpha || b.Graphic.Type != 3 || !b.PlayerStart || b.m_untouched != 2) ++fails;
    if (fails) { std::printf("00464498_TEST FAIL\n"); return 1; }
    std::printf("00464498_TEST PASS\n");
    return 0;
}
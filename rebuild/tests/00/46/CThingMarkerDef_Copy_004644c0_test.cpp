#include <cstdio>
// Behaviour test for CThingMarkerDef::Copy (0x004644c0): base copy + Graphic + TriggerType + Properties.
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
struct CThingBaseDef {
    long m_base;
    void CopyBase(const CThingBaseDef* src) { m_base = src->m_base; }
};
struct CThingMarkerDef : CThingBaseDef {
    CEngineGraphic Graphic; long TriggerType; unsigned long Properties; long m_untouched;
    void Copy(const CThingBaseDef* src)
    {
        const CThingMarkerDef* s = static_cast<const CThingMarkerDef*>(src);
        CThingBaseDef::CopyBase(src);
        Graphic = s->Graphic;
        TriggerType = s->TriggerType;
        Properties = s->Properties;
    }
};
int main()
{
    CThingMarkerDef a, b;
    a.m_base = 5; a.Graphic.BankIndex = 9; a.Graphic.AnimStep = 1.5f; a.Graphic.RenderSizeX = 2.0f; a.Graphic.AdditiveAlpha = true; a.Graphic.Type = 3; a.TriggerType = 2; a.Properties = 0x80000001UL; a.m_untouched = 1;
    b.m_base = 0; b.Graphic.BankIndex = 0; b.Graphic.AnimStep = 0; b.Graphic.RenderSizeX = 0; b.Graphic.AdditiveAlpha = false; b.Graphic.Type = 0; b.TriggerType = 0; b.Properties = 0; b.m_untouched = 2;
    b.Copy(&a);
    int fails = 0;
    if (b.m_base != 5 || b.Graphic.BankIndex != 9 || b.Graphic.AnimStep != 1.5f || b.Graphic.RenderSizeX != 2.0f) ++fails;
    if (!b.Graphic.AdditiveAlpha || b.Graphic.Type != 3 || b.TriggerType != 2 || b.Properties != 0x80000001UL || b.m_untouched != 2) ++fails;
    if (fails) { std::printf("004644c0_TEST FAIL\n"); return 1; }
    std::printf("004644c0_TEST PASS\n");
    return 0;
}
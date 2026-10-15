#include <cstdio>
// Behaviour test for CThingHolySite::OnCreate (0x0083BA20): graphic copied, extents set on success.
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
struct CThingHolySiteDef { CEngineGraphic Graphic; bool PlayerStart; };
struct CGraphicRecord { CEngineGraphic m_graphic; };
struct CThingHolySite {
    const CThingHolySiteDef* m_pDef;
    CGraphicRecord* m_pGraphicRecord;
    float m_drawExtentX, m_drawExtentY;
    bool baseResult;
    bool BaseOnCreate() { return baseResult; }
    bool OnCreate()
    {
        m_pGraphicRecord->m_graphic = m_pDef->Graphic;
        if (BaseOnCreate()) { m_drawExtentX = 100.0f; m_drawExtentY = 100.0f; return true; }
        return false;
    }
};
int main()
{
    CThingHolySiteDef def; def.Graphic.BankIndex = 77; def.Graphic.AnimStep = 0.5f; def.Graphic.RenderSizeX = 3.0f;
    def.Graphic.AdditiveAlpha = true; def.Graphic.Type = 4; def.PlayerStart = false;
    CGraphicRecord rec; rec.m_graphic.BankIndex = 0; rec.m_graphic.AnimStep = 0; rec.m_graphic.RenderSizeX = 0; rec.m_graphic.AdditiveAlpha = false; rec.m_graphic.Type = 0;
    CThingHolySite t; t.m_pDef = &def; t.m_pGraphicRecord = &rec; t.m_drawExtentX = 1.0f; t.m_drawExtentY = 2.0f;
    int fails = 0;
    t.baseResult = false;
    if (t.OnCreate()) ++fails;
    if (rec.m_graphic.BankIndex != 77 || rec.m_graphic.Type != 4 || !rec.m_graphic.AdditiveAlpha || rec.m_graphic.AnimStep != 0.5f) ++fails;
    if (t.m_drawExtentX != 1.0f || t.m_drawExtentY != 2.0f) ++fails;
    t.baseResult = true;
    if (!t.OnCreate()) ++fails;
    if (t.m_drawExtentX != 100.0f || t.m_drawExtentY != 100.0f) ++fails;
    if (fails) { std::printf("0083ba20_TEST FAIL\n"); return 1; }
    std::printf("0083ba20_TEST PASS\n");
    return 0;
}
#include <cstdio>
// Behaviour test for 0064f7fc.

// NPlayerGui::CDrawQuestInfo::AddBar: both sprites are looked up first, the bar is built from all seven values, appended
// through AddElement and its new ID returned.
struct CCharString { const char* m_pData; };
struct CRGBColour { unsigned char R, G, B, A; };
struct CQuestInfoElement { long ID; virtual ~CQuestInfoElement() { } };
struct CBar : CQuestInfoElement {
    float Value, MaxValue; CRGBColour Colour, FadeColour; long Graphic1, Graphic2; float Scale;
    CBar(float v, float m, const CRGBColour& c, const CRGBColour& f, long g1, long g2, float s)
        : Value(v), MaxValue(m), Colour(c), FadeColour(f), Graphic1(g1), Graphic2(g2), Scale(s) { }
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    explicit CCountedPointer(T* object) : m_pObject(object) { }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : m_pObject(other.m_pObject) { }
    T* operator->() const { return m_pObject; }
};
struct CDrawQuestInfo {
    long nextId; int lookups; CQuestInfoElement* last;
    long GetScriptSpriteBySymbol(const CCharString& symbol) { ++lookups; return (long)(symbol.m_pData[0] - 'a'); }
    void AddElement(CCountedPointer<CQuestInfoElement> element) { element->ID = ++nextId; last = element.m_pObject; }
    long AddBar(float value, float maxValue, const CRGBColour& colour, const CRGBColour& fadeColour,
                const CCharString& symbol1, const CCharString& symbol2, float scale)
    {
        long graphic1 = GetScriptSpriteBySymbol(symbol1);
        long graphic2 = GetScriptSpriteBySymbol(symbol2);
        CCountedPointer<CBar> element(new CBar(value, maxValue, colour, fadeColour, graphic1, graphic2, scale));
        AddElement(element);
        return element->ID;
    }
};
int main()
{
    CDrawQuestInfo q; q.nextId = 0; q.lookups = 0; q.last = 0;
    CCharString s1; s1.m_pData = "c"; CCharString s2; s2.m_pData = "f";
    CRGBColour c = { 1, 2, 3, 4 }; CRGBColour f = { 5, 6, 7, 8 };
    long id = q.AddBar(1.0f, 10.0f, c, f, s1, s2, 0.5f);
    CBar* bar = static_cast<CBar*>(q.last);
    int ok = id == 1 && q.lookups == 2 && bar->Graphic1 == 2 && bar->Graphic2 == 5 && bar->MaxValue == 10.0f
          && bar->Colour.B == 3 && bar->FadeColour.A == 8 && bar->Scale == 0.5f;
    delete bar;
    if (!ok) { std::printf("0064f7fc_TEST FAIL\n"); return 1; }
    std::printf("0064f7fc_TEST PASS\n");
    return 0;
}
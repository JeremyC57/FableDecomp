// Behaviour test for NUISystem::CManager::CreateLevelSlider @ 0x00427DC9.
#include <cstdio>
#include <cstring>
#include <map>

class CCharString { public: const char* text; CCharString(const char* t) : text(t) {} };
class CWideString { public: long value; };
static CWideString LongToWideString(long v) { CWideString w; w.value = v; return w; }
class C2DCoordI { public: long X, Y; };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class C2DVector { public: float X, Y; };
struct SComponentState { C2DVector Position; C2DVector Size; CRGBColour Colour; };
typedef std::map<unsigned long, SComponentState> CStateMap;

struct CComponent {
    const char* name; CStateMap states; long textSet; const CWideString* lastText; long lastNumber;
    CComponent* children[3];
    CComponent(const char* n) : name(n), textSet(0), lastText(0), lastNumber(-1) { children[0] = children[1] = children[2] = 0; }
    CStateMap& GetStateMap() { return states; }
    CComponent* FindChild(CCharString n)
    {
        for (int i = 0; i < 3; ++i) if (children[i] && std::strcmp(children[i]->name, n.text) == 0) return children[i];
        return 0;
    }
    void SetText(const CWideString& t, bool a, bool b) { if (a && b) { ++textSet; lastText = &t; lastNumber = t.value; } }
};
static CComponent g_slider("UI_LEVEL_SLIDER"), g_text("UI_LEVEL_SLIDER_TEXT"), g_number("UI_LEVEL_SLIDER_NUMBER"), g_fill("UI_LEVEL_SLIDER_FILL_SPRITE");

struct CManager {
    CComponent* CreateComponent(const CCharString& name, long) { return std::strcmp(name.text, "UI_LEVEL_SLIDER") == 0 ? &g_slider : 0; }
    CComponent* CreateLevelSlider(const CWideString& text, const C2DCoordI& position, long level, long maxLevel, const CRGBColour& colour);
};

CComponent* CManager::CreateLevelSlider(const CWideString& text, const C2DCoordI& position, long level, long maxLevel, const CRGBColour& colour)
{
    CComponent* slider;
    {
        CCharString name("UI_LEVEL_SLIDER");
        slider = CreateComponent(name, 0);
    }
    CStateMap& states = slider->GetStateMap();
    states[0].Position.X = (float)position.X;
    states[0].Position.Y = (float)position.Y;
    CComponent* caption = slider->FindChild(CCharString("UI_LEVEL_SLIDER_TEXT"));
    if (caption)
        caption->SetText(text, true, true);
    CComponent* number = slider->FindChild(CCharString("UI_LEVEL_SLIDER_NUMBER"));
    if (number)
        number->SetText(LongToWideString(level), true, true);
    CComponent* fill = slider->FindChild(CCharString("UI_LEVEL_SLIDER_FILL_SPRITE"));
    if (fill) {
        CStateMap& fillStates = fill->GetStateMap();
        float fraction = maxLevel == 0 ? 1.0f : (float)level / maxLevel;
        fillStates[0].Size.X = fraction * 32.0f;
        CRGBColour tint(colour.R, colour.G, colour.B, 0xdc);
        fillStates[0].Colour = tint;
    }
    return slider;
}

int main()
{
    g_slider.children[0] = &g_text; g_slider.children[1] = &g_number; g_slider.children[2] = &g_fill;
    CManager m; CWideString caption; caption.value = 77; C2DCoordI pos; pos.X = 10; pos.Y = 20;
    CRGBColour col(200, 100, 50, 255);
    CComponent* s = m.CreateLevelSlider(caption, pos, 3, 4, col);
    int ok = s == &g_slider && g_slider.states[0].Position.X == 10.0f && g_slider.states[0].Position.Y == 20.0f
          && g_text.textSet == 1 && g_text.lastNumber == 77 && g_number.textSet == 1 && g_number.lastNumber == 3
          && g_fill.states[0].Size.X == 24.0f && g_fill.states[0].Colour.R == 200 && g_fill.states[0].Colour.G == 100
          && g_fill.states[0].Colour.B == 50 && g_fill.states[0].Colour.A == 0xdc;
    m.CreateLevelSlider(caption, pos, 3, 0, col);
    ok = ok && g_fill.states[0].Size.X == 32.0f;
    if (!ok) { std::printf("00427dc9_TEST FAIL\n"); return 1; }
    std::printf("00427dc9_TEST PASS\n");
    return 0;
}
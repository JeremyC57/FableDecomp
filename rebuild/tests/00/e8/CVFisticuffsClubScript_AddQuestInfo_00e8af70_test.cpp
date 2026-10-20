// Behaviour test for 0x00E8AF70 CV_FisticuffsClubScript::AddQuestInfo: panel shown, bar arguments, id stored.
#include <cstdio>
#include <cstring>
struct CCharString { char text[32]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CGameScriptInterface {
    bool shown; float value, maxValue, scale; CRGBColour c1, c2; char icon[32], text[32];
    CGameScriptInterface() : shown(false), c1(0,0,0,0), c2(0,0,0,0) { }
    virtual void DisplayQuestInfo(bool show) { shown = show; }
    virtual long AddQuestInfoBar(float v, float m, const CRGBColour& a, const CRGBColour& b, const CCharString& i,
                                 const CCharString& t, float s)
    { value = v; maxValue = m; c1 = a; c2 = b; std::strcpy(icon, i.text); std::strcpy(text, t.text); scale = s; return 42; }
};
struct CV_FisticuffsClubScript {
    CGameScriptInterface* Game; long HitID; long AllowedHits;
    void AddQuestInfo()
    {
        Game->DisplayQuestInfo(true);
        HitID = Game->AddQuestInfoBar((float)AllowedHits, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255),
                                      "HUD_HERO_ICON", "", 1.0f);
    }
};
int main()
{
    CGameScriptInterface g; CV_FisticuffsClubScript s; s.Game = &g; s.HitID = 0; s.AllowedHits = 3;
    s.AddQuestInfo();
    float three = 3.0f, one = 1.0f;
    if (!g.shown || s.HitID != 42 || g.value != three || g.maxValue != 0.0f || g.scale != one
        || g.c1.G != 255 || g.c1.R != 0 || g.c1.A != 255 || g.c2.R != 255 || g.c2.G != 0 || g.c2.B != 0
        || std::strcmp(g.icon, "HUD_HERO_ICON") || g.text[0]) {
        std::printf("00e8af70_TEST FAIL\n"); return 1;
    }
    std::printf("00e8af70_TEST PASS\n");
    return 0;
}
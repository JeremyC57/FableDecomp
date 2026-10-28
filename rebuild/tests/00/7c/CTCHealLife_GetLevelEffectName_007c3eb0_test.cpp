// Behaviour test for 0x007C3EB0 CTCHealLife::GetLevelEffectName: LEVEL1 by default, LEVEL2/3/4 for spell levels 1/2/3.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    char text[32];
    CCharString(const char* t, long length = -1) { (void)length; std::strcpy(text, t); }
    CCharString(const CCharString& other) { std::strcpy(text, other.text); }
    ~CCharString() { text[0] = 0; }
    const CCharString& operator=(const char* t) { std::strcpy(text, t); return *this; }
};

class CTCHealLife {
public:
    long SpellLevel;
    CCharString GetLevelEffectName()
    {
        CCharString name("HEALING_EFFECT_LEVEL1");
        switch (SpellLevel) {
        case 1:
            name = "HEALING_EFFECT_LEVEL2";
            break;
        case 2:
            name = "HEALING_EFFECT_LEVEL3";
            break;
        case 3:
            name = "HEALING_EFFECT_LEVEL4";
            break;
        }
        return name;
    }
};

int main()
{
    const char* want[6] = { "HEALING_EFFECT_LEVEL1", "HEALING_EFFECT_LEVEL2", "HEALING_EFFECT_LEVEL3",
                            "HEALING_EFFECT_LEVEL4", "HEALING_EFFECT_LEVEL1", "HEALING_EFFECT_LEVEL1" };
    long levels[6] = { 0, 1, 2, 3, 4, -1 };
    bool ok = true;
    for (int i = 0; i < 6; ++i) {
        CTCHealLife tc; tc.SpellLevel = levels[i];
        CCharString s = tc.GetLevelEffectName();
        if (std::strcmp(s.text, want[i]) != 0) ok = false;
    }
    if (!ok) { std::printf("007c3eb0_TEST FAIL\n"); return 1; }
    std::printf("007c3eb0_TEST PASS\n");
    return 0;
}
// Behaviour test for CTCInventoryStats::AddHeroAbility (0x005D64DD): a copy of the source's logic over mock components.
#include <cstdio>

class CWideString { public: int Id; };
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
class CRGBColour { public: unsigned char R, G, B, A; };
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
struct CUIState { unsigned long Flag; long Type; float Position[2]; float Zoom[2]; CRGBColour Colour; };
struct CStateMap {
    CUIState States[2];
    CUIState& operator[](unsigned long key) { return States[key]; }
};

struct CComponent;
template <class T> class CCountedPointer { public:
    T* Data;
    CCountedPointer() : Data(0) { }
    ~CCountedPointer() { }
    void Reset(T* data) { Data = data; }
};

struct CComponent {
    CStateMap Map;
    CComponent* Child;
    int AddChildCalls;
    CComponent() : Child(0), AddChildCalls(0) { Map.States[0].Colour.A = 255; }
    virtual void AddChild(CCountedPointer<CComponent>& child) { Child = child.Data; ++AddChildCalls; }
    virtual CStateMap& GetStates() { return Map; }
};

struct SliderArgs {
    int NameId; long X, Y, Value, Maximum; float R, Possible, Width, Height, BackA; bool A, B;
};
static SliderArgs g_args;
static CComponent g_slider;
struct CManager {
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* CreateTickSlider(const CWideString& name, const C2DCoordI& position, long value, long maximum,
                                 const CRGBFloatColour& colour, float possibleValue, float width, bool a, bool b,
                                 const CRGBFloatColour& backColour, float height)
    {
        g_args.NameId = name.Id; g_args.X = position.X; g_args.Y = position.Y; g_args.Value = value;
        g_args.Maximum = maximum; g_args.R = colour.R; g_args.Possible = possibleValue; g_args.Width = width;
        g_args.A = a; g_args.B = b; g_args.BackA = backColour.A; g_args.Height = height;
        return &g_slider;
    }
};

enum EHeroAbility { ABILITY_A = 3 };
class CTCInventoryExperience { public:
    CWideString GetExperienceAbilityName(long ability) { CWideString s; s.Id = 100 + ability; return s; }
    CRGBFloatColour GetExperienceAbilityColour(EHeroAbility ability) { return CRGBFloatColour((float)ability, 0, 0, 1); }
};
class CTCHeroExperience { public:
    long Amount; float Fraction;
    long GetAbilityPossibleLevelUpAmount(EHeroAbility, float& fraction) { fraction = Fraction; return Amount; }
};
class CTCInventoryAbilities { public:
    long Level, Maximum;
    mutable int LevelCalls;
    long GetAbilityLevel(EHeroAbility) const { ++LevelCalls; return Level; }
    long GetAbilityMaximumLevel(EHeroAbility) const { return Maximum; }
};

class CThing { public:
    CTCInventoryExperience* Experience;
    CTCHeroExperience* Hero;
    CTCInventoryAbilities* Abilities;
    void GetTC(CTCInventoryExperience** out) { *out = Experience; }
    void GetTC(CTCHeroExperience** out) { *out = Hero; }
    void PeekTC(const CTCInventoryAbilities** out) const { *out = Abilities; }
};

class CTCInventoryStats { public:
    CThing* ParentThing;
    void AddHeroAbility(CComponent* parent, EHeroAbility ability, float& yPos);
};

void CTCInventoryStats::AddHeroAbility(CComponent* parent, EHeroAbility ability, float& yPos)
{
    CTCInventoryExperience* experience;
    ParentThing->GetTC(&experience);
    CWideString name = experience->GetExperienceAbilityName(ability);
    CTCHeroExperience* heroExperience;
    ParentThing->GetTC(&heroExperience);
    CManager* manager = CManager::GetInstance();
    const CTCInventoryAbilities* abilities;
    ParentThing->PeekTC(&abilities);
    CCountedPointer<CComponent> slider;
    abilities->GetAbilityLevel(ability);
    long level = abilities->GetAbilityLevel(ability);
    float possible = 0.0f;
    long amount = heroExperience->GetAbilityPossibleLevelUpAmount(ability, possible);
    possible += (float)(amount + level + 1);
    slider.Reset(manager->CreateTickSlider(name, C2DCoordI(0, (long)yPos), level + 1,
                                           abilities->GetAbilityMaximumLevel(ability) + 1,
                                           experience->GetExperienceAbilityColour(ability), possible, 303.0f,
                                           false, false, CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 73.0f));
    if (level == -1) {
        CComponent* component = slider.Data;
        component->GetStates()[0].Colour.A = 100;
    }
    parent->AddChild(slider);
    yPos += 30.0f;
}

static int fail(const char* why) { std::printf("005d64dd_TEST FAIL %s\n", why); return 1; }

int main()
{
    CTCInventoryExperience exp;
    CTCHeroExperience hero; hero.Amount = 2; hero.Fraction = 0.5f;
    CTCInventoryAbilities ab; ab.Level = 1; ab.Maximum = 4; ab.LevelCalls = 0;
    CThing thing; thing.Experience = &exp; thing.Hero = &hero; thing.Abilities = &ab;
    CTCInventoryStats stats; stats.ParentThing = &thing;
    CComponent parent;
    float y = 45.75f;
    stats.AddHeroAbility(&parent, ABILITY_A, y);
    if (g_args.NameId != 103) return fail("name");
    if (g_args.X != 0 || g_args.Y != 45) return fail("position");
    if (g_args.Value != 2 || g_args.Maximum != 5) return fail("value/maximum");
    if (g_args.Possible != 4.5f) return fail("possible");
    if (g_args.R != 3.0f || g_args.Width != 303.0f || g_args.Height != 73.0f || g_args.BackA != 1.0f) return fail("style");
    if (g_args.A || g_args.B) return fail("flags");
    if (ab.LevelCalls != 2) return fail("level calls");
    if (g_slider.Map.States[0].Colour.A != 255) return fail("dimmed a known ability");
    if (parent.Child != &g_slider || parent.AddChildCalls != 1) return fail("child");
    if (y != 75.75f) return fail("row step");

    ab.Level = -1;
    stats.AddHeroAbility(&parent, ABILITY_A, y);
    if (g_args.Value != 0 || g_args.Y != 75) return fail("missing ability slider");
    if (g_slider.Map.States[0].Colour.A != 100) return fail("missing ability not dimmed");
    if (parent.AddChildCalls != 2 || y != 105.75f) return fail("second row");

    std::printf("005d64dd_TEST PASS\n");
    return 0;
}
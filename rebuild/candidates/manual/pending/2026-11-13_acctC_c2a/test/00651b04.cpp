// Behaviour test for 0x00651B04 NPlayerGui::CDrawAbilityButtons::Draw(const CEnginePrimitivePrepareInfo&): a host model of the
// row's decisions with recording stand-ins for the engine: the `normal` choice (normal<->special transition, special buttons
// hidden, or the quick-access menu active), ring / top-ring sprites versus the favourite ones, the normal ABXY buttons (alpha
// halved for the run button when hero run is unavailable, the pressed button choosing the middle graphic), the interact-border
// loop that runs after `if (normal)` whatever `normal` is, the special buttons (uncastable overlay at a third of the alpha at
// handle size - k, the shield-off graphic, graphic 0 for an unavailable ability), the middle-ring fallback graphic and the
// ability-button scale advance clamped at 1.
#include <cstdio>

class C2DVector { public: float X, Y; C2DVector() { } C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); } };

struct SButtonDef { long ControllerType; long ButtonGraphic; C2DVector Offset; };
struct SSpecial { bool HasDef; long Ability; bool Available; bool Able; bool Shield; bool SpecialAvailable; long Graphic; };

struct SDef {
    C2DVector RingsPos, MiddleOffset;
    long RingsGraphic, RingsFavouriteGraphic, RunGraphic, UncastableGraphic, ShieldOffGraphic, MiddleGraphic, BorderGraphic;
    SButtonDef Buttons[4]; long NumButtons;
    SButtonDef Specials[3]; long NumSpecials;
};

struct SCall { int kind; long index; long graphic; float alpha; float x, y; };
static SCall g_calls[128]; static int g_n = 0;
static void Rec(int kind, long index, long graphic, float alpha, const C2DVector& p)
{ SCall& c = g_calls[g_n++]; c.kind = kind; c.index = index; c.graphic = graphic; c.alpha = alpha; c.x = p.X; c.y = p.Y; }
enum { RING = 1, BUTTON, BORDER, SPECIAL, UNCASTABLE, MIDDLE };

static C2DVector Anchor(const C2DVector& p) { return C2DVector(p.X + 160.0f, p.Y + 120.0f); }

struct SState {
    bool NormalToSpecial, SpecialToNormal, ShowSpecial, MenuActive, HeroRunAvailable, Paused;
    long Pressed;            // index of the button whose joystick event is queued, -1 for none
    long Ability[4];         // GetAbilityButton(i)
    SSpecial Special[3];
    long HandleCount;
    float Scale, Elapsed;
};

static void Draw(const SDef& def, SState& s)
{
    long ringsGraphic = 0;
    bool normal = false;
    if (s.NormalToSpecial || s.SpecialToNormal || !s.ShowSpecial)
        normal = true;
    else if (s.MenuActive)
        normal = true;
    C2DVector pos = Anchor(def.RingsPos);
    if (normal) { if (def.RingsGraphic > 0) Rec(RING, 0, def.RingsGraphic, 1.0f, pos); }
    else if (def.RingsFavouriteGraphic > 0) Rec(RING, 0, def.RingsFavouriteGraphic, 1.0f, pos);
    if (normal) {
        for (long i = 0; i < def.NumButtons; ++i) {
            float alpha = 1.0f;
            if (ringsGraphic == 0 && s.Pressed == i) ringsGraphic = def.Buttons[i].ButtonGraphic;
            long ability = s.Ability[i];
            if (ability == def.RunGraphic && !s.HeroRunAvailable) alpha *= 0.5f;
            Rec(BUTTON, i + 2, ability, alpha, Anchor(def.RingsPos) + def.Buttons[i].Offset);
        }
    }
    for (long j = 0; j < def.NumButtons; ++j)
        if (def.Buttons[j].ControllerType == 0) Rec(BORDER, j, def.BorderGraphic, 1.0f, Anchor(def.RingsPos) + def.Buttons[j].Offset);
    if ((s.NormalToSpecial || s.SpecialToNormal || s.ShowSpecial) && !normal) {
        for (long k = 0; k < def.NumSpecials; ++k) {
            float specialAlpha = 0.75f;
            if (ringsGraphic == 0 && s.Pressed == 10 + k) ringsGraphic = def.Specials[k].ButtonGraphic;
            const SSpecial& sp = s.Special[k];
            long graphic = sp.Graphic;
            float drawAlpha = specialAlpha;
            if (sp.HasDef) {
                if (sp.Available) {
                    if (!(!sp.Able ? sp.Ability == 4 && sp.Shield : sp.SpecialAvailable)) {
                        drawAlpha = specialAlpha * (1.0f / 3.0f);
                        Rec(UNCASTABLE, s.HandleCount - k, def.UncastableGraphic, drawAlpha, Anchor(def.RingsPos) + def.Specials[k].Offset);
                    }
                    if (sp.Ability == 4 && sp.Shield) graphic = def.ShieldOffGraphic;
                } else
                    graphic = 0;
            }
            Rec(SPECIAL, k, graphic, drawAlpha, Anchor(def.RingsPos) + def.Specials[k].Offset);
        }
    }
    if (ringsGraphic == 0) ringsGraphic = def.MiddleGraphic;
    if (ringsGraphic > 0) Rec(MIDDLE, 0, ringsGraphic, 1.0f, Anchor(def.RingsPos) + def.MiddleOffset);
    if (!s.Paused && (s.NormalToSpecial || s.SpecialToNormal)) {
        s.Scale = s.Scale + s.Elapsed * 5.0f;
        if (s.Scale >= 1.0f) { s.Scale = 1.0f; s.NormalToSpecial = false; s.SpecialToNormal = false; }
    }
}

static int g_fail = 0;
static void Check(bool c, int line) { if (!c) { std::printf("fail at line %d\n", line); ++g_fail; } }
#define CHECK(c) Check((c), __LINE__)
static int Count(int kind) { int n = 0; for (int i = 0; i < g_n; ++i) if (g_calls[i].kind == kind) ++n; return n; }
static const SCall* Find(int kind, int nth) { for (int i = 0; i < g_n; ++i) if (g_calls[i].kind == kind && nth-- == 0) return &g_calls[i]; return 0; }

int main()
{
    SDef def = { C2DVector(10.0f, 20.0f), C2DVector(1.0f, 2.0f), 100, 101, 7, 300, 301, 400, 500 };
    def.NumButtons = 3; def.NumSpecials = 2;
    for (int i = 0; i < 3; ++i) { def.Buttons[i].ControllerType = i == 1 ? 0 : 1; def.Buttons[i].ButtonGraphic = 200 + i; def.Buttons[i].Offset = C2DVector(3.0f * i, -4.0f * i); }
    for (int k = 0; k < 2; ++k) { def.Specials[k].ControllerType = 1; def.Specials[k].ButtonGraphic = 250 + k; def.Specials[k].Offset = C2DVector(5.0f + k, 6.0f); }

    // 1. normal mode (special buttons hidden): rings, three buttons, run button halved, pressed button picks the middle graphic.
    SState s = { false, false, false, false, false, false, 2, { 7, 8, 9, 0 } };
    s.HandleCount = 5; s.Scale = 0.5f; s.Elapsed = 0.2f;
    g_n = 0; Draw(def, s);
    CHECK(Find(RING, 0) && Find(RING, 0)->graphic == 100 && Find(RING, 0)->x == 170.0f);
    CHECK(Count(BUTTON) == 3 && Find(BUTTON, 0)->alpha == 0.5f && Find(BUTTON, 1)->alpha == 1.0f && Find(BUTTON, 2)->index == 4);
    CHECK(Find(BUTTON, 2)->x == 176.0f && Find(BUTTON, 2)->y == 132.0f);
    CHECK(Count(BORDER) == 1 && Find(BORDER, 0)->index == 1);
    CHECK(Count(SPECIAL) == 0 && Find(MIDDLE, 0)->graphic == 202 && Find(MIDDLE, 0)->x == 171.0f);
    CHECK(s.Scale == 0.5f);

    // 2. special mode: favourite ring, no normal buttons, border loop still runs, uncastable overlay + shield-off graphic.
    SState t = { false, false, true, false, true, false, -1, { 0 } };
    t.HandleCount = 5; t.Scale = 0.9f; t.Elapsed = 0.1f;
    SSpecial a = { true, 4, true, false, true, false, 60 }; SSpecial b = { true, 2, false, true, false, true, 61 };
    t.Special[0] = a; t.Special[1] = b;
    g_n = 0; Draw(def, t);
    CHECK(Find(RING, 0)->graphic == 101 && Count(BUTTON) == 0 && Count(BORDER) == 1);
    CHECK(Count(UNCASTABLE) == 0);                       // not able, ability 4 with the shield: castable
    CHECK(Find(SPECIAL, 0)->graphic == 301 && Find(SPECIAL, 1)->graphic == 0);
    CHECK(Find(MIDDLE, 0)->graphic == 400);
    t.Special[0].Shield = false; t.Special[0].Graphic = 62;
    g_n = 0; Draw(def, t);
    CHECK(Count(UNCASTABLE) == 1 && Find(UNCASTABLE, 0)->index == 5 && Find(UNCASTABLE, 0)->graphic == 300);
    CHECK(Find(UNCASTABLE, 0)->alpha == 0.75f * (1.0f / 3.0f) && Find(SPECIAL, 0)->alpha == Find(UNCASTABLE, 0)->alpha);
    CHECK(Find(SPECIAL, 0)->graphic == 62 && Find(SPECIAL, 0)->x == 175.0f && Find(SPECIAL, 0)->y == 146.0f);

    // 3. quick-access menu active forces normal; a transition advances the scale and clamps it at 1.
    SState u = t; u.MenuActive = true; u.NormalToSpecial = true; u.Scale = 0.9f; u.Elapsed = 0.1f;
    g_n = 0; Draw(def, u);
    CHECK(Count(BUTTON) == 3 && Count(SPECIAL) == 0 && u.Scale == 1.0f && !u.NormalToSpecial);
    SState v = t; v.SpecialToNormal = true; v.Scale = 0.2f; v.Elapsed = 0.05f;
    g_n = 0; Draw(def, v);
    CHECK(Count(BUTTON) == 3 && v.Scale > 0.44f && v.Scale < 0.46f && v.SpecialToNormal);
    v.Paused = true; v.Scale = 0.2f; Draw(def, v);
    CHECK(v.Scale == 0.2f);

    if (g_fail == 0) std::printf("00651b04_TEST PASS\n");
    return g_fail;
}

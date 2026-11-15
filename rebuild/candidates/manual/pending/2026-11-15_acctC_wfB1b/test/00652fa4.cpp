// Behaviour test for 0x00652FA4 NPlayerGui::CDrawQuickAccessIcons::Draw(const CEnginePrimitivePrepareInfo&): a host model of the
// row's decisions with recording stand-ins for the engine (the style of the twin row 0x00651B04's test): the early return for a
// dead hero, the handle-vector sizes, ring / favourite ring sprite, the top ring (favourite, recall, aggressive graphic; alpha
// halved without the guild seal, scaled by the seal alpha) with its tooltip, the context-sensitive loop (first queued button
// picks the d-pad graphic, seal / lamp effects at index i + 9, charge graphic, tooltip nudged for button 3), the quick-access
// loop (effects at index i, uncastable cross with a third of the fade, shield-off graphic, graphic 0 for an unavailable ability,
// charge graphic for the slot or for -2, number (i + 1) % 10), killing unused effects except 9, the main seal effect 9, the
// flash sprite chosen by FlashButton 13..16 with the fade alpha, and the d-pad middle sprite fallback.
#include <cstdio>

class C2DVector { public: float X, Y; C2DVector() { } C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    bool IsNonZero() const { return X != 0.0f || Y != 0.0f; } };

struct SButton { C2DVector Offset; long ButtonGraphic; };
struct SDef {
    C2DVector RingsPos, TopOffset, TooltipOffset, LevelUpOffset, MainLevelUpOffset, MiddleOffset, CrossOffset;
    long RingsGraphic, RingsFavouriteGraphic, TopGraphic, TopFavouriteGraphic, TopAggressiveGraphic, RecallGraphic, DPadRecallGraphic;
    long UncastableGraphic, ShieldOffGraphic, DPadGraphic, SealEffect, LampEffect, MainSealEffect;
    SButton Context[4]; long NumContext;
    SButton Quick[4]; long NumQuick;
};
struct SItem { long Item; bool HasObjDef; long Property; bool HasAbility; long Ability; bool Available; bool Able; bool SpecialAvailable; long Graphic; };
struct SState {
    bool HeroAlive, NormalToSpecial, SpecialToNormal, ShowSpecial, Recall, Aggressive, Tooltips, HasAction, LightActive, Shield;
    long HUDAlpha, SealIndex, SealCount, Pressed, ChargingButtonIndex, FlashButton;
    float SealAlpha, FlashFadeAlpha;
    SItem Context[4]; SItem Quick[4];
    bool EffectAlive[12];
    long PrimitiveHandles, SpecialHandles, CrossHandles;
    bool Drawing;
};
struct SCall { int kind; long index; long graphic; long alpha; float x, y; };
static SCall g_calls[160]; static int g_n = 0;
static void Rec(int kind, long index, long graphic, long alpha, const C2DVector& p)
{ SCall& c = g_calls[g_n++]; c.kind = kind; c.index = index; c.graphic = graphic; c.alpha = alpha; c.x = p.X; c.y = p.Y; }
enum { RING = 1, TOP, TIP, ICON, EFFECT, CHARGE, CROSS, NUMBER, KILL, FLASH, DPAD, LEVELUP };
static C2DVector Anchor(const C2DVector& p) { return C2DVector(p.X, p.Y + 120.0f); }

static void Draw(const SDef& def, SState& s)
{
    Rec(LEVELUP, 0, 0, 0, C2DVector(0.0f, 0.0f));
    if (!s.HeroAlive)
        return;
    s.Drawing = true;
    s.PrimitiveHandles = def.NumContext + 5; s.SpecialHandles = def.NumQuick; s.CrossHandles = def.NumQuick;
    long dpadGraphic = 0;
    C2DVector pos = Anchor(def.RingsPos);
    bool special = !s.NormalToSpecial && !s.SpecialToNormal && s.ShowSpecial;
    if (special) { if (def.RingsFavouriteGraphic > 0) Rec(RING, 0, def.RingsFavouriteGraphic, s.HUDAlpha & 0xff, pos); }
    else if (def.RingsGraphic > 0) Rec(RING, 0, def.RingsGraphic, s.HUDAlpha & 0xff, pos);
    pos = pos + def.TopOffset;
    long topGraphic;
    if (special) topGraphic = def.TopFavouriteGraphic;
    else { topGraphic = def.TopGraphic; if (s.Recall) topGraphic = def.RecallGraphic; }
    long alpha = s.HUDAlpha;
    if (s.SealIndex > 0 && s.SealCount <= 0) alpha /= 2;
    if (topGraphic > 0) {
        if (s.Aggressive) { topGraphic = def.TopAggressiveGraphic; alpha = s.HUDAlpha; }
        alpha = (long)(s.SealAlpha * alpha);
        Rec(TOP, 1, topGraphic, alpha & 0xff, pos);
        if (s.Tooltips && s.SealCount > 0 && s.HasAction) Rec(TIP, -1, 0, 0xff, Anchor(def.RingsPos + def.TooltipOffset));
    }
    long i = 0;
    bool effectUsed[12] = { false, false, false, false, false, false, false, false, false, false, false, false };
    for (; i < def.NumContext; ++i) {
        float fade = 1.0f;
        if (dpadGraphic == 0 && s.Pressed == i) dpadGraphic = def.Context[i].ButtonGraphic;
        const SItem& it = s.Context[i];
        long graphic = it.Graphic;
        C2DVector p = def.Context[i].Offset + Anchor(def.RingsPos);
        if (it.Item > 0 && it.HasObjDef) {
            if (it.Property & 0x20) {
                fade = s.SealAlpha;
                Rec(LEVELUP, i, 0, 0, p + def.LevelUpOffset);
                if (s.Recall) graphic = def.DPadRecallGraphic;
                if (s.ChargingButtonIndex != -1) { Rec(EFFECT, i + 9, def.SealEffect, 0, p); effectUsed[i + 9] = true; }
            }
            if ((it.Property & 1) && s.LightActive) { Rec(EFFECT, i + 9, def.LampEffect, 0, p); effectUsed[i + 9] = true; }
        }
        Rec(ICON, i + 2, graphic, (long)(fade * 255.0f), p);
        if (s.ChargingButtonIndex == i) Rec(CHARGE, i, 0, 0, def.Context[i].Offset);
        if (graphic > 0 && s.Tooltips && s.HasAction) {
            C2DVector text(p.X, p.Y - 12.0f);
            if (i == 3) { text.X += 32.0f; text.Y += 16.0f; }
            Rec(TIP, i, 0, 0xff, text);
        }
    }
    for (i = 0; i < def.NumQuick; ++i) {
        float fade = 1.0f;
        const SItem& it = s.Quick[i];
        long graphic = it.Graphic;
        long slot = def.NumContext + i;
        C2DVector p = Anchor(def.RingsPos + def.Quick[i].Offset);
        if (it.Item > 0) {
            if (it.HasObjDef) {
                if (it.Property & 0x20) {
                    fade = s.SealAlpha;
                    Rec(LEVELUP, i, 0, 0, p + def.LevelUpOffset);
                    if (s.Recall) graphic = def.DPadRecallGraphic;
                    if (s.ChargingButtonIndex != -1) { Rec(EFFECT, i, def.SealEffect, 0, p); effectUsed[i] = true; }
                }
                if ((it.Property & 1) && s.LightActive) { Rec(EFFECT, i, def.LampEffect, 0, p); effectUsed[i] = true; }
            }
            if (it.HasAbility) {
                if (it.Available) {
                    if (!(!it.Able ? it.Ability == 4 && s.Shield : it.SpecialAvailable)) {
                        Rec(CROSS, i, def.UncastableGraphic, (long)(fade * 255.0f), p + def.CrossOffset);
                        fade *= 1.0f / 3;
                    }
                    if (it.Ability == 4 && s.Shield) graphic = def.ShieldOffGraphic;
                } else
                    graphic = 0;
            }
        }
        Rec(ICON, 100 + i, graphic, (long)(fade * 255.0f), p);
        if (s.ChargingButtonIndex == slot) Rec(CHARGE, slot, 0, 0, def.Quick[i].Offset);
        else if (s.ChargingButtonIndex == -2) Rec(CHARGE, -2, 0, 0, C2DVector(0.0f, 0.0f));
        if (graphic > 0 && s.Tooltips) Rec(NUMBER, (i + 1) % 10, 0, 0xff, p);
    }
    for (unsigned long e = 0; e < 12; ++e)
        if (e != 9 && !effectUsed[e] && s.EffectAlive[e]) { Rec(KILL, (long)e, 0, 0, C2DVector(0.0f, 0.0f)); s.EffectAlive[e] = false; }
    if (s.ChargingButtonIndex != -1) Rec(EFFECT, 9, def.MainSealEffect, 0, Anchor(def.MainLevelUpOffset + def.RingsPos));
    else if (s.EffectAlive[9]) { Rec(KILL, 9, 0, 0, C2DVector(0.0f, 0.0f)); s.EffectAlive[9] = false; }
    if (dpadGraphic == 0) dpadGraphic = def.DPadGraphic;
    if (s.FlashButton != 0) {
        dpadGraphic = def.DPadGraphic;
        long flashGraphic = 0;
        switch (s.FlashButton) {
        case 13: flashGraphic = def.Quick[0].ButtonGraphic; break;
        case 14: flashGraphic = def.Quick[1].ButtonGraphic; break;
        case 15: flashGraphic = def.Quick[2].ButtonGraphic; break;
        case 16: flashGraphic = def.Quick[3].ButtonGraphic; break;
        }
        if (flashGraphic > 0)
            Rec(FLASH, 7, flashGraphic, (unsigned char)(long)(s.FlashFadeAlpha * 255.0f), Anchor(def.RingsPos) + def.MiddleOffset);
    }
    if (dpadGraphic > 0) Rec(DPAD, 6, dpadGraphic, 0xff, Anchor(def.RingsPos) + def.MiddleOffset);
}

static int g_fail = 0;
static void Check(bool c, int line) { if (!c) { std::printf("fail at line %d\n", line); ++g_fail; } }
#define CHECK(c) Check((c), __LINE__)
static int Count(int kind) { int n = 0; for (int i = 0; i < g_n; ++i) if (g_calls[i].kind == kind) ++n; return n; }
static const SCall* Find(int kind, int nth) { for (int i = 0; i < g_n; ++i) if (g_calls[i].kind == kind && nth-- == 0) return &g_calls[i]; return 0; }
static SState Base()
{
    SState s; for (unsigned k = 0; k < sizeof(s); ++k) ((char*)&s)[k] = 0;
    s.HeroAlive = true; s.HUDAlpha = 200; s.SealIndex = 5; s.SealCount = 1; s.Pressed = -1; s.ChargingButtonIndex = -1;
    s.SealAlpha = 0.5f; s.FlashFadeAlpha = 0.25f;
    return s;
}

int main()
{
    SDef def;
    def.RingsPos = C2DVector(10.0f, 20.0f); def.TopOffset = C2DVector(1.0f, 2.0f); def.TooltipOffset = C2DVector(3.0f, 4.0f);
    def.LevelUpOffset = C2DVector(5.0f, 6.0f); def.MainLevelUpOffset = C2DVector(7.0f, 8.0f); def.MiddleOffset = C2DVector(9.0f, 10.0f);
    def.CrossOffset = C2DVector(11.0f, 12.0f);
    def.RingsGraphic = 100; def.RingsFavouriteGraphic = 101; def.TopGraphic = 110; def.TopFavouriteGraphic = 111; def.TopAggressiveGraphic = 112;
    def.RecallGraphic = 113; def.DPadRecallGraphic = 114; def.UncastableGraphic = 120; def.ShieldOffGraphic = 121; def.DPadGraphic = 130;
    def.SealEffect = 140; def.LampEffect = 141; def.MainSealEffect = 142;
    def.NumContext = 4; def.NumQuick = 4;
    for (int k = 0; k < 4; ++k) {
        def.Context[k].Offset = C2DVector(2.0f * k, 3.0f * k); def.Context[k].ButtonGraphic = 200 + k;
        def.Quick[k].Offset = C2DVector(-2.0f * k, 1.0f + k); def.Quick[k].ButtonGraphic = 210 + k;
    }

    // 1. dead hero: only the level-up position reset, nothing drawn, handles untouched.
    SState d = Base(); d.HeroAlive = false; g_n = 0; Draw(def, d);
    CHECK(g_n == 1 && g_calls[0].kind == LEVELUP && !d.Drawing && d.PrimitiveHandles == 0);

    // 2. normal mode, empty slots: ring + top ring at seal alpha, d-pad fallback graphic, handle counts.
    SState s = Base(); g_n = 0; Draw(def, s);
    CHECK(s.Drawing && s.PrimitiveHandles == 9 && s.SpecialHandles == 4 && s.CrossHandles == 4);
    CHECK(Find(RING, 0)->graphic == 100 && Find(RING, 0)->alpha == 200 && Find(RING, 0)->y == 140.0f);
    CHECK(Find(TOP, 0)->graphic == 110 && Find(TOP, 0)->alpha == 100 && Find(TOP, 0)->x == 11.0f && Find(TOP, 0)->y == 142.0f);
    CHECK(Count(TIP) == 0 && Count(ICON) == 8 && Count(NUMBER) == 0 && Count(FLASH) == 0);
    CHECK(Find(DPAD, 0)->graphic == 130 && Find(DPAD, 0)->x == 19.0f && Find(DPAD, 0)->y == 150.0f);
    CHECK(Find(ICON, 0)->index == 2 && Find(ICON, 4)->index == 100 && Find(ICON, 5)->x == 8.0f && Find(ICON, 5)->y == 142.0f);

    // 3. no guild seal in the inventory halves the top alpha; aggressive mode restores it and swaps the graphic; recall graphic.
    SState t = Base(); t.SealCount = 0; t.Recall = true; g_n = 0; Draw(def, t);
    CHECK(Find(TOP, 0)->graphic == 113 && Find(TOP, 0)->alpha == 50);
    t.Aggressive = true; g_n = 0; Draw(def, t);
    CHECK(Find(TOP, 0)->graphic == 112 && Find(TOP, 0)->alpha == 100);
    t.SealIndex = 0; t.Aggressive = false; g_n = 0; Draw(def, t);
    CHECK(Find(TOP, 0)->alpha == 100);                       // no seal definition: not halved

    // 4. favourite mode: favourite ring and top graphics (recall ignored); tooltips need the seal and an assigned action.
    SState f = Base(); f.ShowSpecial = true; f.Recall = true; f.Tooltips = true; f.HasAction = true; g_n = 0; Draw(def, f);
    CHECK(Find(RING, 0)->graphic == 101 && Find(TOP, 0)->graphic == 111);
    CHECK(Count(TIP) == 1 && Find(TIP, 0)->index == -1 && Find(TIP, 0)->x == 13.0f && Find(TIP, 0)->y == 144.0f);
    f.SealCount = 0; g_n = 0; Draw(def, f); CHECK(Count(TIP) == 0);
    f.SealCount = 1; f.NormalToSpecial = true; g_n = 0; Draw(def, f); CHECK(Find(RING, 0)->graphic == 100 && Find(TOP, 0)->graphic == 113);

    // 5. context-sensitive loop: pressed button picks the d-pad graphic, seal item fades and charges an effect at i + 9,
    //    lamp effect, charge graphic, tooltip nudged for button 3.
    SState c = Base(); c.Pressed = 2; c.Tooltips = true; c.HasAction = true; c.ChargingButtonIndex = 1; c.LightActive = true;
    SItem seal = { 7, true, 0x20, false, 0, false, false, false, 50 }; SItem lamp = { 8, true, 1, false, 0, false, false, false, 51 };
    SItem plain = { 9, true, 0, false, 0, false, false, false, 52 };
    c.Context[1] = seal; c.Context[2] = lamp; c.Context[3] = plain; c.EffectAlive[10] = c.EffectAlive[11] = c.EffectAlive[3] = c.EffectAlive[9] = true;
    g_n = 0; Draw(def, c);
    CHECK(Find(DPAD, 0)->graphic == 202);
    CHECK(Find(ICON, 1)->alpha == 127 && Find(ICON, 1)->graphic == 50 && Find(ICON, 2)->alpha == 255);
    CHECK(Find(EFFECT, 0)->index == 10 && Find(EFFECT, 0)->graphic == 140 && Find(EFFECT, 1)->index == 11 && Find(EFFECT, 1)->graphic == 141);
    CHECK(Count(CHARGE) == 1 && Find(CHARGE, 0)->index == 1 && Find(CHARGE, 0)->x == 2.0f);
    CHECK(Count(TIP) == 4 && Find(TIP, 1)->index == 1 && Find(TIP, 1)->y == 131.0f && Find(TIP, 3)->index == 3 && Find(TIP, 3)->x == 48.0f && Find(TIP, 3)->y == 153.0f);
    CHECK(Count(KILL) == 1 && Find(KILL, 0)->index == 3 && c.EffectAlive[10] && c.EffectAlive[9]);   // used effects and 9 survive
    CHECK(Find(EFFECT, 2)->index == 9 && Find(EFFECT, 2)->graphic == 142 && Find(EFFECT, 2)->x == 17.0f && Find(EFFECT, 2)->y == 148.0f);
    c.ChargingButtonIndex = -1; c.Recall = true; g_n = 0; Draw(def, c);
    CHECK(Find(ICON, 1)->graphic == 114 && Count(EFFECT) == 1 && Count(KILL) == 2 && !c.EffectAlive[9] && !c.EffectAlive[10]);

    // 6. quick-access loop: uncastable cross and a third of the fade, shield-off graphic, graphic 0 when unavailable,
    //    effects at index i, charge graphic for the slot and for -2, the number tooltip.
    SState q = Base(); q.Tooltips = true; q.ChargingButtonIndex = 6;
    SItem un = { 20, false, 0, true, 2, true, true, false, 60 };     // able but not specially available: uncastable
    SItem sh = { 21, false, 0, true, 4, true, false, false, 61 };    // not able, ability 4: castable only with the shield
    SItem na = { 22, false, 0, true, 3, false, true, true, 62 };     // ability not available: no graphic
    SItem ql = { 23, true, 0x21, false, 0, false, false, false, 63 };
    q.Quick[0] = un; q.Quick[1] = sh; q.Quick[2] = na; q.Quick[3] = ql; q.LightActive = true;
    g_n = 0; Draw(def, q);
    CHECK(Count(CROSS) == 2 && Find(CROSS, 0)->index == 0 && Find(CROSS, 0)->graphic == 120 && Find(CROSS, 0)->alpha == 255);
    CHECK(Find(CROSS, 0)->x == 21.0f && Find(CROSS, 0)->y == 153.0f && Find(ICON, 4)->alpha == 85 && Find(ICON, 4)->graphic == 60);
    CHECK(Find(ICON, 5)->graphic == 61 && Find(ICON, 6)->graphic == 0 && Find(ICON, 6)->alpha == 255);
    CHECK(Find(ICON, 7)->alpha == 127 && Find(EFFECT, 0)->index == 3 && Find(EFFECT, 1)->index == 3 && Find(EFFECT, 1)->graphic == 141);
    CHECK(Count(CHARGE) == 1 && Find(CHARGE, 0)->index == 6 && Find(CHARGE, 0)->x == -4.0f);
    CHECK(Count(NUMBER) == 3 && Find(NUMBER, 0)->index == 1 && Find(NUMBER, 2)->index == 4);
    q.Shield = true; q.ChargingButtonIndex = -2; g_n = 0; Draw(def, q);
    CHECK(Count(CROSS) == 1 && Find(ICON, 5)->graphic == 121 && Find(ICON, 5)->alpha == 255 && Count(CHARGE) == 4 && Find(CHARGE, 0)->index == -2);

    // 7. flash: forces the d-pad graphic back to the default, sprite on handle 7 with the fade alpha; unknown buttons draw nothing.
    SState fl = Base(); fl.Pressed = 0; fl.FlashButton = 15; g_n = 0; Draw(def, fl);
    CHECK(Count(FLASH) == 1 && Find(FLASH, 0)->graphic == 212 && Find(FLASH, 0)->alpha == 63 && Find(FLASH, 0)->index == 7);
    CHECK(Find(DPAD, 0)->graphic == 130 && Find(FLASH, 0)->x == 19.0f);
    fl.FlashButton = 12; g_n = 0; Draw(def, fl); CHECK(Count(FLASH) == 0 && Find(DPAD, 0)->graphic == 130);
    fl.FlashButton = 0; g_n = 0; Draw(def, fl); CHECK(Find(DPAD, 0)->graphic == 200);
    fl.FlashButton = 13; g_n = 0; Draw(def, fl); CHECK(Find(FLASH, 0)->graphic == 210);
    fl.FlashButton = 16; g_n = 0; Draw(def, fl); CHECK(Find(FLASH, 0)->graphic == 213);

    if (g_fail == 0) std::printf("00652fa4_TEST PASS\n");
    return g_fail;
}

// Behaviour test for CTCWife::CheckAppearanceChanges (0x006D1250): a standalone copy of the row's logic over small models of the hero's
// TCs (attachable appearance modifiers -> clothing / hair def-index lists, morph -> tattoo things, stats -> fatness), the definition
// manager (def index -> object def -> appearance-modifier sub-def), the wife's FrameNoticedChangeInAppearance[16], her three
// appearance bools (categories 13..15) and ThingsSeenThePlayerWear (a set of def indices). CWorld::GetFrame counts up, so every
// stamped frame also proves the ORDER of the notices. Each scenario is checked against hand-computed results, then every mutant
// (one changed decision of the row) must be caught by at least one scenario.
#include <cstdio>

static int g_mutant = 0;
static long g_frame = 1000;
static long GetFrame() { return g_frame++; }

struct CAppearanceModifierDef { long CoversBodyAreaFlags; long SuitPart; };
struct CThingObjectDef { long Index; bool HasModifier; CAppearanceModifierDef Modifier; };
struct CTattooThing { long CoversBodyAreaFlags; long GlobalIndex; };   // tattoo sub-def flags + GetDefGlobalIndexFromName(name)

static CThingObjectDef g_defs[16];
static int g_def_count = 0;
static bool GetDef(long index, const CThingObjectDef** out)
{
    for (int i = 0; i < g_def_count; ++i)
        if (g_defs[i].Index == index) { *out = &g_defs[i]; return true; }
    return false;
}
static bool GetSubDef(const CThingObjectDef* def, const CAppearanceModifierDef** out)
{
    if (!def->HasModifier) return false;
    *out = &def->Modifier;
    return true;
}

struct CLongList { long Items[8]; int Count; };
struct CModifiers { CLongList Clothing; CLongList Hair; };
struct CMorph { CTattooThing Tattoos[8]; int Count; };
struct CStats { float Fatness; };
struct CHero { CModifiers* Modifiers; CMorph* Morph; CStats* Stats; };

struct CLongSet {
    long Items[32]; int Count;
    bool Contains(long v) const { for (int i = 0; i < Count; ++i) if (Items[i] == v) return true; return false; }
    void Insert(long v) { if (!Contains(v)) Items[Count++] = v; }
};

struct CWife {
    float FatnessChangeThresholdForComment;    // PHeroMarriageDef->FatnessChangeThresholdForComment
    CLongSet ThingsSeenThePlayerWear;
    long FrameNoticedChangeInAppearance[16];
    float LastFatnessChangePoint;
    bool BooleanHusbandAppearances[4];
    bool& AppearanceBool(long category) { return BooleanHusbandAppearances[category - 12]; }
    void CheckAppearanceChanges(CHero& hero);
};

static float FloatAbs(float f) { return f < 0.0f ? -f : f; }

void CWife::CheckAppearanceChanges(CHero& hero)
{
    long covered_areas = 0;
    bool hair_visible = true;
    if (hero.Modifiers) {
        CLongList& clothing = hero.Modifiers->Clothing;
        for (int it = 0; it < clothing.Count; ++it) {
            long def_index = clothing.Items[it];
            const CThingObjectDef* object_def;
            const CAppearanceModifierDef* modifier;
            if (GetDef(def_index, &object_def)) {
                if (GetSubDef(object_def, &modifier)) {
                    long covers = modifier->CoversBodyAreaFlags;
                    covered_areas |= covers;
                    bool unseen = !ThingsSeenThePlayerWear.Contains(def_index);
                    if (unseen && (covers || g_mutant == 1)) {
                        FrameNoticedChangeInAppearance[modifier->SuitPart] = GetFrame();
                        ThingsSeenThePlayerWear.Insert(def_index);
                    }
                }
            }
        }
        CLongList& hair = hero.Modifiers->Hair;
        for (int it = 0; it < hair.Count; ++it) {
            long def_index = hair.Items[it];
            const CThingObjectDef* object_def;
            const CAppearanceModifierDef* modifier;
            if (GetDef(def_index, &object_def)) {
                if (GetSubDef(object_def, &modifier)) {
                    if (hair_visible && (modifier->CoversBodyAreaFlags & 0x10))
                        hair_visible = false;
                    bool unseen = !ThingsSeenThePlayerWear.Contains(def_index);
                    long new_areas = g_mutant == 5 ? (modifier->CoversBodyAreaFlags & covered_areas) : (modifier->CoversBodyAreaFlags & ~covered_areas);
                    if (unseen && new_areas) {
                        long category;
                        if (g_mutant == 2)
                            category = (modifier->CoversBodyAreaFlags & 0x80) ? 7 : ((modifier->CoversBodyAreaFlags & 0x100) ? 8 : 6);
                        else
                            category = (modifier->CoversBodyAreaFlags & 0x100) ? 8 : ((modifier->CoversBodyAreaFlags & 0x80) ? 7 : 6);
                        FrameNoticedChangeInAppearance[category] = GetFrame();
                        ThingsSeenThePlayerWear.Insert(def_index);
                    }
                }
            }
        }
        if (AppearanceBool(14) != ((covered_areas & 0xe) == 0)) {
            if (!AppearanceBool(14)) {
                AppearanceBool(14) = true;
                FrameNoticedChangeInAppearance[14] = GetFrame();
            } else {
                AppearanceBool(14) = false;
                FrameNoticedChangeInAppearance[14] = 0;
            }
        }
        if (AppearanceBool(15) != ((covered_areas & 8) == 0)) {
            if (!AppearanceBool(15)) {
                AppearanceBool(15) = true;
                FrameNoticedChangeInAppearance[15] = GetFrame();
            } else {
                AppearanceBool(15) = false;
                FrameNoticedChangeInAppearance[15] = 0;
            }
        }
        if (AppearanceBool(14) && g_mutant != 6)
            FrameNoticedChangeInAppearance[15] = 0;
    }
    if (!AppearanceBool(13) && hair_visible) {
        AppearanceBool(13) = true;
        FrameNoticedChangeInAppearance[13] = GetFrame();
    } else if (AppearanceBool(13) && !hair_visible) {
        AppearanceBool(13) = hair_visible;
        FrameNoticedChangeInAppearance[13] = 0;
    }
    if (hero.Morph) {
        for (int i = 0; i < hero.Morph->Count; ++i) {
            const CTattooThing& tattoo = hero.Morph->Tattoos[i];
            bool uncovered = !(tattoo.CoversBodyAreaFlags & covered_areas);
            if (g_mutant == 3) uncovered = !uncovered;
            if (uncovered) {
                long def_index = tattoo.GlobalIndex;
                if (def_index > 0 || (g_mutant == 7 && def_index == 0)) {
                    if (!ThingsSeenThePlayerWear.Contains(def_index)) {
                        FrameNoticedChangeInAppearance[9] = GetFrame();
                        ThingsSeenThePlayerWear.Insert(def_index);
                    }
                }
            }
        }
    }
    if (hero.Stats) {
        float change = hero.Stats->Fatness - LastFatnessChangePoint;
        if (FloatAbs(change) > FatnessChangeThresholdForComment) {
            if (g_mutant != 8)
                LastFatnessChangePoint = hero.Stats->Fatness;
            long noticed = change > 0.0f ? 10 : 11;
            long cleared = change > 0.0f ? 11 : 10;
            if (g_mutant == 4) { long t = noticed; noticed = cleared; cleared = t; }
            FrameNoticedChangeInAppearance[noticed] = GetFrame();
            FrameNoticedChangeInAppearance[cleared] = 0;
        }
    }
}

static void AddDef(long index, bool has_modifier, long covers, long suit_part)
{
    CThingObjectDef& d = g_defs[g_def_count++];
    d.Index = index; d.HasModifier = has_modifier; d.Modifier.CoversBodyAreaFlags = covers; d.Modifier.SuitPart = suit_part;
}

static void ResetWife(CWife& w)
{
    w.FatnessChangeThresholdForComment = 1.5f;
    w.ThingsSeenThePlayerWear.Count = 0;
    for (int i = 0; i < 16; ++i) w.FrameNoticedChangeInAppearance[i] = -1;
    w.LastFatnessChangePoint = 10.0f;
    for (int i = 0; i < 4; ++i) w.BooleanHusbandAppearances[i] = false;
}

static int g_failures;
static void Expect(bool ok, int scenario, int line)
{
    if (!ok) { ++g_failures; if (g_mutant == 0) std::printf("scenario %d failed at line %d\n", scenario, line); }
}
#define EXPECT(s, c) Expect((c), (s), __LINE__)

static void Scenario1()
{
    // clothing: 100 new (covers 0x0A, suit part 3), 101 already seen (0x04, part 5), 102 no def, 103 new but covers nothing,
    // 104 def without modifier; hair: 200 new (0x190 -> category 8 ahead of 7, hides hair), 201 new (0x80 -> 7), 202 covers only already-covered
    // areas; tattoos: covers 1 new index 300, covers 2 (covered), index 0, 300 again; fatness 10 -> 12.
    CWife w; ResetWife(w);
    w.ThingsSeenThePlayerWear.Insert(101);
    w.AppearanceBool(13) = true;
    CModifiers m;
    m.Clothing.Count = 5; m.Clothing.Items[0] = 100; m.Clothing.Items[1] = 101; m.Clothing.Items[2] = 102; m.Clothing.Items[3] = 103; m.Clothing.Items[4] = 104;
    m.Hair.Count = 3; m.Hair.Items[0] = 200; m.Hair.Items[1] = 201; m.Hair.Items[2] = 202;
    CMorph morph; morph.Count = 4;
    morph.Tattoos[0].CoversBodyAreaFlags = 1; morph.Tattoos[0].GlobalIndex = 300;
    morph.Tattoos[1].CoversBodyAreaFlags = 2; morph.Tattoos[1].GlobalIndex = 301;
    morph.Tattoos[2].CoversBodyAreaFlags = 0x20; morph.Tattoos[2].GlobalIndex = 0;
    morph.Tattoos[3].CoversBodyAreaFlags = 0x40; morph.Tattoos[3].GlobalIndex = 300;
    CStats stats; stats.Fatness = 12.0f;
    CHero hero; hero.Modifiers = &m; hero.Morph = &morph; hero.Stats = &stats;
    g_frame = 1000;
    w.CheckAppearanceChanges(hero);
    EXPECT(1, w.FrameNoticedChangeInAppearance[3] == 1000);
    EXPECT(1, w.FrameNoticedChangeInAppearance[5] == -1);
    EXPECT(1, w.FrameNoticedChangeInAppearance[8] == 1001);
    EXPECT(1, w.FrameNoticedChangeInAppearance[7] == 1002);
    EXPECT(1, w.FrameNoticedChangeInAppearance[6] == -1);
    EXPECT(1, w.FrameNoticedChangeInAppearance[14] == -1 && !w.AppearanceBool(14));
    EXPECT(1, w.FrameNoticedChangeInAppearance[15] == -1 && !w.AppearanceBool(15));
    EXPECT(1, w.FrameNoticedChangeInAppearance[13] == 0 && !w.AppearanceBool(13));
    EXPECT(1, w.FrameNoticedChangeInAppearance[9] == 1003);
    EXPECT(1, w.FrameNoticedChangeInAppearance[10] == 1004 && w.FrameNoticedChangeInAppearance[11] == 0);
    EXPECT(1, w.LastFatnessChangePoint == 12.0f);
    EXPECT(1, g_frame == 1005);
    EXPECT(1, w.ThingsSeenThePlayerWear.Count == 5);
    EXPECT(1, w.ThingsSeenThePlayerWear.Contains(100) && w.ThingsSeenThePlayerWear.Contains(200) && w.ThingsSeenThePlayerWear.Contains(201));
    EXPECT(1, w.ThingsSeenThePlayerWear.Contains(300) && !w.ThingsSeenThePlayerWear.Contains(301) && !w.ThingsSeenThePlayerWear.Contains(0));
    EXPECT(1, !w.ThingsSeenThePlayerWear.Contains(103) && !w.ThingsSeenThePlayerWear.Contains(202));
}

static void Scenario2()
{
    // no modifiers TC (hair stays visible: category 13 noticed), no morph TC, fatness 10 -> 7 (thinner: 11 noticed, 10 cleared).
    CWife w; ResetWife(w);
    CStats stats; stats.Fatness = 7.0f;
    CHero hero; hero.Modifiers = 0; hero.Morph = 0; hero.Stats = &stats;
    g_frame = 2000;
    w.CheckAppearanceChanges(hero);
    EXPECT(2, w.AppearanceBool(13) && w.FrameNoticedChangeInAppearance[13] == 2000);
    EXPECT(2, w.FrameNoticedChangeInAppearance[11] == 2001 && w.FrameNoticedChangeInAppearance[10] == 0);
    EXPECT(2, w.LastFatnessChangePoint == 7.0f);
    EXPECT(2, w.FrameNoticedChangeInAppearance[14] == -1 && w.FrameNoticedChangeInAppearance[15] == -1);
    EXPECT(2, g_frame == 2002);
}

static void Scenario3()
{
    // modifiers with nothing worn: no body clothing (14) and no torso clothing (15) are noticed, then 15 is cleared because 14 is
    // set; hair visible and already noticed; a tattoo over nothing covered with an index seen before; fatness change too small.
    CWife w; ResetWife(w);
    w.AppearanceBool(13) = true;
    w.ThingsSeenThePlayerWear.Insert(400);
    CModifiers m; m.Clothing.Count = 0; m.Hair.Count = 0;
    CMorph morph; morph.Count = 1; morph.Tattoos[0].CoversBodyAreaFlags = 0x8; morph.Tattoos[0].GlobalIndex = 400;
    CStats stats; stats.Fatness = 11.0f;
    CHero hero; hero.Modifiers = &m; hero.Morph = &morph; hero.Stats = &stats;
    g_frame = 3000;
    w.CheckAppearanceChanges(hero);
    EXPECT(3, w.AppearanceBool(14) && w.FrameNoticedChangeInAppearance[14] == 3000);
    EXPECT(3, w.AppearanceBool(15) && w.FrameNoticedChangeInAppearance[15] == 0);
    EXPECT(3, w.AppearanceBool(13) && w.FrameNoticedChangeInAppearance[13] == -1);
    EXPECT(3, w.FrameNoticedChangeInAppearance[9] == -1);
    EXPECT(3, w.FrameNoticedChangeInAppearance[10] == -1 && w.FrameNoticedChangeInAppearance[11] == -1 && w.LastFatnessChangePoint == 10.0f);
    EXPECT(3, g_frame == 3002);
}

static void Scenario4()
{
    // body clothing worn again (0x02, no torso 0x08): 14 cleared; 15 stays set (no torso clothing) and is not cleared;
    // a tattoo under the clothing is not noticed; a new tattoo with index 0 is ignored.
    CWife w; ResetWife(w);
    w.AppearanceBool(13) = true; w.AppearanceBool(14) = true; w.AppearanceBool(15) = true;
    w.FrameNoticedChangeInAppearance[14] = 77; w.FrameNoticedChangeInAppearance[15] = 88;
    CModifiers m; m.Clothing.Count = 1; m.Clothing.Items[0] = 110; m.Hair.Count = 0;
    CMorph morph; morph.Count = 2;
    morph.Tattoos[0].CoversBodyAreaFlags = 0x2; morph.Tattoos[0].GlobalIndex = 500;
    morph.Tattoos[1].CoversBodyAreaFlags = 0x1; morph.Tattoos[1].GlobalIndex = 0;
    CHero hero; hero.Modifiers = &m; hero.Morph = &morph; hero.Stats = 0;
    g_frame = 4000;
    w.CheckAppearanceChanges(hero);
    EXPECT(4, w.FrameNoticedChangeInAppearance[2] == 4000);
    EXPECT(4, !w.AppearanceBool(14) && w.FrameNoticedChangeInAppearance[14] == 0);
    EXPECT(4, w.AppearanceBool(15) && w.FrameNoticedChangeInAppearance[15] == 88);
    EXPECT(4, w.FrameNoticedChangeInAppearance[9] == -1 && !w.ThingsSeenThePlayerWear.Contains(500) && !w.ThingsSeenThePlayerWear.Contains(0));
    EXPECT(4, g_frame == 4001);
}

static int RunAll()
{
    g_failures = 0;
    Scenario1(); Scenario2(); Scenario3(); Scenario4();
    return g_failures;
}

int main()
{
    g_def_count = 0;
    AddDef(100, true, 0x0A, 3);
    AddDef(101, true, 0x04, 5);
    AddDef(103, true, 0x00, 4);
    AddDef(104, false, 0x01, 1);
    AddDef(110, true, 0x02, 2);
    AddDef(200, true, 0x190, 0);
    AddDef(201, true, 0x80, 0);
    AddDef(202, true, 0x02, 0);
    g_mutant = 0;
    if (RunAll() != 0) { std::printf("006d1250_TEST FAIL\n"); return 1; }
    for (int mutant = 1; mutant <= 8; ++mutant) {
        g_mutant = mutant;
        if (RunAll() == 0) { std::printf("mutant %d not caught\n006d1250_TEST FAIL\n", mutant); return 1; }
    }
    g_mutant = 0;
    std::printf("006d1250_TEST PASS\n");
    return 0;
}

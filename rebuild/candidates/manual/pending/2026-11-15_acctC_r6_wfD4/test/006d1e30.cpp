// Behaviour test for 0x006D1E30 CTCWife::FrameUpdate: a model of the row's decisions over a fake world (frame counter, hero, perception,
// opinion values, marriage def). Checked: the brain chosen after marriage / divorce (once), the attitude refresh every third zone-active
// frame, the hate timers (divorce warning start offset, divorce pending, warning pending), the deeds posted while the wife sees the hero
// (divorce deed returns at once; warning; sex offer only after both love-at-home delays; seen-you after the absence threshold; house
// dressing only for a higher dress level inside her home), the gift-opinion distance (averaged once per elapsed period), the marriage
// state written to the hero stats (just married 1, love 3, hate 2, none otherwise), the courting love attitude (blend 0.9 / 0.1 when
// rising, decay when the evaluation is late and no ring, override at the threshold, first love: lover count + scoreboard activity 0xb),
// the attraction icon (scale 0.5 * attraction, pulsing alpha at full attraction) and the culling of remembered gifts.
#include <cstdio>
#include <cmath>
#include <map>
#include <vector>

struct Def {
    float SecsHateBeforeDivorce, SecsHateBetweenWarnings, SecsLoveHomeBeforeSex, SecsLoveHomeBetweenSex, SecsSinceSeeingYou;
    long SecsGiftOpinion, SecsGiftOpportunities, SecsLoveAttitude, SecsCheckAppearance, FramesCullGifts, SecsRememberGifts;
    float JustMarriedSecs, DecaySecs, LoveThreshold;
};
struct World {
    long Frame, FPS;
    bool ZoneActive, HeroAlive, Sees, Aware, HeroInHome;
    long Attitude, DressLevel, Lovers, ScoreActivity;
    float ScoreValue, Opinion3, Opinion4, ReactionStrength;
    bool HasStats, MarriageKnown;
    long MarriageState, StatesSet;
    std::vector<long> Deeds;
    float IconScale; long IconAlpha; bool IconOn;
};
struct Wife {
    bool NeedsToChangeBrain; long Brain;                        // 1 = BRAIN_WIFE, 2 = BRAIN_DIVORCED_WIFE
    long FrameMarried, AttitudeCounter, OpinionAttitude, ForcedAttitude;
    long FrameEnteredHate, FrameLastWarning, FrameEnteredLoveHome, FrameLastSexOffer, FrameLastAware, DressCommented;
    bool DivorcePending, WarningPending, JustMarried, Courtable, HasBeenInLove, Ring;
    long FrameLastGiftOpinion; float GiftDistance;
    long FrameLastLove; float Love;
    long FrameLastCulled; std::map<long, long> Gifts;
};
static Def D = { 10.0f, 4.0f, 2.0f, 5.0f, 3.0f, 1, 1000, 2, 1000, 5, 1, 5.0f, 1000.0f, 0.8f };

static float Limit(float v, float lo, float hi) { return v < lo ? lo : v > hi ? hi : v; }
static void Deed(World& w, long type) { w.Deeds.push_back(type); }

static void FrameUpdate(Wife& m, World& w)
{
    bool married = m.FrameMarried <= w.Frame;
    if (m.NeedsToChangeBrain) { m.Brain = married ? 1 : 2; m.NeedsToChangeBrain = false; }
    if (w.ZoneActive) {
        if (m.AttitudeCounter % 3 == 0) { m.OpinionAttitude = w.Attitude; m.AttitudeCounter = 0; }
        ++m.AttitudeCounter;
    }
    long frame = w.Frame;
    if (married) {
        if (m.OpinionAttitude == 17) {
            if (m.FrameEnteredHate == 0x7fffffff) {
                m.FrameEnteredHate = w.Frame;
                m.FrameLastWarning = (long)(m.FrameEnteredHate - w.FPS * D.SecsHateBetweenWarnings);
            } else if (!m.DivorcePending && (float)(w.Frame - m.FrameEnteredHate) > w.FPS * D.SecsHateBeforeDivorce) {
                m.DivorcePending = true;
            } else if (!m.WarningPending && (float)(w.Frame - m.FrameLastWarning) > w.FPS * D.SecsHateBetweenWarnings) {
                m.WarningPending = true;
            }
        } else {
            m.FrameEnteredHate = 0x7fffffff;
        }
        if (w.HeroAlive && w.Sees && w.Aware) {
            if (m.DivorcePending) { m.DivorcePending = false; Deed(w, 0x59); return; }
            if (m.WarningPending) { m.WarningPending = false; m.FrameLastWarning = w.Frame; Deed(w, 0x58); }
            if (m.OpinionAttitude == 13 && w.HeroInHome) {
                if (m.FrameEnteredLoveHome == 0x7fffffff) {
                    m.FrameEnteredLoveHome = w.Frame;
                    m.FrameLastSexOffer = (long)(m.FrameEnteredLoveHome - w.FPS * D.SecsLoveHomeBetweenSex);
                } else if ((float)(w.Frame - m.FrameEnteredLoveHome) > w.FPS * D.SecsLoveHomeBeforeSex &&
                           (float)(w.Frame - m.FrameLastSexOffer) > w.FPS * D.SecsLoveHomeBetweenSex) {
                    m.FrameLastSexOffer = w.Frame; Deed(w, 0x5a);
                }
            } else {
                m.FrameEnteredLoveHome = 0x7fffffff;
            }
            if ((float)(frame - m.FrameLastAware) > w.FPS * D.SecsSinceSeeingYou) { Deed(w, 0x51); m.FrameLastAware = frame; }
            if (w.HeroInHome && w.DressLevel > m.DressCommented) { Deed(w, 0x56); m.DressCommented = w.DressLevel; }
        }
        long gift_period = D.SecsGiftOpinion * w.FPS;
        if (frame - m.FrameLastGiftOpinion > gift_period) {
            float a = 1.0f - Limit(w.Opinion3, -1.0f, 1.0f), b = 1.0f - Limit(w.Opinion4, -1.0f, 1.0f);
            float distance = (float)std::sqrt(a * a + b * b);
            while (frame - m.FrameLastGiftOpinion > gift_period) {
                m.FrameLastGiftOpinion += gift_period;
                m.GiftDistance = (distance + m.GiftDistance) * 0.5f;
            }
        }
        if (w.HeroAlive && w.MarriageKnown && w.MarriageState != 4) {
            long state = 0;
            if ((float)(w.Frame - m.FrameMarried) < w.FPS * D.JustMarriedSecs) { m.JustMarried = true; state = 1; }
            else {
                m.JustMarried = false;
                if (m.OpinionAttitude == 13 || m.OpinionAttitude == 14) state = 3;
                else if (m.OpinionAttitude >= 15 && m.OpinionAttitude <= 17) state = 2;
            }
            if (state != 0) { w.MarriageState = state; ++w.StatesSet; }
        }
    } else if (m.Courtable) {
        long love_period = D.SecsLoveAttitude * w.FPS, elapsed = frame - m.FrameLastLove;
        if (elapsed > love_period) {
            float strength = w.ReactionStrength;
            m.Love = strength > m.Love ? (m.Love * 9.0f + strength) * 0.1f : strength;
            m.FrameLastLove += love_period;
            elapsed -= love_period;
            if (elapsed > love_period) {
                if (!m.Ring) m.Love -= elapsed * (1.0f / (w.FPS * D.DecaySecs));
                m.FrameLastLove = frame;
            }
            if (m.Love >= D.LoveThreshold) {
                m.ForcedAttitude = 12;
                if (!m.HasBeenInLove) {
                    m.HasBeenInLove = true;
                    if (w.HeroAlive && w.HasStats) { ++w.Lovers; w.ScoreActivity = 0xb; w.ScoreValue = (float)w.Lovers; }
                }
            } else if (m.ForcedAttitude == 12) {
                m.ForcedAttitude = 18;
            }
        }
        if (m.Love > 0.0f) {
            float attraction = m.Love / D.LoveThreshold;
            if (attraction > 1.0f) attraction = 1.0f;
            long alpha = 0xff;
            if (attraction == 1.0f) { alpha = (w.Frame * 4) % 0x1ff; if (alpha > 0xff) alpha = 0x1ff - alpha; }
            w.IconOn = true; w.IconScale = attraction * 0.5f; w.IconAlpha = alpha;
        } else {
            w.IconOn = false; w.IconScale = 1.0f; w.IconAlpha = 0xff;
        }
    }
    if (w.Frame - m.FrameLastCulled > D.FramesCullGifts) {
        m.FrameLastCulled = w.Frame;
        for (std::map<long, long>::iterator it = m.Gifts.begin(); it != m.Gifts.end();) {
            std::map<long, long>::iterator next = it; ++next;
            if (w.Frame - it->second > D.SecsRememberGifts * w.FPS) m.Gifts.erase(it);
            it = next;
        }
    }
}

static Wife NewWife(long married)
{
    Wife m = {};
    m.NeedsToChangeBrain = true; m.FrameMarried = married;
    m.FrameEnteredHate = m.FrameEnteredLoveHome = 0x7fffffff;
    return m;
}
static World NewWorld(long frame)
{
    World w = {};
    w.Frame = frame; w.FPS = 10; w.ZoneActive = true; w.HeroAlive = w.Sees = w.Aware = true;
    w.MarriageKnown = true; w.HasStats = true;
    return w;
}

int main()
{
    bool ok = true;
    // married wife in hate: brain, attitude, hate timers, then the divorce deed returns before the gift opinion.
    Wife m = NewWife(0); World w = NewWorld(100); w.Attitude = 17; w.Opinion3 = 2.0f; w.Opinion4 = 0.0f;
    m.Gifts[1] = 0; m.Gifts[2] = 95;
    FrameUpdate(m, w);
    ok = ok && m.Brain == 1 && !m.NeedsToChangeBrain && m.OpinionAttitude == 17 && m.AttitudeCounter == 1;
    ok = ok && m.FrameEnteredHate == 100 && m.FrameLastWarning == 60 && w.Deeds.size() == 1 && w.Deeds[0] == 0x51;
    ok = ok && m.GiftDistance == 0.998046875f && m.FrameLastGiftOpinion == 90 && w.MarriageState == 2 && w.StatesSet == 1;
    ok = ok && m.Gifts.size() == 1 && m.Gifts.count(2) == 1;                         // gift 1 older than 1 s * 10 fps
    w.Frame = 160; w.Deeds.clear(); FrameUpdate(m, w);                             // warning pending only (100 frames > 40, 60 <= 100)
    ok = ok && !m.DivorcePending && w.Deeds.size() == 2 && w.Deeds[0] == 0x58 && w.Deeds[1] == 0x51 && m.FrameLastWarning == 160 && !m.WarningPending;
    w.Frame = 201; w.Deeds.clear(); FrameUpdate(m, w);                             // 101 > 100: divorce pending, then posted at once
    ok = ok && w.Deeds.size() == 1 && w.Deeds[0] == 0x59 && !m.DivorcePending && m.FrameLastGiftOpinion == 150 && m.AttitudeCounter == 3;
    // love at home: sex offer only once both delays passed; house dressing commented once per higher level.
    Wife l = NewWife(0); World v = NewWorld(10); v.Attitude = 13; v.HeroInHome = true; v.DressLevel = 2;
    FrameUpdate(l, v);
    ok = ok && l.FrameEnteredLoveHome == 10 && l.FrameLastSexOffer == -40 && v.Deeds.size() == 1 && v.Deeds[0] == 0x56 && l.DressCommented == 2;
    v.Frame = 25; v.Deeds.clear(); FrameUpdate(l, v);
    ok = ok && v.Deeds.empty() && v.MarriageState == 1 && l.JustMarried;            // 15 frames: not yet 20 at home
    v.Frame = 31; v.Deeds.clear(); FrameUpdate(l, v);
    ok = ok && v.Deeds.size() == 2 && v.Deeds[0] == 0x5a && v.Deeds[1] == 0x51 && l.FrameLastSexOffer == 31;
    // divorced, courting: brain 2, love rising / override / first love, icon.
    Wife c = NewWife(1000000); c.Courtable = true; World u = NewWorld(50); u.ReactionStrength = 1.0f; u.Lovers = 3;
    FrameUpdate(c, u);
    ok = ok && c.Brain == 2 && c.Love > 0.0f && c.FrameLastLove == 50 && c.ForcedAttitude == 0 && u.IconOn && u.IconAlpha == 0xff;
    for (long f = 75; f <= 600; f += 25) { u.Frame = f; FrameUpdate(c, u); }
    ok = ok && u.IconScale == 0.5f && u.IconAlpha == 155 && c.HasBeenInLove && c.ForcedAttitude == 12 && u.Lovers == 4 && u.ScoreActivity == 0xb && u.ScoreValue == 4.0f;
    u.Frame = 700; u.ReactionStrength = 0.0f; c.Ring = true; FrameUpdate(c, u);                    // a falling evaluation drops the override
    ok = ok && c.Love == 0.0f && c.ForcedAttitude == 18 && !u.IconOn;
    std::printf(ok ? "006d1e30_TEST PASS\n" : "006d1e30_TEST FAIL\n");
    return ok ? 0 : 1;
}

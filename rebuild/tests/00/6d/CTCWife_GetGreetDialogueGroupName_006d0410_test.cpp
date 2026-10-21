// Behaviour test for CTCWife::GetGreetDialogueGroupName (0x006D0410): a wife married to the player (wedding frame <=
// now) offering sex says the go-to-bed line and stamps the offer frame, otherwise the attitude greet line
// (prefix + attitude name); a courting wife says the marriage line when willing, the need-house / need-ring line when
// in love (attitude 12), else (when she can be married and the chance roll is under the def probability) one of four
// advice lines picked by the LCG; anything else gives false.
#include <cstdio>
#include <string>

static long g_frame = 100;
static unsigned long g_seed = 1;
static float g_roll = 0.5f;
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static const char* g_attitudeNames[] = { "HATE", "DISLIKE", "NEUTRAL", "LIKE", "LOVE" };
static const char* g_advice[4] = { "A0", "A1", "A2", "A3" };

struct Wife {
    long frameMarried; bool offeringSex, courting, willing, ring, canMarry; int attitude, attitudeToPlayer;
    float probability; mutable long frameSexOffer;
    bool Get(std::string& name) const
    {
        if (frameMarried <= g_frame) {
            if (offeringSex) { name = "TEXT_AI_WIFE_SEX_OFFER_TO_GO_TO_BED"; frameSexOffer = g_frame; return true; }
            name = std::string("TEXT_AI_WIFE_OPINION_ACT_GREET_TO_HERO_") + g_attitudeNames[attitudeToPlayer];
            return true;
        }
        if (!courting) return false;
        if (willing) { name = "TEXT_AI_COMMENT_COURTSHIP_MARRIAGE"; return true; }
        if (courting && attitude == 12) {
            name = ring ? "TEXT_AI_COMMENT_COURTSHIP_NEED_HOUSE" : "TEXT_AI_COMMENT_COURTSHIP_NEED_WEDDING_RING";
            return true;
        }
        if (canMarry && probability > g_roll && courting) {
            g_seed = g_seed * 0x24a1 + 0x24df; g_seed = Ror13(g_seed);
            name = g_advice[g_seed % 4]; return true;
        }
        return false;
    }
};

int main()
{
    int ok = 1;
    std::string n;
    Wife w = { 50, true, false, false, false, false, 0, 3, 0.0f, 0 };
    ok &= w.Get(n) && n == "TEXT_AI_WIFE_SEX_OFFER_TO_GO_TO_BED" && w.frameSexOffer == 100;
    w.offeringSex = false;
    ok &= w.Get(n) && n == "TEXT_AI_WIFE_OPINION_ACT_GREET_TO_HERO_LIKE";
    Wife c = { 200, false, false, false, false, false, 0, 0, 0.0f, 0 };
    ok &= !c.Get(n);                                         // not married, not courting
    c.courting = true; c.willing = true;
    ok &= c.Get(n) && n == "TEXT_AI_COMMENT_COURTSHIP_MARRIAGE";
    c.willing = false; c.attitude = 12;
    ok &= c.Get(n) && n == "TEXT_AI_COMMENT_COURTSHIP_NEED_WEDDING_RING";
    c.ring = true;
    ok &= c.Get(n) && n == "TEXT_AI_COMMENT_COURTSHIP_NEED_HOUSE";
    c.attitude = 3;
    ok &= !c.Get(n);                                         // cannot be married
    c.canMarry = true; c.probability = 0.25f;
    ok &= !c.Get(n);                                         // roll 0.5 not under 0.25
    c.probability = 0.75f;
    unsigned long s = 1 * 0x24a1 + 0x24df; s = Ror13(s);
    ok &= c.Get(n) && n == g_advice[s % 4] && g_seed == s;
    std::printf(ok ? "006d0410_TEST PASS\n" : "006d0410_TEST FAIL\n");
    return ok ? 0 : 1;
}
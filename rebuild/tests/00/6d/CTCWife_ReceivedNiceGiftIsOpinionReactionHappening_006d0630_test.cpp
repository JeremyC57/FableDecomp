// Behaviour test for CTCWife::ReceivedNiceGiftIsOpinionReactionHappening (0x006D0630): the function's logic over a small copy of
// the model (a wife with her gift map, a world frame, a hero with an opinion deed log that records what is posted).
#include <cstdio>

enum { DEED_NONE = 0, DEED_RECEIVE_WEDDING_RING = 63, DEED_HIGH_PRIORITY_INCOMPREHENSION = 74, DEED_WIFE_GIFT_RECEIVE_ALREADY_GOT = 82 };
enum { ATTITUDE_LOVE = 12 };

static long g_frame = 0;                 // CWorld::GetFrame()
static const long g_constant_fps = 20;   // CMainGameComponent::GetConstantFPS()

struct SGiftDef { bool exists; bool has_gift_sub_def; bool is_wedding_ring; };
static SGiftDef g_defs[8];

struct SDeedLog {
    int  posted_count;
    long last_deed;
    const void* last_target;
    const void* last_victim;
    long last_post_type;
};
struct SHero { bool dead; bool has_deed_log; SDeedLog log; };
static SHero* g_hero = 0;                // PeekWorld().PeekMyPlayer()->GetPControlledCreature()

struct SGiftEntry { long def; long frame; };
struct SWife {
    long ParentThing;                    // stands for the wife's thing (only its address is passed on)
    long SecondsBetweenReceivingGiftOpinionReactionCanHappen;
    long OpinionAttitude;
    bool courting;
    long FrameGotMarriedToThePlayer;
    SGiftEntry GiftsReceived[8];
    int  gift_count;
    long FrameLastReceivedNiceGift;
    bool ReceivedWeddingRing;

    bool IsCourtingHero() const { return courting; }
    bool IsInLoveWithHero() const { return IsCourtingHero() && OpinionAttitude == ATTITUDE_LOVE; }
    SGiftEntry* Find(long def)
    {
        for (int i = 0; i < gift_count; ++i)
            if (GiftsReceived[i].def == def)
                return &GiftsReceived[i];
        return 0;
    }
    bool ReceivedNiceGiftIsOpinionReactionHappening(long object_def_index)
    {
        bool is_wedding_ring = object_def_index >= 0 && object_def_index < 8 && g_defs[object_def_index].exists
                               && g_defs[object_def_index].has_gift_sub_def && g_defs[object_def_index].is_wedding_ring;
        bool result = true;
        long deed = DEED_NONE;
        if (is_wedding_ring && FrameGotMarriedToThePlayer > g_frame) {
            if (!IsInLoveWithHero()) {
                deed = DEED_HIGH_PRIORITY_INCOMPREHENSION;
                result = false;
            } else {
                ReceivedWeddingRing = true;
                deed = DEED_RECEIVE_WEDDING_RING;
            }
        } else {
            SGiftEntry* it = Find(object_def_index);
            if (it == 0) {
                GiftsReceived[gift_count].def = object_def_index;
                GiftsReceived[gift_count].frame = g_frame;
                ++gift_count;
            } else {
                long frame = g_frame;
                if (frame - it->frame > SecondsBetweenReceivingGiftOpinionReactionCanHappen * g_constant_fps) {
                    it->frame = g_frame;
                } else {
                    deed = DEED_WIFE_GIFT_RECEIVE_ALREADY_GOT;
                    result = false;
                }
            }
            if (result)
                FrameLastReceivedNiceGift = g_frame;
        }
        if (deed != DEED_NONE) {
            SHero* hero = g_hero;
            if (hero && !hero->dead) {
                if (hero->has_deed_log) {
                    ++hero->log.posted_count;
                    hero->log.last_deed = deed;
                    hero->log.last_target = &ParentThing;
                    hero->log.last_victim = &ParentThing;
                    hero->log.last_post_type = 1;
                }
            }
        }
        return result;
    }
};

static int g_failures = 0;
static void Check(bool ok, const char* what)
{
    if (!ok) {
        std::printf("  failed: %s\n", what);
        ++g_failures;
    }
}

int main()
{
    g_defs[1].exists = true; g_defs[1].has_gift_sub_def = true; g_defs[1].is_wedding_ring = true;    // the wedding ring
    g_defs[2].exists = true; g_defs[2].has_gift_sub_def = true; g_defs[2].is_wedding_ring = false;   // a nice gift
    g_defs[3].exists = true; g_defs[3].has_gift_sub_def = false; g_defs[3].is_wedding_ring = true;   // no gift sub-def: not a ring

    SHero hero = { false, true, { 0, 0, 0, 0, 0 } };
    g_hero = &hero;
    SWife wife = { 0 };
    wife.SecondsBetweenReceivingGiftOpinionReactionCanHappen = 10;   // 200 frames
    wife.FrameGotMarriedToThePlayer = 0x7fffffff;                    // not married yet
    g_frame = 1000;

    // 1. ring, not married, not in love: refused, incomprehension deed, no map entry
    wife.courting = true; wife.OpinionAttitude = 5;
    Check(!wife.ReceivedNiceGiftIsOpinionReactionHappening(1), "ring refused when not in love");
    Check(hero.log.posted_count == 1 && hero.log.last_deed == DEED_HIGH_PRIORITY_INCOMPREHENSION, "incomprehension deed posted");
    Check(!wife.ReceivedWeddingRing && wife.gift_count == 0 && wife.FrameLastReceivedNiceGift == 0, "refused ring leaves her state");

    // 2. attitude love but not courting: still refused
    wife.courting = false; wife.OpinionAttitude = ATTITUDE_LOVE;
    Check(!wife.ReceivedNiceGiftIsOpinionReactionHappening(1), "ring refused when not courting");
    Check(hero.log.posted_count == 2 && !wife.ReceivedWeddingRing, "second refusal posted");

    // 3. ring, not married, in love: accepted, ring deed, target and victim are the wife
    wife.courting = true;
    Check(wife.ReceivedNiceGiftIsOpinionReactionHappening(1), "ring accepted when in love");
    Check(wife.ReceivedWeddingRing, "ReceivedWeddingRing set");
    Check(hero.log.posted_count == 3 && hero.log.last_deed == DEED_RECEIVE_WEDDING_RING, "wedding ring deed posted");
    Check(hero.log.last_target == &wife.ParentThing && hero.log.last_victim == &wife.ParentThing && hero.log.last_post_type == 1,
          "deed target, victim and post type");
    Check(wife.gift_count == 0 && wife.FrameLastReceivedNiceGift == 0, "ring is not recorded as a gift");

    // 4. first nice gift: recorded with the frame, no deed
    Check(wife.ReceivedNiceGiftIsOpinionReactionHappening(2), "first gift reacts");
    Check(wife.gift_count == 1 && wife.GiftsReceived[0].def == 2 && wife.GiftsReceived[0].frame == 1000, "gift recorded");
    Check(wife.FrameLastReceivedNiceGift == 1000 && hero.log.posted_count == 3, "frame remembered, nothing posted");

    // 5. same gift exactly at the interval (200 frames, not greater): already-got deed, nothing changes
    g_frame = 1200;
    Check(!wife.ReceivedNiceGiftIsOpinionReactionHappening(2), "repeat gift inside the interval does not react");
    Check(hero.log.posted_count == 4 && hero.log.last_deed == DEED_WIFE_GIFT_RECEIVE_ALREADY_GOT, "already-got deed posted");
    Check(wife.GiftsReceived[0].frame == 1000 && wife.FrameLastReceivedNiceGift == 1000 && wife.gift_count == 1, "repeat leaves the record");

    // 6. same gift past the interval: reacts, record and last frame updated, no deed
    g_frame = 1201;
    Check(wife.ReceivedNiceGiftIsOpinionReactionHappening(2), "repeat gift past the interval reacts");
    Check(wife.GiftsReceived[0].frame == 1201 && wife.FrameLastReceivedNiceGift == 1201 && hero.log.posted_count == 4, "record refreshed");

    // 7. a ring once married (frame not in the future) is an ordinary gift
    wife.FrameGotMarriedToThePlayer = 1201;
    wife.ReceivedWeddingRing = false;
    Check(wife.ReceivedNiceGiftIsOpinionReactionHappening(1), "ring after the wedding is a plain gift");
    Check(wife.gift_count == 2 && wife.GiftsReceived[1].def == 1 && !wife.ReceivedWeddingRing && hero.log.posted_count == 4, "ring recorded as gift");

    // 8. a def without a gift sub-def is never a ring; an unknown def (-1, as the give-money action passes) is a plain gift too
    wife.FrameGotMarriedToThePlayer = 0x7fffffff;
    Check(wife.ReceivedNiceGiftIsOpinionReactionHappening(3), "def without gift sub-def is a plain gift");
    Check(wife.ReceivedNiceGiftIsOpinionReactionHappening(-1), "unknown def reacts the first time");
    Check(!wife.ReceivedNiceGiftIsOpinionReactionHappening(-1), "unknown def repeats inside the interval");
    Check(hero.log.posted_count == 5 && hero.log.last_deed == DEED_WIFE_GIFT_RECEIVE_ALREADY_GOT, "repeat of -1 posts already-got");

    // 9. a dead hero, a hero without a deed log and no hero: same result, nothing posted
    hero.dead = true;
    Check(!wife.ReceivedNiceGiftIsOpinionReactionHappening(-1) && hero.log.posted_count == 5, "dead hero gets no deed");
    hero.dead = false; hero.has_deed_log = false;
    Check(!wife.ReceivedNiceGiftIsOpinionReactionHappening(-1) && hero.log.posted_count == 5, "hero without a deed log gets no deed");
    g_hero = 0;
    Check(!wife.ReceivedNiceGiftIsOpinionReactionHappening(-1), "no hero: result unchanged");

    if (g_failures != 0) {
        std::printf("006d0630_TEST FAIL\n");
        return 1;
    }
    std::printf("006d0630_TEST PASS\n");
    return 0;
}
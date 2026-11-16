// Behaviour test for 0x0090D030 CAIStateGroup_MinionGossip::Update: a model of the row's control flow over a fake world, with a
// trace of the actions it performs. Checked: answer a gossip request (originator dead / missing / without a gossip component ->
// Terminate; else take it as gossipee, remove the request, I speak first, the partner listens) or post one and wait up to
// fps * 4 frames for a partner (DoNothing TERMINATE -> Terminate, timeout -> Terminate); greet only as initiator or on a roll < 20
// of 100 (look-at failure / greeting failure -> Terminate), near greeting (2 choices) when closer than 5 units, else far (4);
// the interaction table indexed this_sex * 3 + their_sex (a child counts as sex 0), the 1-in-7 money exchange between two adults,
// the 1-in-2 veto for the same home; walk until not IN_PROGRESS (anything but FINISHED -> Terminate); interact (initiator flag
// passed) only with an interaction; then the conversation loop: non-male listens (into / loop until my turn / out of), male looks
// at the partner until his turn; an adult facing a child crouches (into, rand(4)+3 loops, hand over the turn, out of), else
// talks rand(4)+3 times (rare 1-in-11 pick of the ten talk anims, never the same twice in a row; else 1-in-2 the rare
// CONV_GENERAL, else ST_IDLE; a missing anim does nothing), hands over the turn; the loop repeats while the roll is >= 20; then
// Terminate.
#include <cstdio>
#include <cstring>
#include <string>

enum { IN_PROGRESS = 4, FINISHED = 2, TERMINATE = 1, FAILED = 3 };

struct World {
    // inputs
    bool HasRequest, OriginatorAlive, OriginatorHasGossip;
    int PartnerArrivesAfter;            // frames until the posted request is answered (-1 never)
    long FPS;
    float Distance;
    long ThisSex, TheirSex;             // 0 = child, 1 = male, 2 = female
    bool SameHome, AnimExists;
    int WalkSteps;                      // IN_PROGRESS returns before the result
    int WalkResult, LookAtResult, AnimResult, ListenLoops;
    unsigned long Rolls[64]; int NRoll;
    // outputs
    std::string Trace;
    bool MyTurn, OtherTurn, Initiator;
    const char* Interaction;
};

static unsigned long Roll(World& w, unsigned long range) { return range ? w.Rolls[w.NRoll++] % range : 0; }

static const char* Near[2] = { "ARMS_WIDE_GREET", "HAND_UP_GREET" };
static const char* Far[4] = { "ARMS_WIDE_GREET", "WAVING_AT_DISTANCE", "CALLING_OVER_FOR_FIGHT", "SHOUTING_WITH_HANDS_CUPPED" };
static const char* Rare[1] = { "CONV_GENERAL" };
static const char* Talk[10] = { "T0", "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8", "T9" };
static const char* Interactions[9] = { 0, "RUFFLE_HAIR", "HUG_CHILD", "RUFFLE_HAIR", "SHAKE_HANDS", "KISS", "HUG_CHILD", "HUG", "HUG" };

static int Anim(World& w, const char* name) { w.Trace += name; w.Trace += ' '; return w.AnimResult; }
static void Terminate(World& w) { w.Trace += "TERMINATE"; }

static void Update(World& w)
{
    bool initiator;
    if (w.HasRequest) {
        if (!w.OriginatorAlive) { Terminate(w); return; }
        if (!w.OriginatorHasGossip) { Terminate(w); return; }
        w.Trace += "REMOVE ";
        w.MyTurn = true; w.OtherTurn = false;
        initiator = false;
    } else {
        w.Trace += "POST ";
        long frame = 0, timeout = frame + w.FPS * 4;
        int waited = 0;
        while (w.PartnerArrivesAfter < 0 || waited < w.PartnerArrivesAfter) {
            waited++; frame++;
            if (w.PartnerArrivesAfter == -2) { Terminate(w); return; }   // DoNothing returned TERMINATE
            if (frame > timeout) { Terminate(w); return; }
        }
        w.MyTurn = false; w.OtherTurn = true;
        initiator = true;
    }
    w.Initiator = initiator;
    if (initiator || Roll(w, 100) < 20) {
        if (w.LookAtResult != FINISHED) { Terminate(w); return; }
        const char* greeting = w.Distance < 5.0f ? Near[Roll(w, 2)] : Far[Roll(w, 4)];
        if (Anim(w, greeting) != FINISHED) { Terminate(w); return; }
    }
    long this_sex = w.ThisSex, their_sex = w.TheirSex;
    const char* interaction = Interactions[this_sex * 3 + their_sex];
    if (this_sex != 0 && their_sex != 0 && Roll(w, 7) == 0) interaction = "EXCHANGE_MONEY";
    if (w.SameHome && Roll(w, 2) == 0) interaction = 0;
    w.Interaction = interaction;
    int result, steps = w.WalkSteps;
    do result = steps-- > 0 ? IN_PROGRESS : w.WalkResult; while (result == IN_PROGRESS);
    if (result != FINISHED) { Terminate(w); return; }
    if (interaction) { w.Trace += "INTERACT:"; w.Trace += interaction; w.Trace += initiator ? "/I " : "/R "; }
    do {
        if (this_sex != 1) {
            if (w.LookAtResult == TERMINATE) break;
            if (Anim(w, "INTO") != FINISHED) break;
            int loops = w.ListenLoops;
            while (!w.MyTurn) { if (Anim(w, "LOOP") != FINISHED) break; if (--loops <= 0) w.MyTurn = true; }
            if (Anim(w, "OUTOF") != FINISHED) break;
        } else {
            if (!w.MyTurn) w.MyTurn = true;
        }
        if (their_sex == 0 && this_sex != 0) {
            if (Anim(w, "CROUCH_INTO") != FINISHED) break;
            long count = (long)Roll(w, 4) + 3;
            for (long i = 0; i < count; i++) if (Anim(w, "CROUCH_LOOP") != FINISHED) break;
            w.MyTurn = false; w.OtherTurn = true;
            if (Anim(w, "CROUCH_OUTOF") != FINISHED) break;
        } else {
            w.Trace += "MODE+ ";
            long count = (long)Roll(w, 4) + 3;
            const char* last = 0;
            for (long i = 0; i < count; i++) {
                const char* anim;
                if (Roll(w, 11) == 0) { do anim = Talk[Roll(w, 10)]; while (anim == last); }
                else if (Roll(w, 2) == 0) anim = Rare[Roll(w, 1)];
                else anim = "ST_IDLE";
                last = anim;
                if (w.AnimExists) { if (Anim(w, anim) != FINISHED) break; }
                else w.Trace += "NOTHING ";
            }
            w.MyTurn = false; w.OtherTurn = true;
        }
        w.Trace += "MODE- ";
        w.MyTurn = true;                  // the partner hands the turn back
    } while (Roll(w, 100) >= 20);
    Terminate(w);
}

static World Make()
{
    World w; std::memset(&w.HasRequest, 0, (char*)&w.Trace - (char*)&w.HasRequest);
    w.FPS = 30; w.Distance = 3.0f; w.ThisSex = 1; w.TheirSex = 2; w.AnimExists = true;
    w.WalkResult = FINISHED; w.LookAtResult = FINISHED; w.AnimResult = FINISHED; w.ListenLoops = 1;
    w.MyTurn = false; w.OtherTurn = false; w.Initiator = false; w.Interaction = 0;
    return w;
}

int main()
{
    int ok = 1;
    { World w = Make(); w.HasRequest = true; w.OriginatorAlive = false; Update(w); ok &= w.Trace == "TERMINATE"; }
    { World w = Make(); w.HasRequest = true; w.OriginatorAlive = true; Update(w); ok &= w.Trace == "TERMINATE"; }
    { World w = Make(); w.PartnerArrivesAfter = 121; Update(w); ok &= w.Trace == "POST TERMINATE"; }        // fps*4 = 120 frames
    { World w = Make(); w.PartnerArrivesAfter = -2; Update(w); ok &= w.Trace == "POST TERMINATE"; }
    // answered request, no greeting (roll 20), male->female shake? index 1*3+2 = 5 KISS, no money (roll 1), walk, interact, talk
    { World w = Make(); w.HasRequest = true; w.OriginatorAlive = w.OriginatorHasGossip = true;
      unsigned long r[] = { 20, 1, 0 /*count 3*/, 1, 0, 0 /*rare*/, 1, 1 /*idle*/, 0, 5 /*T5*/, 19 /*end*/ };
      std::memcpy(w.Rolls, r, sizeof r); Update(w);
      ok &= !w.Initiator && w.Trace == "REMOVE INTERACT:KISS/R MODE+ CONV_GENERAL ST_IDLE T5 MODE- TERMINATE"; }
    // posted request answered: initiator greets (near, roll 1 -> HAND_UP), money exchange (roll 7 % 7 == 0)
    { World w = Make(); w.PartnerArrivesAfter = 5; w.Distance = 4.0f;
      unsigned long r[] = { 1, 7, 0, 1, 1, 1, 1, 1, 1, 19 };
      std::memcpy(w.Rolls, r, sizeof r); Update(w);
      ok &= w.Initiator && w.Trace == "POST HAND_UP_GREET INTERACT:EXCHANGE_MONEY/I MODE+ ST_IDLE ST_IDLE ST_IDLE MODE- TERMINATE"; }
    // far greeting, female adult with a child: crouch branch, same-home veto, rare pick never repeats
    { World w = Make(); w.PartnerArrivesAfter = 0; w.Distance = 6.0f; w.ThisSex = 2; w.TheirSex = 0; w.SameHome = true;
      unsigned long r[] = { 2, 0, 0 /*count 3*/, 19 };
      std::memcpy(w.Rolls, r, sizeof r); Update(w);
      ok &= w.Interaction == 0 && w.Trace == "POST CALLING_OVER_FOR_FIGHT INTO LOOP OUTOF CROUCH_INTO CROUCH_LOOP CROUCH_LOOP CROUCH_LOOP CROUCH_OUTOF MODE- TERMINATE"; }
    { World w = Make(); w.HasRequest = true; w.OriginatorAlive = w.OriginatorHasGossip = true;
      unsigned long r[] = { 20, 1, 0 /*count 3*/, 0, 4, 0, 4, 6, 1, 1, 19 };
      std::memcpy(w.Rolls, r, sizeof r); Update(w);
      ok &= w.Trace == "REMOVE INTERACT:KISS/R MODE+ T4 T6 ST_IDLE MODE- TERMINATE"; }
    // walk failure terminates; loop repeats on a roll >= 20; missing anim does nothing
    { World w = Make(); w.HasRequest = true; w.OriginatorAlive = w.OriginatorHasGossip = true; w.WalkSteps = 3; w.WalkResult = FAILED;
      unsigned long r[] = { 20, 1 }; std::memcpy(w.Rolls, r, sizeof r); Update(w); ok &= w.Trace == "REMOVE TERMINATE"; }
    { World w = Make(); w.HasRequest = true; w.OriginatorAlive = w.OriginatorHasGossip = true; w.AnimExists = false;
      unsigned long r[] = { 20, 1, 0, 1, 1, 1, 1, 1, 1, 20, 0, 1, 1, 1, 1, 1, 1, 0 };
      std::memcpy(w.Rolls, r, sizeof r); Update(w);
      ok &= w.Trace == "REMOVE INTERACT:KISS/R MODE+ NOTHING NOTHING NOTHING MODE- MODE+ NOTHING NOTHING NOTHING MODE- TERMINATE"; }
    // two children: no interaction (table entry 0, no money roll), listen, then talk (not crouch)
    { World w = Make(); w.HasRequest = true; w.OriginatorAlive = w.OriginatorHasGossip = true; w.ThisSex = 0; w.TheirSex = 0;
      w.MyTurn = true;
      unsigned long r[] = { 20, 0, 1, 1, 1, 1, 1, 1, 19 };
      std::memcpy(w.Rolls, r, sizeof r); Update(w);
      ok &= w.Interaction == 0 && w.Trace == "REMOVE INTO OUTOF MODE+ ST_IDLE ST_IDLE ST_IDLE MODE- TERMINATE"; }
    if (ok) std::printf("0090d030_TEST PASS\n"); else std::printf("0090d030_TEST FAIL\n");
    return ok ? 0 : 1;
}

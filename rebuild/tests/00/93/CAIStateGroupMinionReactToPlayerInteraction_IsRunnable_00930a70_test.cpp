// Behaviour test for CAIStateGroup_MinionReactToPlayerInteraction::IsRunnable (0x00930A70): model copy, scripted stubs.
#include <cstdio>

class COpinionDeed { public: int Type; int Handle; };
struct CVillage {
    int GuardCount;
    int handledHandle;
    bool IsDeedHandled(int handle) { return handle == handledHandle; }
};
class CTCVillageMember { public: CVillage* village; CVillage* GetPVillage() const { return village; } };
class CTCCreatureOpinionOfHero { public:
    COpinionDeed* deed;
    int outHandle;
    COpinionDeed* FindMostRecentDeed(int* handle, bool) { *handle = outHandle; return deed; }
};
class CTCWife { public: bool married; bool IsMarriedToThePlayer() const { return married; } };
struct CTCInteractionState { bool Active; };
class CThing { public:
    unsigned char m_flags91;
    CTCVillageMember* member;
    CTCCreatureOpinionOfHero* opinion;
    CTCWife* wife;
    CTCInteractionState* state;
    bool HasWife() const { return wife != 0; }
    bool GetInterface(CTCVillageMember** out) const { if (!member) return false; *out = member; return true; }
    bool GetInterface(CTCCreatureOpinionOfHero** out) const { if (!opinion) return false; *out = opinion; return true; }
    bool GetInterface(CTCWife** out) const { if (!wife) return false; *out = wife; return true; }
    bool GetInterface(CTCInteractionState** out) const { if (!state) return false; *out = state; return true; }
};
class CMessageEvent { public: int Type; };
static unsigned int g_frame;
static int g_failed, g_tests, g_far, g_near, g_testResult;
static CThing* g_player;
static CMessageEvent* g_event;
struct CAIStateGroup_MinionReactToPlayerInteraction {
    CThing* creature;
    unsigned int NextReactionFrame[5];
    int ReactionType;
    CThing* GetCreature() const { return creature; }
    CMessageEvent* FindPlayerInteractionEvent() { return g_event; }
    bool IsTooFarFromThePlayer(CThing*) { return g_far != 0; }
    bool IsTooNearToThePlayer(CThing*) { return g_near != 0; }
    bool TestOrDoInteraction(bool test, CThing* player) { ++g_tests; return test && player == g_player && g_testResult; }
    void InteractionFailed() { ++g_failed; }
    bool IsRunnable();
};

bool CAIStateGroup_MinionReactToPlayerInteraction::IsRunnable()
{
    CThing* player = g_player;
    if (!player || (player->m_flags91 & 1))
        return false;
    CMessageEvent* event = FindPlayerInteractionEvent();
    if (!event)
        return false;
    CTCVillageMember* member;
    if (GetCreature()->GetInterface(&member)) {
        CVillage* village = member->GetPVillage();
        CTCCreatureOpinionOfHero* opinion;
        if (village && GetCreature()->GetInterface(&opinion)) {
            int handle;
            COpinionDeed* deed = opinion->FindMostRecentDeed(&handle, false);
            if (deed) {
                int deedHandle = deed->Type == 0x35 ? deed->Handle : handle;
                if (village->GuardCount > 0 && !village->IsDeedHandled(deedHandle)) {
                    CTCWife* wife;
                    if (!GetCreature()->HasWife() || !GetCreature()->GetInterface(&wife) || !wife->IsMarriedToThePlayer()) {
                        InteractionFailed();
                        return false;
                    }
                }
            }
        }
    }
    if (event->Type == 5) {
        if (IsTooFarFromThePlayer(player) || IsTooNearToThePlayer(player)) {
            ReactionType = 5;
            return true;
        }
    } else {
        ReactionType = 5;
        for (int i = 0; i < 5; ++i) {
            if (g_frame > NextReactionFrame[i]) {
                CTCInteractionState* state;
                if (!GetCreature()->GetInterface(&state) || !state->Active || i == 2) {
                    ReactionType = i;
                    break;
                }
            }
        }
        if (ReactionType != 5 && TestOrDoInteraction(true, player))
            return true;
    }
    InteractionFailed();
    return false;
}

int main()
{
    int ok = 1;
    CThing hero = { 0, 0, 0, 0, 0 };
    CVillage village = { 2, 7 };
    CTCVillageMember member = { &village };
    COpinionDeed deed = { 0x35, 9 };
    CTCCreatureOpinionOfHero opinion = { &deed, 7 };
    CTCWife wife = { false };
    CTCInteractionState state = { true };
    CThing me = { 0, &member, &opinion, 0, &state };
    CMessageEvent event = { 5 };
    CAIStateGroup_MinionReactToPlayerInteraction s;
    s.creature = &me; s.ReactionType = 0;
    for (int i = 0; i < 5; ++i) s.NextReactionFrame[i] = 100;
    // no player / dead player / no event
    g_player = 0; g_event = &event;
    if (s.IsRunnable() || g_failed) ok = 0;
    g_player = &hero; hero.m_flags91 = 1;
    if (s.IsRunnable() || g_failed) ok = 0;
    hero.m_flags91 = 0; g_event = 0;
    if (s.IsRunnable() || g_failed) ok = 0;
    g_event = &event;
    // an unhandled crime deed (handle 9) in a guarded village, not married: fails
    if (s.IsRunnable() || g_failed != 1) ok = 0;
    // married wife: passes the crime gate; type 5 and too far -> reaction 5
    me.wife = &wife; wife.married = true; g_far = 1;
    if (!s.IsRunnable() || s.ReactionType != 5) ok = 0;
    // non-0x35 deed uses the out handle 7, which is handled -> no gate; type 5 neither far nor near -> fails
    deed.Type = 3; me.wife = 0; g_far = 0; g_failed = 0;
    if (s.IsRunnable() || g_failed != 1) ok = 0;
    g_near = 1;
    if (!s.IsRunnable()) ok = 0;
    // other event type: no slot is due -> reaction stays 5, fails without testing
    event.Type = 1; g_failed = 0; g_frame = 50; g_tests = 0;
    if (s.IsRunnable() || s.ReactionType != 5 || g_tests || g_failed != 1) ok = 0;
    // all due, state active: slot 2 is chosen; the test decides
    g_frame = 200; g_testResult = 1;
    if (!s.IsRunnable() || s.ReactionType != 2 || g_tests != 1) ok = 0;
    // state inactive: slot 0
    state.Active = false;
    if (!s.IsRunnable() || s.ReactionType != 0) ok = 0;
    // test fails -> InteractionFailed
    g_testResult = 0; g_failed = 0;
    if (s.IsRunnable() || g_failed != 1) ok = 0;
    if (!ok) { std::printf("00930a70_TEST FAIL\n"); return 1; }
    std::printf("00930a70_TEST PASS\n");
    return 0;
}
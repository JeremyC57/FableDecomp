// Behaviour test for 0x0094C050 CAIStateGroup_SellToBuyer::Update() -- a host model of the row's decisions (same control flow as
// src/0094c050.cpp) over a scripted shopkeeper / buyer / shop, checked against the outcomes retail's bytes produce: the buyer choice,
// the speaking wait, the walk back to the stall (with or without the follow-me speech), the 20-second wait for the buyer, the sale
// (customer, reply, accept message, thanks, leaving the interaction) and every refusal / termination path.
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
static float DistSq(const Vec& a, const Vec& b)
{
    float x = a.X - b.X, y = a.Y - b.Y, z = a.Z - b.Z;
    return x * x + y * y + z * z;
}

struct Script {
    int participants[3];          // participant ids (the shopkeeper is id 1)
    int n_participants;
    int speaking_frames;          // IsSpeaking true for this many polls
    int donothing_stop_at;        // AIAction_DoNothing returns 1 (stopping) at this call (-1: never)
    Vec creature_pos, shop_pos, buyer_pos;
    long min_dist, max_dist;      // keeper's GetMinDistFromStall / GetMaxDistFromCustomer
    float radius;                 // creature physics radius
    int move_results[6];          // AIAction_MoveToPosProximity results (4 = running)
    int stay_fail_at;             // StayInShop false at this call (-1: never)
    int buyer_dies_at;            // IS_THING_ALIVE(PBuyer) false from this check on (-1: never)
    int frames_per_wait;          // frames that pass per DoNothing call in the wait loop
    int buyer_arrives_after;      // the buyer is within max_dist after this many wait-loop DoNothing calls
    int reply_results[4];         // speech results in the reply loop
    int thanks_results[4];        // speech results in the thanking loop
};

struct Log {
    int buyer, terminated, refused, follow_speech, moves, stop_speaking, customers_added, replies, messages, message_value, thanks,
        left_interaction, order[8], n_order;
};

class SellModel {
public:
    SellModel(const Script& s, Log& l) : S(s), L(l), Frame(100), Speak(0), DoNothingCalls(0), Moves(0), StayCalls(0), AliveCalls(0),
        WaitCalls(0), Replies(0), Thanks(0), BuyerPos(s.buyer_pos) { std::memset(&L, 0, sizeof(L)); }
    void Update();
private:
    void Note(int what) { if (L.n_order < 8) L.order[L.n_order++] = what; }
    void Terminate() { ++L.terminated; Note(1); }
    void RefuseCustom() { ++L.refused; Note(2); }
    bool IsSpeaking() { return Speak++ < S.speaking_frames; }
    int DoNothing() { ++DoNothingCalls; Frame += S.frames_per_wait; return DoNothingCalls == S.donothing_stop_at ? 1 : 4; }
    bool StayInShop() { ++StayCalls; return StayCalls != S.stay_fail_at; }
    bool IsBuyerAlive() { ++AliveCalls; return L.buyer != 0 && (S.buyer_dies_at < 0 || AliveCalls < S.buyer_dies_at); }
    const Script& S;
    Log& L;
    long Frame;
    int Speak, DoNothingCalls, Moves, StayCalls, AliveCalls, WaitCalls, Replies, Thanks;
    Vec BuyerPos;
};

void SellModel::Update()
{
    for (int i = 0; i < S.n_participants; ++i)
        if (S.participants[i] != 1)
            L.buyer = S.participants[i];
    while (IsSpeaking()) {
        if (DoNothing() == 1) { RefuseCustom(); Terminate(); return; }
    }
    float distance_squared = DistSq(S.shop_pos, S.creature_pos);
    if (distance_squared > S.min_dist * S.min_dist) {
        if (distance_squared > S.min_dist * S.min_dist + S.radius + 0.5f)
            ++L.follow_speech;
        int result;
        do {
            if (!StayInShop() || !IsBuyerAlive()) { Terminate(); RefuseCustom(); return; }
            result = S.move_results[Moves++];
            ++L.moves;
        } while (result == 4);
        ++L.stop_speaking;
        if (result != 2) { Terminate(); RefuseCustom(); return; }
    }
    long end_frame = Frame + 20 * 30;                // 20 * CMainGameComponent::GetConstantFPS() (30 here)
    bool waiting = true;
    while (waiting && end_frame >= Frame) {
        if (!StayInShop() || !IsBuyerAlive()) { RefuseCustom(); Terminate(); return; }
        bool running = DoNothing() == 4;
        if (++WaitCalls > S.buyer_arrives_after) BuyerPos = S.creature_pos;
        waiting = running && DistSq(BuyerPos, S.creature_pos) > S.max_dist * S.max_dist;
    }
    if (!waiting) {
        ++L.customers_added;
        int result;
        do {
            if (!StayInShop() || !IsBuyerAlive()) { RefuseCustom(); Terminate(); return; }
            result = S.reply_results[Replies++];
            ++L.replies;
        } while (result == 4);
        ++L.messages; L.message_value = 1;
        do {
            if (!StayInShop() || !IsBuyerAlive()) { Terminate(); return; }
            result = S.thanks_results[Thanks++];
            ++L.thanks;
        } while (result == 4);
        ++L.left_interaction;
        Terminate();
        return;
    }
    RefuseCustom();
    Terminate();
}

static int failures = 0;
static void Check(bool ok, const char* what)
{
    if (!ok) { std::printf("check failed: %s\n", what); ++failures; }
}

static Script Base()
{
    Script s;
    std::memset(&s, 0, sizeof(s));
    s.participants[0] = 1; s.participants[1] = 7; s.n_participants = 2;
    s.donothing_stop_at = -1;
    s.creature_pos.X = 0; s.shop_pos.X = 1; s.buyer_pos.X = 30;
    s.min_dist = 2; s.max_dist = 10; s.radius = 0.5f;
    s.move_results[0] = 2;
    s.stay_fail_at = -1; s.buyer_dies_at = -1;
    s.frames_per_wait = 1; s.buyer_arrives_after = 3;
    s.reply_results[0] = 4; s.reply_results[1] = 2;
    s.thanks_results[0] = 2;
    return s;
}

int main()
{
    Log l;
    // 1: near the stall, the buyer arrives: the full sale, no walk, accept message value 1, then leave and terminate.
    { Script s = Base(); SellModel m(s, l); m.Update();
      Check(l.buyer == 7 && l.moves == 0 && l.follow_speech == 0, "1 buyer / no walk");
      Check(l.customers_added == 1 && l.replies == 2 && l.messages == 1 && l.message_value == 1 && l.thanks == 1, "1 sale");
      Check(l.left_interaction == 1 && l.terminated == 1 && l.refused == 0, "1 end"); }
    // 2: the buyer keeps speaking until DoNothing reports stopping: refuse then terminate.
    { Script s = Base(); s.speaking_frames = 10; s.donothing_stop_at = 2; SellModel m(s, l); m.Update();
      Check(l.refused == 1 && l.terminated == 1 && l.order[0] == 2 && l.order[1] == 1 && l.customers_added == 0, "2 refuse"); }
    // 3: far from the stall (beyond the radius margin): follow speech, walk until done, then the sale.
    { Script s = Base(); s.shop_pos.X = 5; s.move_results[0] = 4; s.move_results[1] = 4; s.move_results[2] = 2;
      SellModel m(s, l); m.Update();
      Check(l.follow_speech == 1 && l.moves == 3 && l.stop_speaking == 1 && l.customers_added == 1, "3 walk + sale"); }
    // 4: just past the minimum distance but inside the radius margin: no speech, still walks.
    { Script s = Base(); s.shop_pos.X = 2.1f; SellModel m(s, l); m.Update();
      Check(l.follow_speech == 0 && l.moves == 1, "4 walk without speech"); }
    // 5: the walk fails (result 3): terminate then refuse (retail's order at 0x0094C383).
    { Script s = Base(); s.shop_pos.X = 5; s.move_results[0] = 3; SellModel m(s, l); m.Update();
      Check(l.terminated == 1 && l.refused == 1 && l.order[0] == 1 && l.order[1] == 2 && l.stop_speaking == 1, "5 walk fails"); }
    // 6: the shop closes during the walk: terminate then refuse, no stop-speaking.
    { Script s = Base(); s.shop_pos.X = 5; s.move_results[0] = 4; s.move_results[1] = 2; s.stay_fail_at = 2;
      SellModel m(s, l); m.Update();
      Check(l.order[0] == 1 && l.order[1] == 2 && l.stop_speaking == 0 && l.moves == 1, "6 shop closes while walking"); }
    // 7: the buyer never comes: the 20-second wait times out, refuse then terminate.
    { Script s = Base(); s.buyer_arrives_after = 1000; s.frames_per_wait = 50; SellModel m(s, l); m.Update();
      Check(l.refused == 1 && l.terminated == 1 && l.order[0] == 2 && l.customers_added == 0, "7 time out"); }
    // 8: DoNothing stops during the wait: the buyer counts as arrived (the sale goes on).
    { Script s = Base(); s.buyer_arrives_after = 1000; s.donothing_stop_at = 1; SellModel m(s, l); m.Update();
      Check(l.customers_added == 1 && l.refused == 0, "8 stopping ends the wait"); }
    // 9: the buyer dies during the reply: refuse then terminate.
    { Script s = Base(); s.reply_results[0] = 4; s.reply_results[1] = 4; s.buyer_dies_at = 6; SellModel m(s, l); m.Update();
      Check(l.customers_added == 1 && l.refused == 1 && l.order[0] == 2 && l.order[1] == 1 && l.messages == 0, "9 buyer dies"); }
    // 10: the shop closes during the thanks: terminate only, no leaving.
    { Script s = Base(); s.stay_fail_at = 7; SellModel m(s, l); m.Update();
      Check(l.messages == 1 && l.thanks == 0 && l.terminated == 1 && l.refused == 0 && l.left_interaction == 0, "10 closes at thanks"); }
    if (failures) { std::printf("0094c050_TEST FAIL\n"); return 1; }
    std::printf("0094c050_TEST PASS\n");
    return 0;
}
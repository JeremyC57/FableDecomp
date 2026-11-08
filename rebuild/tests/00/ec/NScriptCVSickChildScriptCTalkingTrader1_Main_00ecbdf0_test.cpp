// Behaviour test for 0x00ECBDF0 NScript::CV_SickChildScript::CTalkingTrader1::Main: a portable model of trader A's thread --
// removed once the fishing-spot mushroom is got; otherwise every frame: when the chat timer is idle and both the hero (20 m) and
// trader B (10 m) are near, one of four chats (CHAT1..CHAT4, CHAT3 with a third line) then the next chat (wrapping after the
// fourth) and the timer set to 16; talked to: INTRO / REPEATA / REPEATB_10 in turn (the counter wraps), the chat timer pushed back
// by 5, and OTHERTRADERDEAD_10 instead when trader B is dead; hit by the hero while alive: killable from the fourth hit, and the
// ONHIT_<n> line (n from 10 in steps of 10, back to 10 when the text entry is missing).
#include <cstdio>
#include <string>
#include <vector>

struct World {
    int frame, terminateAt; bool mushroom, removed; int timer; bool otherAlive; int maxOnHit;
    long timesTalkedTo, timesHit; bool killable;
    std::vector<int> near, talk, hit;
    std::vector<std::string> log;
    World() : frame(0), terminateAt(0), mushroom(false), removed(false), timer(0), otherAlive(false), maxOnHit(0),
              timesTalkedTo(0), timesHit(0), killable(false) {}   // VC7.1 leaves World() scalars unset
    bool T() const { return frame >= terminateAt; }
    bool at(const std::vector<int>& v) const { return frame < (int)v.size() && v[frame] != 0; }
};

static void TraderMain(World& w)
{
    ++w.frame; if (w.T()) return;
    if (w.mushroom) { w.removed = true; }
    long chatNumber = 0, lineNumber = 10;
    static const char* chats[4] = { "CHAT1", "CHAT2", "CHAT3", "CHAT4" };
    while (!w.T()) {
        if (w.timer == 0 && w.at(w.near)) {
            w.log.push_back(chats[chatNumber]);
            if (chatNumber == 2) w.log.push_back("CHAT3_30");
            chatNumber = chatNumber < 3 ? chatNumber + 1 : 0;
            w.timer = 16;
        }
        if (w.at(w.talk)) {
            std::string s;
            switch (w.timesTalkedTo) {
            case 0: s = "INTRO"; break;
            case 1: s = "REPEATA"; break;
            case 2: s = "REPEATB_10"; w.timesTalkedTo = -1; break;
            }
            ++w.timesTalkedTo;
            w.timer += 5;
            w.log.push_back(w.otherAlive ? s : std::string("OTHERTRADERDEAD_10"));
        }
        if (w.at(w.hit)) {
            if (w.timesHit > 2) w.killable = true;
            if (lineNumber > w.maxOnHit) lineNumber = 10;
            char buf[32]; std::sprintf(buf, "ONHIT_%ld", lineNumber);
            lineNumber += 10;
            w.log.push_back(buf);
            ++w.timesHit;
        }
        if (w.timer > 0) --w.timer;
        ++w.frame;
    }
}

int main()
{
    World w;
    w.terminateAt = 40; w.otherAlive = true; w.maxOnHit = 20;
    w.near.assign(60, 0); w.talk.assign(60, 0); w.hit.assign(60, 0);
    w.near[2] = 1;                                 // CHAT1, timer 16
    w.talk[3] = w.talk[4] = w.talk[5] = w.talk[6] = 1;   // INTRO, REPEATA, REPEATB_10, INTRO
    w.hit[7] = w.hit[8] = w.hit[9] = w.hit[10] = 1;      // ONHIT_10, 20, 10 (missing 30), 20; killable on the 4th
    TraderMain(w);
    bool ok = !w.removed && w.log.size() == 9 && w.log[0] == "CHAT1" && w.log[1] == "INTRO" && w.log[3] == "REPEATB_10"
              && w.log[4] == "INTRO" && w.log[5] == "ONHIT_10" && w.log[6] == "ONHIT_20" && w.log[7] == "ONHIT_10"
              && w.log[8] == "ONHIT_20" && w.killable && w.timesHit == 4;
    World v;
    v.terminateAt = 10; v.otherAlive = false; v.maxOnHit = 30; v.mushroom = true;
    v.near.assign(20, 0); v.talk.assign(20, 0); v.hit.assign(20, 0);
    v.talk[2] = 1;
    TraderMain(v);
    ok = ok && v.removed && v.log.size() == 1 && v.log[0] == "OTHERTRADERDEAD_10" && v.timesTalkedTo == 1;
    if (!ok) { std::printf("00ecbdf0_TEST FAIL\n"); return 1; }
    std::printf("00ecbdf0_TEST PASS\n");
    return 0;
}
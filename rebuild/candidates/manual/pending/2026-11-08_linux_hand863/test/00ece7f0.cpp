// Behaviour test for 0x00ECE7F0 NScript::CV_SickChildScript::CWitch::Main: a portable model of the witch's thread -- scripted
// control taken, then until four ingredients are in: talked to, the CS_SICK_WITCH intro once (objective 02), else the mushroom
// question when the hero carries one (yes: take them all, NEED_MORE_CURE_10 while under four, else CS_SICK_POTION with the
// EndPotion, objective 03, information cleared and the loop left; no: NOT_GIVEN_MORE_MUSHROOMS_10) or WAITING_FOR_CURE; hit by the
// hero: ONHIT_10/20/30 in turn (the counter wraps after 30); afterwards GOT_CURE when talked to and the same hit lines, until the
// thread ends.
#include <cstdio>
#include <string>
#include <vector>

struct World {
    int frame, terminateAt, heroMushrooms;
    std::vector<int> talk, hit, answer;    // per frame: talked to / hit / question answer (1 yes, 0 no)
    std::vector<std::string> log;
    bool introDone; long got; bool infoCleared;
    bool T() const { return frame >= terminateAt; }
    void NSF() { ++frame; }
    bool Talk() const { return frame < (int)talk.size() && talk[frame] != 0; }
    bool Hit() const { return frame < (int)hit.size() && hit[frame] != 0; }
};

static void HitLine(World& w, long& timesHit)
{
    ++timesHit;
    std::string text;
    switch (timesHit) {
    case 1: text = "ONHIT_10"; break;
    case 2: text = "ONHIT_20"; break;
    case 3: text = "ONHIT_30"; timesHit = 0; break;
    default: text = "ONHIT_20"; timesHit = 0; break;
    }
    w.log.push_back(text);
}

static void WitchMain(World& w)
{
    w.NSF(); if (w.T()) return;
    long timesHit = 0;
    while (w.got < 4) {
        w.NSF(); if (w.T()) return;
        if (w.Talk()) {
            if (!w.introDone) { w.log.push_back("CS_SICK_WITCH"); w.log.push_back("OBJECTIVE_02"); w.introDone = true; }
            else if (w.heroMushrooms > 0 && w.got < 4) {
                w.log.push_back("QUESTION_GIVE_MUSHROOM");
                if (w.answer[w.frame] == 1) {
                    bool more = true;
                    while (more) {
                        if (w.heroMushrooms > 0) { w.NSF(); if (w.T()) return; --w.heroMushrooms; ++w.got; }
                        else more = false;
                    }
                    if (w.got < 4) w.log.push_back("NEED_MORE_CURE_10");
                    else { w.log.push_back("CS_SICK_POTION"); w.log.push_back("OBJECTIVE_03"); w.infoCleared = true; break; }
                } else w.log.push_back("NOT_GIVEN_MORE_MUSHROOMS_10");
            } else w.log.push_back("WAITING_FOR_CURE");
        }
        if (w.Hit()) HitLine(w, timesHit);
    }
    if (w.T()) return;
    for (; !w.T(); w.NSF()) {
        if (w.Talk()) w.log.push_back("GOT_CURE");
        if (w.Hit()) HitLine(w, timesHit);
    }
}

int main()
{
    World w = World();
    w.terminateAt = 30; w.heroMushrooms = 0; w.introDone = false; w.got = 0; w.infoCleared = false;
    w.talk.assign(40, 0); w.hit.assign(40, 0); w.answer.assign(40, 1);
    w.talk[2] = 1;                       // intro
    w.talk[3] = 1;                       // no mushrooms yet: waiting
    w.hit[4] = w.hit[5] = w.hit[6] = w.hit[7] = 1;   // 10, 20, 30, then wrapped to 10
    w.talk[8] = 1; w.answer[8] = 0;      // carries none -> waiting (mushrooms given below at frame 9)
    WitchMain(w);
    bool ok = w.introDone && w.got == 0 && w.log.size() >= 7 && w.log[0] == "CS_SICK_WITCH" && w.log[2] == "WAITING_FOR_CURE"
              && w.log[3] == "ONHIT_10" && w.log[4] == "ONHIT_20" && w.log[5] == "ONHIT_30" && w.log[6] == "ONHIT_10";

    World v = World();
    v.terminateAt = 40; v.heroMushrooms = 2; v.introDone = true; v.got = 0; v.infoCleared = false;
    v.talk.assign(50, 0); v.hit.assign(50, 0); v.answer.assign(50, 1);
    v.talk[2] = 1; v.answer[2] = 0;      // refuses
    v.talk[3] = 1;                       // gives two -> need more
    WitchMain(v);
    ok = ok && v.got == 2 && v.log.size() >= 4 && v.log[0] == "QUESTION_GIVE_MUSHROOM" && v.log[1] == "NOT_GIVEN_MORE_MUSHROOMS_10"
            && v.log[3] == "NEED_MORE_CURE_10" && !v.infoCleared;

    World u = World();
    u.terminateAt = 40; u.heroMushrooms = 4; u.introDone = true; u.got = 0; u.infoCleared = false;
    u.talk.assign(50, 0); u.hit.assign(50, 0); u.answer.assign(50, 1);
    u.talk[2] = 1;                       // gives four -> potion, leaves the loop
    u.talk[12] = 1; u.hit[13] = 1;       // afterwards: GOT_CURE, ONHIT_10
    WitchMain(u);
    ok = ok && u.got == 4 && u.infoCleared && u.log.size() == 5 && u.log[1] == "CS_SICK_POTION" && u.log[3] == "GOT_CURE"
            && u.log[4] == "ONHIT_10";
    if (!ok) { std::printf("00ece7f0_TEST FAIL\n"); return 1; }
    std::printf("00ece7f0_TEST PASS\n");
    return 0;
}

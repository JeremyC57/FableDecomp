// Behaviour test for CModeStandBase::UpdateIdleAnim (0x00862780): pose/special idle choice, idle-counter reset and which
// animation (type 0x19 special, type 6 pose, or none) is started for each situation.
#include <cstdio>
#include <string>

struct Situation {
    bool weaponsOut, hasCombat, unarmedIdle;
    bool hasInfo, waveInfo, infoFlag;
    long idleCounter, waveFrames, idleFrames;
    bool heroAlive;
    float dx, dy;
    bool busy;
    bool animsQueued, looping, loopType19, force;
    std::string previousPose;
};

struct Result {
    std::string started;       // "" when nothing started
    int type;
    long idleCounter;
    std::string pose;
};

static Result Run(const Situation& s)
{
    Result r;
    r.type = 0;
    r.idleCounter = s.idleCounter;
    bool subtle = false;
    std::string pose;
    if (s.weaponsOut && s.hasCombat && s.unarmedIdle)
        pose = "ST_IDLE_MELEE";
    else {
        subtle = true;
        pose = "ST_IDLE_SUBTLE";
    }
    std::string special;
    bool reset = false;
    if (s.infoFlag && s.hasInfo && s.waveInfo && s.idleCounter > s.waveFrames && s.heroAlive) {
        float d2 = s.dy * s.dy + s.dx * s.dx;
        if (!(d2 < 9.0f)) {
            reset = true;
            special = "STANDARD_WAVE_INFO";
        }
    }
    if (special == "" && s.idleCounter > s.idleFrames) {
        reset = true;
        special = "ST_IDLE";
    }
    if (!s.busy) {
        if (reset)
            r.idleCounter = 0;
        bool notLoopingSpecial = !s.animsQueued || !s.looping || !s.loopType19 || s.force;
        if (subtle && special != "" && notLoopingSpecial) {
            r.started = special;
            r.type = 0x19;
        } else if (pose != "" && (pose != s.previousPose || !s.animsQueued || !s.looping || s.force)) {
            r.started = pose;
            r.type = 6;
        }
    }
    r.pose = pose;
    return r;
}

static Situation Base()
{
    Situation s;
    s.weaponsOut = false; s.hasCombat = true; s.unarmedIdle = true;
    s.hasInfo = false; s.waveInfo = false; s.infoFlag = false;
    s.idleCounter = 10; s.waveFrames = 50; s.idleFrames = 100;
    s.heroAlive = true; s.dx = 0.0f; s.dy = 0.0f;
    s.busy = false;
    s.animsQueued = true; s.looping = true; s.loopType19 = false; s.force = false;
    s.previousPose = "ST_IDLE_SUBTLE";
    return s;
}

int main()
{
    bool ok = true;
    Situation s = Base();
    Result r = Run(s);
    ok = ok && r.started == "" && r.pose == "ST_IDLE_SUBTLE" && r.idleCounter == 10;
    s.previousPose = "ST_IDLE_MELEE";
    r = Run(s);
    ok = ok && r.started == "ST_IDLE_SUBTLE" && r.type == 6;
    s = Base(); s.weaponsOut = true;
    r = Run(s);
    ok = ok && r.pose == "ST_IDLE_MELEE" && r.started == "ST_IDLE_MELEE" && r.type == 6;
    s = Base(); s.idleCounter = 150;
    r = Run(s);
    ok = ok && r.started == "ST_IDLE" && r.type == 0x19 && r.idleCounter == 0;
    s = Base(); s.idleCounter = 60; s.infoFlag = s.hasInfo = s.waveInfo = true; s.dx = 3.0f; s.dy = 1.0f;
    r = Run(s);
    ok = ok && r.started == "STANDARD_WAVE_INFO" && r.idleCounter == 0;
    s.dx = 1.0f;
    r = Run(s);
    ok = ok && r.started == "" && r.idleCounter == 60;
    s = Base(); s.idleCounter = 150; s.busy = true;
    r = Run(s);
    ok = ok && r.started == "" && r.idleCounter == 150;
    s = Base(); s.idleCounter = 150; s.weaponsOut = true; s.previousPose = "ST_IDLE_MELEE";
    r = Run(s);
    ok = ok && r.started == "" && r.idleCounter == 0;
    std::printf(ok ? "00862780_TEST PASS\n" : "00862780_TEST FAIL\n");
    return ok ? 0 : 1;
}
// Behaviour test for CAIStateGroup_KrakenAttack::Update 0x00924C70: the fight loop's phase machine. Phase 0 waits for every
// tentacle hit, rises and enters phase 2 with the health/frame baseline; phase 2 low-strikes (head hit -> no strikes left,
// else phase 1 and one strike fewer) and, once the strikes run out after the first rise, roars + high-attacks and refills them;
// phase 1 returns to phase 2 on damage or after HeadIdleTime; a return-to-water order resets to phase 0 with no strikes left.
#include <cstdio>
#include <string>
#include <vector>

struct Kraken {
    bool TentaclesHit, TargetAlive, HeadHit, ReturnToWater;
    long LowStrikesBeforeHigh;
    float HeadIdleTime;     // seconds
};

struct Fight {
    Kraken K;
    long Frame;
    float Health;
    long Fps;
    std::vector<std::string> Actions;
    long phase, attacksLeft, startFrame;
    float startHealth;
    bool first;

    void Start() { attacksLeft = K.LowStrikesBeforeHigh; phase = 0; startFrame = 0; startHealth = 0.0f; first = true; }
    void Step()
    {
        switch (phase) {
        case 0:
            if (K.TentaclesHit) {
                if (K.TargetAlive) Actions.push_back("RiseUp");
                phase = 2;
                Actions.push_back("HeadAttackMode");
                startFrame = Frame;
                startHealth = Health;
            } else {
                Actions.push_back("Wait");
            }
            break;
        case 1:
            if (Health < startHealth || Frame - startFrame > (long)(Fps * K.HeadIdleTime)) { phase = 2; break; }
            Actions.push_back("Wait");
            break;
        case 2:
            if (K.TargetAlive) {
                if (!first && attacksLeft <= 0) {
                    Actions.push_back("Roar");
                    Actions.push_back("HighAttack");
                    attacksLeft = K.LowStrikesBeforeHigh;
                    startFrame = Frame;
                    phase = 1;
                    startHealth = Health;
                } else {
                    Actions.push_back("LowPose");
                    if (K.HeadHit) { attacksLeft = 0; K.HeadHit = false; }
                    else { startFrame = Frame; phase = 1; startHealth = Health; --attacksLeft; }
                }
                break;
            }
            Actions.push_back("Wait");
            break;
        }
        if (K.ReturnToWater) {
            Actions.push_back("ReturnToWater");
            K.ReturnToWater = false;
            first = false;
            phase = 0;
            attacksLeft = 0;
        }
    }
};

static bool Last(const Fight& f, const char* a) { return !f.Actions.empty() && f.Actions.back() == a; }

int main()
{
    Fight f;
    f.K.TentaclesHit = false; f.K.TargetAlive = true; f.K.HeadHit = false; f.K.ReturnToWater = false;
    f.K.LowStrikesBeforeHigh = 2; f.K.HeadIdleTime = 2.0f;
    f.Frame = 0; f.Health = 100.0f; f.Fps = 30;
    f.Start();
    f.Step();
    if (f.phase != 0 || !Last(f, "Wait")) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    f.K.TentaclesHit = true; f.Frame = 10;
    f.Step();
    if (f.phase != 2 || f.startFrame != 10 || f.Actions[1] != "RiseUp") { std::printf("00924c70_TEST FAIL\n"); return 1; }
    // first low strike: no head hit -> phase 1, one strike fewer
    f.Step();
    if (f.phase != 1 || f.attacksLeft != 1 || !Last(f, "LowPose")) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    // idle under the limit waits, past 60 frames goes back to phase 2
    f.Frame = 70; f.Step();
    if (f.phase != 1) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    f.Frame = 71; f.Step();
    if (f.phase != 2) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    // head hit on the strike: strikes run out, still phase 2
    f.K.HeadHit = true; f.Step();
    if (f.phase != 2 || f.attacksLeft != 0 || f.K.HeadHit) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    // first rise: no roar yet even with no strikes left
    f.Step();
    if (!Last(f, "LowPose") || f.phase != 1 || f.attacksLeft != -1) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    // damage ends phase 1 at once; then a return-to-water order resets everything
    f.Health = 90.0f; f.Step();
    if (f.phase != 2) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    f.K.ReturnToWater = true; f.Step();
    if (f.phase != 0 || f.first || f.attacksLeft != 0 || !Last(f, "ReturnToWater")) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    // after the first rise, no strikes left -> roar + high attack, strikes refilled
    f.Frame = 200; f.Step();
    f.Step();
    if (!Last(f, "HighAttack") || f.Actions[f.Actions.size() - 2] != "Roar" || f.attacksLeft != 2 || f.phase != 1) {
        std::printf("00924c70_TEST FAIL\n"); return 1;
    }
    // a dead target only waits in phase 2
    f.phase = 2; f.K.TargetAlive = false; f.Step();
    if (!Last(f, "Wait") || f.phase != 2) { std::printf("00924c70_TEST FAIL\n"); return 1; }
    std::printf("00924c70_TEST PASS\n");
    return 0;
}

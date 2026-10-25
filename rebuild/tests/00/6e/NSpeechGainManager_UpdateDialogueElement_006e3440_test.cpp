// Behaviour test for 0x006E3440 NSpeechGainManager::UpdateDialogueElement (model copy): false without a live speaker
// that has a sound player playing the line; otherwise the output gain snaps to the target when within one step, else
// moves by elapsedFrames * the attack (rising) or release (falling) delta, and the player gets the new volume.
#include <cstdio>

static float GFFloatAbs(float f)
{
    *(unsigned int*)&f &= 0x7fffffff;
    return f;
}

static const float g_Epsilon = 1e-4f;

struct ResponseTimes { float attack; float release; };
static ResponseTimes g_ResponseTimes[3] = { { 0.1f, -0.05f }, { 0.2f, -0.1f }, { 0.3f, -0.15f } };

struct Player {
    bool playing; float volume; int calls;
    bool IsSamplePlaying(unsigned int) const { return playing; }
    void SetSpecificSampleVolume(unsigned int, float v) { volume = v; ++calls; }
};
struct Thing { bool dead; Player* player; };
struct Info { Thing* speaker; unsigned int soundID; int layer; float outputGain; float target; };

static bool UpdateDialogueElement(Info& info, int elapsedFrames)
{
    if (info.speaker && !info.speaker->dead) {
        Thing* speaker = info.speaker;
        if (speaker->player) {
            Player* player = speaker->player;
            if (player->IsSamplePlaying(info.soundID)) {
                float targetGain = info.target;
                float diff = targetGain - info.outputGain;
                if (GFFloatAbs(diff) > g_Epsilon) {
                    float step = elapsedFrames * (diff > 0.0f ? g_ResponseTimes[info.layer].attack
                                                              : g_ResponseTimes[info.layer].release);
                    info.outputGain = GFFloatAbs(diff) > GFFloatAbs(step) ? step + info.outputGain : targetGain;
                    player->SetSpecificSampleVolume(info.soundID, info.outputGain);
                }
                return true;
            }
        }
    }
    return false;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

int main()
{
    Player p = { true, 0.0f, 0 };
    Thing t = { false, &p };
    Info rise = { &t, 7, 1, 0.0f, 1.0f };
    int ok = UpdateDialogueElement(rise, 2) && Near(rise.outputGain, 0.4f) && Near(p.volume, 0.4f) && p.calls == 1;
    ok = ok && UpdateDialogueElement(rise, 10) && Near(rise.outputGain, 1.0f) && p.calls == 2;
    ok = ok && UpdateDialogueElement(rise, 1) && Near(rise.outputGain, 1.0f) && p.calls == 2;   // within epsilon
    Info fall = { &t, 7, 2, 1.0f, 0.0f };
    ok = ok && UpdateDialogueElement(fall, 2) && Near(fall.outputGain, 0.7f) && Near(p.volume, 0.7f);
    ok = ok && GFFloatAbs(-2.5f) == 2.5f && GFFloatAbs(3.0f) == 3.0f;
    Thing dead = { true, &p };
    Info gone = { &dead, 7, 0, 0.0f, 1.0f };
    ok = ok && !UpdateDialogueElement(gone, 1);
    Thing silent = { false, 0 };
    gone.speaker = &silent;
    ok = ok && !UpdateDialogueElement(gone, 1);
    Player stopped = { false, 0.0f, 0 };
    Thing quiet = { false, &stopped };
    gone.speaker = &quiet;
    ok = ok && !UpdateDialogueElement(gone, 1) && stopped.calls == 0;
    gone.speaker = 0;
    ok = ok && !UpdateDialogueElement(gone, 1);
    if (!ok) { std::printf("006e3440_TEST FAIL\n"); return 1; }
    std::printf("006e3440_TEST PASS\n");
    return 0;
}
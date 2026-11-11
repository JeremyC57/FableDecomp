// Behaviour test for 0x006E3EC0 NSpeechGainManager::Initialise: the last update frame is cleared, the speaker counts and the
// response table are sized to three layers (shrinking or growing, the counts filled with 0), the active dialogue list is
// emptied, and each layer's attack / release seconds become per-frame deltas (frame/attack, else 1; -frame/release, else -1).
#include <cstdio>
#include <vector>
#include <list>

struct CDialogueLayerDef { float ResponseTimeSecsAttack, ResponseTimeSecsRelease; };
struct CResponseTimes {
    CResponseTimes() : AttackMaxDeltaPerFrame(99.0f), ReleaseMaxDeltaPerFrame(99.0f) { }
    CResponseTimes(float attack_secs, float release_secs, long fps)
    {
        float frame_secs = 1.0f / fps;
        if (attack_secs > frame_secs) AttackMaxDeltaPerFrame = frame_secs / attack_secs; else AttackMaxDeltaPerFrame = 1.0f;
        if (release_secs > frame_secs) ReleaseMaxDeltaPerFrame = frame_secs * (-1.0f / release_secs); else ReleaseMaxDeltaPerFrame = -1.0f;
    }
    float AttackMaxDeltaPerFrame, ReleaseMaxDeltaPerFrame;
};

static long g_ConstantFPS = 30;
static long g_LastUpdateFrame;
static std::vector<unsigned short> g_LayerSpeakerCounts;
static std::list<int> g_ActiveDialogue;
static std::vector<CResponseTimes> g_ResponseTimes;

static void Initialise(const std::vector<CDialogueLayerDef>& layers)
{
    g_LastUpdateFrame = 0;
    g_LayerSpeakerCounts.resize(3, 0);
    g_ActiveDialogue.clear();
    g_ResponseTimes.resize(3);
    for (int i = 0; i < 3; ++i)
        g_ResponseTimes[i] = CResponseTimes(layers[i].ResponseTimeSecsAttack, layers[i].ResponseTimeSecsRelease, g_ConstantFPS);
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

#define FAIL { std::printf("006e3ec0_TEST FAIL\n"); return 1; }
int main()
{
    std::vector<CDialogueLayerDef> layers(3);
    layers[0].ResponseTimeSecsAttack = 0.5f;  layers[0].ResponseTimeSecsRelease = 0.25f;
    layers[1].ResponseTimeSecsAttack = 0.01f; layers[1].ResponseTimeSecsRelease = 2.0f;
    layers[2].ResponseTimeSecsAttack = 1.0f;  layers[2].ResponseTimeSecsRelease = 0.0f;
    g_LastUpdateFrame = 77;
    for (int i = 0; i < 5; ++i) g_LayerSpeakerCounts.push_back((unsigned short)(i + 4));
    g_ActiveDialogue.push_back(1); g_ActiveDialogue.push_back(2);
    Initialise(layers);
    if (g_LastUpdateFrame != 0 || g_LayerSpeakerCounts.size() != 3 || g_LayerSpeakerCounts[2] != 6 || !g_ActiveDialogue.empty()) FAIL;
    if (g_ResponseTimes.size() != 3) FAIL;
    float frame = 1.0f / 30;
    if (!Near(g_ResponseTimes[0].AttackMaxDeltaPerFrame, frame / 0.5f) || !Near(g_ResponseTimes[0].ReleaseMaxDeltaPerFrame, -frame / 0.25f)) FAIL;
    if (!Near(g_ResponseTimes[1].AttackMaxDeltaPerFrame, 1.0f) || !Near(g_ResponseTimes[1].ReleaseMaxDeltaPerFrame, -frame / 2.0f)) FAIL;
    if (!Near(g_ResponseTimes[2].AttackMaxDeltaPerFrame, frame) || !Near(g_ResponseTimes[2].ReleaseMaxDeltaPerFrame, -1.0f)) FAIL;
    g_LayerSpeakerCounts.clear();
    Initialise(layers);                                   // growing path: the counts are filled with zero
    if (g_LayerSpeakerCounts.size() != 3 || g_LayerSpeakerCounts[0] != 0 || g_LayerSpeakerCounts[2] != 0) FAIL;
    std::printf("006e3ec0_TEST PASS\n");
    return 0;
}
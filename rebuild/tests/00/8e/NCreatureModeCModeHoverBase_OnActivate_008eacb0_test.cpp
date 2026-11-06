// Behaviour test for NCreatureMode::CModeHoverBase::OnActivate (0x008EACB0), the hover-state table it builds: three IDLE sectors
// (0..121, 120..241, 240..1 degrees as fractions of a turn, radius 100, movement type 0), then for each speed group (SLOW 8,
// MEDIUM 9, FAST 10) whose "<prefix>FORWARD_<SPEED>" animation the creature has, FORWARD / RIGHT / BACK / LEFT sectors centred
// on 0 / 90 / 180 / 270 degrees (-45..46, 45..136, 135..226, 225..316). The current state is the best one for heading (1,0,0);
// a model.
#include <cstdio>
#include <cstring>
#include <vector>

struct State { char Name[40]; float Low, High, Rad; long Type; float Speed; };

static bool HasAnim(const char* name, const char* const* anims, int n)
{
    for (int i = 0; i < n; ++i)
        if (std::strcmp(name, anims[i]) == 0)
            return true;
    return false;
}

static void Push(std::vector<State>& v, const char* prefix, const char* suffix, float low, float high, long type)
{
    State s;
    std::strcpy(s.Name, prefix); std::strcat(s.Name, suffix);
    s.Low = low / 360.0f; s.High = high / 360.0f; s.Rad = 100.0f; s.Type = type; s.Speed = 0.0f;
    v.push_back(s);
}

static void Build(std::vector<State>& v, const char* prefix, const char* const* anims, int n)
{
    static const char* const speeds[3] = { "SLOW", "MEDIUM", "FAST" };
    Push(v, prefix, "IDLE", 0.0f, 121.0f, 0);
    Push(v, prefix, "IDLE", 120.0f, 241.0f, 0);
    Push(v, prefix, "IDLE", 240.0f, 1.0f, 0);
    for (int s = 0; s < 3; ++s) {
        char forward[40];
        std::strcpy(forward, prefix); std::strcat(forward, "FORWARD_"); std::strcat(forward, speeds[s]);
        if (!HasAnim(forward, anims, n))
            continue;
        char suffix[32];
        std::strcpy(suffix, "FORWARD_"); std::strcat(suffix, speeds[s]); Push(v, prefix, suffix, -45.0f, 46.0f, 8 + s);
        std::strcpy(suffix, "RIGHT_"); std::strcat(suffix, speeds[s]); Push(v, prefix, suffix, 45.0f, 136.0f, 8 + s);
        std::strcpy(suffix, "BACK_"); std::strcat(suffix, speeds[s]); Push(v, prefix, suffix, 135.0f, 226.0f, 8 + s);
        std::strcpy(suffix, "LEFT_"); std::strcat(suffix, speeds[s]); Push(v, prefix, suffix, 225.0f, 316.0f, 8 + s);
    }
}

int main()
{
    static const char* const anims[2] = { "HOVER_FORWARD_SLOW", "HOVER_FORWARD_FAST" };
    std::vector<State> v;
    Build(v, "HOVER_", anims, 2);
    bool ok = v.size() == 11
        && std::strcmp(v[0].Name, "HOVER_IDLE") == 0 && v[0].Low == 0.0f && v[0].High == 121.0f / 360.0f && v[0].Rad == 100.0f
        && v[2].High == 1.0f / 360.0f && v[2].Type == 0
        && std::strcmp(v[3].Name, "HOVER_FORWARD_SLOW") == 0 && v[3].Low == -0.125f && v[3].Type == 8
        && std::strcmp(v[6].Name, "HOVER_LEFT_SLOW") == 0 && v[6].Low == 0.625f
        && std::strcmp(v[7].Name, "HOVER_FORWARD_FAST") == 0 && v[7].Type == 10
        && std::strcmp(v[10].Name, "HOVER_LEFT_FAST") == 0 && v[10].Speed == 0.0f;
    std::vector<State> w;
    Build(w, "HOVER_", anims, 0);
    ok = ok && w.size() == 3;
    std::printf(ok ? "008eacb0_TEST PASS\n" : "008eacb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
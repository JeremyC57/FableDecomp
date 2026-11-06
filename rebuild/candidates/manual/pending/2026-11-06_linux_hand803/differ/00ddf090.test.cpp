// Behaviour test for CQ_PrisonWardenGameScript::CPrisonWarden::Main (0x00DDF090): noise grows with the hero's movement
// (full rate when heard, 0.4 when not), decays by 0.01 when he stands still and is clamped to [0, 1]; reaching 1 is a catch
// that resets it and takes one poem line back.
#include <cstdio>

struct Warden {
    float Noise, Modifier; long PoemLines, Catches;
    void Frame(float moved, bool heard)
    {
        if (heard) Noise += moved * Modifier;
        else if (moved > 0.0f) Noise += Modifier * 0.4f * moved;
        else Noise -= 0.01f;
        if (Noise >= 1.0f) { PoemLines = PoemLines - 1; ++Catches; Noise = 0.0f; }
        else if (Noise < 0.0f) Noise = 0.0f;
        else if (Noise > 1.0f) Noise = 1.0f;
    }
};

int main()
{
    Warden w = { 0.0f, 0.5f, 3, 0 };
    w.Frame(0.0f, false);
    bool ok = w.Noise == 0.0f;
    w.Frame(1.0f, false);
    ok = ok && w.Noise > 0.19f && w.Noise < 0.21f;
    w.Frame(2.0f, true);
    ok = ok && w.Catches == 1 && w.PoemLines == 2 && w.Noise == 0.0f;
    if (!ok) { std::printf("00ddf090_TEST FAIL\n"); return 1; }
    std::printf("00ddf090_TEST PASS\n");
    return 0;
}

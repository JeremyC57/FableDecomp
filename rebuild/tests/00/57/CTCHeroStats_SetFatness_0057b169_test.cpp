#include <cstdio>
// Behaviour model of CTCHeroStats::SetFatness (0057b169): the value is clamped to [0,1], stored, and forwarded to the hero
// morph when the thing has one (interface 3).
static const float& Limit(const float& v, const float& lo, const float& hi) { return v < lo ? lo : (hi < v ? hi : v); }
struct Morph { float value; };
struct Stats {
    float Value; Morph* morph;
    void Set(float v)
    {
        Value = Limit(v, 0.0f, 1.0f);
        if (morph) morph->value = Value;
    }
};
int main()
{
    Morph m = { -1.0f };
    Stats s = { 0.5f, &m };
    s.Set(0.75f);
    if (s.Value != 0.75f || m.value != 0.75f) { std::printf("0057b169_TEST FAIL\n"); return 1; }
    s.Set(3.0f);
    if (s.Value != 1.0f || m.value != 1.0f) { std::printf("0057b169_TEST FAIL\n"); return 1; }
    s.morph = 0; s.Set(-2.0f);
    if (s.Value != 0.0f || m.value != 1.0f) { std::printf("0057b169_TEST FAIL\n"); return 1; }
    std::printf("0057b169_TEST PASS\n");
    return 0;
}
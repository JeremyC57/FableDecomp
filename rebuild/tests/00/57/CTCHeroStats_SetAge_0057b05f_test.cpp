#include <cstdio>
// Behaviour model of CTCHeroStats::SetAge (0x0057B05F): the age is clamped to the def's [min,max] and stored; when the
// thing has a hero morph it receives (age - min) / (max - min).
static const float& Limit(const float& v, const float& lo, const float& hi) { return v < lo ? lo : (hi < v ? hi : v); }
struct Morph { float age; bool called; };
struct Stats {
    float Age; float MinAge; float MaxAge; Morph* morph;
    void SetAge(float age)
    {
        float minAge = MinAge;
        float maxAge = MaxAge;
        Age = Limit(age, minAge, maxAge);
        if (morph) { morph->age = (Age - minAge) / (maxAge - minAge); morph->called = true; }
    }
};
int main()
{
    Morph m = { 0.0f, false };
    Stats s = { 0.0f, 20.0f, 60.0f, &m };
    s.SetAge(30.0f);
    if (s.Age != 30.0f || !m.called || m.age != 0.25f) { std::printf("0057b05f_TEST FAIL\n"); return 1; }
    s.SetAge(100.0f);
    if (s.Age != 60.0f || m.age != 1.0f) { std::printf("0057b05f_TEST FAIL\n"); return 1; }
    s.morph = 0; s.SetAge(5.0f);
    if (s.Age != 20.0f) { std::printf("0057b05f_TEST FAIL\n"); return 1; }
    std::printf("0057b05f_TEST PASS\n");
    return 0;
}
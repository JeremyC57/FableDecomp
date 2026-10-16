// behaviour test for 0x00750D30 CTCExplodingObject::OnSerialise
#include <cstdio>
#include <cstring>
struct Ctx {   // a "loading" context that resets every value to its default and records the keys
    int count; char keys[5][32];
    void Note(const char* n) { std::strcpy(keys[count++], n); }
    void Transfer(const char* n, float& v, const float& d) { Note(n); v = d; }
    void Transfer(const char* n, long& v, const long& d) { Note(n); v = d; }
    void Transfer(const char* n, bool& v, const bool& d) { Note(n); v = d; }
};
struct Exploding {
    float MaxDamage, Radius; long FireDamage; bool Triggered; float TriggerRadius;
    void OnSerialise(Ctx& c, bool)
    {
        c.Transfer("MaxDamage", MaxDamage, 0.0f);
        c.Transfer("Radius", Radius, 0.0f);
        c.Transfer("FireDamage", FireDamage, 10L);
        c.Transfer("TriggeredOnCreatureProximity", Triggered, false);
        c.Transfer("TriggerRadius", TriggerRadius, 0.0f);
    }
};
int main()
{
    Ctx c; c.count = 0;
    Exploding e = { 5.0f, 6.0f, 3, true, 2.0f };
    e.OnSerialise(c, true);
    bool ok = c.count == 5 && e.MaxDamage == 0.0f && e.FireDamage == 10 && !e.Triggered && e.TriggerRadius == 0.0f
        && std::strcmp(c.keys[2], "FireDamage") == 0 && std::strcmp(c.keys[4], "TriggerRadius") == 0;
    if (!ok) { std::printf("00750d30_TEST FAIL\n"); return 1; }
    std::printf("00750d30_TEST PASS\n");
    return 0;
}

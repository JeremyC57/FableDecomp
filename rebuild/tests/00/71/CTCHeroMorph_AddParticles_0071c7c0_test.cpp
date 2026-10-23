// Behaviour test for CTCHeroMorph::AddParticles (0071c7c0): range test with epsilon, interpolation factor, attach modes and list recording
#include <cstdio>
#include <cmath>

struct Entry { long type; bool interpolate; long mode; long typeA, typeB; float minV, maxV; };
struct Result { int created, attached[4], recorded; float lastT; long lastA, lastB; };

static const float kEps = 0.0001f;

static void AddParticles(const Entry* entries, int count, long type, float value, Result& r)
{
    for (int i = 0; i < count; ++i) {
        const Entry& e = entries[i];
        if (e.type != type)
            continue;
        if (!(e.minV - kEps <= value && kEps + e.maxV >= value))
            continue;
        if (e.interpolate && e.typeB > 0) {
            float t = 1.0f;
            if (std::fabs(e.maxV - e.minV) > kEps)
                t = (value - e.minV) / (e.maxV - e.minV);
            r.lastT = t;
            r.lastB = e.typeB;
        } else {
            r.lastB = 0;
        }
        r.lastA = e.typeA;
        ++r.created;
        if (e.mode >= 1 && e.mode <= 3)
            ++r.attached[e.mode];
        ++r.recorded;
    }
}

int main()
{
    int fails = 0;
    Entry entries[] = {
        { 1, false, 1, 10, 0, 0.0f, 0.5f },
        { 1, true, 2, 20, 21, 0.25f, 0.75f },
        { 1, true, 3, 30, 31, 0.5f, 0.5f },
        { 2, false, 1, 40, 0, 0.0f, 1.0f },
    };
    Result r = { 0, { 0, 0, 0, 0 }, 0, 0.0f, 0, 0 };
    AddParticles(entries, 4, 1, 0.5f, r);
    if (r.created != 3 || r.attached[1] != 1 || r.attached[2] != 1 || r.attached[3] != 1 || r.recorded != 3) ++fails;
    if (r.lastT != 1.0f || r.lastA != 30 || r.lastB != 31) ++fails;
    Result q = { 0, { 0, 0, 0, 0 }, 0, 0.0f, 0, 0 };
    AddParticles(entries, 4, 1, 0.375f, q);
    if (q.created != 2 || q.lastT != 0.25f || q.lastA != 20) ++fails;
    Result z = { 0, { 0, 0, 0, 0 }, 0, 0.0f, 0, 0 };
    AddParticles(entries, 4, 1, 0.75005f, z);
    if (z.created != 1) ++fails;
    AddParticles(entries, 4, 3, 0.5f, z);
    if (z.created != 1) ++fails;
    if (fails) {
        printf("0071c7c0_TEST FAIL\n");
        return 1;
    }
    printf("0071c7c0_TEST PASS\n");
    return 0;
}
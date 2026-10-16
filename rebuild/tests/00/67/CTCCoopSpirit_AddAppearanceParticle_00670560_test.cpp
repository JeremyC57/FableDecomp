// behaviour test for 0x00670560 CTCCoopSpirit::AddAppearanceParticle
#include <cstdio>
#include <cstring>
struct CThing;
struct Emitter { CThing* attachedTo; long bone; const char* boneName; long flags; float scale;
    void AttachToThing(CThing* t, long b, const char* n, long f, float s) { attachedTo = t; bone = b; boneName = n; flags = f; scale = s; } };
struct CThing {
    unsigned int bits[9]; Emitter* em; float pos;
    template<class T> bool GetInterface(T** out, long type) { if (bits[type >> 5] & (1u << (type & 31))) { *out = em; return true; } return false; }
};
static CThing* g_created = 0;
static const char* g_lastName = 0;
static CThing* Create(const char* name, float, bool) { g_lastName = name; return g_created; }
struct Spirit {
    CThing* ParentThing;
    void AddAppearanceParticle(const char* name)
    {
        CThing* particle = Create(name, ParentThing->pos, false);
        if (particle) {
            Emitter* emitter = 0;
            particle->GetInterface(&emitter, 0x4b);
            emitter->AttachToThing(ParentThing, 0x88, "", 0, 1.1f);
        }
    }
};
int main()
{
    CThing me; me.pos = 1;
    Emitter em = { 0, 0, 0, 0, 0 };
    CThing part; for (int i = 0; i < 9; ++i) part.bits[i] = 0; part.bits[2] = 0x800; part.em = &em;
    Spirit s; s.ParentThing = &me;
    s.AddAppearanceParticle("P1");
    bool ok = em.attachedTo == 0 && !std::strcmp(g_lastName, "P1");
    g_created = &part;
    s.AddAppearanceParticle("P2");
    ok = ok && em.attachedTo == &me && em.bone == 0x88 && em.boneName[0] == 0 && em.flags == 0 && em.scale == 1.1f;
    if (!ok) { std::printf("00670560_TEST FAIL\n"); return 1; }
    std::printf("00670560_TEST PASS\n");
    return 0;
}
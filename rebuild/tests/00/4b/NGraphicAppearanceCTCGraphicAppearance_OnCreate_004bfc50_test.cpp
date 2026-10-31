// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::OnCreate @ 0x004BFC50: cache animation TC (type 0x5A) if
// present; if the base def has an appearance sub-def, set the lighting channel {channel,0,0} with (true,true); release def.
#include <cstdio>
struct Def { long RefCount; long LightingChannel; int destroyed; };
struct Entry { long type; void* component; };
struct Thing {
    unsigned long bits[9]; Entry entries[4]; int count; Def* subDef;
    Entry* LowerBound(long t) { int i = 0; while (i < count && entries[i].type < t) ++i; return &entries[i]; }
    bool HasTC(long t) const { return (bits[t >> 5] & (1UL << (t & 31))) != 0; }
};
struct ChannelDesc { unsigned char c0, c1, blend, pad; };
static ChannelDesc g_lastDesc; static int g_setCalls; static bool g_b1, g_b2;
struct Appearance {
    Thing* ParentThing; void* PTCAnimation;
    void SetLightingChannel(const ChannelDesc& d, bool a, bool b) { g_lastDesc = d; g_b1 = a; g_b2 = b; ++g_setCalls; }
    void OnCreate()
    {
        Thing* parent = ParentThing;
        void* animation;
        if (parent->HasTC(0x5a)) {
            Entry* it = parent->LowerBound(0x5a);
            Entry* last = &parent->entries[parent->count];
            animation = (it == last || 0x5a < it->type ? last : it)->component;
            PTCAnimation = animation;
        }
        Def* def = 0;
        if (parent->subDef) { def = parent->subDef; ++def->RefCount; }   // GetSubDef
        if (def) {
            ChannelDesc d; d.c0 = (unsigned char)def->LightingChannel; d.c1 = 0; d.blend = 0; d.pad = 0x77;
            SetLightingChannel(d, true, true);
        }
        if (def) { if (--def->RefCount == 0) def->destroyed = 1; }
    }
};
int main()
{
    static int anim;
    Thing t = {}; t.count = 2; t.entries[0].type = 0x10; t.entries[1].type = 0x5a; t.entries[1].component = &anim;
    t.entries[2].component = 0;
    t.bits[0x5a >> 5] |= 1UL << (0x5a & 31);
    Def d = { 1, 7, 0 }; t.subDef = &d;
    Appearance a; a.ParentThing = &t; a.PTCAnimation = (void*)1;
    a.OnCreate();
    bool ok = a.PTCAnimation == &anim && g_setCalls == 1 && g_lastDesc.c0 == 7 && g_lastDesc.c1 == 0 && g_lastDesc.blend == 0 &&
              g_b1 && g_b2 && d.RefCount == 1 && !d.destroyed;
    Thing u = {}; u.count = 0; u.subDef = 0;
    Appearance b; b.ParentThing = &u; b.PTCAnimation = (void*)3;
    b.OnCreate();
    ok = ok && b.PTCAnimation == (void*)3 && g_setCalls == 1;
    if (!ok) { std::printf("004bfc50_TEST FAIL\n"); return 1; }
    std::printf("004bfc50_TEST PASS\n");
    return 0;
}
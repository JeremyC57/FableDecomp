// behaviour test for 0x0071A8F0 CTCParticleAttachmentTracker::SetParticleAsDetached(CThing&): the argument is wrapped in a temporary intelligent pointer that
// is removed from the AttachedParticles container and released again
#include <cstdio>
struct CThing { int tag; };
static int gLive = 0;
struct CIntelligentPointer {
    const CThing* p;
    CIntelligentPointer(const CThing* t) : p(t) { ++gLive; }
    CIntelligentPointer(const CIntelligentPointer& o) : p(o.p) { ++gLive; }
    ~CIntelligentPointer() { --gLive; }
};
struct Container {                                           // list (remove) / vector (push_back) of intelligent pointers
    const CThing* items[8];
    int n;
    Container() : n(0) {}
    void push_back(const CIntelligentPointer& x) { items[n++] = x.p; }
    void remove(const CIntelligentPointer& x)
    {
        int k = 0;
        for (int i = 0; i < n; ++i) if (items[i] != x.p) items[k++] = items[i];
        n = k;
    }
};
struct Owner {
    Container AttachedParticles;
    void SetParticleAsDetached(CThing& thing) { AttachedParticles.remove(CIntelligentPointer(&thing)); }
};
int main()
{
    int fails = 0;
    CThing a, b, c;
    a.tag = 1; b.tag = 2; c.tag = 3;
    Owner o;
    o.AttachedParticles.items[0] = &a; o.AttachedParticles.items[1] = &b; o.AttachedParticles.items[2] = &a; o.AttachedParticles.items[3] = &c; o.AttachedParticles.n = 4;
    o.SetParticleAsDetached(a);
    if (o.AttachedParticles.n != 2) fails |= 1;                         // every entry naming the thing goes
    if (o.AttachedParticles.items[0] != &b || o.AttachedParticles.items[1] != &c) fails |= 2;
    o.SetParticleAsDetached(a);
    if (o.AttachedParticles.n != 2) fails |= 4;                         // absent: unchanged
    if (gLive != 0) fails |= 64;                             // the temporary pointer is released
    if (fails) { std::printf("0071a8f0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071a8f0_TEST PASS\n");
    return 0;
}
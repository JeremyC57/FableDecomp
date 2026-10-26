// behaviour test for 0x007C0FB0 CTCShotDefinitionBase::AddHitThing(const CThing*): the argument is wrapped in a temporary intelligent pointer that
// is appended to the HitThingsList container and released again
#include <cstdio>
class CThing { public: int tag; };
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
    Container HitThingsList;
    void AddHitThing(const CThing* thing) { HitThingsList.push_back(CIntelligentPointer(thing)); }
};
int main()
{
    int fails = 0;
    CThing a, b, c;
    a.tag = 1; b.tag = 2; c.tag = 3;
    Owner o;
    o.AddHitThing(&a);
    o.AddHitThing(&b);
    if (o.HitThingsList.n != 2) fails |= 1;
    if (o.HitThingsList.items[0] != &a || o.HitThingsList.items[1] != &b) fails |= 2;   // appended in order
    if (gLive != 0) fails |= 64;                             // the temporary pointer is released
    if (fails) { std::printf("007c0fb0_TEST FAIL %d\n", fails); return 1; }
    std::printf("007c0fb0_TEST PASS\n");
    return 0;
}
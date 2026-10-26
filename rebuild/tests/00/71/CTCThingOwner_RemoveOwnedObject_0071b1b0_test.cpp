// behaviour test for 0x0071B1B0 CTCThingOwner::RemoveOwnedObject(CThing&): the argument is wrapped in a temporary intelligent pointer that
// is removed from the ObjectList container and released again
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
    Container ObjectList;
    void RemoveOwnedObject(CThing& thing) { ObjectList.remove(CIntelligentPointer(&thing)); }
};
int main()
{
    int fails = 0;
    CThing a, b, c;
    a.tag = 1; b.tag = 2; c.tag = 3;
    Owner o;
    o.ObjectList.items[0] = &a; o.ObjectList.items[1] = &b; o.ObjectList.items[2] = &a; o.ObjectList.items[3] = &c; o.ObjectList.n = 4;
    o.RemoveOwnedObject(a);
    if (o.ObjectList.n != 2) fails |= 1;                         // every entry naming the thing goes
    if (o.ObjectList.items[0] != &b || o.ObjectList.items[1] != &c) fails |= 2;
    o.RemoveOwnedObject(a);
    if (o.ObjectList.n != 2) fails |= 4;                         // absent: unchanged
    if (gLive != 0) fails |= 64;                             // the temporary pointer is released
    if (fails) { std::printf("0071b1b0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071b1b0_TEST PASS\n");
    return 0;
}
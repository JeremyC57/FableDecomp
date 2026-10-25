// Behaviour test for CThingManager::CopyHeroToHero (0x0051F0E0): for each of the interface types the source thing has,
// types 3, 4, 0x5E, 0x5C and 0x68 are copied with CopyTCState(source component, true) (type 3 resets the target's
// component first) and types 0x6F and 0x14 through the target component's virtual copy; types the source lacks are
// skipped -- through a copy of the body over a stand-in thing with a presence bitset and a sorted interface map.
#include <cstdio>
#include <vector>

struct Component {
    int type;
    int resets;
    const Component* copiedFrom;
    Component(int t) : type(t), resets(0), copiedFrom(0) { }
    void ResetState() { ++resets; }
    void CopyStateFrom(const Component* source) { copiedFrom = source; }
};

struct Thing {
    unsigned int bits[9];
    std::vector<Component*> components;      // sorted by type
    std::vector<int> copiedTypes;
    Thing() { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(Component* c) { bits[c->type / 32] |= 1u << (c->type % 32); components.push_back(c); }
    bool Has(int type) const { return (bits[type / 32] & (1u << (type % 32))) != 0; }
    Component* Get(int type) const
    {
        for (unsigned int i = 0; i < components.size(); ++i)
            if (components[i]->type == type) return components[i];
        return 0;
    }
    void CopyTCState(Component* source, bool flag) { if (flag) copiedTypes.push_back(source->type); }
};

static void CopyHeroToHero(Thing& from, Thing& to)
{
    if (from.Has(3)) {
        Component* source = from.Get(3);
        to.Get(3)->ResetState();
        to.CopyTCState(source, true);
    }
    const int plain[4] = { 4, 0x5e, 0x5c, 0x68 };
    for (int i = 0; i < 4; ++i)
        if (from.Has(plain[i]))
            to.CopyTCState(from.Get(plain[i]), true);
    if (from.Has(0x6f))
        to.Get(0x6f)->CopyStateFrom(from.Get(0x6f));
    if (from.Has(0x14))
        to.Get(0x14)->CopyStateFrom(from.Get(0x14));
}

int main()
{
    Component f3(3), f5e(0x5e), f6f(0x6f), f14(0x14);
    Component t3(3), t6f(0x6f), t14(0x14);
    Thing from, to;
    from.Add(&f3); from.Add(&f14); from.Add(&f5e); from.Add(&f6f);
    to.Add(&t3); to.Add(&t14); to.Add(&t6f);
    CopyHeroToHero(from, to);
    if (t3.resets != 1 || to.copiedTypes.size() != 2 || to.copiedTypes[0] != 3 || to.copiedTypes[1] != 0x5e ||
        t6f.copiedFrom != &f6f || t14.copiedFrom != &f14) {
        std::printf("0051f0e0_TEST FAIL\n");
        return 1;
    }
    std::printf("0051f0e0_TEST PASS\n");
    return 0;
}
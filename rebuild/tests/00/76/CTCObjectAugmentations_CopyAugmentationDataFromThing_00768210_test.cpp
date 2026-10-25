// Behaviour test for 0x00768210 CTCObjectAugmentations::CopyAugmentationDataFromThing(const CThing&).
#include <cstdio>
#include <vector>

struct Def { int refs; };

struct DefPtr {
    Def* p;
    DefPtr(Def* d) : p(d) { if (p) ++p->refs; }
    DefPtr(const DefPtr& o) : p(o.p) { if (p) ++p->refs; }
    ~DefPtr() { if (p) --p->refs; }
    DefPtr& operator=(const DefPtr& o)
    {
        if (p != o.p) { if (p) --p->refs; p = o.p; if (p) ++p->refs; }
        return *this;
    }
};

struct Augs;

struct Thing {
    unsigned int presentBits[9];
    Augs* component;                                        // the lower-bound result for interface 0xBF
    bool PeekAugs(const Augs** out) const
    {
        const int type = 0xbf;
        if (presentBits[type >> 5] & (1u << (type & 31))) { *out = component; return true; }
        return false;
    }
};

struct Augs {
    std::vector<DefPtr> Augmentations;
    std::vector<std::vector<int> > ParticleEffects;
    std::vector<std::vector<int> > AttackParticleEffects;
    int attached, morphed;
    Augs() : attached(0), morphed(0) { }
    void CopyFrom(const Thing& thing)
    {
        const Augs* other;
        if (thing.PeekAugs(&other)) {
            ParticleEffects.clear();
            AttackParticleEffects.clear();
            Augmentations.clear();
            Augmentations.reserve(other->Augmentations.size());
            for (unsigned int i = 0; i < other->Augmentations.size(); ++i) Augmentations.push_back(other->Augmentations[i]);
            ++attached;
            ++morphed;
        }
    }
};

int main()
{
    int ok = 1;
    if ((0xbf >> 5) != 5 || (0xbf & 31) != 31) ok = 0;      // the sign bit of bitset dword 5 (+0x34)
    Def d1 = { 0 }, d2 = { 0 }, d3 = { 0 };
    {
        Augs src, dst;
        src.Augmentations.push_back(DefPtr(&d1));
        src.Augmentations.push_back(DefPtr(&d2));
        src.Augmentations.push_back(DefPtr(0));
        dst.Augmentations.push_back(DefPtr(&d3));
        dst.ParticleEffects.resize(2);
        dst.AttackParticleEffects.resize(3);
        Thing t;
        for (int i = 0; i < 9; ++i) t.presentBits[i] = 0;
        t.component = &src;
        dst.CopyFrom(t);                                    // no component: nothing happens
        if (dst.attached != 0 || dst.Augmentations.size() != 1 || dst.ParticleEffects.size() != 2) ok = 0;
        t.presentBits[5] = 0x80000000u;
        dst.CopyFrom(t);
        if (dst.attached != 1 || dst.morphed != 1) ok = 0;
        if (!dst.ParticleEffects.empty() || !dst.AttackParticleEffects.empty()) ok = 0;
        if (dst.Augmentations.size() != 3 || dst.Augmentations[0].p != &d1 || dst.Augmentations[2].p != 0) ok = 0;
        if (d1.refs != 2 || d2.refs != 2 || d3.refs != 0) ok = 0;
    }
    if (d1.refs != 0 || d2.refs != 0) ok = 0;
    if (!ok) { std::printf("00768210_TEST FAIL\n"); return 1; }
    std::printf("00768210_TEST PASS\n");
    return 0;
}
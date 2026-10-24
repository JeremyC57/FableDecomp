#include <cstdio>
// Behaviour test: CTCLight::AttachParticles -- only when the parent thing has interface 0x82 (bit 2 of the bitset word 4),
// the particle attacher found through the interface map gets AttachParticles if its particle list is empty.
struct CTCBase { int Kind; };
struct CTCParticleAttacher : CTCBase {
    int* ParticleList_start; int* ParticleList_finish; int Attached;
    void AttachParticles() { ++Attached; }
};
struct Entry { long type; CTCBase* component; };
struct CThing {
    unsigned long m_presentBits[9];
    Entry* first; Entry* last;
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    CTCBase* GetTC(long type)
    {
        Entry* it = first;
        while (it != last && it->type < type) ++it;   // LowerBound 0x0040F020
        return (it == last || type < it->type ? last : it)->component;
    }
};
struct CTCLight {
    CThing* ParentThing;
    void AttachParticles()
    {
        CThing* thing = ParentThing;
        if (thing->HasTC(0x82)) {
            CTCParticleAttacher* attacher = (CTCParticleAttacher*)thing->GetTC(0x82);
            if (attacher->ParticleList_start == attacher->ParticleList_finish)
                attacher->AttachParticles();
        }
    }
};
int main()
{
    CTCParticleAttacher pa; pa.Kind = 0x82; pa.ParticleList_start = pa.ParticleList_finish = 0; pa.Attached = 0;
    CTCBase other = { 0x10 };
    Entry entries[3] = { { 0x10, &other }, { 0x82, &pa }, { 0, 0 } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.first = entries; thing.last = entries + 2;
    CTCLight light = { &thing };
    light.AttachParticles();
    if (pa.Attached != 0) { std::printf("00641260_TEST FAIL\n"); return 1; }
    thing.m_presentBits[4] = 4;   // interface 0x82
    light.AttachParticles();
    if (pa.Attached != 1) { std::printf("00641260_TEST FAIL\n"); return 1; }
    int particle;
    pa.ParticleList_start = &particle; pa.ParticleList_finish = &particle + 1;
    light.AttachParticles();
    if (pa.Attached != 1) { std::printf("00641260_TEST FAIL\n"); return 1; }
    std::printf("00641260_TEST PASS\n");
    return 0;
}
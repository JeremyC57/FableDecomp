#include <cstdio>
#include <new>
struct CThing { int id; };
struct CTCBase { CThing* Thing; CTCBase(CThing* t) : Thing(t) { } virtual ~CTCBase() { } };
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } virtual ~CIntelligentPointer() { } };
struct C3DVector { float X, Y, Z; };
struct CTCHighlightItem : CTCBase {
    bool StartRunning, Running; long LifeTimer; C3DVector StartPos; CIntelligentPointer PParticleEmitter;
    CTCHighlightItem(CThing* thing) : CTCBase(thing) { Running = false; StartRunning = false; LifeTimer = 0; }
};
int main()
{
    CThing owner;
    unsigned char raw[sizeof(CTCHighlightItem)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0x77;
    CTCHighlightItem* h = new (raw) CTCHighlightItem(&owner);
    if (h->Thing != &owner || h->StartRunning || h->Running || h->LifeTimer != 0 || h->PParticleEmitter.m_pItem != 0) { std::printf("0077c920_TEST FAIL\n"); return 1; }
    std::printf("0077c920_TEST PASS\n");
    return 0;
}
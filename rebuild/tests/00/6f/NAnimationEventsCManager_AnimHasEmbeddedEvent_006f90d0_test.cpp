// Behaviour test for 0x006F90D0 NAnimationEvents::CManager::AnimHasEmbeddedEvent: the animation stats temporary answers HasEvent and is released (standalone copy of the logic)
#include <cstdio>
#include <cstring>
struct Stats { const char* events[2]; bool HasEvent(const char* e) const { for (int i = 0; i < 2; ++i) if (events[i] && !std::strcmp(events[i], e)) return true; return false; } };
struct Counted { Stats* Data; int* Refs; Counted(Stats* s, int* r) : Data(s), Refs(r) { ++*Refs; } Counted(const Counted& o) : Data(o.Data), Refs(o.Refs) { ++*Refs; } ~Counted() { --*Refs; } Stats* operator->() const { return Data; } };
struct Info { Stats S; int Refs; Counted GetAnimationStats() const { return Counted((Stats*)&S, (int*)&Refs); } };
struct Bank { Info anims[2]; const Info* PeekAnimation(unsigned long i) const { return &anims[i]; } };
struct Manager { Bank* B; bool AnimHasEmbeddedEvent(unsigned long a, const char* e) const { return B->PeekAnimation(a)->GetAnimationStats()->HasEvent(e); } };

int main()
{
    Bank b; b.anims[0].S.events[0] = "FOOTSTEP"; b.anims[0].S.events[1] = 0; b.anims[1].S.events[0] = 0; b.anims[1].S.events[1] = "HIT";
    b.anims[0].Refs = 0; b.anims[1].Refs = 0; Manager m; m.B = &b;
    if (!m.AnimHasEmbeddedEvent(0, "FOOTSTEP") || m.AnimHasEmbeddedEvent(0, "HIT") || !m.AnimHasEmbeddedEvent(1, "HIT")) { std::printf("006f90d0_TEST FAIL\n"); return 1; }
    if (b.anims[0].Refs != 0 || b.anims[1].Refs != 0) { std::printf("006f90d0_TEST FAIL\n"); return 1; }
    std::printf("006f90d0_TEST PASS\n");
    return 0;
}
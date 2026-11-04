// Behaviour test for CTCParticleAttacher::DetachParticles (0x007F8860): kill every live particle (Kill(false)), skip null
// pointers, then empty the list.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Particle { int killed; void Kill(bool) { ++killed; } };
struct Attacher { Particle* list[4]; int count;
    void DetachParticles() { for (int i = 0; i < count; ++i) if (list[i]) list[i]->Kill(false); count = 0; } };
int main()
{
    Particle a = { 0 }, b = { 0 };
    Attacher t; t.list[0] = &a; t.list[1] = 0; t.list[2] = &b; t.count = 3;
    t.DetachParticles();
    bool ok = a.killed == 1 && b.killed == 1 && t.count == 0;
    printf(ok ? "007f8860_TEST PASS\n" : "007f8860_TEST FAIL\n");
    return ok ? 0 : 1;
}
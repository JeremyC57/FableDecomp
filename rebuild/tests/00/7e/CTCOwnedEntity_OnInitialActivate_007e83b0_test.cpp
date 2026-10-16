#include <cstdio>
// Behaviour model of CTCOwnedEntity::OnInitialActivate (0x007E83B0): mirror a live owner's drawable and limbo states.
struct Thing { bool dead, drawable, limbo; };
static void Activate(Thing* owner, Thing& self)
{
    if (owner && !owner->dead) { self.drawable = owner->drawable; self.limbo = owner->limbo; }
}
int main()
{
    Thing owner = { false, true, true }, dead = { true, false, false }, self = { false, false, false };
    Activate(&dead, self);
    if (self.drawable || self.limbo) { std::printf("007e83b0_TEST FAIL\n"); return 1; }
    Activate(&owner, self);
    if (!self.drawable || !self.limbo) { std::printf("007e83b0_TEST FAIL\n"); return 1; }
    std::printf("007e83b0_TEST PASS\n");
    return 0;
}
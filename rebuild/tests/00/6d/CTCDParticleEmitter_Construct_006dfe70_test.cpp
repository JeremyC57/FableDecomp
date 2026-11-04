// Behaviour test for CTCDParticleEmitter::Construct (0x006DFE70): a model of the row -- the physics component receives the
// position with the identity orientation, then the particle type is stored and only the IndependantObject (bit 2) and
// ForceParticleToLoad (bit 1) bits of the flag byte change.
#include <stdio.h>
struct Vec { float x, y, z; };
struct Physics { Vec pos; int calls; void ConstructFromParams(const Vec& p) { pos = p; ++calls; } };
struct Emitter {
    Physics* physics; long ParticleTypeID; unsigned char flags;
    void Construct(long type, const Vec& position, bool independent, bool forceLoad)
    {
        physics->ConstructFromParams(position);
        ParticleTypeID = type;
        flags = (unsigned char)((flags & 0xF9) | ((independent ? 1 : 0) << 2) | ((forceLoad ? 1 : 0) << 1));
    }
};
int main()
{
    Physics ph = { { 0, 0, 0 }, 0 };
    Emitter e = { &ph, 0, 0xFF };
    Vec p = { 1, 2, 3 };
    e.Construct(42, p, false, true);
    int ok = ph.calls == 1 && ph.pos.y == 2 && e.ParticleTypeID == 42 && e.flags == 0xFB;
    e.flags = 0x01; e.Construct(7, p, true, false);
    ok = ok && e.flags == 0x05 && e.ParticleTypeID == 7;
    printf(ok ? "006dfe70_TEST PASS\n" : "006dfe70_TEST FAIL\n");
    return ok ? 0 : 1;
}
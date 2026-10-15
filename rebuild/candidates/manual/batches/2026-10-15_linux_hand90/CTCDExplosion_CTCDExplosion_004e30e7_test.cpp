#include <cstdio>
#include <cstring>
#include <new>
// Behaviour of CTCDExplosion::CTCDExplosion (0x004E30E7): zero radii, -1 life timer and ring, CreatedEmitter false,
// NeedToTriggerExplosion true, null def pointer, empty vectors, zero evolution timer; damage and positions untouched.
struct CThing { int id; };
struct CTCDriverBase {
    virtual long v00() { return 0; }
    CThing* m_parent;
    CTCDriverBase(CThing* parent) : m_parent(parent) {}
};
struct C3DVector { float x, y, z; C3DVector() {} };
struct CDefPointer_Retail { void* Object; CDefPointer_Retail() : Object(0) {} };
struct CPtrVector { void* m_pFirst; void* m_pLast; void* m_pEnd; CPtrVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {} };
struct CTCDExplosion : CTCDriverBase {
    float DamageRadius;
    float PhysicsRadius;
    float MaxDamage;
    long FireDamage;
    long ExplosionLifeTimer;
    C3DVector StartPos;
    C3DVector EndPos;
    CDefPointer_Retail PExplosionDef;
    bool CreatedEmitter;
    bool NeedToTriggerExplosion;
    CPtrVector Emitters;
    long ExplosionEvolutionTimer;
    long CurrentExplosionRing;
    CPtrVector HitThings;
    CPtrVector ImmuneThings;
    CPtrVector SpecifiedThingsToHit;
    virtual long v00() { return 4; }
    CTCDExplosion(CThing* parent)
        : CTCDriverBase(parent),
          DamageRadius(0.0f), PhysicsRadius(0.0f), ExplosionLifeTimer(-1),
          CreatedEmitter(false), NeedToTriggerExplosion(true),
          ExplosionEvolutionTimer(0), CurrentExplosionRing(-1)
    {
    }
};
union UStorage { double d; void* p; unsigned char bytes[sizeof(CTCDExplosion)]; };
int main()
{
    CThing thing;
    UStorage storage;
    std::memset(storage.bytes, 0x11, sizeof(storage.bytes));
    long pattern;
    std::memset(&pattern, 0x11, sizeof(pattern));
    CTCDExplosion* e = new (storage.bytes) CTCDExplosion(&thing);
    if (e->m_parent != &thing || e->DamageRadius != 0.0f || e->PhysicsRadius != 0.0f || e->ExplosionLifeTimer != -1
        || e->PExplosionDef.Object != 0 || e->CreatedEmitter || !e->NeedToTriggerExplosion || e->Emitters.m_pFirst != 0
        || e->ExplosionEvolutionTimer != 0 || e->CurrentExplosionRing != -1 || e->HitThings.m_pLast != 0
        || e->ImmuneThings.m_pEnd != 0 || e->SpecifiedThingsToHit.m_pFirst != 0 || e->FireDamage != pattern || e->v00() != 4) {
        std::printf("004e30e7_TEST FAIL\n"); return 1;
    }
    std::printf("004e30e7_TEST PASS\n");
    return 0;
}

// Behaviour test for 0x008D5280 CCreatureAction_FireMissileTest constructor (targetless overload): every base argument
// is forwarded, the sound pair is silent, the target pointer is explicitly set to null and the global index stored.
#include <cstdio>

class CThingCreatureBase { public: int id; };
class CThing { public: int id; };
class CAnimationEntry { public: int id; };
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CBaseIntelligentPointer {
    const void* m_pItem; int m_sets;
    CBaseIntelligentPointer() : m_pItem((const void*)1), m_sets(0) { }
    void SetPItem(const void* p) { m_pItem = p; ++m_sets; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem(item); }
};
class CCreatureActionBase { public:
    CThingCreatureBase* m_owner; float m_a, m_b; CAnimationEntry* m_anim; CSoundPair m_sound; long m_c, m_d;
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* anim, const CSoundPair& s, long c, long d)
        : m_owner(owner), m_a(a), m_b(b), m_anim(anim), m_sound(s), m_c(c), m_d(d) { }
};
class CCreatureAction_FireMissileTest : public CCreatureActionBase { public:
    CIntelligentPointer<const CThing> PTarget; long GlobalIndex;
    CCreatureAction_FireMissileTest(CThingCreatureBase* owner, long gi, CAnimationEntry* anim, float speed, long c, long d)
        : CCreatureActionBase(owner, -1.0f, speed, anim, CSoundPair(), c, d), PTarget(0), GlobalIndex(gi) { }
};
int main()
{
    CThingCreatureBase owner = { 1 }; CAnimationEntry anim = { 3 };
    CCreatureAction_FireMissileTest a(&owner, 23, &anim, 0.25f, 7, 8);
    if (a.m_owner != &owner || a.m_a != -1.0f || a.m_b != 0.25f || a.m_anim != &anim || a.m_sound.FirstSound != 0 ||
        a.m_sound.LastSound != 0 || a.m_c != 7 || a.m_d != 8 || a.PTarget.m_pItem != 0 || a.PTarget.m_sets != 1 ||
        a.GlobalIndex != 23) { std::printf("008d5280_TEST FAIL\n"); return 1; }
    std::printf("008d5280_TEST PASS\n");
    return 0;
}
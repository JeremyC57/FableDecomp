// Behaviour test for 0x00838b60 CThingGameObject::IsGivingHeroStatChangesOnBeingHit
#include <cstdio>
struct CThingGameObjectDef {
    long m_refCount;
    int  m_destroyed;
    unsigned char AddToComboMultiplierOnHit : 1;
    unsigned char GiveHeroStatChangesOnBeingHit : 1;
    virtual void Destroy() { ++m_destroyed; }
};
struct CDefPointer {
    CThingGameObjectDef* m_p;
    CDefPointer(CThingGameObjectDef* p) : m_p(p) {}
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_refCount; }
    ~CDefPointer()
    {
        long count = m_p->m_refCount - 1;
        m_p->m_refCount = count;
        if (count == 0)
            m_p->Destroy();
    }
    CThingGameObjectDef* operator->() const { return m_p; }
};
class CThingGameObject { public:
    CDefPointer   PBaseDef;
    unsigned char AddToComboMultiplierOnHitOverride : 1;
    unsigned char AddToComboMultiplierOnHitOverrideSet : 1;
    unsigned char GiveHeroStatChangesOnBeingHitOverride : 1;
    unsigned char GiveHeroStatChangesOnBeingHitOverrideSet : 1;
    CThingGameObject(CThingGameObjectDef* d) : PBaseDef(d) {}
    bool IsGivingHeroStatChangesOnBeingHit() const
    {
        if (GiveHeroStatChangesOnBeingHitOverrideSet)
            return GiveHeroStatChangesOnBeingHitOverride;
        CDefPointer def = PBaseDef;
        return def->GiveHeroStatChangesOnBeingHit;
    }
};
int main()
{
    CThingGameObjectDef d; d.m_refCount = 1; d.m_destroyed = 0;
    d.AddToComboMultiplierOnHit = 0; d.GiveHeroStatChangesOnBeingHit = 0;
    d.GiveHeroStatChangesOnBeingHit = 1;
    CThingGameObject o(&d);
    o.AddToComboMultiplierOnHitOverride = 0; o.AddToComboMultiplierOnHitOverrideSet = 0;
    o.GiveHeroStatChangesOnBeingHitOverride = 0; o.GiveHeroStatChangesOnBeingHitOverrideSet = 0;
    bool ok = o.IsGivingHeroStatChangesOnBeingHit() && d.m_refCount == 1 && d.m_destroyed == 0;   // from the def, count restored
    d.GiveHeroStatChangesOnBeingHit = 0; if (o.IsGivingHeroStatChangesOnBeingHit()) ok = false;
    o.GiveHeroStatChangesOnBeingHitOverrideSet = 1; o.GiveHeroStatChangesOnBeingHitOverride = 1;
    if (!o.IsGivingHeroStatChangesOnBeingHit()) ok = false;                                        // override wins
    o.GiveHeroStatChangesOnBeingHitOverride = 0; d.GiveHeroStatChangesOnBeingHit = 1; if (o.IsGivingHeroStatChangesOnBeingHit()) ok = false;
    if (!ok) { std::printf("00838b60_TEST FAIL\n"); return 1; }
    std::printf("00838b60_TEST PASS\n");
    return 0;
}
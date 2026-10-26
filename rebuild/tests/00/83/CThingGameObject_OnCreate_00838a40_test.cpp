// Behaviour test for 0x00838A40 CThingGameObject::OnCreate
#include <cstdio>
class CThing { public: int x; };
struct CBaseIntelligentPointer {
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* item) { PItem = item; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
class CThingPhysical { public:
    bool m_ok;
    bool OnCreate() { return m_ok; }
};
class CThingGameObject : public CThingPhysical { public:
    CIntelligentPointerThing PThingStandingOn;
    unsigned char AddToComboMultiplierOnHit : 1;
    unsigned char AddToComboMultiplierOnHitOverrideSet : 1;
    unsigned char GiveHeroStatChangesOnBeingHit : 1;
    unsigned char GiveHeroStatChangesOnBeingHitOverrideSet : 1;
    unsigned char m_other : 4;
    CThingGameObject(CThing* standing) : PThingStandingOn(standing) {}
    bool OnCreate()
    {
        if (CThingPhysical::OnCreate()) {
            PThingStandingOn = CIntelligentPointerThing(0);
            AddToComboMultiplierOnHit = 0;
            AddToComboMultiplierOnHitOverrideSet = 0;
            GiveHeroStatChangesOnBeingHit = 0;
            GiveHeroStatChangesOnBeingHitOverrideSet = 0;
            return true;
        }
        return false;
    }
};
int main()
{
    CThing floor;
    CThingGameObject o(&floor);
    o.AddToComboMultiplierOnHit = 1; o.AddToComboMultiplierOnHitOverrideSet = 1;
    o.GiveHeroStatChangesOnBeingHit = 1; o.GiveHeroStatChangesOnBeingHitOverrideSet = 1; o.m_other = 9;
    o.m_ok = false;
    bool ok = !o.OnCreate() && o.PThingStandingOn.GetPItem() == &floor && o.AddToComboMultiplierOnHit == 1;
    o.m_ok = true;
    if (!o.OnCreate() || o.PThingStandingOn.GetPItem() != 0) ok = false;
    if (o.AddToComboMultiplierOnHit || o.AddToComboMultiplierOnHitOverrideSet || o.GiveHeroStatChangesOnBeingHit
        || o.GiveHeroStatChangesOnBeingHitOverrideSet || o.m_other != 9) ok = false;
    if (!ok) { std::printf("00838a40_TEST FAIL\n"); return 1; }
    std::printf("00838a40_TEST PASS\n");
    return 0;
}
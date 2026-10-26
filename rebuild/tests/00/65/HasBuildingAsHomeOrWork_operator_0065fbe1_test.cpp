// Behaviour test for 0x0065FBE1 HasBuildingAsHomeOrWork::operator(): true when the creature is alive and either its
// home building or (failing that) its work building is the held building; false for a dead creature.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CThing;
struct CBaseIntelligentPointer {
    void*   VTable;
    CThing* PItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThing { public:
    unsigned char _pad_0x00[0x168];
};
struct CBuildingPointer : CBaseIntelligentPointer { };
class CThingAICreature : public CThing { public:
    CBuildingPointer HomeBuilding;                    // +0x168
    CBuildingPointer WorkBuilding;                    // +0x170
    const CBuildingPointer& GetHomeBuilding() const { return HomeBuilding; }
    const CBuildingPointer& GetWorkBuilding() const { return WorkBuilding; }
};

bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>& thing);   // 0x004AB8F0

class HasBuildingAsHomeOrWork { public:
    CThing* Building;                                 // +0x00
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const;
};

bool HasBuildingAsHomeOrWork::operator()(const CIntelligentPointer<CThingAICreature>& creature) const
{
    if (IS_THING_ALIVE(creature)) {
        if (creature.GetPItem()->GetHomeBuilding().GetPItem() == Building ||
            creature.GetPItem()->GetWorkBuilding().GetPItem() == Building)
            return true;
    }
    return false;
}


static int g_alive = 1;
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>& thing) { return g_alive && thing.PItem != 0; }

int main()
{
    static CThing buildingA, buildingB, buildingC;
    static CThingAICreature creature;
    memset(&creature, 0, sizeof(creature));
    creature.HomeBuilding.PItem = &buildingA;
    creature.WorkBuilding.PItem = &buildingB;
    CIntelligentPointer<CThingAICreature> ptr;
    ptr.VTable = 0;
    ptr.PItem = &creature;

    HasBuildingAsHomeOrWork f;
    int ok = 1;
    f.Building = &buildingA; if (!f(ptr)) ok = 0;       // home
    f.Building = &buildingB; if (!f(ptr)) ok = 0;       // work
    f.Building = &buildingC; if (f(ptr)) ok = 0;        // neither
    g_alive = 0;
    f.Building = &buildingA; if (f(ptr)) ok = 0;        // dead creature
    g_alive = 1;
    creature.HomeBuilding.PItem = 0;
    f.Building = &buildingB; if (!f(ptr)) ok = 0;       // homeless worker

    printf(ok ? "0065fbe1_TEST PASS\n" : "0065fbe1_TEST FAIL\n");
    return ok ? 0 : 1;
}
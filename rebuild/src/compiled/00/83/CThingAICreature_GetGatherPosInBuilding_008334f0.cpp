// CThingAICreature::GetGatherPosInBuilding(CIntelligentPointer<const CThingBuilding> building) -- the building's GATHER_POSITION helper point, else (creature has navigation) the building position when navigable, the nearest navigable position within the building's physics radius, or the first navigable cell of an 11x11 half-unit grid around it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008334F0, length 543 bytes (`ret 0xC`: hidden return + the 8-byte by-value pointer).
// Leaf: ?GetGatherPosInBuilding@CThingAICreature@@QBE?AVC3DVector@@V?$CIntelligentPointer@$$CBVCThingBuilding@@@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Name: Ego_r.pdb donor order IsMessageEventWithinSightRange / ...HearingRange / GetSleepTimes / GetGatherPosInBuilding
//   matches retail 0x00833230 / 0x008332E0 / 0x00833390 (all landed) / 0x008334F0; the PDB signature (const, C3DVector by
//   value, CIntelligentPointer<const CThingBuilding> by value) matches `ret 0xC` and the closing ~CBaseIntelligentPointer.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 and ::~CBaseIntelligentPointer @ 0x00A01C10 (Ego_r/FableWin
//   spellings), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, NGraphicAppearance::
//   CTCGraphicAppearance::GetHelperPoint @ 0x004C0560, CThing::GetPos @ 0x004C73D0, the const CVectorMap LowerBound
//   @ 0x004365B0 (inlined GetTC on interface 7), CTCCreatureNavigation::IsPosNavigable @ 0x006CBA50 and
//   ::GetNearestNavigablePosTo @ 0x006CB8E0 (Ego_r donor order GetNearestFleePosition / GetNearestNavigablePosTo /
//   IsCurrentlyNavigating = retail 0x006CB840 / 0x006CB8E0 / 0x006CB8F0).
// Layout: retail CThing interface bitset +0x20, CVectorMap +0x44, PhysicsTC +0x60, PTCGraphicAppearance +0x64 (Ego_r
//   +0x70/+0x74 less the 4-byte-narrower CVectorMap and the 8-byte-narrower list); CTCPhysicsBase::Radius +0x34 (engine/CTCPhysicsBase.h).
//   Local CThing model as in the landed sibling GetSleepTimes 0x00833390 (engine/CThingAICreature.h is a flat pad class).
#include "fable_compiler.h"
#include "engine/CBaseObject.h"
#include "engine/CTCPhysicsBase.h"                     // Radius +0x34

class CCharString { public:
    void* PData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

#include "engine/C3DVector.h"

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& pos, float scale) const; // 0x004C0560
};
}

class CTCCreatureNavigation { public:
    bool IsPosNavigable(const C3DVector& pos) const;                                          // 0x006CBA50
    bool GetNearestNavigablePosTo(const C3DVector& pos, float radius, C3DVector& out) const;  // 0x006CB8E0
};

class CThing : public CBaseObject { public:
    unsigned char m_pad08[0x20 - 0x08];
    unsigned long m_presentBits[9];                         // +0x20 UsingComponents bitset
    CInterfaceMap m_interfaces;                             // +0x44 Components
    void*         UpdateComponentsList;                     // +0x54 list<CTCBase*>
    CCharString   SerialisationSectionName;                 // +0x58
    void*         PTCDriver;                                // +0x5c
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance; // +0x64
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
    const C3DVector& GetPos() const;                        // 0x004C73D0
};

class CThingBuilding : public CThing { public:
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CThingAICreature : public CThing { public:
    C3DVector GetGatherPosInBuilding(CIntelligentPointer<const CThingBuilding> building) const;
};

C3DVector CThingAICreature::GetGatherPosInBuilding(CIntelligentPointer<const CThingBuilding> building) const
{
    NGraphicAppearance::CTCGraphicAppearance* appearance = building->PTCGraphicAppearance;
    C3DVector pos(0.0f, 0.0f, 0.0f);
    bool found;
    {
        CCharString helperName("GATHER_POSITION");
        found = appearance->GetHelperPoint(helperName, 0, 0, pos, 1.0f);
    }
    if (!found && HasTC(7)) {
        CTCCreatureNavigation* navigation = GetTC<CTCCreatureNavigation>(7);
        if (navigation->IsPosNavigable(building->GetPos())) {
            pos = building->GetPos();
        } else {
            CTCPhysicsBase* physics = building->PhysicsTC;
            found = navigation->GetNearestNavigablePosTo(building->GetPos(), physics->Radius, pos);
            if (!found) {
                for (float x = -5.0f; !found && x <= 5.0f; x += 1.0f) {
                    for (float y = -5.0f; !found && y <= 5.0f; y += 1.0f) {
                        pos.X = x * 0.5f + building->GetPos().X;
                        pos.Y = y * 0.5f + building->GetPos().Y;
                        pos.Z = building->GetPos().Z;
                        found = navigation->IsPosNavigable(pos);
                    }
                }
            }
        }
    }
    return pos;
}
// CThingAICreature::GetGatherPosInBuilding(CIntelligentPointer<const CThingBuilding> building) -- the building's GATHER_POSITION helper point, else (creature has navigation) the building position when navigable, the nearest navigable position within the building's physics radius, or the first navigable cell of an 11x11 half-unit grid around it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008334F0, length 543 bytes (`ret 0xC`: hidden return + the 8-byte by-value pointer).
// Leaf: ?GetGatherPosInBuilding@CThingAICreature@@QBE?AVC3DVector@@V?$CIntelligentPointer@$$CBVCThingBuilding@@@@@Z
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
#include "engine/CBaseObject.h"
#include "engine/CTCPhysicsBase.h"                     // Radius +0x34

class CCharString { public:
    void* PData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* endOfStorage;
    long    pad0c;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};


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
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    const C3DVector& GetPos() const;                        // 0x004C73D0
};

class CThingBuilding : public CThing { public:
};

class CBaseIntelligentPointer { public:
    CBaseObjectPointer* PData;
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* operator->() const { return static_cast<T*>(GetPItem()); }
};

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
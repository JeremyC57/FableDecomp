// CTCDummyVillager::CanBeRecreated() -- a dummy can be brought back only when respawnable, its village (if a village member) is not flagged, and neither its home nor its work building is a buyable house that the player owns or that starts empty (unless rented); a blocked house pushes the recreation day to at least today
// Leaf: CanBeRecreated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072F9E0, 400 bytes (`ret`). Ego_r.pdb `?CanBeRecreated@CTCDummyVillager@@QAE_NXZ`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interfaces 0x23 village member, 0xd5 buyable house),
// CTCVillageMember::GetPVillage @ 0x007EE540, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThingBuilding::
// GetIsAvailableForResidency @ 0x0082E0E0, CTCBuyableHouse::IsInitiallyEmpty @ 0x006BF890 (byte twin label
// CTCShotDefinitionBase::CanBeHitByMeleeStrike: PDef->InitiallyEmpty), CTCBase::GetWorld @ 0x00686A70.
// Layout: Ego_r.pdb CTCDummyVillager: HomeBuilding +0x10, WorkBuilding +0x20, DayToBeRecreated +0x40, Respawnable +0x48;
// CTCBuyableHouse OwnedByPlayer +0x11, Rented +0x12; the village's flag byte +0xb3 (retail offset; PDB name not pinned).
// Shape: interface lookups are the landed HasTC + ternary GetTC (each building: GetPItem() null test, then a named GetPItem()
// local for HasTC and GetTC); the recreation day is `Day = max(Day, today)` through the
// STLport reference max (`cmp ; lea ; jl`), written identically for both buildings so retail's tail-merge forms.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CThingBuilding : public CThing { public:
    bool GetIsAvailableForResidency() const;    // 0x0082E0E0
};
class CTCVillage { public:
    unsigned char _pad00[0xb3];
    bool          FlagB3;                       // +0xb3
};
class CTCVillageMember { public:                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
class CTCBuyableHouse { public:                        // interface 0xd5
    unsigned char _pad00[0x11];
    bool          OwnedByPlayer;                // +0x11
    bool          Rented;                       // +0x12
    bool IsInitiallyEmpty() const;              // 0x006BF890
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CGameClock {
    unsigned char _pad00[0x14];
    long Day;                                   // +0x14
};
class CWorld { public:
    unsigned char _pad00[0x1c];
    CGameClock* Clock;                          // +0x1c
};
template<class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

class CTCDummyVillager { public:
    virtual void v00();
    CThing*                             ParentThing;        // +0x04 (CTCBase)
    unsigned char                       _pad08[0x8];
    CIntelligentPointer<CThingBuilding> HomeBuilding;       // +0x10
    unsigned long                       HomeBuildingUID[2]; // +0x18
    CIntelligentPointer<CThingBuilding> WorkBuilding;       // +0x20
    unsigned long                       WorkBuildingUID[2]; // +0x28
    unsigned char                       Creature[8];        // +0x30
    unsigned long                       CreatureUID[2];     // +0x38
    long                                DayToBeRecreated;   // +0x40
    long                                FrameToBeRecreated; // +0x44
    bool                                Respawnable;        // +0x48
    CWorld* GetWorld();                                     // 0x00686A70 (CTCBase)
    bool CanBeRecreated();
};

bool CTCDummyVillager::CanBeRecreated()
{
    if (!Respawnable)
        return false;
    if (ParentThing->HasTC(0x23)) {
        CTCVillageMember* member = ParentThing->GetTC<CTCVillageMember>(0x23);
        if (member->GetPVillage() && member->GetPVillage()->FlagB3)
            return false;
    }
    if (HomeBuilding.GetPItem()) {
        CThingBuilding* home = HomeBuilding.GetPItem();
        if (home->HasTC(0xd5)) {
            CTCBuyableHouse* house = home->GetTC<CTCBuyableHouse>(0xd5);
            if (!HomeBuilding.GetPItem()->GetIsAvailableForResidency())
                return false;
            if ((house->OwnedByPlayer || house->IsInitiallyEmpty()) && !house->Rented) {
                long today = GetWorld()->Clock->Day;
                DayToBeRecreated = Max(DayToBeRecreated, today);
                return false;
            }
        }
    }
    if (WorkBuilding.GetPItem()) {
        CThingBuilding* work = WorkBuilding.GetPItem();
        if (work->HasTC(0xd5)) {
            CTCBuyableHouse* house = work->GetTC<CTCBuyableHouse>(0xd5);
            if ((house->OwnedByPlayer || house->IsInitiallyEmpty()) && !house->Rented) {
                long today = GetWorld()->Clock->Day;
                DayToBeRecreated = Max(DayToBeRecreated, today);
                return false;
            }
        }
    }
    return true;
}
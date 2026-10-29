// CTCDummyVillager::OnInitialActivate() -- relinks the home/work buildings and the creature from their saved UIDs, re-attaches the creature to its buildings and village, and recreates a dead villager when its day has come
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730070, 752 bytes, `ret`.
// Callees: CTCBase::GetThingManager() const @ 0x00686AE0, CThingManager::FindThingByUID(unsigned __int64) @ 0x0051ED80,
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10,
//          IS_THING_ALIVE<CIntelligentPointer<CThingBuilding>> @ 0x0065F8CA (__fastcall), CThingAICreature::SetWorkBuilding
//          @ 0x00512260, CThingAICreature::SetHomeBuilding @ 0x00512210, LowerBound @ 0x0040F020, the 0x90 component's
//          virtual slot 6, CTCVillageMember::GetVillage() @ 0x007EE540, CTCVillage::AddThingToVillage @ 0x0065B854,
//          CTCDummyVillager::CanBeRecreated() @ 0x0072F9E0, CTCBase::GetWorld() @ 0x00686A70, CThingPhysical::GetPos()
//          @ 0x004C73D0, CTCDummyVillager::CreateVillager @ 0x0072F750.
// Layout (Ego_r.pdb, matches retail): HomeBuilding +0x10 / UID +0x18, WorkBuilding +0x20 / UID +0x28, Creature +0x30 /
//         UID +0x38, DayToBeRecreated +0x40, Respawnable +0x48; CThing type byte +0x10 (1 creature, 3 building);
//         CThingAICreature HomeBuilding +0x168, WorkBuilding +0x170.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PData;
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
};

class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x10 - 4];
    unsigned char ThingType;                        // +0x10 (1 creature, 3 building)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long PresentBits[9];                   // +0x20
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    void* PPositionData;                            // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                          // +0x91 (bit 0: being destroyed)
    __forceinline bool HasTC(long type) const
    {
        if (PresentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
    const C3DVector& GetPos() const;           // 0x004C73D0
};
class CThingBuilding : public CThing { };
class CThingAICreature : public CThing {
public:
    unsigned char _pad94[0x168 - 0x94];
    CIntelligentPointer<CThingBuilding> HomeBuilding;   // +0x168
    CIntelligentPointer<CThingBuilding> WorkBuilding;   // +0x170
    void SetWorkBuilding(CThingBuilding* building); // 0x00512260
    void SetHomeBuilding(CThingBuilding* building); // 0x00512210
};
inline bool IsAlive(const CThing* thing) { return thing && !(thing->Flags91 & 1); }
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingBuilding>& thing);   // 0x0065F8CA

class CThingManager;
class CWorld;
class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void OnVillageChanged();                // slot 6 (+0x18)
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
protected:
    CThingManager& GetThingManager();   // 0x00686AE0
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCVillage : public CTCBase {
public:
    void AddThingToVillage(CThing* thing);          // 0x0065B854
};
class CTCVillageMember : public CTCBase {
public:
    CTCVillage* GetVillage();                       // 0x007EE540
};
class CThingManager {
public:
    CThing* FindThingByUID(unsigned __int64 uid);   // 0x0051ED80
};
struct CGameClock { unsigned char _pad[0x14]; long Day; };
class CWorld { public: unsigned char _pad[0x1c]; CGameClock* PClock; };

class CTCDummyVillagerBase : public CTCBase {
public:
};

class CTCDummyVillager : public CTCDummyVillagerBase {
public:
    long DefType;                                   // +0x0C
    CIntelligentPointer<CThingBuilding> HomeBuilding;   // +0x10
    unsigned __int64 HomeBuildingUID;               // +0x18
    CIntelligentPointer<CThingBuilding> WorkBuilding;   // +0x20
    unsigned __int64 WorkBuildingUID;               // +0x28
    CIntelligentPointer<CThingAICreature> Creature; // +0x30
    unsigned __int64 CreatureUID;                   // +0x38
    long DayToBeRecreated;                          // +0x40
    long FrameToBeRecreated;                        // +0x44
    bool Respawnable;                               // +0x48
    bool Guard;                                     // +0x49
    bool CanBeRecreated();                          // 0x0072F9E0
    CThingAICreature* CreateVillager(CTCVillage* village, const C3DVector& position, bool autoPlace);   // 0x0072F750
    void OnInitialActivate();
};

void CTCDummyVillager::OnInitialActivate()
{
    if (HomeBuildingUID) {
        CThing* thing = GetThingManager().FindThingByUID(HomeBuildingUID);
        if (thing) {
            if (thing->ThingType == 3)
                HomeBuilding = CIntelligentPointer<CThingBuilding>((CThingBuilding*)thing);
        } else
            Respawnable = false;
    }
    if (WorkBuildingUID) {
        CThing* thing = GetThingManager().FindThingByUID(WorkBuildingUID);
        if (thing) {
            if (thing->ThingType == 3)
                WorkBuilding = CIntelligentPointer<CThingBuilding>((CThingBuilding*)thing);
        } else
            Respawnable = false;
    }
    if (CreatureUID) {
        CThing* thing = GetThingManager().FindThingByUID(CreatureUID);
        if (thing && thing->ThingType == 1) {
            Creature = CIntelligentPointer<CThingAICreature>((CThingAICreature*)thing);
            CThingAICreature* c = Creature.Get();
            if (!IsAlive(c->WorkBuilding.Get()) && IS_THING_ALIVE(WorkBuilding)) {
                Creature->SetWorkBuilding(WorkBuilding.Get());
                CThingAICreature* creature = Creature.Get();
                if (creature->HasTC(0x90))
                    creature->GetTC(0x90)->OnVillageChanged();
            }
            CThingAICreature* c2 = Creature.Get();
            if (!IsAlive(c2->HomeBuilding.Get()) && IS_THING_ALIVE(HomeBuilding))
                Creature->SetHomeBuilding(HomeBuilding.Get());
            if (Thing->HasTC(0x23)) {
                CTCVillage* village = ((CTCVillageMember*)Thing->GetTC(0x23))->GetVillage();
                if (village)
                    village->AddThingToVillage(Creature.Get());
            }
        }
    }
    if ((!Creature.Get() || (Creature->Flags91 & 1)) && CanBeRecreated() && GetWorld().PClock->Day >= DayToBeRecreated &&
        Thing->HasTC(0x23)) {
        CTCVillage* village = ((CTCVillageMember*)Thing->GetTC(0x23))->GetVillage();
        if (village)
            CreateVillager(village, village->Thing->GetPos(), false);
    }
}
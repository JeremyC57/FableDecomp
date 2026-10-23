#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
#include <stdio.h>
#include <string.h>
// CTCDummyVillager::OnInitialActivate() -- relinks the home/work buildings and the creature from their saved UIDs, re-attaches the creature to its buildings and village, and recreates a dead villager when its day has come
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730070, 752 bytes, `ret`.
// Callees: CTCBase::GetThingManager() const @ 0x00686AE0, CThingManager::FindThingByUID(unsigned __int64) @ 0x0051ED80,
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10,
//          IS_THING_ALIVE<CIntelligentPointer<CThingBuilding>> @ 0x0065F8CA (__fastcall), CThingAICreature::SetWorkBuilding
//          @ 0x00512260, CThingAICreature::SetHomeBuilding @ 0x00512210, LowerBound @ 0x0040F020, the 0x90 component's
//          virtual slot 6, CTCVillageMember::GetVillage() @ 0x007EE540, CTCVillage::AddThingToVillage @ 0x0065B854,
//          CTCDummyVillager::CanBeRecreated() @ 0x0072F9E0, CTCBase::GetWorld() @ 0x00686A70, CThingPhysical::GetPosition()
//          @ 0x004C73D0, CTCDummyVillager::CreateVillager @ 0x0072F750.
// Layout (Ego_r.pdb, matches retail): HomeBuilding +0x10 / UID +0x18, WorkBuilding +0x20 / UID +0x28, Creature +0x30 /
//         UID +0x38, DayToBeRecreated +0x40, Respawnable +0x48; CThing type byte +0x10 (1 creature, 3 building);
//         CThingAICreature HomeBuilding +0x168, WorkBuilding +0x170.
struct C3DVector { float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PData;
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
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
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    const C3DVector& GetPosition() const;           // 0x004C73D0
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

class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void OnVillageChanged();                // slot 6 (+0x18)
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
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
struct CWorld { unsigned char _pad[0x1c]; CGameClock* PClock; };

class CTCDummyVillagerBase : public CTCBase {
public:
    CThingManager* GetThingManager() const;         // 0x00686AE0
    CWorld* GetWorld() const;                       // 0x00686A70
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
        CThing* thing = GetThingManager()->FindThingByUID(HomeBuildingUID);
        if (thing) {
            if (thing->ThingType == 3)
                HomeBuilding = CIntelligentPointer<CThingBuilding>((CThingBuilding*)thing);
        } else
            Respawnable = false;
    }
    if (WorkBuildingUID) {
        CThing* thing = GetThingManager()->FindThingByUID(WorkBuildingUID);
        if (thing) {
            if (thing->ThingType == 3)
                WorkBuilding = CIntelligentPointer<CThingBuilding>((CThingBuilding*)thing);
        } else
            Respawnable = false;
    }
    if (CreatureUID) {
        CThing* thing = GetThingManager()->FindThingByUID(CreatureUID);
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
    if ((!Creature.Get() || (Creature->Flags91 & 1)) && CanBeRecreated() && GetWorld()->PClock->Day >= DayToBeRecreated &&
        Thing->HasTC(0x23)) {
        CTCVillage* village = ((CTCVillageMember*)Thing->GetTC(0x23))->GetVillage();
        if (village)
            CreateVillager(village, village->Thing->GetPosition(), false);
    }
}

// ---- test stubs ----
static int g_setWork, g_setHome, g_villageChanged, g_added, g_created, g_canRecreate;
static CThing* g_uidThings[8];
static CThingManager g_mgr;
static CGameClock g_clock;
static CWorld g_world;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { for (SEntry* e = first; e != last; ++e) if (e->type >= *type) return e; return last; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { PData = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)PData; }
void CBaseIntelligentPointer::SetPItem(CThing* t) { PData = t; }
void CThing::v00() {}
const C3DVector& CThing::GetPosition() const { static C3DVector v; return v; }
void CThingAICreature::SetWorkBuilding(CThingBuilding* b) { ++g_setWork; WorkBuilding.SetPItem(b); }
void CThingAICreature::SetHomeBuilding(CThingBuilding* b) { ++g_setHome; HomeBuilding.SetPItem(b); }
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingBuilding>& t) { return IsAlive(t.GetPItem()); }
void CTCBase::v00() {} void CTCBase::v01() {} void CTCBase::v02() {} void CTCBase::v03() {} void CTCBase::v04() {} void CTCBase::v05() {}
void CTCBase::OnVillageChanged() { ++g_villageChanged; }
static CThing* g_lastAdded;
void CTCVillage::AddThingToVillage(CThing* t) { ++g_added; g_lastAdded = t; }
static CTCVillage g_village;
CTCVillage* CTCVillageMember::GetVillage() { return &g_village; }
CThing* CThingManager::FindThingByUID(unsigned __int64 uid) { return uid < 8 ? g_uidThings[uid] : 0; }
CThingManager* CTCDummyVillagerBase::GetThingManager() const { return &g_mgr; }
CWorld* CTCDummyVillagerBase::GetWorld() const { return &g_world; }
bool CTCDummyVillager::CanBeRecreated() { return g_canRecreate != 0; }
CThingAICreature* CTCDummyVillager::CreateVillager(CTCVillage* v, const C3DVector&, bool) { if (v == &g_village) ++g_created; return 0; }
static void addTC(CThing& t, CInterfaceMap::SEntry* e, long type, CTCBase* c)
{
    t.PresentBits[type >> 5] |= 1UL << (type & 31);
    e->type = type; e->component = c; t.Interfaces.first = e; t.Interfaces.last = e + 1;
}
int main()
{
    int ok = 1;
    static CThing villagerThing; static CThingBuilding home, work; static CThingAICreature creature;
    static CTCVillageMember member; static CTCBase creatureComp;
    static CInterfaceMap::SEntry e1, e2;
    home.ThingType = 3; work.ThingType = 3; creature.ThingType = 1;
    addTC(villagerThing, &e1, 0x23, &member);
    addTC(creature, &e2, 0x90, &creatureComp);
    g_village.Thing = &villagerThing;
    g_uidThings[1] = &home; g_uidThings[2] = &work; g_uidThings[3] = &creature;
    g_world.PClock = &g_clock; g_clock.Day = 10;
    CTCDummyVillager v;
    v.Thing = &villagerThing; v.HomeBuildingUID = 1; v.WorkBuildingUID = 2; v.CreatureUID = 3;
    v.Respawnable = true; v.DayToBeRecreated = 5; g_canRecreate = 1;
    v.OnInitialActivate();
    if (v.HomeBuilding.GetPItem() != &home || v.WorkBuilding.GetPItem() != &work || v.Creature.GetPItem() != &creature) ok = 0;
    if (g_setWork != 1 || g_setHome != 1 || g_villageChanged != 1 || g_added != 1 || g_lastAdded != &creature || g_created != 0) ok = 0;
    if (creature.WorkBuilding.GetPItem() != &work || creature.HomeBuilding.GetPItem() != &home || !v.Respawnable) ok = 0;
    // second pass: creature already has buildings -> no re-set
    v.OnInitialActivate();
    if (g_setWork != 1 || g_setHome != 1 || g_added != 2) ok = 0;
    // dead villager path: unknown home uid, no creature, recreated
    CTCDummyVillager w;
    w.Thing = &villagerThing; w.HomeBuildingUID = 7; w.WorkBuildingUID = 0; w.CreatureUID = 0;
    w.Respawnable = true; w.DayToBeRecreated = 11;
    w.OnInitialActivate();
    if (w.Respawnable || g_created != 0) ok = 0;
    w.DayToBeRecreated = 10;
    w.OnInitialActivate();
    if (g_created != 1) ok = 0;
    if (ok) printf("00730070_TEST PASS\n"); else printf("fail w%d h%d c%d a%d r%d\n", g_setWork, g_setHome, g_villageChanged, g_added, g_created);
    return ok ? 0 : 1;
}
// Grade: DIFFER(657v643) -- frame 0x28 v 0x24: retail builds the ScriptName "" temporary in the dead village argument slot; the Creature flag |= 1 is a load/add/or/store sequence in ours (retail or byte ptr [eax+0x1c9],1); the def release re-reads the pointer; intelligent-pointer temporary vtable store scheduled differently
// CTCDummyVillager::CreateVillager(CTCVillage*, const C3DVector&, bool) -- creates the villager creature at the position, links it to its home/work buildings and the village, sets up its sexuality and notifies the buildable houses
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072F750, 656 bytes, `ret 0Ch`, returns the created CThingAICreature*.
// Callees: CTCVillage::GetNextGuardTypeAndIncrement @ 0x00658FA2, CCharString(const char*, long) @ 0x0099EBF0,
//          CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0, CThingAICreature::Create @ 0x00833800
//          (__fastcall), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10,
//          CTCVillage::AddThingToVillage @ 0x0065B854, CDefClassBase::GetSubDef<CVillageDef> @ 0x00512FE0,
//          CTCVillage::GetNumberOfGayVillagers @ 0x00658F41, CThingAICreature::AutoSetupSexuality(float) @ 0x008330D0,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCBuyableHouse::MovedInNotification @ 0x006BF860.
// Layout (Ego_r.pdb, matches retail): CTCDummyVillager DefType +0x0C, HomeBuilding +0x10, WorkBuilding +0x20,
//         Creature +0x30, DayToBeRecreated +0x40, FrameToBeRecreated +0x44, Guard +0x49; CThingAICreatureInit 0x1C bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* s, long len = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CVillageDef : public CDefClassBase {
public:
    unsigned char _pad08[0x34 - 0x08];
    float GayVillagerRatio;                         // +0x34
};
class CThingDefHolder : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CVillageDef>* out) const;   // 0x00512FE0
};

class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 4];
    unsigned long PresentBits[9];                   // +0x20
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CThingDefHolder* PDef;                          // +0x70
    unsigned char _pad74[0x91 - 0x74];
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
};
class CThingBuilding : public CThing { };
class CThingAICreature : public CThing {
public:
    unsigned char _pad94[0x1c9 - 0x94];
    unsigned char Flags1C9;                         // +0x1C9
    void AutoSetupSexuality(float gayRatio);        // 0x008330D0
};

struct CThingAICreatureInit {
    CThing*         OriginalThing;                  // +0x00
    CThing*         PreviousThing;                  // +0x04
    CCharString     ScriptName;                     // +0x08
    CCharString     OverridingBrainName;            // +0x0C
    bool            PlayerCharacter;                // +0x10
    bool            Drawable;                       // +0x11
    bool            PlaceAtEndOfList;               // +0x12
    bool            EnableCreatureAutoPlacing;      // +0x13
    CThingBuilding* HomeBuilding;                   // +0x14
    CThingBuilding* WorkBuilding;                   // +0x18
    CThingAICreatureInit(CThing* original = 0, CThing* previous = 0, bool playerCharacter = false,
                         const CCharString& scriptName = "", const CCharString& overridingBrainName = "", bool drawable = true)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName),
          OverridingBrainName(overridingBrainName), PlayerCharacter(playerCharacter), Drawable(drawable),
          PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false), HomeBuilding(0), WorkBuilding(0)
    {
    }
};
CThingAICreature* __fastcall CreateAICreature(long defIndex, const C3DVector* position, long flags, CThingAICreatureInit* init);   // 0x00833800

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
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
    T* operator->() const { return (T*)GetPItem(); }
    T* Get() const { return (T*)GetPItem(); }
};

class CTCBuyableHouse {
public:
    void MovedInNotification(long day);             // 0x006BF860
};
class CTCVillage {
public:
    virtual void v00();
    CThing* Thing;                                  // +0x04
    long GetNextGuardTypeAndIncrement();            // 0x00658FA2
    void AddThingToVillage(CThing* thing);          // 0x0065B854
    long GetNumberOfGayVillagers();                 // 0x00658F41
};

class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
};

class CTCDummyVillager : public CTCBase {
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
    CThingAICreature* CreateVillager(CTCVillage* village, const C3DVector& position, bool autoPlace);
};

CThingAICreature* CTCDummyVillager::CreateVillager(CTCVillage* village, const C3DVector& position, bool autoPlace)
{
    if (Guard)
        DefType = village->GetNextGuardTypeAndIncrement();
    CThingAICreatureInit init;
    init.HomeBuilding = HomeBuilding.Get();
    init.WorkBuilding = WorkBuilding.Get();
    init.EnableCreatureAutoPlacing = !autoPlace;
    Creature = CreateAICreature(DefType, &position, 4, &init);
    village->AddThingToVillage(Creature.Get());
    ((CThingAICreature*)Creature.GetPItem())->Flags1C9 |= 1;
    CDefPointer<const CVillageDef> villageDef;
    float gayRatio = 0.0f;
    if (village->Thing->PDef->GetSubDef(&villageDef)) {
        gayRatio = villageDef->GayVillagerRatio;
        if (gayRatio > 0.0f && village->GetNumberOfGayVillagers() <= 0)
            gayRatio = 1.0f;
    }
    Creature->AutoSetupSexuality(gayRatio);
    if (HomeBuilding.Get() && !(HomeBuilding->Flags91 & 1)) {
        CThingBuilding* building = HomeBuilding.Get();
        if (building->HasTC(0xd5))
            ((CTCBuyableHouse*)building->GetTC(0xd5))->MovedInNotification(DayToBeRecreated);
    }
    if (WorkBuilding.Get() && !(WorkBuilding->Flags91 & 1)) {
        CThingBuilding* building = WorkBuilding.Get();
        if (building->HasTC(0xd5))
            ((CTCBuyableHouse*)building->GetTC(0xd5))->MovedInNotification(DayToBeRecreated);
    }
    DayToBeRecreated = 0x7fffffff;
    FrameToBeRecreated = 0x7fffffff;
    return Creature.Get();
}

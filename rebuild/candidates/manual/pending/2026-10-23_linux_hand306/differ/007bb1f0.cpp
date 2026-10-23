// Grade: DIFFER(959v960) -- flow/calls match; GetTC map register ebp v ebx, the ParentThing load for AttachOpinionSource scheduled before GetWorld in retail, Trespassed store placement, deed-escalation tail register allocation
// CTCTrespasser::FrameUpdate() -- track the building the creature is inside: on entering a new private building reset, mark it trespassed, gather its occupants and attach the trespasser opinion source (or post the inside-building deed for a public one); while trespassing escalate and post the trespass deeds; after leaving let the deed level decay
// Leaf: ?FrameUpdate@CTCTrespasser
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BB1F0, 960 bytes, plain `ret`.
// Callees: LowerBound 0x0040F020 (interfaces 0x75 location, 0xBD building trespass, 0x51 deed log, 0x53); CBaseIntelligentPointer()
//   0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~ 0x00A01C10; the virtual reset (slot 28, +0x70); CTCBase
//   GetThingSearchTools 0x00686D90 / GetWorld 0x00686A70; CThingSearchTools::GetAllThings<..HasBuildingAsHomeOrWork>
//   0x007BB660; COpinionReactionManager::AttachOpinionSource 0x00700780; CTCHeroOpinionDeedLog::PostDeed 0x006C6990;
//   CThing::GetPosition 0x004C73D0; CTCTrespasser::GetDeedType 0x007BADD0 / SeeIfAnyOccupantsKnowAboutDeed 0x007BB150;
//   CWorld::GetWorldFrame 0x0049D870; CCharString 0x0099EBF0 / ~ 0x0099EAE0; frame rate 0x01375550.
// Layout (retail CTCTrespasser = Ego_r.pdb, packed pointer): InsideBuildingNotTrespassingDeedHandle +0x0c, DeedHandles
//   +0x10, CurrentDeedHandleIndex +0x1c, OccupantsKnowOfDeedHandleIndex +0x20, FrameSomeoneWasInformedOfLastDeed +0x24,
//   FramePlayerLeft +0x28, AttachedOpinionSource +0x2c, TrespassingThing +0x2d, OccupantsOfCurrentTrespassingThing +0x38.
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CBaseObject;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
#pragma pack(pop)
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags & 1))
            return true;
    }
    return false;
}
struct CTCLocation {                                    // interface 0x75 (descriptive)
    unsigned char _pad00[0x0c];
    CBaseIntelligentPointer InsideBuilding;             // +0x0c
};
struct CTCBuildingTrespass {                            // interface 0xBD (descriptive)
    unsigned char _pad00[0x0c];
    bool Private;                                       // +0x0c
    bool Trespassed;                                    // +0x0d
};
struct CTCHeroOpinionDeedLog {                          // interface 0x51
    long PostDeed(long type, const C3DVector* pos, CThing* target, CThing* creature, long postType, bool a, bool b,
                  void* faction, bool c, long d);       // 0x006C6990
};
struct CWitnessList { void* _M_head; };
struct CThingFilter_HasBuildingAsHomeOrWork { CThing* Building; CThingFilter_HasBuildingAsHomeOrWork(CThing* b) : Building(b) { } };
struct CThingSearchTools {
    void GetAllThings(const CThingFilter_HasBuildingAsHomeOrWork& filter, CWitnessList* out);   // 0x007BB660
};
struct COpinionReactionManager {
    bool AttachOpinionSource(CThing* thing, const CCharString& name, void* source);   // 0x00700780
};
struct CWorld {
    unsigned char _pad00[0x74];
    COpinionReactionManager* OpinionReactions;          // +0x74
    static unsigned long GetWorldFrame();               // 0x0049D870
};
extern long g_FrameRate;                                // 0x01375550

__forceinline bool IsUntrespassedPrivate(CThing* building)
{
    if (building && !(building->m_flags & 1) && building->HasTC(0xbd)) {
        CTCBuildingTrespass* trespass = building->GetTC<CTCBuildingTrespass>(0xbd);
        if (trespass->Private && !trespass->Trespassed)
            return true;
    }
    return false;
}
#pragma pack(push, 1)
struct CTCTrespasser {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void Reset();                               // slot 28 (+0x70)
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    long InsideBuildingNotTrespassingDeedHandle;        // +0x0c
    long DeedHandles[3];                                // +0x10
    long CurrentDeedHandleIndex;                        // +0x1c
    long OccupantsKnowOfDeedHandleIndex;                // +0x20
    unsigned long FrameSomeoneWasInformedOfLastDeed;    // +0x24
    unsigned long FramePlayerLeft;                      // +0x28
    bool AttachedOpinionSource;                         // +0x2c
    CIntelligentPointer<CThing> TrespassingThing;       // +0x2d
    unsigned char _pad35[0x38 - 0x35];
    CWitnessList OccupantsOfCurrentTrespassingThing;    // +0x38
    CWorld* GetWorld();                                 // 0x00686A70
    CThingSearchTools* GetThingSearchTools();           // 0x00686D90
    long GetDeedType(long index);                       // 0x007BADD0
    bool SeeIfAnyOccupantsKnowAboutDeed(long index, long type);   // 0x007BB150
    void FrameUpdate();
};
#pragma pack(pop)

void CTCTrespasser::FrameUpdate()
{
    if (!ParentThing->HasTC(0x75))
        return;
    CBaseIntelligentPointer& inside = ParentThing->GetTC<CTCLocation>(0x75)->InsideBuilding;
    CThing* building = inside.GetPItem();
    if (building != TrespassingThing.GetPItem() || IsUntrespassedPrivate(building)) {
        Reset();
        if (building && !(building->m_flags & 1) && building->HasTC(0xbd)) {
            CTCBuildingTrespass* trespass = building->GetTC<CTCBuildingTrespass>(0xbd);
            if (trespass->Private) {
                trespass->Trespassed = true;
                GetThingSearchTools()->GetAllThings(CThingFilter_HasBuildingAsHomeOrWork(building), &OccupantsOfCurrentTrespassingThing);
                FrameSomeoneWasInformedOfLastDeed = 0;
                AttachedOpinionSource = GetWorld()->OpinionReactions->AttachOpinionSource(ParentThing, "OPINION_SOURCE_TRESPASSER", 0);
            } else if (ParentThing->HasTC(0x51)) {
                CThing* parent = ParentThing;
                InsideBuildingNotTrespassingDeedHandle = parent->GetTC<CTCHeroOpinionDeedLog>(0x51)->PostDeed(0x4d, &parent->GetPosition(), 0, 0, 2, false, true, 0, false, 0);
            }
        }
    }
    TrespassingThing = CIntelligentPointer<CThing>(building);
    CThing* current;
    if (IS_THING_ALIVE(TrespassingThing) && (current = TrespassingThing.GetPItem())->HasTC(0xbd)
        && current->GetTC<CTCBuildingTrespass>(0xbd)->Trespassed) {
        long last = CurrentDeedHandleIndex - 1;
        bool noDeed = last < 0 || DeedHandles[last] == -1;
        if (CurrentDeedHandleIndex < 3) {
            if (!noDeed && OccupantsKnowOfDeedHandleIndex != last && !SeeIfAnyOccupantsKnowAboutDeed(last, GetDeedType(last))) {
                if (TrespassingThing.GetPItem()->HasTC(0x53))
                    CurrentDeedHandleIndex = 3;
            } else if (CWorld::GetWorldFrame() - FrameSomeoneWasInformedOfLastDeed > (unsigned long)(g_FrameRate * 5))
                ++CurrentDeedHandleIndex;
        }
        if (ParentThing->HasTC(0x51)) {
            CTCHeroOpinionDeedLog* log = ParentThing->GetTC<CTCHeroOpinionDeedLog>(0x51);
            for (long i = 0; i < CurrentDeedHandleIndex; ++i) {
                if (i >= 3)
                    break;
                if (DeedHandles[i] == -1)
                    DeedHandles[i] = log->PostDeed(GetDeedType(i), &ParentThing->GetPosition(), inside.GetPItem(), 0, 2, true, true, 0, false, 0);
            }
        }
    }
    if (!IS_THING_ALIVE(TrespassingThing) && CurrentDeedHandleIndex > 0) {
        unsigned long left = FramePlayerLeft;
        unsigned long delay = g_FrameRate * 10;
        if (CWorld::GetWorldFrame() - left > delay) {
            --CurrentDeedHandleIndex;
            FramePlayerLeft = left + delay;
        }
    }
}

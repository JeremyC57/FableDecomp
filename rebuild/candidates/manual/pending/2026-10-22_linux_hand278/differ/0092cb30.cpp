// CAIStateGroup_GoToBed::IsHeroTrespassingPerceived() -- true when the living hero stands inside this creature's home (then work) building, the building is trespassable (CTCTrespassable 0xbd flag) and the creature has heard him (CTCPerceiveThing 0x34)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092CB30, length 610 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
// CPlayer::GetHeroThing @ 0x00487DC0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound (const) @ 0x004365B0.
// Shape: two independent `IS_ALIVE(hero) && ...` chains, one per building (HomeBuilding +0x168, WorkBuilding +0x170); every
// building access re-reads the brain's creature and calls GetPItem again, as retail does.
// Layout: CThing interface bitset at +0x20, interface map +0x44, flags +0x91 (retail); CThingAICreature HomeBuilding +0x168 /
// WorkBuilding +0x170 (landed 0x00931010). Interface 0x75 is the hero's building tracker (intelligent pointer at +0x0c).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline void GetInterface(long interfaceType, T** out) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        *out = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CTCBuildingTracker {                     // interface 0x75 (on the hero)
    unsigned char _pad0[0x0c];
    CBaseIntelligentPointer Building;           // +0x0c
};
struct CTCTrespassable {                        // interface 0xbd
    unsigned char _pad0[0x0c];
    bool Trespassable;                          // +0x0c
};
struct CTCPerceiveThing {                       // interface 0x34
    unsigned char _pad0[0x32];
    bool Heard;                                 // +0x32
};

inline bool IS_THING_ALIVE(const CThing* thing) { return thing != 0 && !(thing->m_flags91 & 1); }

struct CThingAICreature : CThing {
    unsigned char _pad94[0x168 - 0x94];
    CBaseIntelligentPointer HomeBuilding;       // +0x168
    CBaseIntelligentPointer WorkBuilding;       // +0x170
};

struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};

struct CAIBrain {
    virtual void v00();
    unsigned char _pad[0x14];
    CThingAICreature* PCreature;                // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    bool IsHeroTrespassingPerceived();
};

bool CAIStateGroup_GoToBed::IsHeroTrespassingPerceived()
{
    const CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (IS_THING_ALIVE(hero) && hero->HasTC(0x75)) {
        CTCBuildingTracker* tracker = hero->GetTC<CTCBuildingTracker>(0x75);
        if (IS_THING_ALIVE(GetCreature()->HomeBuilding.GetPItem())) {
            const CThing* home = GetCreature()->HomeBuilding.GetPItem();
            CTCTrespassable* trespassable;
            if (home->HasTC(0xbd) && (home->GetInterface(0xbd, &trespassable), trespassable->Trespassable)
                && tracker->Building.GetPItem() == GetCreature()->HomeBuilding.GetPItem()
                && GetCreature()->HasTC(0x34) && GetCreature()->GetTC<CTCPerceiveThing>(0x34)->Heard)
                return true;
        }
    }
    if (IS_THING_ALIVE(hero) && hero->HasTC(0x75)) {
        CTCBuildingTracker* tracker = hero->GetTC<CTCBuildingTracker>(0x75);
        if (IS_THING_ALIVE(GetCreature()->WorkBuilding.GetPItem())) {
            const CThing* work = GetCreature()->WorkBuilding.GetPItem();
            CTCTrespassable* trespassable;
            if (work->HasTC(0xbd) && (work->GetInterface(0xbd, &trespassable), trespassable->Trespassable)
                && tracker->Building.GetPItem() == GetCreature()->WorkBuilding.GetPItem()
                && GetCreature()->HasTC(0x34) && GetCreature()->GetTC<CTCPerceiveThing>(0x34)->Heard)
                return true;
        }
    }
    return false;
}

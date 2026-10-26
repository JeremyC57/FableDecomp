// CTCCreatureGroupBoastingCrowd::~CTCCreatureGroupBoastingCrowd() -- while its hero is still alive: leave the crowd states, clear every opinion source the crowd pushed on the hero, and drop the hero's boast (interface 0x73); then the hero pointer and the creature-group base go
// Leaf: ??1CTCCreatureGroupBoastingCrowd
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00784BD0, 196 bytes (plain `ret`). Vtable 0x0126B5DC.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCCreatureGroupBoastingCrowd::EnterCrowdState(long) @ 0x00784260,
// CTCBase::GetWorld() @ 0x00686A70, COpinionReactionManager::ClearOpinionSource(CThing*, bool) @ 0x007008C0, CVectorMap<
// ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the interface-0x73 component's boast removal @ 0x00783240 (manifest
// "CTCCreatureGroupBoastingCrowd_RemoveBoast"; named descriptively), ~CBaseIntelligentPointer @ 0x00A01C10, ~CTCCreatureGroup
// @ 0x004E5C71 (out of line: destroys the member map, then ~CTCBase; manifest label "_Dest_val<...>" is propagated).
// Layout: local model -- engine/CTCCreatureGroupBoastingCrowd.h is Ego_r.pdb (PHero +0x28, HeroOpinionSourcesPushed +0x40); the
// retail CTCCreatureGroup base is 0x24 bytes (its STLport map is 12 bytes), so PHero sits at +0x24 and the pushed-source count
// at +0x3c. CWorld's opinion reaction manager +0x74; CThing bitset +0x20 (0x73 = bit 19 of the +0x2c word), dead bit +0x91.
// Shape: a user destructor (own vtable store first); the manager is read into a named local before ClearOpinionSource's pushes;
// the count loop is `while (count > 0) { ...; --count; }` (rotated, the decrement stored then tested); the boast component is
// a named local loaded before its argument's GetPItem; the hero pointer member has no destructor of its own.
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public: 
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};   // CIntelligentPointer<CThing>
class COpinionReactionManager { public:
    void ClearOpinionSource(CThing* thing, bool flag);  // 0x007008C0
};
class CWorld { public:
    unsigned char            _pad00[0x74];
    COpinionReactionManager* OpinionReactionManager;    // +0x74
};
struct CTCBoastComponent {                              // the hero's interface-0x73 component
    void RemoveBoast(CThing* hero);                     // 0x00783240
};
class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCCreatureGroup : public CTCBase { public:                     // retail 0x24 bytes
    unsigned long MemberCount;                          // +0x0c
    unsigned long ActiveMemberCount;                    // +0x10
    bool          ShutdownOnEmpty;                      // +0x14
    void*         Members[3];                           // +0x18 STLport map (12 bytes)
    virtual ~CTCCreatureGroup();                        // 0x004E5C71
};
class CTCCreatureGroupBoastingCrowd : public CTCCreatureGroup { public:   // vtable 0x0126B5DC
    CIntelligentPointer<CThing> PHero;                          // +0x24 (PDB +0x28)
    long CrowdState;                                    // +0x2c
    long CurrentEncouragementState;                     // +0x30
    bool HeroBoasted;                                   // +0x34
    bool HeroMadeGoodBoast;                             // +0x35
    long CurrentOpinionDeedHandle;                      // +0x38
    long HeroOpinionSourcesPushed;                      // +0x3c (PDB +0x40)
    long FrameToClearRemainingOpinionSources;           // +0x40
    long FrameToLeavePostBoastState;                    // +0x44
    void EnterCrowdState(long state);                   // 0x00784260
    virtual ~CTCCreatureGroupBoastingCrowd();
};

CTCCreatureGroupBoastingCrowd::~CTCCreatureGroupBoastingCrowd()
{
    if (PHero.GetPItem() && !PHero.GetPItem()->IsDead()) {
        EnterCrowdState(0);
        while (HeroOpinionSourcesPushed > 0) {
            COpinionReactionManager* manager = GetWorld().OpinionReactionManager;
            manager->ClearOpinionSource(PHero.GetPItem(), true);
            --HeroOpinionSourcesPushed;
        }
        CThing* hero = PHero.GetPItem();
        if (hero->HasTC(0x73)) {
            CTCBoastComponent* boast = (CTCBoastComponent*)hero->GetTC(0x73);
            boast->RemoveBoast(PHero.GetPItem());
        }
    }
}
// CTCFollowed::QuitAIFollower(CThingAICreature* creature) -- release an AI follower: drop it from both follower lists, tell the follow-action component, post a "stopped following" opinion deed at the hero, clear its emote icon, and if it has neither a live home nor a live work building stop persisting it
// Leaf: QuitAIFollower
//
// Retail 0x006D56E0, 358 bytes (`ret 4`).
// Callees: CTCFollowed::IsAIFollower @ 0x006D4DB0, list<CIntelligentPointer<CThingAICreature>>::remove @ 0x0065DE48 and
// list<CIntelligentPointer<CThing>>::remove @ 0x0065DE8A (catalog "SetScriptBrushThingsToMove", propagated), CBaseIntelligentPointer
// ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, the component-map LowerBound @ 0x0040F020
// (kinds 0x7a and 0x51), the 0x7a component's follower handler @ 0x006AEBA0, CThing::GetPosition @ 0x004C73D0,
// CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990 (deed 0x31), CTCFollowed::RemoveEmoteIcon @ 0x006D5050, CThing::SetAsGameStatePersisted
// @ 0x004C7880.
// Layout: Ego_r.pdb CTCFollowed Followers +0x0c, AIFollowers +0x10 (retail one-pointer lists); CThingAICreature HomeBuilding +0x168,
// WorkBuilding +0x170 (PDB +0x190/+0x198); CThing bitset +0x20, map +0x44, Dead bit 0 of +0x91, LevelPersistent bit 4 of +0x92.
// Shape: each remove gets an intelligent-pointer temporary (base ctor, vtable store, SetPItem, direct dtor); the two lookups are the
// out-parameter GetInterface with the result unchecked; the deed lookup's thing is a named local reused for the position.
class CTCBase;
class C3DVector;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte
// {type, component} entries): VC7.1 then knows the key's address does not escape.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char _flags91 : 7;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;        // +0x92
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    template<class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;            // 0x004C73D0
    void SetAsGameStatePersisted(bool persisted);           // 0x004C7880
};
class C3DVector { public: float X, Y, Z; };
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10 (slot 0)
    void* PData;
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CThing* item);                            // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
class CThingBuilding : public CThing { };
class CThingAICreature : public CThing {
public:
    unsigned char _pad94[0x168 - 0x94];
    CIntelligentPointer<CThingBuilding> HomeBuilding;       // +0x168
    CIntelligentPointer<CThingBuilding> WorkBuilding;       // +0x170
};
static inline bool IsAlive(const CThing* thing) { return thing && !thing->Dead; }

template <class T> struct _List_node;
template <class T> struct CRetailList {                     // STLport list: the header node pointer
    _List_node<T>* _M_node;
    void remove(const T& value);                            // 0x0065DE48 / 0x0065DE8A
};

struct CTCFollowActionHandler {
    void HandleFollowerQuit(CThing* follower);              // 0x006AEBA0 (component 0x7a)
};
class CFaction;
class CTCHeroOpinionDeedLog { public:
    long PostDeed(long deed, const C3DVector& position, CThing* doer, CThingAICreature* creature, long postType, bool a, bool b,
                  CFaction* faction, bool c, long d);       // 0x006C6990
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};
class CTCFollowed : public CTCBase {
public:
    CRetailList<CIntelligentPointer<CThing> >           Followers;     // +0x0c
    CRetailList<CIntelligentPointer<CThingAICreature> > AIFollowers;   // +0x10
    bool IsAIFollower(CThingAICreature* creature);          // 0x006D4DB0
    void RemoveEmoteIcon(CThingAICreature* creature);       // 0x006D5050
    void QuitAIFollower(CThingAICreature* creature);
};

void CTCFollowed::QuitAIFollower(CThingAICreature* creature)
{
    if (IsAIFollower(creature)) {
        AIFollowers.remove(CIntelligentPointer<CThingAICreature>(creature));
        Followers.remove(CIntelligentPointer<CThing>(creature));
        CTCFollowActionHandler* handler;
        ParentThing->GetInterface(&handler, 0x7a);
        handler->HandleFollowerQuit(creature);
        CThing* thing = ParentThing;
        CTCHeroOpinionDeedLog* deeds;
        thing->GetInterface(&deeds, 0x51);
        deeds->PostDeed(0x31, thing->GetPosition(), creature, creature, 1, false, false, 0, false, 0);
        RemoveEmoteIcon(creature);
        if (!IsAlive(creature->HomeBuilding.GetPItem()) && !IsAlive(creature->WorkBuilding.GetPItem())) {
            creature->SetAsGameStatePersisted(false);
            creature->LevelPersistent = false;
        }
    }
}
// CTCFollowed::QuitAIFollower(CThingAICreature* creature) -- release an AI follower: drop it from both follower lists, tell the follow-action component, post a "stopped following" opinion deed at the hero, clear its emote icon, and if it has neither a live home nor a live work building stop persisting it
// Leaf: QuitAIFollower
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
//
// Retail 0x006D56E0, 358 bytes (`ret 4`).
// Callees: CTCFollowed::IsAIFollower @ 0x006D4DB0, list<CIntelligentPointer<CThingAICreature>>::remove @ 0x0065DE48 and
// list<CIntelligentPointer<CThing>>::remove @ 0x0065DE8A (catalog "SetScriptBrushThingsToMove", propagated), CBaseIntelligentPointer
// ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, the component-map LowerBound @ 0x0040F020
// (kinds 0x7a and 0x51), the 0x7a component's follower handler @ 0x006AEBA0, CThing::GetPos @ 0x004C73D0,
// CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990 (deed 0x31), CTCFollowed::RemoveEmoteIcon @ 0x006D5050, CThing::SetAsGameStatePersisted
// @ 0x004C7880.
// Layout: Ego_r.pdb CTCFollowed Followers +0x0c, AIFollowers +0x10 (retail one-pointer lists); CThingAICreature HomeBuilding +0x168,
// WorkBuilding +0x170 (PDB +0x190/+0x198); CThing bitset +0x20, map +0x44, Dead bit 0 of +0x91, LevelPersistent bit 4 of +0x92.
// Shape: each remove gets an intelligent-pointer temporary (base ctor, vtable store, SetPItem, direct dtor); the two lookups are the
// out-parameter GetInterface with the result unchecked; the deed lookup's thing is a named local reused for the position.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
class CTCBase;
#include "engine/C3DVector.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char _flags91 : 7;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;        // +0x92
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
            *ppOut = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;            // 0x004C73D0
    void SetAsGameStatePersisted(bool persisted);           // 0x004C7880
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
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThingBuilding : public CThing { };
class CThingAICreature : public CThing {
public:
    unsigned char _pad94[0x168 - 0x94];
    CIntelligentPointer<CThingBuilding> HomeBuilding;       // +0x168
    CIntelligentPointer<CThingBuilding> WorkBuilding;       // +0x170
};
static inline bool IsAlive(const CThing* thing) { return thing && !thing->Dead; }

// The follower lists are the real STLport 4.6 std::list (stlport/fable_containers.h, via engine/CVectorMap.h); remove is declared only
// (FABLE_RETAIL_STLP_LIST_REMOVE_DECL): retail calls the instances out of line.

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
    std::list<CIntelligentPointer<CThing> >           Followers;     // +0x0c
    std::list<CIntelligentPointer<CThingAICreature> > AIFollowers;   // +0x10
    bool IsAIFollower(const CThingAICreature* creature) const;   // 0x006D4DB0
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
        deeds->PostDeed(0x31, thing->GetPos(), creature, creature, 1, false, false, 0, false, 0);
        RemoveEmoteIcon(creature);
        if (!IsAlive(creature->HomeBuilding.GetPItem()) && !IsAlive(creature->WorkBuilding.GetPItem())) {
            creature->SetAsGameStatePersisted(false);
            creature->LevelPersistent = false;
        }
    }
}
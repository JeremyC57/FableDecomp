// CIsOpinionSourceWithReaction::operator()(CThing* source) -- true when the thing is an opinion source the state group should react to: not itself (unless allowed), remember it, work out the 79 reaction flags (the hard-coded ones, follower gating and each reaction's can-react test), get a reaction type from the reaction manager, then the unknown-threat and awareness checks
// Grade: DIFFER(443v451) -- control flow and calls match; frame 0x24 vs 0x20 (retail shares the GetInterface key, the CIntelligentPointer temporary and the strength-reduced loop offset in one slot), the owner is compared from memory instead of edi, and the line-of-sight bool is zero-extended instead of retail's arg-slot byte store + dword reload
// Retail 0x0095FB40, 450 bytes, `ret 4`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0 (const copy), CBaseIntelligentPointer ctor
//   0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer vtable 0x01238C6C),
//   the per-reaction can-react member pointers (table 0x013BB758, 79 entries of 0x28 bytes, the 16-byte pointer
//   first), COpinionReactionManager::GetAttitudeOrReactionType 0x00701010, __fastcall IsAwareOfThingImplementation
//   0x00864700 (`ret 4`: creature, thing, line-of-sight flag).
// Layout: this as the landed ctor 0x0095F9A0 plus Reaction +0x10 (PDB). CAIStateGroup_OpinionReactionRoot (PDB = retail):
//   Info +0x04 (creature +0x18), ReactionFlags bool[79] +0x1C, ThingReactingTo +0x6B (unaligned, so the model is
//   packed). CTCOpinionSource (retail = PDB less 4 from the narrower DefStack vector): owner +0x04, UseNavLineOfSight
//   +0x18, ReactToSelfPermitted +0x74, AlwaysAwareOf +0x75, UnknownThreatSource +0x76; interface 0x50. Reaction manager
//   per-reaction follower flags +0x4FD. CTCCreatureOpinionOfHero target pointer +0x344. CThing UID +0x18 (two words).
// Shape: the GetInterface out-parameter idiom of the landed ctor; the flag is one `&&`/`||` expression stored to the
//   group and re-read; the can-react member pointer is copied to a local before the null test.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CUID {
    long Low, High;
    bool operator==(const CUID& other) const { return Low == other.Low && High == other.High; }
};
class CThing {
public:
    unsigned char _pad00[0x18];
    CUID          UID;                              // +0x18
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingAICreature : public CThing { };
bool __fastcall IsAwareOfThingImplementation(CThingAICreature* creature, CThing* thing, bool lineOfSight);   // 0x00864700
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    void* m_pItem;
    void SetPItem(void* item);                      // 0x00A01B90
    void* GetPItem() const;                         // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CTCOpinionSource {
    void*         __vftable;
    CThing*       Owner;                            // +0x04
    unsigned char _pad08[0x10];
    bool          UseNavLineOfSight;                // +0x18
    unsigned char _pad19[0x74 - 0x19];
    bool          ReactToSelfPermitted;             // +0x74
    bool          AlwaysAwareOf;                    // +0x75
    bool          UnknownThreatSource;              // +0x76
};
struct CTCCreatureOpinionOfHero {
    unsigned char               _pad00[0x344];
    CIntelligentPointer<CThing> Target;             // +0x344
};
struct CCreatureInfo {
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                     // +0x18
};
struct CAIStateGroup_OpinionReactionRoot;
typedef bool (CAIStateGroup_OpinionReactionRoot::*CanReactFunc)();   // 16 bytes (declared before the class)
struct SReactionInfo {                              // 0x28 bytes
    CanReactFunc  CanReact;                         // +0x00
    unsigned char _pad10[0x18];
};
extern SReactionInfo g_ReactionInfo[79];            // 0x013BB758
#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot {
    void*                       __vftable;
    CCreatureInfo*              Info;               // +0x04
    unsigned char               _pad08[0x14];
    bool                        ReactionFlags[79];  // +0x1C
    CIntelligentPointer<CThing> ThingReactingTo;    // +0x6B
    CThingAICreature* GetCreature() { return Info->Creature; }
};
#pragma pack(pop)
struct COpinionReactionManager {
    unsigned char _pad00[0x4fd];
    bool          FollowerReactionFlags[79];        // +0x4FD
    long GetAttitudeOrReactionType(CThing* thing, bool flag, bool* reactionFlags, CTCOpinionSource* source);   // 0x00701010
};

class CIsOpinionSourceWithReaction {
public:
    CAIStateGroup_OpinionReactionRoot* PStateGroup;    // +0x00
    CTCCreatureOpinionOfHero*          PTCOpinion;     // +0x04
    COpinionReactionManager*           ReactionManager;// +0x08
    bool                               IsFollowingPlayer;            // +0x0c
    bool                               IsAwareOfPlayerForReactions;  // +0x0d
    long                               Reaction;                     // +0x10
    bool operator()(CThing* source);
};

bool CIsOpinionSourceWithReaction::operator()(CThing* source)
{
    CTCOpinionSource* opinionSource;
    if (source->GetInterface(&opinionSource, 0x50)
        && (opinionSource->ReactToSelfPermitted || source->UID.Low != PStateGroup->GetCreature()->UID.Low
            || source->UID.High != PStateGroup->GetCreature()->UID.High)) {
        PStateGroup->ThingReactingTo = CIntelligentPointer<CThing>(source);
        for (unsigned long i = 0; i < 79; i++) {
            PStateGroup->ReactionFlags[i] = i >= 18 && (!IsFollowingPlayer || ReactionManager->FollowerReactionFlags[i]);
            if (PStateGroup->ReactionFlags[i]) {
                CanReactFunc canReact = g_ReactionInfo[i].CanReact;
                PStateGroup->ReactionFlags[i] = (!canReact || (PStateGroup->*canReact)()) ? true : false;
            }
        }
        Reaction = ReactionManager->GetAttitudeOrReactionType(PStateGroup->GetCreature(), false, PStateGroup->ReactionFlags, opinionSource);
        if (Reaction
            && (!opinionSource->UnknownThreatSource
                || (!IsAwareOfPlayerForReactions && PTCOpinion && opinionSource->Owner == PTCOpinion->Target.GetPItem()))
            && (opinionSource->AlwaysAwareOf
                || IsAwareOfThingImplementation(PStateGroup->GetCreature(), source, opinionSource->UseNavLineOfSight)))
            return true;
    }
    return false;
}

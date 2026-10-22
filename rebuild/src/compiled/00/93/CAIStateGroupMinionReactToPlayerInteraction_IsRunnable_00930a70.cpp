// CAIStateGroup_MinionReactToPlayerInteraction::IsRunnable() -- react to the live hero's pending interaction event: a villager in a guarded village (guards on duty) who knows of an unhandled crime only reacts when married to the hero; a type-5 interaction is runnable when the hero is too far or too near (reaction 5); otherwise pick the first reaction slot whose next-reaction frame has passed (slot 2 always, the others only while the 0xC3 component's flag is set) and test the interaction; a failure calls InteractionFailed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00930A70, length 559 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: GetPWorld @ 0x008FCFC0, CWorld::GetCurrentPlayer @ 0x0049D9D0, CPlayer::GetPCharacter @ 0x00487DC0,
// FindPlayerInteractionEvent @ 0x0092EDC0, CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface 0x23
// CTCVillageMember, 0x4F CTCCreatureOpinionOfHero, 0xD7 CTCWife, 0xC3), CTCVillageMember::GetPVillage @ 0x007EE540 (manifest
// "IS_THING_ALIVE" is wrong), CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsCrimeToRunAndFindAGuard> @
// 0x006610C7, the village's deed test @ 0x0065914B (manifest "CBankFileAsyncData::StartRead" is wrong), CTCWife::
// IsMarriedToThePlayer @ 0x006CEC80, IsTooFarFromThePlayer @ 0x0092ECD0, IsTooNearToThePlayer @ 0x0092ED40,
// GetWorldFrame @ 0x0049D870, TestOrDoInteraction @ 0x0092FAB0, InteractionFailed @ 0x0092F240.
// Layout: engine/CAIStateGroup_MinionReactToPlayerInteraction.h flattens the base and types ReactionType as bytes, so the
// model is local: NextReactionFrame[5] +0x14, ReactionType +0x28 (Ego_r.pdb); the village's +0x88 count and the event's
// +0x3c type are descriptive.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for the const CVectorMap::LowerBound copy (STLport lower_bound over the 8-byte entries).
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct COpinionDeed {
    long Type;                                          // +0x00
    unsigned char _pad04[0x24 - 0x04];
    long Handle;                                        // +0x24
};
struct COpinionDeedFilter_IsCrimeToRunAndFindAGuard {
    COpinionDeedFilter_IsCrimeToRunAndFindAGuard() { }
};
struct CTCCreatureOpinionOfHero {                       // interface 0x4f
    COpinionDeed* FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard& filter, long* handle,
                                     bool flag);        // 0x006610C7
};
COpinionDeed* FindDeedStandIn(CTCCreatureOpinionOfHero* opinion, bool flag, long* handle);
#pragma auto_inline(off)
// Same-TU stand-in for FindMostRecentDeed<COpinionDeedFilter_IsCrimeToRunAndFindAGuard> 0x006610C7: it writes only its
// handle out-parameter, so VC7.1 lets the handle share its frame word with the lookup keys (retail's LTCG view).
COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard&, long* handle,
                                                           bool flag)
{
    long found;
    COpinionDeed* deed = FindDeedStandIn(this, flag, &found);
    *handle = found;
    return deed;
}
#pragma auto_inline(on)
struct CVillage {
    unsigned char _pad[0x88];
    long GuardCount;                                    // +0x88 (descriptive)
    bool IsDeedHandled(long handle);                    // 0x0065914B (descriptive)
};
struct CTCVillageMember {                               // interface 0x23
    CVillage* GetPVillage() const;                      // 0x007EE540
};
struct CTCWife {                                        // interface 0xd7
    bool IsMarriedToThePlayer() const;                  // 0x006CEC80
};
struct CTCInteractionState {                            // interface 0xc3 (descriptive)
    unsigned char _pad[0x0c];
    bool Active;                                        // +0x0c
};
struct CPlayer {
    CThing* GetPCharacter();                            // 0x00487DC0
};
struct CWorld {
    CPlayer* GetCurrentPlayer() const;                  // 0x0049D9D0
};
struct CMessageEvent {
    unsigned char _pad[0x3c];
    long Type;                                          // +0x3c (descriptive)
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                                  // +0x18
};
unsigned long GetWorldFrame();                          // 0x0049D870
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                   // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                                // 0x008FCFC0
};
struct CAIStateGroup_MinionReactToPlayerInteraction : CAIStateGroupBase {
    unsigned long NextReactionFrame[5];                 // +0x14
    long ReactionType;                                  // +0x28
    CMessageEvent* FindPlayerInteractionEvent();        // 0x0092EDC0
    bool IsTooFarFromThePlayer(CThing* player);         // 0x0092ECD0
    bool IsTooNearToThePlayer(CThing* player);          // 0x0092ED40
    bool TestOrDoInteraction(bool test, CThing* player);   // 0x0092FAB0
    void InteractionFailed();                           // 0x0092F240
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionReactToPlayerInteraction::IsRunnable()
{
    CThing* player = GetPWorld()->GetCurrentPlayer()->GetPCharacter();
    if (!player || (player->m_flags91 & 1))
        return false;
    CMessageEvent* event = FindPlayerInteractionEvent();
    if (!event)
        return false;
    CTCVillageMember* member;
    if (static_cast<const CThing*>(GetCreature())->GetInterface(&member, 0x23)) {
        CVillage* village = member->GetPVillage();
        CTCCreatureOpinionOfHero* opinion;
        if (village && static_cast<const CThing*>(GetCreature())->GetInterface(&opinion, 0x4f)) {
            long handle;
            COpinionDeed* deed = opinion->FindMostRecentDeed(COpinionDeedFilter_IsCrimeToRunAndFindAGuard(),
                                                            &handle, false);
            if (deed) {
                long deedHandle = deed->Type == 0x35 ? deed->Handle : handle;
                if (village->GuardCount > 0 && !village->IsDeedHandled(deedHandle)) {
                    CTCWife* wife;
                    if (!GetCreature()->HasTC(0xd7)
                            || !static_cast<const CThing*>(GetCreature())->GetInterface(&wife, 0xd7)
                            || !wife->IsMarriedToThePlayer()) {
                        InteractionFailed();
                        return false;
                    }
                }
            }
        }
    }
    if (event->Type == 5) {
        if (IsTooFarFromThePlayer(player) || IsTooNearToThePlayer(player)) {
            ReactionType = 5;
            return true;
        }
    } else {
        ReactionType = 5;
        for (long i = 0; i < 5; ++i) {
            if (GetWorldFrame() > NextReactionFrame[i]) {
                CTCInteractionState* state;
                if (!static_cast<const CThing*>(GetCreature())->GetInterface(&state, 0xc3) || !state->Active
                        || i == 2) {
                    ReactionType = i;
                    break;
                }
            }
        }
        if (ReactionType != 5 && TestOrDoInteraction(true, player))
            return true;
    }
    InteractionFailed();
    return false;
}
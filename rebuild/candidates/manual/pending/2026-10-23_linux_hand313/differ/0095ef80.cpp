// CAIStateGroup_OpinionReactionRoot::IsRunnable() -- pick the thing to react to (the manager's forced target, else the nearest opinion source with a reaction), fill the per-reaction enable flags from the manager's modes, then ask the manager for the reaction and install its update function
// Grade: DIFFER(1536v1528) -- first pass: this register in the second half (ebx v ebp), the 0x20000 mask not CSEd into ebp, the per-reaction table/flag loop register roles, frame 0x38 v 0x30
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x10];
    unsigned char ThingType;                    // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline unsigned long HasTCWord(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(T*, long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    long GetMode();                             // 0x00662010
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {     // vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item);   // 0x004A96F0
};
bool __fastcall IsSameThing(CThing* thing, const CBaseIntelligentPointer& pointer);   // 0x005CBDFA
struct CThingListNode {
    CThingListNode* _M_next;
    CThingListNode* _M_prev;
    CBaseIntelligentPointer _M_data;            // +0x08
};
struct CThingList {
    CThingListNode* _M_node;
};
struct COpinionReactionManager {
    unsigned char _pad0[0x198];
    long ReactionModes[79];                     // +0x198
    unsigned char _pad2d4[0x4ae - 0x2d4];
    bool ReactionAllowedInAnyWeather[79];       // +0x4AE
    unsigned char _pad4fd[0x714 - 0x4fd];
    CBaseIntelligentPointer PForcedSource;      // +0x714
    CThingList OpinionSources;                  // +0x71C
    long GetAttitudeOrReactionType(CThingCreatureBase* creature, long a, bool* flags, long b);   // 0x00701010
};
struct CWorld {
    unsigned char _pad[0x74];
    COpinionReactionManager* PReactionManager;  // +0x74
};
struct CPlayer {
    unsigned char _pad[0x20a];
    bool InCutscene;                            // +0x20A
    CThing* GetCharacter();                     // 0x00487DC0
};
struct CWorldPlayers {
    CPlayer* GetMainPlayer();                   // 0x0049D9D0
};
struct CTCOpinionTarget {                       // interface 0x34
    unsigned char _pad[0x18];
    float Range;                                // +0x18
    unsigned char _pad1c[0x2f - 0x1c];
    bool ReactionsEnabled;                      // +0x2F
    CThing* GetTarget();                        // 0x007446A0
};
struct CTCFollowed { bool IsAIFollower(CThingCreatureBase* c); };          // 0x006D4D70 (interface 0xBC)
struct CTCRegionFollower { bool IsRegionFollower(CThingCreatureBase* c); }; // 0x006AEB60 (interface 0x7A)
struct CTCTargetHolder {                        // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32();
    virtual CThing* GetTarget();                // slot 33 (+0x84)
};
struct CWeatherState { virtual void v00(); virtual long GetWeather(); };
struct CTCWeatherHolder { CWeatherState* GetState(); };    // 0x0073A6E0 (interface 0x31)
extern float g_zero;                            // 0x0122DEDC
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
#pragma pointers_to_members(full_generality, multiple_inheritance)
class CAIStateGroup_OpinionReactionRoot;
struct SUpdateFunc { long a, b, c, d; };
struct SReactionEntry {                         // 40 bytes, table 0x013BB748
    SUpdateFunc UpdateFunc;
    bool (CAIStateGroup_OpinionReactionRoot::*CanReact)();   // +0x10
    unsigned char _pad18[0x10];
};
extern SReactionEntry g_reactionTable[];        // 0x013BB748
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    CWorldPlayers* GetPWorldPlayers();          // 0x008FCFC0
};
#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase {
public:
    void* PTCTalk;                              // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1C
    CBaseIntelligentPointer ThingReactingTo;    // +0x6B
    unsigned char _pad73[5];
    SUpdateFunc CurrentUpdateFunc;              // +0x78
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8A
    unsigned char _pad8b;
    long ExtraDataFloat;                        // +0x8C
    long ExtraDataSLONG;                        // +0x90
    CThing* ExtraDataPThing;                    // +0x94
    virtual bool IsRunnable();
    __forceinline bool IsFollower(CThing* t)
    {
        return t->HasTCWord(0xbc) && t->GetTC((CTCFollowed*)0, 0xbc)->IsAIFollower(GetCreature());
    }
    __forceinline bool IsRegionFollower(CThing* t)
    {
        return t->HasTCWord(0x7a) && t->GetTC((CTCRegionFollower*)0, 0x7a)->IsRegionFollower(GetCreature());
    }
};
#pragma pack(pop)
struct CIsOpinionSourceWithReaction {
    CAIStateGroup_OpinionReactionRoot* PStateGroup;
    void* PTCOpinion;
    COpinionReactionManager* ReactionManager;
    bool IsFollowingPlayer;
    bool IsAwareOfPlayerForReactions;
    long Reaction;                              // +0x10
    CIsOpinionSourceWithReaction(CAIStateGroup_OpinionReactionRoot* group);   // 0x0095F9A0
    bool operator()(CThing* thing);             // 0x0095FB40
};

bool CAIStateGroup_OpinionReactionRoot::IsRunnable()
{
    ExtraDataFloat = 0;
    ExtraDataSLONG = 0;
    ExtraDataPThing = 0;
    if (GetCreature()->GetMode() == 3)
        return false;
    COpinionReactionManager* manager = GetPThingWorld()->PReactionManager;
    CThing* source = manager->PForcedSource.GetPItem();
    CIsOpinionSourceWithReaction hasReaction(this);
    if (source == 0) {
        CThingList& sources = GetPThingWorld()->PReactionManager->OpinionSources;
        for (CThingListNode* node = sources._M_node->_M_next; node != sources._M_node; node = node->_M_next) {
            CIntelligentPointerThing candidate(node->_M_data);
            if (candidate.GetPItem() && !(candidate.GetPItem()->m_flags91 & 1)) {
                CThingCreatureBase* creature = GetCreature();
                const C3DVector& theirs = candidate.GetPItem()->GetPosition();
                const C3DVector& mine = creature->GetPosition();
                float dx = mine.X - theirs.X;
                float dy = mine.Y - theirs.Y;
                float dz = mine.Z - theirs.Z;
                if ((source == 0 || dx * dx + dz * dz + dy * dy < g_zero) && hasReaction(candidate.GetPItem()))
                    source = candidate.GetPItem();
            }
        }
    }
    if (source != 0 && hasReaction(source) && hasReaction.Reaction != 0) {
        CurrentReactionType = hasReaction.Reaction;
        CurrentUpdateFunc = g_reactionTable[hasReaction.Reaction].UpdateFunc;
        return true;
    }
    ThingReactingTo.SetPItem(0);
    if (manager->PForcedSource.GetPItem() != 0)
        return false;
    CTCOpinionTarget* opinion;
    if (GetCreature()->HasTCWord(0x34))
        opinion = GetCreature()->GetTC((CTCOpinionTarget*)0, 0x34);
    CThing* target = opinion->GetTarget();
    if (target && !(target->m_flags91 & 1) && opinion->Range != g_zero) {
        if (target->HasTCWord(0x50))
            return false;
        CIntelligentPointerConstThing pointer(target);
        ThingReactingTo.SetPItem(pointer.GetPItem());
    }
    if (!ThingReactingTo.GetPItem() || (ThingReactingTo.GetPItem()->m_flags91 & 1))
        return false;
    ReactingToPlayer = ThingReactingTo.GetPItem()->ThingType == 2 && ThingReactingTo.GetPItem()->HasTCWord(0x51);
    bool following = IsFollower(ThingReactingTo.GetPItem()) || IsRegionFollower(ThingReactingTo.GetPItem());
    if (ReactingToPlayer && following)
        return false;
    bool isTarget = false;
    bool heroBusy = false;
    CThing* reactingTo = ThingReactingTo.GetPItem();
    if (reactingTo->HasTCWord(8) && reactingTo->GetTC((CTCTargetHolder*)0, 8)->GetTarget() == GetCreature()) {
        isTarget = true;
        CPlayer* player = ((CWorldPlayers*)GetPThingWorld())->GetMainPlayer();
        if (player && IsSameThing(player->GetCharacter(), ThingReactingTo) && player->InCutscene)
            heroBusy = true;
    }
    long weather = 0;
    if (GetCreature()->HasTCWord(0x31))
        weather = GetCreature()->GetTC((CTCWeatherHolder*)0, 0x31)->GetState()->GetWeather();
    for (long i = 0; i < 79; ++i) {
        bool& flag = ReactionFlags[i];
        if (i < 0x12) {
            flag = false;
            continue;
        }
        switch (manager->ReactionModes[i]) {
        case 0: flag = true; break;
        case 1: flag = !isTarget; break;
        case 2: flag = isTarget && !heroBusy; break;
        case 3: flag = !heroBusy; break;
        case 4: flag = heroBusy; break;
        case 5: flag = isTarget; break;
        }
        if (!flag)
            continue;
        if (!manager->ReactionAllowedInAnyWeather[i]
            && (weather == 6 || weather == 0x29 || weather == 0x2b || weather == 0x2a || weather == 0x2d || weather == 0xb)) {
            flag = false;
            continue;
        }
        if (flag) {
            bool (CAIStateGroup_OpinionReactionRoot::*canReact)() = g_reactionTable[i].CanReact;
            flag = !canReact || (this->*canReact)() ? true : false;
        }
    }
    long reaction = manager->GetAttitudeOrReactionType(GetCreature(), 0, ReactionFlags, 0);
    if (!reaction)
        return false;
    if (GetCreature()->HasTCWord(0x34))
        opinion = GetCreature()->GetTC((CTCOpinionTarget*)0, 0x34);
    if (!opinion->ReactionsEnabled)
        return false;
    CurrentReactionType = reaction;
    CurrentUpdateFunc = g_reactionTable[reaction].UpdateFunc;
    return true;
}

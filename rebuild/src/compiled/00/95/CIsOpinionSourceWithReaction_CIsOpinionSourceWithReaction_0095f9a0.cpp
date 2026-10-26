// CIsOpinionSourceWithReaction::CIsOpinionSourceWithReaction(CAIStateGroup_OpinionReactionRoot* group) -- opinion-reaction filter: remember the state group and the world's reaction manager, work out whether the creature follows the player (AI follower or region follower of the hero's thing), whether it is aware of the player for reactions (its opinion-of-hero target is the hero with a non-zero awareness and the flag set), and cache its opinion-of-hero component
// Leaf: ??0CIsOpinionSourceWithReaction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095F9A0, length 416 bytes (`ret 4`).
// Callees: the out-of-line accessors 0x008FCFC0 (group -> player, manifest label CPlayer::GetCurrentMode is a byte-twin),
//   0x0049D9D0 (player -> player character), 0x00487DC0 (character -> thing, the handle at +0x2c), 0x007446A0 (opinion TC -> target
//   thing, the handle at +0xc); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (const copy); CTCFollowed::IsAIFollower
//   @ 0x006D4D70, CTCRegionFollower::IsRegionFollower @ 0x006AEB60. Data: the world @ 0x013B86A0 (+0x24 -> +0x74 reaction manager);
//   float 0x0122DEDC 0.0f.
// Layout: Ego_r.pdb CIsOpinionSourceWithReaction (0x14): PStateGroup +0, PTCOpinion +4, ReactionManager +8, IsFollowingPlayer +0xc,
//   IsAwareOfPlayerForReactions +0xd. CThing interface bits +0x20 / interface map +0x44 as in the landed CTCWound ctor 0x00757680.
//   Interfaces: 0xbc TCFollowed, 0x7a TCRegionFollower, 0x34 TCCreatureOpinionOfHero (awareness float +0x18, flag +0x2f), 0x4f the
//   cached PTCOpinion.
// Shape: the GetInterface out-parameter idiom with one ternary entry pick; each flag is ONE `&&`/`||` expression assigned to the member
//   (retail's `mov eax,1 / xor eax,eax` then `mov [ebp+0xc],al`; if-statements or inline bool helpers add a frame byte and `mov al,1`);
//   the creature is re-read in each call argument; the hero thing is a named local (ebx) set by an if/else.
class CTCBase;
struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
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
class CThingAICreature;
class CTCFollowed { public:
    bool IsAIFollower(CThingAICreature* creature);  // 0x006D4D70
};
class CTCRegionFollower { public:
    bool IsRegionFollower(CThing* thing);           // 0x006AEB60
};
class CTCCreatureOpinionOfHero { public:
    unsigned char _pad00[0x18];
    float         Awareness;                        // +0x18
    unsigned char _pad1c[0x2f - 0x1c];
    bool          AwareOfPlayer;                    // +0x2f
    CThing* GetTargetThing();                       // 0x007446A0
};
struct CPlayerCharacter {
    CThing* GetThing();                             // 0x00487DC0
};
class CPlayer { public:
    CPlayerCharacter* GetCharacter();               // 0x0049D9D0
};
struct CCreatureInfo {
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                     // +0x18
};
class CAIStateGroup_OpinionReactionRoot { public:
    void*          __vftable;
    CCreatureInfo* Info;                            // +0x04
    CPlayer* GetPlayer();                           // 0x008FCFC0
    CThingAICreature* GetCreature() { return Info->Creature; }
};
class COpinionReactionManager;
struct CWorldOpinion {
    unsigned char            _pad00[0x74];
    COpinionReactionManager* ReactionManager;       // +0x74
};
class CWorld { public:
    unsigned char  _pad00[0x24];
    CWorldOpinion* Opinion;                         // +0x24
};
extern CWorld* g_pWorld;                            // 0x013B86A0

class CIsOpinionSourceWithReaction {
public:
    CAIStateGroup_OpinionReactionRoot* PStateGroup;    // +0x00
    CTCCreatureOpinionOfHero*          PTCOpinion;     // +0x04
    COpinionReactionManager*           ReactionManager;// +0x08
    bool                               IsFollowingPlayer;            // +0x0c
    bool                               IsAwareOfPlayerForReactions;  // +0x0d
    CIsOpinionSourceWithReaction(CAIStateGroup_OpinionReactionRoot* group);
};

CIsOpinionSourceWithReaction::CIsOpinionSourceWithReaction(CAIStateGroup_OpinionReactionRoot* group)
    : PStateGroup(group), PTCOpinion(0), ReactionManager(g_pWorld->Opinion->ReactionManager)
{
    CThing* hero;
    if (group->GetPlayer()->GetCharacter())
        hero = group->GetPlayer()->GetCharacter()->GetThing();
    else
        hero = 0;
    CTCFollowed* followed;
    CTCRegionFollower* regionFollower;
    IsFollowingPlayer = hero
        && ((hero->GetInterface(&followed, 0xbc) && followed->IsAIFollower(group->GetCreature()))
            || (hero->GetInterface(&regionFollower, 0x7a) && regionFollower->IsRegionFollower((CThing*)group->GetCreature())));
    CTCCreatureOpinionOfHero* opinion;
    IsAwareOfPlayerForReactions = PStateGroup && ((CThing*)PStateGroup->GetCreature())->GetInterface(&opinion, 0x34)
        && opinion->GetTargetThing() == hero && opinion->Awareness != 0.0f && opinion->AwareOfPlayer;
    if (PStateGroup)
        ((CThing*)PStateGroup->GetCreature())->GetInterface(&PTCOpinion, 0x4f);
}
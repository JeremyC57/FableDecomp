// CAIStateGroup_DeedReactionRoot::OnReplace() -- after a delivered reaction to a real deed, record the deed's reaction flags and frame on the creature's reaction component and maybe give the reacting thing a tutorial (a virtual deed only remembers its id); then cancel the speech bookkeeping and stop the creature talking
// Retail: Fable.exe (TLC, VC7.1) @ 0x00960760, 592 bytes. ?OnReplace@CAIStateGroup_DeedReactionRoot@@UAEXXZ (Ego_r.pdb).
// Callees: CInterfaceMap lower_bound 0x0040F020 / const 0x004365B0, CTCDeedEffects::GetDeedEffectsDef 0x006C5630,
//   CTCBase::GetWorld 0x00686A70, CWorld::GetWorldFrame 0x0049D870, CTCTutorial::IsActive 0x006CFE80 /
//   OnDeedReactionDone 0x006CEF20, CTCTutorialGiver::GiveTutorialForCategory 0x006C9FA0, GetPWorld 0x008FCFC0,
//   ReleaseDeedSpeech 0x006FEAE0, CTCTalk::StopCurrentSpeech 0x006E2E60, CBaseIntelligentPointer::GetPItem 0x00A01B50.
// Layout: PDB state-group fields ThingReactingTo +0x14 .. DeliveredSpeech +0x35; brain creature +0x18; retail CThing
//   interface bitset +0x20, map +0x44, dead flag +0x91 bit 0; deed effects def RefCount +4, SpeechReaction +0x7c bit 7;
//   reaction component ReactionFrame +0x0c, Reacted/SpeechReacted +0x340/+0x341; world PSpeech +0x74 (all from retail operands).
// Shape: HasTC forceinline (bitset test inline); deed-effects def SpeechReaction is a bit-field so the sign test is
//   `test cl,cl; jns`; CDefPointer release is a member Release() with --RefCount; the speech release names
//   `CWorldObject* world = GetPWorld()` first so the call precedes the argument pushes.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
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
struct CDeedEffectsDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x7c - 8];
    unsigned char Flags7cLow : 7;               // +0x7c
    unsigned char SpeechReaction : 1;           // +0x7c bit 7
    void Release() { if (--RefCount == 0) Destroy(); }
};
template<class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing { public:
};
class CWorld { public: static unsigned long GetWorldFrame(); };   // 0x0049D870
struct CTCDeedReaction {                        // interface 0x4F
    unsigned char _pad00[0x0c];
    unsigned long ReactionFrame;                // +0x0c
    unsigned char _pad10[0x340 - 0x10];
    bool Reacted;                               // +0x340
    bool SpeechReacted;                         // +0x341
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
};
struct CTCDeedEffects {                         // interface 0x51
    CDefPointer<CDeedEffectsDef> GetDeedEffectsDef(long deedType) const;   // 0x006C5630
};
struct CTCTutorial {                            // interface 0xD7
    bool IsActive() const;                      // 0x006CFE80
    void OnDeedReactionDone();                  // 0x006CEF20
};
struct CTCTutorialGiver {                       // interface 0x29
    void GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
class CTCTalk { public:                                // interface 0x0F
    void StopCurrentSpeech();                   // 0x006E2E60
};
struct CSpeechManager {
    void ReleaseDeedSpeech(long deedType, long flag);   // 0x006FEAE0
};
struct CWorldObject {
    unsigned char _pad00[0x74];
    CSpeechManager* PSpeech;                    // +0x74
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorldObject* GetPWorld() const;            // 0x008FCFC0
};
struct CAIStateGroup_DeedReactionRoot : CAIStateGroupBase {
    CBaseIntelligentPointer ThingReactingTo;    // +0x14
    long CurrentDeedType;                       // +0x1c
    long CurrentDeedHandle;                     // +0x20
    bool DeedIsVirtual;                         // +0x24
    unsigned char _pad25[0x28 - 0x25];
    unsigned long LastThingOpinionSourceUID[2]; // +0x28
    long LastOpinionSourceVirtualDeedID;        // +0x30
    bool DeliveredReaction;                     // +0x34
    bool DeliveredSpeech;                       // +0x35
    void OnReplace();
};

void CAIStateGroup_DeedReactionRoot::OnReplace()
{
    if (DeliveredReaction) {
        if (!DeedIsVirtual) {
            CThingCreatureBase* creature = GetCreature();
            CTCDeedReaction* reaction;
            if (creature->HasTC(0x4f))
                reaction = (CTCDeedReaction*)creature->GetTC(0x4f);
            if (IS_THING_ALIVE(ThingReactingTo)) {
                const CThing* thing = ThingReactingTo.GetPItem();
                const CTCDeedEffects* effects;
                if (thing->HasTC(0x51))
                    effects = (const CTCDeedEffects*)thing->GetTC(0x51);
                CDefPointer<CDeedEffectsDef> def = effects->GetDeedEffectsDef(CurrentDeedType);
                reaction->Reacted = true;
                if (def->SpeechReaction) {
                    reaction->Reacted = true;
                    reaction->SpeechReacted = true;
                }
                reaction->ReactionFrame = reaction->GetWorld()->GetWorldFrame();
                const CThingCreatureBase* me = GetCreature();
                if (me->HasTC(0xd7) && ((const CTCTutorial*)me->GetTC(0xd7))->IsActive()) {
                    CThing* target = ThingReactingTo.GetPItem();
                    if (target->HasTC(0x29)) {
                        CTCTutorialGiver* giver = (CTCTutorialGiver*)target->GetTC(0x29);
                        if (giver)
                            giver->GiveTutorialForCategory(0x11, false);
                    }
                }
            }
        } else
            LastOpinionSourceVirtualDeedID = CurrentDeedHandle;
        if (CurrentDeedType == 0x59) {
            CThingCreatureBase* creature = GetCreature();
            if (creature->HasTC(0xd7)) {
                CTCTutorial* tutorial = (CTCTutorial*)creature->GetTC(0xd7);
                if (tutorial)
                    tutorial->OnDeedReactionDone();
            }
        }
    }
    if (DeliveredSpeech) {
        CWorldObject* world = GetPWorld();
        world->PSpeech->ReleaseDeedSpeech(CurrentDeedType, 0);
    }
    CThingCreatureBase* creature = GetCreature();
    if (creature->HasTC(0x0f))
        ((CTCTalk*)creature->GetTC(0x0f))->StopCurrentSpeech();
}
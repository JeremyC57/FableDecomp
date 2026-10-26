// CAIStateGroup_OpinionReactionRoot::HeroTitleToSelfUpdate() -- say the hero title's "comment to self" line to the thing being reacted to; true when the speech action completed
// Leaf: ?HeroTitleToSelfUpdate@CAIStateGroup_OpinionReactionRoot@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009859A0, length 186 bytes, plain `ret`.
// PDB (Ego_r, donor 0x009A5C90): ?HeroTitleToSelfUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ. Only reference is the
// opinion-reaction table initializer @ 0x01222D8C (record 0x013BBE00, "OPINION_REACTION_TYPE_HEROTITLE_TO_SELF",
// runnable test 0x00985890 = HeroTitleToSelfIsRunnable).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ThingReactingTo, packed +0x6B, twice), CVectorMap::LowerBound
// const @ 0x004365B0 (inlined GetInterface, type 0x29 = CTCHero), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem
// @ 0x00A01B90 (the two by-value pointer arguments built in their slots: CIntelligentPointer<const CThing> vtable
// 0x01237F1C, CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8), CAIStateGroupBase::
// AIAction_SpeakAISpeechToTarget six-argument @ 0x009812C0.
// Shape: the bitset::test form of GetInterface (its bool materialised to a frame byte, result unused; the miss arm
// reads the never-written out pointer, which shares the lookup key's word); CTCHero::PHeroTitleSubDef (+0x18) is a
// named local loaded before the second GetPItem (the named target), the tag (+0x38 HeroTitleCommentToSelfTag) read
// at the push. Result `== 2` (EAIActionReturn completed).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType) const
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};

class CHeroTitleDef {
public:
    unsigned char _pad0[0x28];
    long Title;                                 // +0x28
    long HeroTitleGuiTag;                       // +0x2c
    long HeroTitleGreetToHeroTag;               // +0x30
    long HeroTitleCommentAtHeroTag;             // +0x34
    long HeroTitleCommentToSelfTag;             // +0x38
};

class CTCHero {                                 // interface type 0x29
public:
    unsigned char _pad0[0x18];
    const CHeroTitleDef* PHeroTitleSubDef;      // +0x18 CDefPointer<const CHeroTitleDef>
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {       // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

enum EAIActionRepeat { AI_ACTION_REPEAT_NONE, AI_ACTION_REPEAT_ONCE };
enum EAIActionReturn { AI_ACTION_RETURN_NONE, AI_ACTION_RETURN_IN_PROGRESS, AI_ACTION_RETURN_COMPLETED };

class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    EAIActionReturn AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerConstThing target, unsigned long speech, bool interrupt, long priority);   // 0x009812C0
};

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase {
public:
    void* PTCTalk;                              // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CBaseIntelligentPointer ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
private:
    bool HeroTitleToSelfUpdate();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::HeroTitleToSelfUpdate()
{
    CTCHero* hero;
    ThingReactingTo.GetPItem()->GetInterfaceT(&hero, 0x29);
    const CHeroTitleDef* title = hero->PHeroTitleSubDef;
    CThing* target = ThingReactingTo.GetPItem();
    return AIAction_SpeakAISpeechToTarget(AI_ACTION_REPEAT_ONCE, PBrain->PCreature, target,
        title->HeroTitleCommentToSelfTag, true, 0x93) == AI_ACTION_RETURN_COMPLETED;
}
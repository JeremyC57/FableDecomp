// CAIStateGroup_MinionGuardGreetPlayer::Update() -- greet the living player: push the village's next-greet frame 120 frames ahead, pick a speech (an opinion-graded TEXT_AI_GUARD_GREET_* tag when the hero-stats field is clear, else the creature opinion's greet group), speak it, then set the next-greet frame 1200 (spoken) or 120 frames ahead; always Terminate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929820, length 623 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CWorld::GetCurrentPlayer @ 0x0049D9D0, CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap::LowerBound @ 0x0040F020
// (creature 0x23 / 0x4F lookups) and its const instantiation @ 0x004365B0 (the player's interface 4), GetWorldFrame
// @ 0x0049D870, CTCVillageMember::GetPVillage @ 0x007EE540, _ftol @ 0x00BFEA70, CCharString() @ 0x0099E4B0 /
// operator=(const char*) @ 0x0099EFE0 / ~CCharString @ 0x0099EAE0, GetPThingWorld @ 0x008FCFA0, GetOpinionValue
// @ 0x00701750, CCharString_ComputeCRC32 @ 0x00404310 (__fastcall), GetTagFromCRC @ 0x009CC410,
// CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex @ 0x00683C10, CBaseIntelligentPointer ctor @ 0x00A01B10 /
// SetPItem @ 0x00A01B90, AIAction_SpeakAISpeechToTarget @ 0x009812C0, Terminate @ 0x008FCF40.
// Data: g_pMainGameComponent @ 0x013B86A0; "TEXT_AI_GUARD_GREET_NEUTRAL/FRIENDLY/WARY" @ 0x012919E4/0x012919C4/0x012919A4;
// 0.3f @ 0x0129199C, -0.3f @ 0x012919C0.
// Layout: local models (PBrain +4; brain PCreature +0x18; thing presence bits +0x20, interface map +0x44, flags +0x91;
// village NextGreetFrame +0xAC; main component +0x14 tag table, +0x24 world; thing world +0x74 opinion manager;
// interface 4 = CTCHeroStats, its +0x114 word is tested for zero -- PDB NumTeleports, retail meaning unverified).
// Shape: the three lookups are the word-local HasTC bound to `const bool&` (the byte spill at +0x13); the 0x23 and 0x4F
// results are used unchecked (a miss reads the key slot); the next-greet frame goes through a float local and a village
// local; the creature and tag-table locals are loaded ahead of the calls and the opinion manager comes through an inline
// accessor (the `mov ecx,eax` after the argument pushes).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
};
class CThingCreatureBase : public CThing { public: };
class CThingPlayerCreature : public CThingCreatureBase { public: };
struct CVillage {
    unsigned char _pad[0xac];
    long NextGreetFrame;                        // +0xAC
};
class CTCVillageMember { public:
    CVillage* GetPVillage();                    // 0x007EE540
};
class CTCHeroStats { public:
    unsigned char _pad[0x114];
    long Field114;                              // +0x114
};
class CTCCreatureOpinionOfHero { public:
    unsigned long GetGreetDialogueGroupIndex(long attitude);    // 0x00683C10
};
class CCharString { public:
    void* m_pData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    const CCharString& operator=(const char* text);   // 0x0099EFE0
};
unsigned long __fastcall CCharString_ComputeCRC32(const CCharString& text);    // 0x00404310
struct CTagTable {
    unsigned long GetTagFromCRC(unsigned long crc) const;   // 0x009CC410
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;      // 0x00487DC0
};
struct COpinionManager {
    float GetOpinionValue(CThing* thing, long opinion);     // 0x00701750
};
class CWorld { public:
    CPlayer* GetCurrentPlayer();                // 0x0049D9D0
    unsigned char _pad[0x74];
    COpinionManager* POpinions;                 // +0x74
    COpinionManager* GetPOpinions() const { return POpinions; }
};
class CMainGameComponent { public:
    unsigned char _pad[0x14];
    CTagTable* PTags;                           // +0x14
    unsigned char _pad18[0x0c];
    CWorld* PWorld;                             // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
long GetWorldFrame();                           // 0x0049D870
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
    void Terminate();                           // 0x008FCF40
    long AIAction_SpeakAISpeechToTarget(long repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerConstThing target, unsigned long speech, bool interrupt, long priority);   // 0x009812C0
};
struct CAIStateGroup_MinionGuardGreetPlayer : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_MinionGuardGreetPlayer::Update()
{
    CThingPlayerCreature* player = g_pMainGameComponent->PWorld->GetCurrentPlayer()->GetPControlledCreature();
    if (player && !(player->m_flags91 & 1)) {
        CTCVillageMember* member;
        GetCreature()->GetInterface(&member, 0x23);
        float next = (float)(GetWorldFrame() + 120);
        CVillage* village = member->GetPVillage();
        village->NextGreetFrame = (long)next;
        unsigned long speech;
        const CTCHeroStats* stats;
        if (((const CThing*)player)->GetInterface(&stats, 4) && stats->Field114 == 0) {
            CCharString text;
            CThingCreatureBase* creature = GetCreature();
            float opinion = PeekWorld().GetPOpinions()->GetOpinionValue(creature, 0);
            text = "TEXT_AI_GUARD_GREET_NEUTRAL";
            if (opinion > 0.3f)
                text = "TEXT_AI_GUARD_GREET_FRIENDLY";
            if (opinion < -0.3f)
                text = "TEXT_AI_GUARD_GREET_WARY";
            CTagTable* tags = g_pMainGameComponent->PTags;
            speech = tags->GetTagFromCRC(CCharString_ComputeCRC32(text));
        } else {
            CTCCreatureOpinionOfHero* opinionTC;
            GetCreature()->GetInterface(&opinionTC, 0x4f);
            speech = opinionTC->GetGreetDialogueGroupIndex(0x12);
        }
        float when;
        if (speech && AIAction_SpeakAISpeechToTarget(1, CIntelligentPointerCreature(GetCreature()),
                CIntelligentPointerConstThing(player), speech, false, 0x93) == 2)
            when = (float)(GetWorldFrame() + 1200);
        else
            when = (float)(GetWorldFrame() + 120);
        member->GetPVillage()->NextGreetFrame = (long)when;
    }
    Terminate();
}
// CAIStateGroup_GameMaster::OfferPlayerAGame() -- play OFFER_GAME, speak the table game's banter at the hero until it finishes, then schedule the next reaction 5..14 seconds ahead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094CE60, length 437 bytes.
// Leaf: OfferPlayerAGame@CAIStateGroup_GameMaster
// Callees: GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0,
//          CBaseIntelligentPointer GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90, CVectorMap::LowerBound
//          @ 0x0040F020 (interface 0xcf), CTCTavernGame::GetBanter @ 0x005EA3C0, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          CCreatureAction_PlayAnimation @ 0x008424F0, ~CCreatureActionBase @ 0x00693EF0 (manifest "CPhysicsMeshInfo" is
//          propagated), AIAction_PerformAction @ 0x0097D8B0, AIAction_SpeakAISpeechToTarget @ 0x009812C0, Terminate
//          @ 0x008FCF40, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CWorld::GetFrame @ 0x0049D870 (manifest
//          "GFGetPreMainMemoryUsed" is propagated), g_framesPerSecond @ 0x01375550.
// Shape: the bitset::test GetInterface (bool materialised to a frame byte, ternary end/hit pick straight into ecx; the miss
//          path reads the never-written slot); the animation string is scoped to the PerformAction block so the lookup key
//          shares its slot; `do { r = Speak(...) } while (r == 4)`; the random seconds and a NAMED fps local are taken
//          before GFGetCurrentFrame (fps rides in ebx across the call).
// Layout: PDB CAIStateGroup_GameMaster (176 bytes) derives CAIStateGroup_GoToPub: PTable +0x24 (CIntelligentPointer<CThingObject>),
//          TimeNextReactToPlayer +0xa8; no engine header for these classes (flattened bases), so modelled locally.
class CTCBase;
class CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool TestTC(long interfaceType) const       // std::bitset::test
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return has;
    }
};
class CThingPlayerCreature : public CThing { };
class CThingCreatureBase;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer { // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };                     // 0x00487DC0
class CPlayerManager { public: CPlayer* GetMainPlayer(); };            // 0x00449970
class CTCTavernGame { public: long GetBanter(); };                     // 0x005EA3C0 (interface 0xcf)
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_NONE, AI_ACTION_REPEAT_1, AI_ACTION_REPEAT_2 };
enum EAIActionReturn { AI_RET_0, AI_RET_1, AI_RET_2, AI_RET_3, AI_RET_4 };
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern long g_framesPerSecond;                  // 0x01375550
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    void Terminate();                           // 0x008FCF40
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    EAIActionReturn AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    EAIActionReturn AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerConstThing target, unsigned long speech, bool interrupt, long priority);   // 0x009812C0
};
struct CAIStateGroup_GameMaster : CAIStateGroupBase {
    unsigned char _pad14[0x10];
    CBaseIntelligentPointer PTable;             // +0x24
    unsigned char _pad2c[0xa8 - 0x2c];
    long TimeNextReactToPlayer;                 // +0xa8
    bool WasAllowedToFollowPlayer;              // +0xac
    bool OfferPlayerAGame();
};

bool CAIStateGroup_GameMaster::OfferPlayerAGame()
{
    CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetPControlledCreature();
    if (hero == 0 || !IS_THING_ALIVE(PTable))
        return false;
    CTCTavernGame* game;
    PTable.GetPItem()->GetInterface(&game, 0xcf);
    long banter = game->GetBanter();
    {
        CCharString animation("OFFER_GAME");
        AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(), false, true, false, 0, 0));
    }
    EAIActionReturn result;
    do {
        result = AIAction_SpeakAISpeechToTarget(AI_ACTION_REPEAT_2, GetCreature(), hero, banter, false, 0x22);
    } while (result == AI_RET_4);
    if (result == AI_RET_1) {
        Terminate();
        return false;
    }
    unsigned long seconds = WorldRandomRange(*GetPWorldSeed(), 10);
    long fps = g_framesPerSecond;
    TimeNextReactToPlayer = CWorld::GetFrame() + (seconds + 5) * fps;
    return true;
}
// CAIStateGroup_GameMaster::OfferPlayerAGame() -- play OFFER_GAME, speak the table game's banter at the hero until it finishes, then schedule the next reaction 5..14 seconds ahead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094CE60, length 437 bytes.
// Leaf: OfferPlayerAGame@CAIStateGroup_GameMaster
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Callees: GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0,
//          CBaseIntelligentPointer GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90, CVectorMap::LowerBound
//          @ 0x0040F020 (interface 0xcf), CTCTavernGame::GetBanter @ 0x005EA3C0, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          CCreatureAction_PlayAnimation @ 0x008424F0, ~CCreatureActionBase @ 0x00693EF0 (manifest "CPhysicsMeshInfo" is
//          propagated), AIAction_PerformAction @ 0x0097D8B0, AIAction_SpeakAISpeechToTarget @ 0x009812C0, Terminate
//          @ 0x008FCF40, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CWorld::GetFrame @ 0x0049D870 (manifest
//          "GFGetPreMainMemoryUsed" is propagated), CMainGameComponent::GetConstantFPS() @ 0x01375550.
// Shape: the bitset::test GetInterface (bool materialised to a frame byte, ternary end/hit pick straight into ecx; the miss
//          path reads the never-written slot); the animation string is scoped to the PerformAction block so the lookup key
//          shares its slot; `do { r = Speak(...) } while (r == 4)`; the random seconds and a NAMED fps local are taken
//          before GFGetCurrentFrame (fps rides in ebx across the call).
// Layout: PDB CAIStateGroup_GameMaster (176 bytes) derives CAIStateGroup_GoToPub: PTable +0x24 (CIntelligentPointer<CThingObject>),
//          TimeNextReactToPlayer +0xa8; no engine header for these classes (flattened bases), so modelled locally.
class CTCBase;
class CThing;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CThingCreatureBase;
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)((CThing*)item)); }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer { // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)((CThing*)item)); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

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
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
            // 0x00449970
class CTCTavernGame { public: long GetBanter(); };                     // 0x005EA3C0 (interface 0xcf)
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_NONE, AI_ACTION_REPEAT_1, AI_ACTION_REPEAT_2 };
enum EAIActionReturn { AI_RET_0, AI_RET_1, AI_RET_2, AI_RET_3, AI_RET_4 };
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
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
    protected:
    CPlayerManager& GetPlayerManager() const;        // 0x008FCF90
    public:
    void Terminate();                           // 0x008FCF40
    protected:
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    public:
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
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero == 0 || !IS_THING_ALIVE(PTable))
        return false;
    CTCTavernGame* game;
    ((CThing*)PTable.GetPItem())->GetTC(TCI_TAVERN_GAME, &game);
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
    long fps = CMainGameComponent::GetConstantFPS();
    TimeNextReactToPlayer = CWorld::GetFrame() + (seconds + 5) * fps;
    return true;
}
//   CTCDiceGame::GetCurrentPlayer() (`return CurrentPlayer.GetPItem();`) compared as `DiceGame->GetCurrentPlayer() ==
//   GetCreature()`: the extra inline level gives DiceGame its own temp (eax, `lea ecx,[eax+10h]`) and lets the creature load
//   (edi) schedule ahead of the call, with the later action/IsFree block keeping retail's colouring. Direct
//   `CurrentPlayer.GetPItem()` in either operand order, operator== forms, or a creature local all give 503/505 DIFFERs.
// CAIStateGroup_BanditPlayDice::OnReplace() -- leave the dice game (free the position, drop the player count, clear the current player if it is us, post the watching-out animation), re-sheathe, then re-roll the join/initiate frames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00928E20, length 505 bytes.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C), CTCDiceGame::SetCurrentPlayer @ 0x00929550 (manifest
//          "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          CCreatureAction_PlayAnimation @ 0x008424F0, CBrainOutput @ 0x00835620, IsFreeToPerformAction @ 0x00662210,
//          CCountedPointer::Reset @ 0x00668D90, SetBrainOutput @ 0x0088B8F0, ~CCreatureActionBase @ 0x00693EF0,
//          UnsheatheDefaultWeapon @ 0x00664A50, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, GFGetCurrentFrame
//          @ 0x0049D870, __ftol2 @ 0x00BFEA70.
// Layout: PDB CAIStateGroup_BanditPlayDice: DiceGamePosition +0x14 (CTCExclusiveUse*, User +0xc), DiceGame +0x18 (CTCDiceGame*,
//          NumberOfPlayers +0xc, CurrentPlayer +0x10), FrameToInitiate +0x1c, FrameToJoin +0x20, WasCarryingWeapon +0x24.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#include <stdint.h>
#endif
#include <stdio.h>
#include <string.h>
class CTCBase;
class CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CCreatureActionBase;
struct CCreatureActionQueue {
    long GetCurrentActionID();                  // 0x00692620
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad4c[0x100 - 0x4c];
    CCreatureActionQueue* PActionQueue;         // +0x100
    void UnsheatheDefaultWeapon();              // 0x00664A50
    bool IsPerformingAnyAction();               // 0x006622A0
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CCountBlock {
    long Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    void Reset(void* object);                   // 0x00668D90
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:                           // 0x48 bytes
    unsigned long Flags;
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;            // +0x40
    CBrainOutput();                             // 0x00835620
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                // slot 11 (+0x2c)
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
class CTCExclusiveUse { public:
    unsigned char _pad[0xc];
    CIntelligentPointerThing User;              // +0x0c
    void SetUser(CThing* user) { User = CIntelligentPointerThing(user); }
};
class CTCDiceGame { public:
    unsigned char _pad[0xc];
    long NumberOfPlayers;                       // +0x0c
    CIntelligentPointerThing CurrentPlayer;     // +0x10
    void SetCurrentPlayer(CThing* player);      // 0x00929550
    CThing* GetCurrentPlayer() const { return CurrentPlayer.GetPItem(); }
};
long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
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
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
};
struct CAIStateGroup_BanditPlayDice : CAIStateGroupBase {
    CTCExclusiveUse* DiceGamePosition;          // +0x14
    CTCDiceGame* DiceGame;                      // +0x18
    long FrameToInitiate;                       // +0x1c
    long FrameToJoin;                           // +0x20
    bool WasCarryingWeapon;                     // +0x24
    void OnReplace();
};

void CAIStateGroup_BanditPlayDice::OnReplace()
{
    if (DiceGame) {
        DiceGamePosition->SetUser(0);
        --DiceGame->NumberOfPlayers;
        if (DiceGame->GetCurrentPlayer() == GetCreature())
            DiceGame->SetCurrentPlayer(0);
        DiceGame = 0;
        CCreatureAction_PlayAnimation action(GetCreature(), "ST_DICEGAME_WATCHING_OUTOF", CSoundPair(), false, true, false, 0, 0);
        CBrainOutput output;
        if (GetCreature()->IsFreeToPerformAction(&action)) {
            output.Flags |= 8;
            CCreatureActionBase* performed = &action;
            output.PAction.Reset(performed->Clone());
            PBrain->SetBrainOutput(output);
        }
    }
    if (WasCarryingWeapon)
        GetCreature()->UnsheatheDefaultWeapon();
    FrameToJoin = (long)(GFGetCurrentFrame() + (WorldRandomRange(*GetPWorldSeed(), 750) + 2250) * 0.5f);
    FrameToInitiate = GFGetCurrentFrame() + WorldRandomRange(*GetPWorldSeed(), 750) + 2250;
}

// ---- test stubs ----
static int g_unsheathe, g_setCurrent, g_setOutput, g_free, g_actions, g_clones;
static unsigned long g_outFlags; static void* g_outAction; static void* g_cloneResult;
static const char* g_lastAnim; static CThingCreatureBase* g_animCreature;
static unsigned long g_seed;
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CTCDiceGame::SetCurrentPlayer(CThing* player) { ++g_setCurrent; CurrentPlayer.SetPItem(player); }
CCharString::CCharString(const char* text, long) : m_pData((void*)text) { }
CCharString::~CCharString() { }
void CCreatureActionBase::v00() { } void CCreatureActionBase::v01() { } void CCreatureActionBase::v02() { }
void CCreatureActionBase::v03() { } void CCreatureActionBase::v04() { } void CCreatureActionBase::v05() { }
void CCreatureActionBase::v06() { } void CCreatureActionBase::v07() { } void CCreatureActionBase::v08() { }
void CCreatureActionBase::v09() { } void CCreatureActionBase::v10() { }
void* CCreatureActionBase::Clone() const { ++g_clones; return g_cloneResult; }
CCreatureActionBase::~CCreatureActionBase() { }
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
    const CSoundPair&, bool, bool, bool, long, long) { ++g_actions; g_animCreature = creature; g_lastAnim = (const char*)animation.m_pData; }
CBrainOutput::CBrainOutput() : Flags(0) { PAction.Object = 0; PAction.Block = 0; }
void CCountedPointer::Reset(void* object) { Object = object; }
void CAIBrain::SetBrainOutput(const CBrainOutput& output) { ++g_setOutput; g_outFlags = output.Flags; g_outAction = output.PAction.Object; }
bool CThingCreatureBase::IsFreeToPerformAction(const CCreatureActionBase*) { return g_free != 0; }
void CThingCreatureBase::UnsheatheDefaultWeapon() { ++g_unsheathe; }
long __fastcall GFGetCurrentFrame() { return 1000; }
unsigned long __fastcall GFROR13(unsigned long v) { v &= 0xFFFFFFFFUL; return ((v >> 13) | (v << 19)) & 0xFFFFFFFFUL; }
CAIStateGroupBase::~CAIStateGroupBase() { }
unsigned long* CAIStateGroupBase::GetPWorldSeed() const { return &g_seed; }
static unsigned long Next(unsigned long& s) { s = (s * 0x24A1 + 0x24DF) & 0xFFFFFFFFUL; s = ((s >> 13) | (s << 19)) & 0xFFFFFFFFUL; return s % 750; }
static CThingCreatureBase g_me, g_other;
static CAIBrain g_brain;
static void* g_posBuf[16];                      // raw storage: the intelligent pointers have no default ctor
static void* g_gameBuf[16];
#define g_pos (*(CTCExclusiveUse*)g_posBuf)
#define g_game (*(CTCDiceGame*)g_gameBuf)
static int Check(CAIStateGroup_BanditPlayDice& g, unsigned long seed0)
{
    unsigned long s = seed0;
    unsigned long r1 = Next(s), r2 = Next(s);
    return g.FrameToJoin == (long)(1000 + (r1 + 2250) * 0.5f) && g.FrameToInitiate == (long)(1000 + r2 + 2250) && g_seed == s;
}
int main()
{
    int ok = 1;
    g_brain.PCreature = &g_me;
    static CAIStateGroup_BanditPlayDice grp;
    grp.PBrain = &g_brain;
    // 1: in a game as the current player, free to act, was carrying a weapon
    grp.DiceGamePosition = &g_pos; grp.DiceGame = &g_game; grp.WasCarryingWeapon = true;
    g_pos.User.m_pItem = &g_me; g_game.NumberOfPlayers = 3; g_game.CurrentPlayer.m_pItem = &g_me;
    g_free = 1; g_cloneResult = &g_other; g_seed = 12345;
    grp.OnReplace();
    if (g_pos.User.m_pItem != 0 || g_game.NumberOfPlayers != 2 || g_setCurrent != 1 || g_game.CurrentPlayer.m_pItem != 0
        || grp.DiceGame != 0 || g_actions != 1 || g_animCreature != &g_me || !g_lastAnim || strcmp(g_lastAnim, "ST_DICEGAME_WATCHING_OUTOF")
        || g_setOutput != 1 || g_outFlags != 8 || g_outAction != &g_other || g_clones != 1 || g_unsheathe != 1 || !Check(grp, 12345)) { ok = 0; printf("s1\n"); }
    // 2: in a game, someone else is current, not free, no weapon
    grp.DiceGame = &g_game; grp.WasCarryingWeapon = false; g_game.CurrentPlayer.m_pItem = &g_other; g_free = 0; g_seed = 777;
    grp.OnReplace();
    if (g_setCurrent != 1 || g_game.CurrentPlayer.m_pItem != &g_other || g_game.NumberOfPlayers != 1 || g_actions != 2
        || g_setOutput != 1 || g_clones != 1 || g_unsheathe != 1 || grp.DiceGame != 0 || !Check(grp, 777)) { ok = 0; printf("s2\n"); }
    // 3: not in a game: only the weapon and the frames
    grp.WasCarryingWeapon = true; g_seed = 42;
    grp.OnReplace();
    if (g_actions != 2 || g_unsheathe != 2 || g_game.NumberOfPlayers != 1 || !Check(grp, 42)) { ok = 0; printf("s3\n"); }
    if (ok) printf("00928e20_TEST PASS\n");
    return ok ? 0 : 1;
}
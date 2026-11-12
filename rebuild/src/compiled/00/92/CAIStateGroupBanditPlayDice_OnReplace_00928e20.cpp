//   CTCDiceGame::GetCurrentPlayer() (`return CurrentPlayer.GetPItem();`) compared as `DiceGame->GetCurrentPlayer() ==
// Leaf: ?OnReplace@CAIStateGroup_BanditPlayDice@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
//   GetCreature()`: the extra inline level gives DiceGame its own temp (eax, `lea ecx,[eax+10h]`) and lets the creature load
//   (edi) schedule ahead of the call, with the later action/IsFree block keeping retail's colouring. Direct
//   `CurrentPlayer.GetPItem()` in either operand order, operator== forms, or a creature local all give 503/505 DIFFERs.
// CAIStateGroup_BanditPlayDice::OnReplace() -- leave the dice game (free the position, drop the player count, clear the current player if it is us, post the watching-out animation), re-sheathe, then re-roll the join/initiate frames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00928E20, length 505 bytes.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C), CTCDiceGame::SetCurrentPlayer @ 0x00929550 (manifest
//          "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          CCreatureAction_PlayAnimation @ 0x008424F0, CBrainOutput @ 0x00835620, IsFreeToPerformAction @ 0x00662210,
//          CCountedPointer::Reset @ 0x00668D90, SetOutput @ 0x0088B8F0, ~CCreatureActionBase @ 0x00693EF0,
//          UnsheatheDefaultWeapon @ 0x00664A50, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CWorld::GetFrame
//          @ 0x0049D870, __ftol2 @ 0x00BFEA70.
// Layout: PDB CAIStateGroup_BanditPlayDice: DiceGamePosition +0x14 (CTCExclusiveUse*, User +0xc), DiceGame +0x18 (CTCDiceGame*,
//          NumberOfPlayers +0xc, CurrentPlayer +0x10), FrameToInitiate +0x1c, FrameToJoin +0x20, WasCarryingWeapon +0x24.
class CTCBase;
class CThing;
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCreatureActionBase;
struct CCreatureActionQueue {
    long GetCurrentActionID();                  // 0x00692620
};

class CThingCreatureBase : public CThing { public:
    unsigned char _pad4c[0x1c];   // own members from +0x98
    CCreatureActionQueue* PActionQueue;         // +0x100
    void UnsheatheDefaultWeapon();              // 0x00664A50
    bool IsPerformingAnyAction();               // 0x006622A0
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

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
    void SetOutput(const CBrainOutput& output);   // 0x0088B8F0 ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z (Ego_r.pdb + FableWin.pdb; was the stand-in SetOutput; name-fix 2026-11-12)
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
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
class CTCExclusiveUse { public:
    unsigned char _pad[0xc];
    CIntelligentPointer<CThing> User;              // +0x0c
    void SetUser(CThing* user) { User = CIntelligentPointer<CThing>(user); }
};
class CTCDiceGame { public:
    unsigned char _pad[0xc];
    long NumberOfPlayers;                       // +0x0c
    CIntelligentPointer<CThing> CurrentPlayer;     // +0x10
    void SetCurrentPlayer(CThing* player);      // 0x00929550
    CThing* GetCurrentPlayer() const { return CurrentPlayer.Get(); }
};
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
long __fastcall GFROR13(unsigned long value);   // 0x00497890
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
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    public:
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
            PBrain->SetOutput(output);
        }
    }
    if (WasCarryingWeapon)
        GetCreature()->UnsheatheDefaultWeapon();
    FrameToJoin = (long)(CWorld::GetFrame() + (WorldRandomRange(*GetPWorldSeed(), 750) + 2250) * 0.5f);
    FrameToInitiate = CWorld::GetFrame() + WorldRandomRange(*GetPWorldSeed(), 750) + 2250;
}
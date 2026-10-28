// CTCTavernGame::EnterGame() -- start playing: enter (or leave on a pending quit) the game state, hide the hero in limbo, bind the hero's game player, close the hero's menu, open the game UI, and seed the bets and money from the hero's purse and the def's minimum bet
// Leaf: ?EnterGame@CTCTavernGame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EC0F0, 373 bytes (plain `ret`).
// Callees: CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0; the hero's virtual slot 28 (+0x70);
//          CThing::SetInLimbo(bool) 0x004C8CF0; CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer*) 0x005ED4A0 (the
//          inline GetDef, as the landed GetTotalWinnings 0x005EA310); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//          0x0040F020 (interfaces 0xD0 = word 6 bit 16 and 0xF6 = word 7 bit 22); CTCGamePlayer::SetGame(CTCTavernGame*)
//          0x005EDC00; the menu component's RequestClose() 0x00625920; CPlayerGui::Close(bool) 0x00438400 and the game-UI
//          opener 0x00437FF0 (manifest label is a propagated STL name); CTCTavernGame::GetHeroStats 0x005EB4B0; the def's
//          slot 1; the virtual LeaveGame (slot 30) through the inline SetState.
// Globals: quit request 0x013BAE1C, g_pMainGameComponent 0x013B86A0, g_pPlayerGui 0x013B8790.
// Layout (retail CTCTavernGame, Ego_r.pdb - 8 to CurrentMoney, - 0x1c from the state block): ParentThing +0x04,
//          IsBeingPlayed +0x20, GameWon +0x21, GameFinished +0x28, BaseBet +0x34, LastBet +0x38, TotalWinnings +0x3c,
//          CurrentMoney +0x40, NumConsecutiveRoundsWon +0xa4, TavernGameState +0xc0, PreviousTavernGameState +0xc4,
//          GamePlayed +0xd8; CThing PDef +0x70; CTavernGameDef MinBet +0x110 (PDB offset holds); CTCHeroStats Money +0x3c
//          (PDB - 8); CPlayerGui bool +0x2bd.
// Shape: SetState(1) inlined; the two unchecked GetInterface calls sit in their own `{}` blocks with the stats pointer
//        (one shared frame word); the game player's lookup reassigns the entry (component via eax), the menu's is one
//        ternary (component straight into ecx); the def is dereferenced before its dtor, so the release has no null test;
//        the GetSubDef stand-in under `auto_inline(off)` (landed GetTotalWinnings precedent) sinks the def zero store below
//        the push and gives the plain `dec eax ; mov [ecx+4],eax` release.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCBase;
typedef long ETavernGameStates;

extern bool g_bTavernGameQuitRequested;         // 0x013BAE1C

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CTavernGameDef : public CDefVirtual { public:
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0x108];
    long MinBet;                                // +0x110
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
class CDefinitionManager { public:
    CTavernGameDef* PeekDefFromGlobalIndex(long index);   // stand-in only
};
class CDefClassBase { public:
    virtual long* GetSubDefIndex(const char* name);       // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef_005ed4a0(CDefPointer& out);  // 0x005ED4A0 (GetSubDef<CTavernGameDef>)
};

// Stand-in for the same-module GetSubDef<CTavernGameDef> body (0x005ED4A0, landed separately; not graded here), as in the
// landed GetTotalWinnings 0x005EA310: compiled above the caller without inlining so VC7.1 sees what it writes.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef_005ed4a0(CDefPointer& out)
{
    long* index = GetSubDefIndex("CTavernGameDef");
    if (index == 0)
        return false;
    CTavernGameDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
    if (out.Object != def) {
        if (out.Object)
            out.Object->Release();
        out.Object = def;
        if (def)
            ++def->m_count;
    }
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetVisible(bool visible);      // slot 28 (+0x70)
    unsigned char _pad_0x04[0x1c];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x24];
    CDefClassBase* PDef;                        // +0x70
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
    template<class T> __forceinline bool GetInterfaceDirect(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    void SetInLimbo(bool inLimbo);              // 0x004C8CF0
};
class CThingPlayerCreature : public CThing { };

class CTCTavernGame;
class CTCGamePlayer { public:
    void SetGame(CTCTavernGame* game);          // 0x005EDC00
};
struct CHeroMenuComponent {
    void RequestClose();                        // 0x00625920
};
class CTCHeroStats : public CTCHeroStatsDonor {
 public:
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CMainGameComponent { public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CPlayerGui { public:
    unsigned char _pad_0x00[0x2bd];
    bool          m_bShowing;                   // +0x2bd
    void Close(bool immediately);               // 0x00438400
    void OpenGameUI();                          // 0x00437FF0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

class CTCTavernGame { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void LeaveGame();                   // slot 30 (+0x78)
    CThing*           ParentThing;              // +0x04 (CTCBase)
    unsigned char     _pad_0x08[0x18];
    bool              IsBeingPlayed;            // +0x20
    bool              GameWon;                  // +0x21
    unsigned char     _pad_0x22[6];
    bool              GameFinished;             // +0x28
    unsigned char     _pad_0x29[0xb];
    long              BaseBet;                  // +0x34
    long              LastBet;                  // +0x38
    long              TotalWinnings;            // +0x3c
    long              CurrentMoney;             // +0x40
    unsigned char     _pad_0x44[0x60];
    long              NumConsecutiveRoundsWon;  // +0xa4
    unsigned char     _pad_0xa8[0x18];
    ETavernGameStates TavernGameState;          // +0xc0
    ETavernGameStates PreviousTavernGameState;  // +0xc4
    unsigned char     _pad_0xc8[0x10];
    bool              GamePlayed;               // +0xd8
    void SetState(ETavernGameStates state)
    {
        if (g_bTavernGameQuitRequested) {
            g_bTavernGameQuitRequested = false;
            LeaveGame();
            return;
        }
        PreviousTavernGameState = TavernGameState;
        TavernGameState = state;
    }
    void GetDef(CDefPointer* out) { ParentThing->PDef->GetSubDef_005ed4a0(*out); }   // out of line 0x005EA000
    void GetHeroStats(CTCHeroStats** stats);    // 0x005EB4B0
    void EnterGame();
};

void CTCTavernGame::EnterGame()
{
    IsBeingPlayed = true;
    SetState(1);
    GamePlayed = false;
    GameFinished = false;
    GameWon = false;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    hero->SetVisible(false);
    hero->SetInLimbo(true);
    CDefPointer def;
    GetDef(&def);
    {
        CTCGamePlayer* gamePlayer;
        hero->GetInterface(&gamePlayer, 0xd0);
        gamePlayer->SetGame(this);
    }
    {
        CHeroMenuComponent* menu;
        hero->GetInterfaceDirect(&menu, 0xf6);
        menu->RequestClose();
    }
    g_pPlayerGui->Close(false);
    g_pPlayerGui->m_bShowing = true;
    g_pPlayerGui->OpenGameUI();
    TotalWinnings = 0;
    {
        CTCHeroStats* stats;
        GetHeroStats(&stats);
        if (BaseBet > stats->Money)
            BaseBet = stats->Money;
        LastBet = def->MinBet;
        CurrentMoney = stats->Money;
        NumConsecutiveRoundsWon = 0;
    }
}
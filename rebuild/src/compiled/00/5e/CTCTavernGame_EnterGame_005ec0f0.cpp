// CTCTavernGame::EnterGame() -- start playing: enter (or leave on a pending quit) the game state, hide the hero in limbo, bind the hero's game player, close the hero's menu, open the game UI, and seed the bets and money from the hero's purse and the def's minimum bet
// Leaf: ?EnterGame@CTCTavernGame
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EC0F0, 373 bytes (plain `ret`).
// Callees: CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0; the hero's virtual slot 28 (+0x70);
//          CThing::SetInLimbo(bool) 0x004C8CF0; CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer*) 0x005ED4A0 (the
//          inline GetDef, as the landed GetTotalWinnings 0x005EA310); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//          0x0040F020 (interfaces 0xD0 = word 6 bit 16 and 0xF6 = word 7 bit 22); CTCGamePlayer::SetGame(CTCTavernGame*)
//          0x005EDC00; the menu component's RequestClose() 0x00625920; CPlayerGui::Close(bool) 0x00438400 and the game-UI
//          opener 0x00437FF0 (manifest label is a propagated STL name); CTCTavernGame::GetHeroStats 0x005EB4B0; the def's
//          slot 1; the virtual LeaveGame (slot 30) through the inline SetState.
// Globals: quit request 0x013BAE1C, CMainGameComponent::Get() 0x013B86A0, (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790.
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
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_compiler.h"
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCBase;
typedef long ETavernGameStates;

extern bool g_bTavernGameQuitRequested;         // 0x013BAE1C

#include "engine/ETCInterfaceType.h"                        // NF64: the shared enum (CTCQuickAccessMenu.h DEACTIVATE includes it; was a local empty enum)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
// NF170 (q4278, 2026-11-20): the sub-def call 0x005ED4A0 is the shared CDefClassBase::GetSubDef<CTavernGameDef> template (Ego_r.pdb +
//   FableWin.pdb ??$GetSubDef@VCTavernGameDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z; the callee pushes the literal
//   'CTavernGameDef'), its noinline body visible (engine/CDefClassBase_GetSubDef.h FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE), on the
//   shared CDefPointer / CTavernGameDef (Ego_r member names) and fable_string.h. Was a local def / CDefPointer / CDefClassBase model (and
//   local string classes) with a stand-in GetSubDef body.
#include "fable_string.h"
#include "engine/CTavernGameDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
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
    unsigned char _pad_0x54[0x1c];
    CDefClassBase* PDef;                        // +0x70
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterfaceDirect(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
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
class CTCHeroStats : public CTCHeroStatsDonor {
 public:
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
};
#include "engine/CCountedPointer.h"
// NF64 (round 7, q10651): the menu component is the hero's CTCQuickAccessMenu (TC 0xF6 = TCI_QUICK_ACCESS_MENU) and its call the
//   Ego_r.pdb 0x0066A140 / FableWin.pdb 0x02119DD0 CTCQuickAccessMenu::Deactivate (engine/CTCQuickAccessMenu.h DEACTIVATE; out of
//   line at 0x00625920); was the stand-in CHeroMenuComponent::RequestClose.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE
class CEnginePrimitivePrepareInfo;
#include "engine/CTCQuickAccessMenu.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char _pad_0x00[0x2bd];
    bool          m_bShowing;                   // +0x2bd
    void Close(bool immediately);               // 0x00438400
    void OpenGameUI();                          // 0x00437FF0
};

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
    void GetDef(CDefPointer<const CTavernGameDef>* out) { ParentThing->PDef->GetSubDef(out); }   // out of line 0x005EA000
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
    CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature();
    hero->SetVisible(false);
    hero->SetInLimbo(true);
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    {
        CTCGamePlayer* gamePlayer;
        hero->GetInterface(&gamePlayer, 0xd0);
        gamePlayer->SetGame(this);
    }
    {
        CTCQuickAccessMenu* menu;
        hero->GetInterfaceDirect(&menu, 0xf6);
        menu->Deactivate();
    }
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Close(false);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().m_bShowing = true;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().OpenGameUI();
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
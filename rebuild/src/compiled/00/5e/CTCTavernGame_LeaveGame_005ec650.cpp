// CTCTavernGame::LeaveGame() -- stop playing: leave (or record) the state, announce the game master's departure, hand the hero back (mode, visibility, game player), close the game UI, pay out the money held, clear the text box, bet and subtitles
// Leaf: ?LeaveGame@CTCTavernGame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EC650, 362 bytes (plain `ret`); CTCTavernGame vtable 0x012560BC slot 30.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CTCBase::GetWorld 0x00686A70 (manifest "CViewBase::
//          GetDisplayEngine" is propagated); CThing::GetPos() const 0x004C73D0; CMessageEventManager::AddMessage
//          0x00687540 (on the world's manager at +0x60); CPlayerManager::GetMainPlayer 0x00449970; CPlayer::RemoveMode
//          0x00634190; CPlayer::GetPControlledCreature 0x00487DC0; the hero's virtual slot 28 (+0x70); CThing::SetInLimbo(bool)
//          0x004C8CF0; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0xD0 = word 6 bit 16);
//          CTCGamePlayer::SetGame(CTCTavernGame*) 0x005EDC00; CPlayerGui::Open 0x004374B0 (manifest "Open" is
//          propagated); CTCTavernGame::GetHeroStats 0x005EB4B0; CTCHeroStats::SetMoney(long, bool) 0x0057A774;
//          CTCTavernGame::ClearBox 0x005EC5C0, ::ClearBetAndMoney 0x005E9F30; CSubtitleRenderer::Clear(bool) 0x00492BAB;
//          the virtual LeaveGame (slot 30) through the inline SetState.
// Globals: quit request 0x013BAE1C, frames per second 0x01375550, g_pMainGameComponent 0x013B86A0, g_pPlayerGui 0x013B8790.
// Layout (retail, as the landed SetStateAfterTime 0x005E9B70): IsBeingPlayed +0x20, CurrentMoney +0x40, TavernGameState
//          +0xc0, PreviousTavernGameState +0xc4, PGameMaster (CIntelligentPointer<CThingAICreature>, PDB +0xec) +0xd0; CPlayerGui
//          subtitle renderer +0x18, a bool at +0x2bd.
// Shape: SetState(0) is inlined (a pending quit calls LeaveGame virtually instead); the frame rate and the message
//        manager are named locals read before the argument pushes (ebp, and the spilled `push ecx` word); the unchecked
//        GetInterface (end/hit choice as one ternary, so the component lands in ecx) and the stats out pointer each sit
//        in their own `{}` block so they share that single frame word.
class CTCBase;
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
typedef long ETavernGameStates;

extern bool g_bTavernGameQuitRequested;         // 0x013BAE1C
extern long g_framesPerSecond;                  // 0x01375550

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
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
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
    void SetInLimbo(bool inLimbo);              // 0x004C8CF0
};
class CThingPlayerCreature : public CThing { };

struct CBaseIntelligentPointer {                // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};

class CMessageEventManager { public:
    void* AddMessage(long type, unsigned long distance, C3DVector* pos, CThing* source, CThing* extra, float a, float b, long c);   // 0x00687540
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessages;          // +0x60
};

class CTCTavernGame;
class CTCGamePlayer { public:
    void SetGame(CTCTavernGame* game);          // 0x005EDC00
};
class CTCHeroStats { public:
    void SetMoney(long money, bool notify);     // 0x0057A774
};
class CPlayer { public:
    void RemoveMode(long mode);                 // 0x00634190
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};
class CMainGameComponent { public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CSubtitleRenderer { public:
    void Clear(bool now);                       // 0x00492BAB
};
class CPlayerGui { public:
    unsigned char     _pad_0x00[0x18];
    CSubtitleRenderer Subtitles;                // +0x18
    unsigned char     _pad_0x19[0x2a4];
    bool              m_bShowing;               // +0x2bd
public:
    void Open();   // 0x004374B0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

class CTCBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void LeaveGame();                   // slot 30 (+0x78)
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCTavernGame : public CTCBase {
public:
    virtual void LeaveGame();
 public:
    unsigned char     _pad_0x04[0x1c];
    bool              IsBeingPlayed;            // +0x20
    unsigned char     _pad_0x21[0x1f];
    long              CurrentMoney;             // +0x40
    unsigned char     _pad_0x44[0x7c];
    ETavernGameStates TavernGameState;          // +0xc0
    ETavernGameStates PreviousTavernGameState;  // +0xc4
    unsigned char     _pad_0xc8[8];
    CIntelligentPointer<CThing> PGameMaster; // +0xd0
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
    void GetHeroStats(CTCHeroStats** stats);    // 0x005EB4B0
    void ClearBox();                            // 0x005EC5C0
    void ClearBetAndMoney();                    // 0x005E9F30
};

void CTCTavernGame::LeaveGame()
{
    IsBeingPlayed = false;
    SetState(0);
    if (PGameMaster.Get() && !PGameMaster.Get()->IsDead()) {
        CThing* gameMaster = PGameMaster.Get();
        long framesPerSecond = g_framesPerSecond;
        CMessageEventManager* messages = GetWorld().m_pMessages;
        messages->AddMessage(0x40, framesPerSecond * 5, (C3DVector*)&gameMaster->GetPos(),
                                       PGameMaster.Get(), 0, 0.0f, 0.0f, 0);
    }
    CPlayer* player = &g_pMainGameComponent->PlayerManager->GetMainPlayer();
    player->RemoveMode(0x1a);
    CThing* hero = player->GetPControlledCreature();
    hero->SetVisible(true);
    hero->SetInLimbo(false);
    {
        CTCGamePlayer* gamePlayer;
        hero->GetInterface(&gamePlayer, 0xd0);
        gamePlayer->SetGame(0);
    }
    g_pPlayerGui->Open();
    g_pPlayerGui->m_bShowing = false;
    {
        CTCHeroStats* stats;
        GetHeroStats(&stats);
        stats->SetMoney(CurrentMoney, true);
    }
    ClearBox();
    ClearBetAndMoney();
    g_pPlayerGui->Subtitles.Clear(false);
}
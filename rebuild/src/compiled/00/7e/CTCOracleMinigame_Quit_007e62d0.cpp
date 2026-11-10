// CTCOracleMinigame::Quit() -- end the oracle minigame: enter the quit stage, take the main player out of the minigame mode, drop pending inputs, wait the def's quit time, hide the minigame info and the subtitle override, show the win/lose text and report the level reached to the online scoreboard
// Leaf: ?Quit@CTCOracleMinigame@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E62D0, 369 bytes (plain `ret`).
// Callees: CTCBase::GetPlayerManager 0x00686D40 (byte twin of the 0x00686D50 the landed SetText calls; manifest
//          "CViewBase::GetDisplayEngine" is propagated); CPlayerManager::GetMainPlayer 0x00449970 / its twin 0x00449960;
//          CPlayer::GetCurrentMode 0x00633BE0, ::RemoveMode 0x00634190, ::GetPControlledCreature 0x00487DC0; free 0x00BFEA14 (list
//          nodes); CWorld::GetWorldFrame 0x0049D870; __ftol2 0x00BFEA70; CDrawQuestInfo::DisplayMiniGameInfo 0x00654593;
//          CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E) / ~CCharString 0x0099EAE0;
//          CSubtitleRenderer::SetButton2OverrideText 0x0048F881; CTCOracleMinigame::SetText 0x007E6230;
//          CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (scoreboard 0x10d);
//          CTCHeroOnlineScoreboard::UpdateScoreForActivity 0x0055F3B0.
// Globals: frames per second 0x01375550, (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790, CMainGameComponent::Get() 0x013B86A0.
// Layout (rebuild/include/engine/CTCOracleMinigame.h is the PDB, data only; the retail STLport list is one sentinel pointer
//          (PDB 12 bytes), so PDef sits at +0xe8 (PDB +0xf0)): Level +0x10, Stage +0x14, WaitTime +0x1c, Success +0xa8,
//          InputsPending +0xe0, PDef +0xe8. Def (descriptive): quit wait seconds +0x28, lose text +0x44, win text +0x48.
//          CPlayerGui: subtitles +0x18, a showing-info flag +0xf8, the quest-info drawer +0x1c8.
// Shape: the inline list clear re-reads the sentinel after every free; the frame rate and the def are named locals read
//          before the world frame call; the scoreboard is the unset-out-pointer GetInterface.
// Name fix (agentNF5 2026-11-10): CPlayer::RemoveMode 0x00634190 takes EPlayerMode (?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z, Ego_r.pdb / FableWin.pdb), was long.
enum EPlayerMode;                                  // MS forward enum (CPlayer::RemoveMode 0x00634190 takes it)
extern "C" void free(void* p);                  // 0x00BFEA14
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);   // 0x0055F3B0
};
class CPlayer { public:
    long GetCurrentMode();                      // 0x00633BE0
    void RemoveMode(EPlayerMode mode);                 // 0x00634190
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
private:
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad_0x00[0x1c];
    const CPlayerManager* PlayerManager;        // +0x1c
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};

class CSubtitleRenderer { public:
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
};
struct CDrawQuestInfo {
    void DisplayMiniGameInfo(long a, long b, long c);       // 0x00654593
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char     _pad_0x00[0x18];
    CSubtitleRenderer Subtitles;                // +0x18
    unsigned char     _pad_0x19[0xf8 - 0x19];
    bool              m_bShowMiniGameInfo;      // +0xf8
    unsigned char     _pad_0xf9[0x1c8 - 0xf9];
    CDrawQuestInfo*   QuestInfo;                // +0x1c8
};
class COracleMinigameDef { public:
    unsigned char _pad00[0x28];
    float QuitWaitTime;                         // +0x28 (seconds)
    unsigned char _pad2c[0x44 - 0x2c];
    unsigned long LoseText;                     // +0x44
    unsigned long WinText;                      // +0x48
};
struct CInputList {                             // std::list<EOracleMinigameInput>: one sentinel pointer
    struct node { node* next; node* prev; long value; };
    node* head;
    void clear()
    {
        node* cur = head->next;
        while (cur != head) {
            node* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        head->next = head;
        head->prev = head;
    }
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCOracleMinigame : public CTCBase { public:
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    unsigned long CurrentText;                  // +0x0c
    long Level;                                 // +0x10
    long Stage;                                 // +0x14 EOracleMinigameState
    long PostWaitStage;                         // +0x18
    long WaitTime;                              // +0x1c
    unsigned char _pad20[0xa8 - 0x20];
    bool Success;                               // +0xa8
    unsigned char _pada9[0xe0 - 0xa9];
    CInputList InputsPending;                   // +0xe0
    CThing* SpeechMarker;                       // +0xe4
    COracleMinigameDef* PDef;                   // +0xe8 CDefPointer<const COracleMinigameDef>
    void SetText(unsigned long text);           // 0x007E6230
    void Quit();
};

void CTCOracleMinigame::Quit()
{
    Stage = 5;
    CPlayer* player = &GetPlayerManager().GetMainPlayer();
    if (player->GetCurrentMode() == 0x29)
        player->RemoveMode((EPlayerMode)(0x29));
    InputsPending.clear();
    long framesPerSecond = CMainGameComponent::GetConstantFPS();
    COracleMinigameDef* def = PDef;
    WaitTime = (long)(CWorld::GetFrame() + framesPerSecond * def->QuitWaitTime);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().QuestInfo->DisplayMiniGameInfo(0, 7, 0);
    {
        CCharString empty("");
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Subtitles.SetButton2OverrideText(empty);
    }
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().m_bShowMiniGameInfo = false;
    if (Success)
        SetText(PDef->WinText);
    else
        SetText(PDef->LoseText);
    CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroOnlineScoreboard* scoreboard;
    hero->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard);
    scoreboard->UpdateScoreForActivity(0x15, (float)Level, false);
}
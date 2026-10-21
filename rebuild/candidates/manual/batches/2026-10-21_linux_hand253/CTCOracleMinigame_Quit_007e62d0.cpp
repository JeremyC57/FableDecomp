// CTCOracleMinigame::Quit() -- end the oracle minigame: enter the quit stage, take the main player out of the minigame mode, drop pending inputs, wait the def's quit time, hide the minigame info and the subtitle override, show the win/lose text and report the level reached to the online scoreboard
// Leaf: ?Quit@CTCOracleMinigame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E62D0, 369 bytes (plain `ret`).
// Callees: CTCBase::GetPlayerManager 0x00686D40 (byte twin of the 0x00686D50 the landed SetText calls; manifest
//          "CViewBase::GetDisplayEngine" is propagated); CPlayerManager::GetMainPlayer 0x00449970 / its twin 0x00449960;
//          CPlayer::GetCurrentMode 0x00633BE0, ::RemoveMode 0x00634190, ::GetHeroThing 0x00487DC0; free 0x00BFEA14 (list
//          nodes); CWorld::GetWorldFrame 0x0049D870; __ftol2 0x00BFEA70; CDrawQuestInfo::DisplayMiniGameInfo 0x00654593;
//          CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E) / ~CCharString 0x0099EAE0;
//          CSubtitleRenderer::SetButton2OverrideText 0x0048F881; CTCOracleMinigame::SetText 0x007E6230;
//          CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (scoreboard 0x10d);
//          CTCHeroOnlineScoreboard::UpdateScoreForActivity 0x0055F3B0.
// Globals: frames per second 0x01375550, g_pPlayerGui 0x013B8790, g_pMainGameComponent 0x013B86A0.
// Layout (rebuild/include/engine/CTCOracleMinigame.h is the PDB, data only; the retail STLport list is one sentinel pointer
//          (PDB 12 bytes), so PDef sits at +0xe8 (PDB +0xf0)): Level +0x10, Stage +0x14, WaitTime +0x1c, Success +0xa8,
//          InputsPending +0xe0, PDef +0xe8. Def (descriptive): quit wait seconds +0x28, lose text +0x44, win text +0x48.
//          CPlayerGui: subtitles +0x18, a showing-info flag +0xf8, the quest-info drawer +0x1c8.
// Shape: the inline list clear re-reads the sentinel after every free; the frame rate and the def are named locals read
//          before the world frame call; the scoreboard is the unset-out-pointer GetInterface.
extern "C" void free(void* p);                  // 0x00BFEA14
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
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
struct CTCHeroOnlineScoreboard {
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);   // 0x0055F3B0
};
struct CPlayer {
    long GetCurrentMode();                      // 0x00633BE0
    void RemoveMode(long mode);                 // 0x00634190
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
    CPlayer* GetMainPlayer() const;             // 0x00449960 (byte twin)
};
struct CMainGameComponent {
    unsigned char   _pad_0x00[0x1c];
    const CPlayerManager* PlayerManager;        // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSubtitleRenderer {
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
};
struct CDrawQuestInfo {
    void DisplayMiniGameInfo(long a, long b, long c);       // 0x00654593
};
struct CPlayerGui {
    unsigned char     _pad_0x00[0x18];
    CSubtitleRenderer Subtitles;                // +0x18
    unsigned char     _pad_0x19[0xf8 - 0x19];
    bool              m_bShowMiniGameInfo;      // +0xf8
    unsigned char     _pad_0xf9[0x1c8 - 0xf9];
    CDrawQuestInfo*   QuestInfo;                // +0x1c8
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
struct COracleMinigameDef {
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
struct CTCOracleMinigame {
    virtual void v00();
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
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
    void SetText(unsigned long text);           // 0x007E6230
    void Quit();
};

void CTCOracleMinigame::Quit()
{
    Stage = 5;
    CPlayer* player = GetPlayerManager()->GetMainPlayer();
    if (player->GetCurrentMode() == 0x29)
        player->RemoveMode(0x29);
    InputsPending.clear();
    long framesPerSecond = g_framesPerSecond;
    COracleMinigameDef* def = PDef;
    WaitTime = (long)(CWorld::GetWorldFrame() + framesPerSecond * def->QuitWaitTime);
    g_pPlayerGui->QuestInfo->DisplayMiniGameInfo(0, 7, 0);
    {
        CCharString empty("");
        g_pPlayerGui->Subtitles.SetButton2OverrideText(empty);
    }
    g_pPlayerGui->m_bShowMiniGameInfo = false;
    if (Success)
        SetText(PDef->WinText);
    else
        SetText(PDef->LoseText);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    CTCHeroOnlineScoreboard* scoreboard;
    hero->GetInterface(&scoreboard, 0x10d);
    scoreboard->UpdateScoreForActivity(0x15, (float)Level, false);
}

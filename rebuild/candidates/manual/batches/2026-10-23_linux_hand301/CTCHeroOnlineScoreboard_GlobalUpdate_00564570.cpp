// CTCHeroOnlineScoreboard::GlobalUpdate(bool) -- opens the scoreboard when requested, then steps the online state machine one network task (log on, upload, clan list, clan logos, choose clan, member list, member logos, scoreboard, display), aborting with a text-symbol message on failure
// Leaf: GlobalUpdate
// Retail: Fable.exe @ 0x00564570, 768 bytes incl. the 9-entry jump table at 0x00564844, `ret 4`. Ego_r.pdb
// `?GlobalUpdate@CTCHeroOnlineScoreboard@@UAEX_N@Z` (vtable slot 26; the manifest names it vfunc_26).
// Callees: ShowScoreboard 0x00563130, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
// LogOnToRegistrationServer(CCharString&) 0x00560D00 (manifest "ProcessLogin"), AbortWithMessage(const CCharString&)
// 0x00563410 (landed), UploadPlayerScores 0x00561450, DownloadListOfMyClans 0x005615C0, DownloadMyClansLogos 0x00561980,
// ChooseAClan 0x0055FEC0, CWideString::operator!= 0x0099B0F0 against the empty wide string 0x013BCA24,
// DownloadMemberListForSelectedClan 0x00561DC0, DownloadLogosForThisClansMembers 0x005621B0,
// DownloadScoreboardForSelectedClan 0x00563480, DisplayScoreboardGUI 0x00563920; the open request is the global bool
// 0x013B8AE1.
// Layout: retail MyClans +0x38, SelectedClan +0x3c, ScoreboardState +0x50 (Ego_r.pdb +0x44/+0x50/+0x70 less the
// STLport map/vector/list shrink; landed AbortWithMessage 0x00563410 has ScoreboardState +0x50).
// Shape: a switch on ScoreboardState (case 1 lowest); every case returns on its own and each case's message string gets
// its own frame word (VC7.1 does not share slots between switch cases).
struct CCharString {
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    bool operator!=(const CWideString& other) const;   // 0x0099B0F0
};
extern const CWideString g_emptyWideString;          // 0x013BCA24
extern bool g_bShowOnlineScoreboard;                 // 0x013B8AE1

struct SListNodeBase {
    SListNodeBase* _M_next;
    SListNodeBase* _M_prev;
};
struct CList {
    SListNodeBase* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};

class CTCHeroOnlineScoreboard {
public:
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    unsigned char  _pad04[0x38 - 0x04];              // vftable at +0x00
    CList          MyClans;                          // +0x38
    CWideString    SelectedClan;                     // +0x3c
    unsigned char  _pad40[0x50 - 0x40];
    long           ScoreboardState;                  // +0x50

    void ShowScoreboard();                           // 0x00563130
    ENetworkSuccess LogOnToRegistrationServer(CCharString& message);   // 0x00560D00
    void AbortWithMessage(const CCharString& textSymbol);              // 0x00563410
    ENetworkSuccess UploadPlayerScores();            // 0x00561450
    ENetworkSuccess DownloadListOfMyClans();         // 0x005615C0
    ENetworkSuccess DownloadMyClansLogos();          // 0x00561980
    void ChooseAClan();                              // 0x0055FEC0
    ENetworkSuccess DownloadMemberListForSelectedClan();   // 0x00561DC0
    ENetworkSuccess DownloadLogosForThisClansMembers();    // 0x005621B0
    ENetworkSuccess DownloadScoreboardForSelectedClan();   // 0x00563480
    void DisplayScoreboardGUI();                     // 0x00563920
    virtual void GlobalUpdate(bool paused);
};

void CTCHeroOnlineScoreboard::GlobalUpdate(bool paused)
{
    if (g_bShowOnlineScoreboard)
        ShowScoreboard();
    switch (ScoreboardState) {
    case 1: {
        CCharString message;
        ENetworkSuccess result = LogOnToRegistrationServer(message);
        if (result == NETWORK_BUSY) {
            AbortWithMessage(message);
            return;
        }
        if (result == NETWORK_SUCCESS)
            ScoreboardState = 2;
        return;
    }
    case 2: {
        ENetworkSuccess result = UploadPlayerScores();
        if (result == NETWORK_BUSY) {
            AbortWithMessage("TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK");
            return;
        }
        if (result == NETWORK_SUCCESS)
            ScoreboardState = 3;
        return;
    }
    case 3: {
        ENetworkSuccess result = DownloadListOfMyClans();
        if (result == NETWORK_BUSY) {
            AbortWithMessage("TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK");
            return;
        }
        if (result == NETWORK_SUCCESS) {
            if (MyClans.empty()) {
                AbortWithMessage("TEXT_GUI_SCOREBOARD_ERROR_NO_CLANS_JOINED");
                return;
            }
            ScoreboardState = 4;
        }
        return;
    }
    case 4: {
        ENetworkSuccess result = DownloadMyClansLogos();
        if (result == NETWORK_BUSY) {
            AbortWithMessage("TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK");
            return;
        }
        if (result == NETWORK_SUCCESS)
            ScoreboardState = 5;
        return;
    }
    case 5:
        ChooseAClan();
        if (SelectedClan != g_emptyWideString)
            ScoreboardState = 6;
        return;
    case 6: {
        ENetworkSuccess result = DownloadMemberListForSelectedClan();
        if (result == NETWORK_BUSY) {
            AbortWithMessage("TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK");
            return;
        }
        if (result == NETWORK_SUCCESS)
            ScoreboardState = 7;
        return;
    }
    case 7: {
        ENetworkSuccess result = DownloadLogosForThisClansMembers();
        if (result == NETWORK_BUSY) {
            AbortWithMessage("TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK");
            return;
        }
        if (result == NETWORK_SUCCESS)
            ScoreboardState = 8;
        return;
    }
    case 8: {
        ENetworkSuccess result = DownloadScoreboardForSelectedClan();
        if (result == NETWORK_BUSY) {
            AbortWithMessage("TEXT_GUI_SCOREBOARD_ERROR_GENERAL_NETWORK");
            return;
        }
        if (result == NETWORK_SUCCESS)
            ScoreboardState = 9;
        return;
    }
    case 9:
        DisplayScoreboardGUI();
        return;
    }
}

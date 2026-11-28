#ifndef FABLE_ENGINE_CTCHEROONLINESCOREBOARD_H
#define FABLE_ENGINE_CTCHEROONLINESCOREBOARD_H
// New header (chunk 2039, row 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject): the hero's online-scoreboard thing component
// on the polymorphic CTCBase (define FABLE_RETAIL_CTCBASE), interface type TCI_HERO_ONLINE_SCOREBOARD = 269 (FableWin.pdb
// ??$GetTC@VCTCHeroOnlineScoreboard@@@CThing@@...; the row's GetTC key 10Dh). The activity enum is NESTED in the class (Ego_r.pdb
// ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z; its enumerator names are not in the
// name sets, so none are declared). UpdateScoreForActivity is out of line in retail at 0x0055F3B0 (`ret 0Ch`). Members opaque
// (Ego_r.pdb 0x88 bytes with Dinkumware containers; the retail size is not needed here).
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#include "stlport/fable_string.h"
#include "boost/scoped_ptr.h"
#include "engine/LHHttp2.h"
#include "engine/CUserProfileManager.h"
#endif
#include "engine/ETCInterfaceType.h"
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
class CTexture;
namespace NUISystem { class CComponent; }
#endif
#if defined(FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK) && !defined(FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS)
#error "Scoreboard NETWORK requires its original MEMBERS layout"
#endif
class CTCHeroOnlineScoreboard : public CTCBase {
public:
#ifdef FABLE_RETAIL_SCOREBOARD_SERIALISE_PASS30
    // Both symbol files identify this override, also present in retail slot 1.
    explicit CTCHeroOnlineScoreboard(CThing&);
    virtual void OnSerialise(CPersistContext& context, bool in_game);
#endif
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
    // Exact original enumerators from both PDB field-list records.
    enum EOnlineScoreboardActivity { OSA_MURDERS=0, OSA_DECAPITATIONS=1, OSA_MULTIPLIER=2, OSA_EXPERIENCE=3, OSA_CHICKEN_KICK_DISTANCE=4, OSA_WEALTH=5, OSA_SEXUAL_ENCOUNTERS=6, OSA_BIGGEST_FISH=7, OSA_ARCHERY_SCORE=8, OSA_SPOUSES=9, OSA_TEMPLE_CONTRIBUTION=10, OSA_ADMIRERS=11, OSA_WEDDINGS=12, OSA_DIVORCES=13, OSA_TIMES_WIDOWED=14, OSA_PUKINGS=15, OSA_RENT_COLLECTED=16, OSA_HIGH_SCORE_CHICKEN_KICK=17, OSA_HIGH_SCORE_PAIRS=18, OSA_HIGH_SCORE_CARD_SORTING=19, OSA_HIGH_SCORE_GUESS_THE_ADDITION=20, OSA_HIGH_SCORE_ORACLE_MINIGAME=21, OSA_HIGH_SCORE_COIN_GOLF_OV=22, OSA_HIGH_SCORE_COIN_GOLF_SS=23, OSA_HIGH_SCORE_BLACKJACK=24, OSA_HIGH_SCORE_HAPENNY=25, OSA_SUCCESSIVE_WINS_PAIRS=26, OSA_SUCCESSIVE_WINS_CARD_SORTING=27, OSA_SUCCESSIVE_WINS_GUESS_THE_ADDITION=28, OSA_SUCCESSIVE_WINS_ORACLE_MINIGAME=29, OSA_SUCCESSIVE_WINS_COIN_GOLF_OV=30, OSA_SUCCESSIVE_WINS_COIN_GOLF_SS=31, OSA_SUCCESSIVE_WINS_BLACKJACK=32, OSA_SUCCESSIVE_WINS_HAPENNY=33, OSA_CRIMES_REPORTED=34, OSA_TOTAL_FINES=35, OSA_TOTAL_FINES_PAID=36, OSA_BRIBES=37, OSA_BIGGEST_FISH_IN_COMPETITION=38, OSA_NUM_ACTIVITIES=39 };
#ifdef FABLE_RETAIL_SCOREBOARD_TRANSACTION_PASS28
    // Both donor TPI records agree on the names and values. Retail UpdateTransaction uses 100 through 104.
    enum ETaskState { ETS_UNSTARTED=100, ETS_PREPARING, ETS_SENDING_REQUEST_ASYNC, ETS_RECEIVING_HEADER_ASYNC, ETS_RECEIVING_DOCUMENT_ASYNC };
#endif
    enum ENetworkSuccess { ENS_SUCCESS=1000, ENS_FAILURE=1001, ENS_PENDING=1002 };
#else
#ifdef FABLE_RETAIL_MINION_CONFIRMATION_PASS29
    enum EOnlineScoreboardActivity { OSA_BRIBES = 37 };
#else
    enum EOnlineScoreboardActivity { };
#endif
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_ONLINE_SCOREBOARD; }
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_SHOWSCOREBOARD
    // NF213 opt-in (2026-11-21, q6333 / q6342; row 0x00563130): Ego_r.pdb / FableWin.pdb ?ShowScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ
    // (public; the row's Leaf) and the by-value signed-in password getter next to GetSignedInUserName below (the row's inline
    // `GetSignedInUserName() == "" || GetSignedInPassword() == ""` test; the password word +0x114 beside the user name +0x110). Needs
    // FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK. Declarations / inline accessor only: no layout change.
    void ShowScoreboard();
private:   // Ego_r.pdb / FableWin.pdb ?GetSignedInPassword@CTCHeroOnlineScoreboard@@AAE?AVCCharString@@XZ (private; 0x005D7780 out of line)
    CCharString GetSignedInPassword() { return CUserProfileManager::Get().GetScoreboardPassword().ToCharString(); }
public:
#endif
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool flag);   // 0x0055F3B0
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_CLOSE
    // NF62 opt-in (2026-11-17, row 0x00587847 CTCInventory::ClosePCInventory): Ego_r.pdb / FableWin.pdb ?Close@CTCHeroOnlineScoreboard@@QAEXXZ,
    // retail 0x00562E70 (Ego_r donor order ClearScoreboard 0x005DA4E0 -> Close 0x005DA530 = retail ClearScoreboard 0x00562E10 -> 0x00562E70;
    // the body drops the HTTP connection and calls ClearScoreboard). The landed self row spells it ResetScoreboard (descriptive): NAME_FIXES.
    void Close();                                                    // 0x00562E70
#endif
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_CLEARSCOREBOARD
    // NF70 opt-in (round 7, row 0x00562E70): Ego_r.pdb 0x005DA4E0 / FableWin.pdb ?ClearScoreboard@CTCHeroOnlineScoreboard@@AAEXXZ (private),
    // retail 0x00562E10 (Close's callee). Declaration only: no layout change.
private:
    void ClearScoreboard();                                          // 0x00562E10
public:
#endif
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
    // Retail opt-in (acctB c2058, row 0x00563920 DisplayScoreboardGUI; the model the same-TU landed rows, e.g. ChooseAClan 0x0055FEC0, carry
    // locally): Ego_r.pdb CTCHeroOnlineScoreboard members with retail STLport sizes -- PictureCache +0x10, LocalScoreboard +0x1C,
    // ActivityInfo +0x28, SessionID +0x34, MyClans +0x38, SelectedClan +0x3C, ClanMembers +0x40, DownloadedClanScoreboard +0x44,
    // ScoreboardState +0x50, TaskState +0x54, PScoreboardScreen +0x58, PClanMenuEntry +0x5C, PConnectingMessage +0x60,
    // PConnectingMessage1 +0x64 -- and the nested Ego_r.pdb classes CActivity / CClanInfo / CClanMember. Needs FABLE_RETAIL_CTCBASE,
    // FABLE_RETAIL_CCHARSTRING / FABLE_RETAIL_CWIDESTRING and FABLE_RETAIL_CCOUNTEDPOINTER. Member functions: declarations only.
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_ORIGINAL_ALIASES_PASS22
    typedef float TActivityScore;
#endif
    class CActivity { public:                                       // CTCHeroOnlineScoreboard::CActivity (Ego_r.pdb, 0x10)
        CCharString   Name;                                         // +0x00
        CCharString   Description;                                  // +0x04
        CCharString   UnitOfMeasurement;                            // +0x08
        unsigned char NumDecPlacesToShow;                           // +0x0C
        bool          LowerIsBetter;                                // +0x0D
        CWideString GetScoreAsString(float score);                  // 0x0055E6B0
    };
    class CClanInfo { public:                                       // CTCHeroOnlineScoreboard::CClanInfo (Ego_r.pdb, 0x14)
        CWideString Name;
        long        ClanID;
        CWideString Description;
        CCharString HomePageURL;
        int         NumMembers;
    };
    class CClanMember { public:                                     // CTCHeroOnlineScoreboard::CClanMember (Ego_r.pdb, 0x0C)
        CWideString Name;                                           // +0x00
        long        MemberID;                                       // +0x04
        CWideString Description;                                    // +0x08
        bool operator==(const CWideString& name) { return Name == name; }
    };
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_ORIGINAL_ALIASES_PASS22
    typedef std::pair<TActivityScore,CWideString> TOnlineSingleScore;
    typedef std::vector<TOnlineSingleScore> TOnlineTopScoresInSingleEvent;
    typedef std::map<EOnlineScoreboardActivity,TOnlineTopScoresInSingleEvent> TOnlineScoreboard;
    typedef std::map<EOnlineScoreboardActivity,TActivityScore> TLocalScoreboard;
#endif
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
    boost::scoped_ptr<LHNetworkLib::LHHttp2> PHttp;
#else
    void*                                       PHttp;              // +0x0C scoped_ptr<LHNetworkLib::LHHttp2>
#endif
    std::map<long, CCountedPointer<CTexture> >  PictureCache;       // +0x10
    std::map<EOnlineScoreboardActivity, float>  LocalScoreboard;    // +0x1C
    std::vector<CActivity>                      ActivityInfo;       // +0x28
    CCharString                                 SessionID;          // +0x34
    std::list<CClanInfo>                        MyClans;            // +0x38
    CWideString                                 SelectedClan;       // +0x3C
    std::list<CClanMember>                      ClanMembers;        // +0x40
    std::map<EOnlineScoreboardActivity, std::vector<std::pair<float, CWideString> > > DownloadedClanScoreboard;   // +0x44
    long                                        ScoreboardState;    // +0x50
    long                                        TaskState;          // +0x54
    NUISystem::CComponent*                      PScoreboardScreen;  // +0x58
    NUISystem::CComponent*                      PClanMenuEntry;     // +0x5C
    NUISystem::CComponent*                      PConnectingMessage; // +0x60
    NUISystem::CComponent*                      PConnectingMessage1;    // +0x64
    bool IsScoreboardEmpty();                                       // 0x0055E7F0
    void DisplayErrorMessage(const CWideString& text);              // 0x0055EF00
private:
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
    ENetworkSuccess ResetAndOpenHTTPAndPrepareRequest(const std::string&,const CCharString&,bool);
    ENetworkSuccess UpdateTransaction();
    int GetDecryptedDocument(std::vector<std::string>&,const CCharString&);
    ENetworkSuccess DownloadScoreboardForSelectedClan();
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_LOGON_PASS28
    // Pass 28 0x00560D00. Both original PDBs and the GlobalUpdate call identify this private method.
    ENetworkSuccess LogOnToRegistrationServer(CCharString& errorText);
#endif
    CCharString GetGameIdent() { return CCharString("Fable1.5\r\nVersion 1\r\n"); }
    CCharString GetCDKey() { return CCharString("THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM"); }
    CCharString GetSignedInUserName() { return CUserProfileManager::Get().GetScoreboardUserName().ToCharString(); }
#endif
    CCharString ActivityEnumToCharString(const EOnlineScoreboardActivity& activity);   // 0x0055E7C0
    CCountedPointer<CTexture> GetCachedImage(const CWideString& folder, long id);     // 0x0055FC90
    void DisplayScoreboardGUI();
#endif
};
#endif

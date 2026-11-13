#ifndef FABLE_ENGINE_CTCHEROONLINESCOREBOARD_H
#define FABLE_ENGINE_CTCHEROONLINESCOREBOARD_H
// New header (chunk 2039, row 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject): the hero's online-scoreboard thing component
// on the polymorphic CTCBase (define FABLE_RETAIL_CTCBASE), interface type TCI_HERO_ONLINE_SCOREBOARD = 269 (FableWin.pdb
// ??$GetTC@VCTCHeroOnlineScoreboard@@@CThing@@...; the row's GetTC key 10Dh). The activity enum is NESTED in the class (Ego_r.pdb
// ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z; its enumerator names are not in the
// name sets, so none are declared). UpdateScoreForActivity is out of line in retail at 0x0055F3B0 (`ret 0Ch`). Members opaque
// (Ego_r.pdb 0x88 bytes with Dinkumware containers; the retail size is not needed here).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
class CTexture;
namespace NUISystem { class CComponent; }
#endif
class CTCHeroOnlineScoreboard : public CTCBase {
public:
    enum EOnlineScoreboardActivity { };
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_ONLINE_SCOREBOARD; }
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool flag);   // 0x0055F3B0
#ifdef FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
    // Retail opt-in (acctB c2058, row 0x00563920 DisplayScoreboardGUI; the model the same-TU landed rows, e.g. ChooseAClan 0x0055FEC0, carry
    // locally): Ego_r.pdb CTCHeroOnlineScoreboard members with retail STLport sizes -- PictureCache +0x10, LocalScoreboard +0x1C,
    // ActivityInfo +0x28, SessionID +0x34, MyClans +0x38, SelectedClan +0x3C, ClanMembers +0x40, DownloadedClanScoreboard +0x44,
    // ScoreboardState +0x50, TaskState +0x54, PScoreboardScreen +0x58, PClanMenuEntry +0x5C, PConnectingMessage +0x60,
    // PConnectingMessage1 +0x64 -- and the nested Ego_r.pdb classes CActivity / CClanInfo / CClanMember. Needs FABLE_RETAIL_CTCBASE,
    // FABLE_RETAIL_CCHARSTRING / FABLE_RETAIL_CWIDESTRING and FABLE_RETAIL_CCOUNTEDPOINTER. Member functions: declarations only.
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
    void*                                       PHttp;              // +0x0C scoped_ptr<LHNetworkLib::LHHttp2>
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
    CCharString ActivityEnumToCharString(const EOnlineScoreboardActivity& activity);   // 0x0055E7C0
    CCountedPointer<CTexture> GetCachedImage(const CWideString& folder, long id);     // 0x0055FC90
    void DisplayScoreboardGUI();
#endif
};
#endif

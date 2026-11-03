#ifndef FABLE_ENGINE_CTCHEROONLINESCOREBOARD_H
#define FABLE_ENGINE_CTCHEROONLINESCOREBOARD_H
// CTCHeroOnlineScoreboard (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): the retail class over FABLE_RETAIL_CTCBASE,
// declarations only (members after the base not modelled). FableWin.pdb ?StatGetInterfaceType@CTCHeroOnlineScoreboard@@SI?AW4ETCInterfaceType@@XZ
// (inline; GetTC key 0x10D), Ego_r.pdb / FableWin.pdb ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z
// (0x0055F3B0; the activity enum is nested).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCHeroOnlineScoreboard : public CTCBase {
public:
    enum EOnlineScoreboardActivity;                          // MS forward enum (rows name the values they use)
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_ONLINE_SCOREBOARD; }
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float value, bool);   // 0x0055F3B0
};
#endif

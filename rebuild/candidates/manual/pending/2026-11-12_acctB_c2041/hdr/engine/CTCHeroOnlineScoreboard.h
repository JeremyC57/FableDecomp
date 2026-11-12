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
class CTCHeroOnlineScoreboard : public CTCBase {
public:
    enum EOnlineScoreboardActivity { };
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_ONLINE_SCOREBOARD; }
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool flag);   // 0x0055F3B0
};
#endif

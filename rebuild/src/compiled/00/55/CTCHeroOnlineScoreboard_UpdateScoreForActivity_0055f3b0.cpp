// CTCHeroOnlineScoreboard::UpdateScoreForActivity(EOnlineScoreboardActivity, float, bool) -- records the hero's local best score for an activity (first score, or a better one; `lowerIsBetter` picks the direction)
// Leaf: ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055F3B0, 206 bytes, `ret 0Ch`.
// Callees: map<EOnlineScoreboardActivity,float>::find(const key&) @ 0x00566F50 (iterator by hidden pointer),
//          map::operator[](const key&) @ 0x005692B0 (float&), GFIntToCharString(long) @ 0x0099F830 (__fastcall,
//          hidden result in ecx), CCharString::CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&,
//          const CCharString&) @ 0x0099F570 (__fastcall), ~CCharString @ 0x0099EAE0 (x3); "New best score in activity ".
// Shape: the find result (a block-scoped iterator sharing its frame word with the later number temporary) is compared with the map's header word directly (`cmp edx,[esi]`); a first score is stored
// and returns at once; otherwise `!(score < best)` (`test ah,5 ; jp`) or `!(score > best)` (`test ah,41h ; jne`)
// leaves, and the improvement builds a "New best score" message (its log call compiled out of the retail build:
// the number temporary first, the literal in the dead flag slot, both destroyed before the store) that dies after
// the new best is stored.
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z (engine/CTCHeroOnlineScoreboard.h).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/GFIntToWideString.h"
#include "engine/CTCHeroOnlineScoreboard.h"
CCharString FABLE_FASTCALL GFIntToCharString(long value);                          // 0x0099F830

void CTCHeroOnlineScoreboard::UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool flag)
{
    if (LocalScoreboard.find(activity) == LocalScoreboard.end()) {
        LocalScoreboard[activity] = score;
        return;
    }
    if (flag) {
        if (!(score < LocalScoreboard[activity]))
            return;
    } else if (!(score > LocalScoreboard[activity])) {
        return;
    }
    CCharString message = CCharString("New best score in activity ") + GFIntToCharString(activity);
    LocalScoreboard[activity] = score;
}

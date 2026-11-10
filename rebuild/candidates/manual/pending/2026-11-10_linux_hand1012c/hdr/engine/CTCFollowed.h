#ifndef FABLE_ENGINE_CTCFOLLOWED_H
#define FABLE_ENGINE_CTCFOLLOWED_H
// NEW (chunk 1012c, row 0x008A7A70 CGameScriptInterface::TeleportAllFollowersToHeroPosition). Without the opt-in the header only
// names the class.
#ifdef FABLE_RETAIL_CTCFOLLOWED
// Retail opt-in: RTTI .?AVCTCFollowed@@ : CTCBase (needs FABLE_RETAIL_CTCBASE, forced below). FableWin.pdb
// ?StatGetInterfaceType@CTCFollowed@@SI?AW4ETCInterfaceType@@XZ (inline: TC 188 = TCI_FOLLOWED, `mov [esp+14h],0BCh` at 0x008A7AB7,
// the UsingComponents word +0x34 bit 28 test at 0x008A7A9E) and Ego_r.pdb / FableWin.pdb
// ?GetFollowers@CTCFollowed@@QAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z, out of line in retail at 0x006D5850
// (called through the non-const component lookup 0x0040F020 with the caller's zero-initialised vector). No members are modelled
// (pointer use only). Needs the includer's std::vector (stlport/fable_containers.h).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCFollowed : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_FOLLOWED; }
    void GetFollowers(std::vector<CThing*>& followers);                  // 0x006D5850
};
#else
class CTCFollowed;
#endif
#endif // FABLE_ENGINE_CTCFOLLOWED_H

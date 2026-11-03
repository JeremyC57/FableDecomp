#ifndef FABLE_ENGINE_CTCFOLLOWED_H
#define FABLE_ENGINE_CTCFOLLOWED_H
// Retail model (chunk 729, hand-written, declarations only): Ego_r.pdb CTCFollowed (TCI_FOLLOWED 188; the PDB members Followers,
// AIFollowers, FrameLastIdleReaction, FramesSincePlayerMoved, LastPlayerPosition from +0x0C are not used and not modelled, so no size
// is asserted). ?GetFollowers@CTCFollowed@@QAEXAAV?$vector@PAVCThing@@...@std@@@Z is retail 0x006D5850.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
class CThing;
class CTCFollowed : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_FOLLOWED; }
    void GetFollowers(std::vector<CThing*>&);                            // 0x006D5850
};
#endif

// CTCEnemy::IsAlliedWithFaction @ 0076c7f0
// Leaf: ?IsAlliedWithFaction@CTCEnemy@@QBE_NPBVCFaction@@@Z
// NF37 (2026-11-16): moved onto the shared engine/CTCEnemy.h (FABLE_RETAIL_CTCENEMY_QUERIES; Ego_r.pdb 0x0077AAA0 / FableWin.pdb
// ?IsAlliedWithFaction@CTCEnemy@@QBE_NPBVCFaction@@@Z). The object at +0x18 is the header's PFaction; its query is
// CFaction::IsAlliedWithFaction(const CFaction&) const 0x006B9460 (Ego_r.pdb ?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CFACTION_ISALLIEDWITHFACTION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCEnemy.h"
#include "engine/CFaction.h"

bool CTCEnemy::IsAlliedWithFaction(const CFaction* faction) const
{
    if (faction != 0) {
        const CFaction* own = PFaction;
        if (own != 0)
            return own->IsAlliedWithFaction(*faction);
    }
    return false;
}

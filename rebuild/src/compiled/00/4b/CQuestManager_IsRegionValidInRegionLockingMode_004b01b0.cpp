// CQuestManager::IsRegionValidInRegionLockingMode(const CRegion& region) const -- true when a quest between its start and end screens claims the region, or when none of those quests claims any region; false otherwise (the second pass only takes each quest's region list)
// Leaf: ?IsRegionValidInRegionLockingMode@CQuestManager@@QBE_NABVCRegion@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B01B0, 277 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?IsRegionValidInRegionLockingMode@CQuestManager@@QBE_NABVCRegion@@@Z; Ego_d.pdb quest_manager.obj record
//   (locals region; any_regions_specified; two `long i` loops each with pactive_quest; regions = the const reference to GetRegions()).
//   Only caller 0x004A3817 (the quest manager global 0x013B89FC, the world map's current region).
// Callees: CQuestManager::GetActiveQuestFromScriptName 0x004AF5B0 (same TU, body below -- its landed text), std::__find<const CRegion* const*,
//   const CRegion*> 0x004B74E0 (inline ContainsRegion; the STLport 4.6 _algobase.c body compiled here, kept out of line by VC7.1 itself), malloc / memmove / free 0x00BFEA0E / 0x00BFEAE6 /
//   0x00BFEA14 (the inline by-value GetRegions copy, kept alive by the `regions` reference and destroyed at the block end).
// Shape: Ego_d's `long i` loops (compared unsigned against size(), retail `jb`). Retail strength-reduces the first loop's element address
//   into ebx with the vector start read once (0x004B01D5 / 0x004B0226) and keeps i in a frame word: VC7.1 does that only with the
//   same-TU body of GetActiveQuestFromScriptName visible (it then knows the lookup writes no memory). Ego_d defines that body before
//   the row; the old draft's pointer-iterator loop (283v277) is not the source.
// Fidelity: `regions` is never read in retail's code: Ego_d records it (and a nested `long r` loop with `const CRegion* pregion`, a debug
//   walk that compiles to nothing in the release build and is omitted); the reference keeps the GetRegions() copy alive to the block end.
// Headers: overlay FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_REGIONLOCK (the REGIONLOCK inline region helpers on the full CActiveQuest record of
//   ACTIVEQUEST + the row's declaration), with the landed 0x004AF5B0 opt-in set (LOADGAMESTATE / ACTIVEQUEST / SAVEGAMESTATE layout).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_REGIONLOCK
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_GETACTIVEQUESTFROMSCRIPTNAME
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CQuestManager.h"

namespace std {
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                                         const random_access_iterator_tag&)   // 0x004B74E0
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}
}

// Same-TU body (Ego_d quest_manager.obj: GetActiveQuestFromScriptName precedes the row; the landed row 0x004AF5B0's text). Visible, it tells
// VC7.1 the lookup writes no memory, so the quest list start stays loop-invariant (retail strength-reduces the element address into ebx).
CQuestManager::CActiveQuest* CQuestManager::GetActiveQuestFromScriptName(const CCharString& name) const
{
    for (std::list<CCountedPointer<CActiveQuest> >::const_iterator it = ActiveQuests.begin(); it != ActiveQuests.end(); ++it) {
        CActiveQuest* quest = it->Data;
        if (quest->QuestName == name)
            return quest;
    }
    return 0;
}

bool CQuestManager::IsRegionValidInRegionLockingMode(const CRegion& region) const
{
    bool any_regions_specified = false;
    for (long i = 0; i < BetweenStartAndEndScreenQuests.size(); ++i) {
        CActiveQuest* pactive_quest = GetActiveQuestFromScriptName(BetweenStartAndEndScreenQuests[i]);
        if (pactive_quest) {
            if (pactive_quest->ContainsRegion(region))
                return true;
            if (pactive_quest->ContainsAnyRegions())
                any_regions_specified = true;
        }
    }
    if (!any_regions_specified)
        return true;
    for (long i = 0; i < BetweenStartAndEndScreenQuests.size(); ++i) {
        CActiveQuest* pactive_quest = GetActiveQuestFromScriptName(BetweenStartAndEndScreenQuests[i]);
        if (pactive_quest) {
            const std::vector<const CRegion*>& regions = pactive_quest->GetRegions();
        }
    }
    return false;
}
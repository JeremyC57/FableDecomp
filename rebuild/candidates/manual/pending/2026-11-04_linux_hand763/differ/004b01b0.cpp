// CQuestManager::IsRegionValidInRegionLockingMode(const CRegion& region) const -- true when a quest between its start and end screens claims the region, or when none of those quests claims any region; false otherwise
// Grade: DIFFER(281v277) -- dl=12: loop-1 begin loaded into the iterator register before the size() guard (retail after); loop-2 eax/ecx v ecx/edx and lea v shl for malloc(n*4); the if+do/while start fix pushes the GetRegions copy ctor out of the inline budget
// Leaf: ?IsRegionValidInRegionLockingMode@CQuestManager@@QBE_NABVCRegion@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B01B0, 275 bytes (`ret 4`). The catalog label
// "CScriptBase::ProcessQuestRegions" is wrong: the only caller (0x004A3817) runs it on the quest manager global 0x013B89FC
// with the world map's current region, and Ego_r.pdb / FableWin.pdb name ?IsRegionValidInRegionLockingMode@CQuestManager@@QBE_NABVCRegion@@@Z.
// Callees: CQuestManager::GetActiveQuestFromScriptName 0x004AF5B0 (landed), STLport std::__find<const CRegion* const*,
//   const CRegion*> 0x004B74E0 (`__fastcall`; the inlined CActiveQuest::ContainsRegion), malloc / memmove / free (MSVCR71
//   import thunks 0x00BFEA0E / 0x00BFEAE6 / 0x00BFEA14: the inlined by-value CActiveQuest::GetRegions copy).
// Headers: engine/CQuestManager.h retail branch + FABLE_RETAIL_CQUESTMANAGER_REGIONLOCK (nested CActiveQuest, Regions +0x10,
//   BetweenStartAndEndScreenQuests +0x4c), STLport vector.
// The __find body (STLport 4.6 _algobase.c, the landed 0x004AFD80 TU form) is compiled here, noinline, so VC7.1 knows it
// writes no memory (retail keeps Regions.end() in ebp across the call).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_REGIONLOCK
#include "stlport/fable_containers.h"
#include "engine/CQuestManager.h"

namespace std {
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
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
#pragma auto_inline(on)
}

bool CQuestManager::IsRegionValidInRegionLockingMode(const CRegion& region) const
{
    bool questHasRegions = false;
    const CCharString* name = BetweenStartAndEndScreenQuests.begin();
    for (unsigned long i = 0; i < BetweenStartAndEndScreenQuests.size(); ++i, ++name) {
        CActiveQuest* quest = GetActiveQuestFromScriptName(*name);
        if (quest) {
            if (quest->ContainsRegion(region))
                return true;
            if (quest->ContainsAnyRegions())
                questHasRegions = true;
        }
    }
    if (!questHasRegions)
        return true;
    for (unsigned long j = 0; j < BetweenStartAndEndScreenQuests.size(); ++j) {
        CActiveQuest* quest = GetActiveQuestFromScriptName(BetweenStartAndEndScreenQuests[j]);
        if (quest)
            quest->GetRegions();
    }
    return false;
}

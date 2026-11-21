// CQuestManager::ActivateQuest(const CCharString& quest_name, bool flag, bool load_resources) -- activate one quest by wrapping its name in a one-element array and handing it to ActivateMultipleQuests
// Leaf: ?ActivateQuest@CQuestManager@@QAE_NABVCCharString@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B4A10, 137 bytes (144 with padding), `ret 0Ch` (__fastcall).
// Identity: Ego_r.pdb `?ActivateQuest@CQuestManager@@QAE_NABVCCharString@@_N1@Z`; the landed callers (for example
//   CCreatureAction_ActivateQuest::Initialise 0x00843FC0) declare it at 0x004B4A10.
// Callees: vector<CCharString>::_M_insert_overflow 0x00433530 (the push_back growth path; a same-TU stand-in body
//   precedes the function, see Shape), CQuestManager::ActivateMultipleQuests(const CArray<CCharString>&, bool, bool)
//   0x004B4260 (same TU in retail; a same-TU stand-in body precedes the function, see Shape), ~CCharString 0x0099EAE0 (the inlined vector destructor), free 0x00BFEA14.
// Shape: `CArray<CCharString> quests; quests.push_back(quest_name); return ActivateMultipleQuests(quests, flag,
//   load_resources);`. VC7.1 folds the empty vector's `_M_finish != _M_end_of_storage` test, so only the
//   `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` call remains (position 0 pushed as an immediate) and the
//   tag temporary is homed on the dead quest-name argument slot -- which needs the growth member's body visible and
//   tag-free (GOTCHAS retry 10f). The bool result is parked in that slot across the inlined STLport ~vector
//   (`~vector() { _Destroy(_M_start, _M_finish); }` by value, then ~_Vector_base's `if (_M_start != 0) deallocate`):
//   retail keeps start/finish in edi/ebp across the element destructors only because the array's address provably
//   does not escape, which VC7.1 sees once ActivateMultipleQuests has a same-TU body (declared-only: start reloaded).
// Label correction: catalogued as `CQuestManager_ActivateQuest` (right).
// Name fix (NF212, 2026-11-21, q6332 / q6341): the header-less CCharStringVector / _CCharStringVectorBase / allocator /
//   _STLP_alloc_proxy / _Destroy / __false_type stand-ins, the local CArray<CCharString> specialization and the local CCharString
//   move onto the shared headers (fable_string.h, stlport/fable_containers.h, engine/CArray.h). The same-TU growth body is now
//   STLport's own (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE, visible and out of line, with the owner row 0x00433530's
//   FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE so its pick-any copy is the owner's), so push_back's growth call binds the
//   protected IAE vector<CCharString>::_M_insert_overflow (was the stand-in ?_M_insert_overflow@CCharStringVector@@...).
//   The local CQuestManager declaration and the ActivateMultipleQuests stand-in body are HEAD debt (engine/CQuestManager.h port
//   queued); the stand-in now reads the names through size() / operator[].
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"

class CQuestManager {
public:
    bool IsQuestRegistered(const CCharString& quest_name) const;   // 0x004B00C0 ?IsQuestRegistered@CQuestManager@@QBE_NABVCCharString@@@Z (Ego_r.pdb; landed row;
                                                                 //   NF28: was GetActiveQuestInfo(const CCharString*)), the per-name step
    bool ActivateMultipleQuests(const CArray<CCharString>& quest_names, bool flag, bool load_resources);   // 0x004B4260
    bool ActivateQuest(const CCharString& quest_name, bool flag, bool load_resources);
};

#pragma auto_inline(off)
// Same-TU stand-in for CQuestManager::ActivateMultipleQuests 0x004B4260 (compiled in this TU in retail, not inlined):
// it walks the names by index and never keeps the array's address, so VC knows the local array does not escape and
// keeps its start/finish words in registers across the element destructors, as retail does.
bool CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& quest_names, bool flag, bool load_resources)
{
    bool activated = false;
    for (unsigned int i = 0; i < quest_names.size(); ++i)
        if (IsQuestRegistered(quest_names[i]) && (flag || load_resources))
            activated = true;
    return activated;
}
#pragma auto_inline(on)

bool CQuestManager::ActivateQuest(const CCharString& quest_name, bool flag, bool load_resources)
{
    CArray<CCharString> quests;
    quests.push_back(quest_name);
    return ActivateMultipleQuests(quests, flag, load_resources);
}
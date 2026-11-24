// CQuestManager::RemoveBoast(long id, CCharString scriptName) -- remove the boast with this id and script name from the boast list
// Leaf: ?RemoveBoast@CQuestManager@@QAEXJVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AFE20, 345 bytes, `ret 8` (by-value CCharString destroyed at the end).
// Callees: CCharString::operator!=(const char*) 0x0099E960 / (const CCharString&) 0x0099E900 (first pass), CBasicString<char>::
//   operator==(const char*) 0x004115A0 and Compare 0x00411570 (both given their landed-equivalent bodies as same-TU
//   stand-ins), ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, free 0x00BFEA14, CWorld::GetFrame 0x0049D870.
// Layout: CQuestManager FrameBoastsLastUpdated +0x64, Boasts +0x68 (retail 0x3c below Ego_r.pdb); CBoast from the PDB.
// Shape: the group pass uses the inline CCharString == ("" and CCharString). The same-TU 0x004115A0 body tells VC7.1 the
//   call writes no memory, so scriptName's block stays in ebp across it (an extern declaration reloads it). The inline
//   ==("") null arm is `if (strcmp(text, "") == 0) return true; return false;`, which branches off the repe cmpsb flags.
// Name fix (NF215, 2026-11-21, q12262): ported onto fable_string.h: the local CBasicStringChar / CCharStringData / CCharString / CWideString
//   models and the same-TU Compare_00411570 / CBasicStringChar::operator== stand-ins are replaced by FABLE_RETAIL_CCHARSTRING with
//   FABLE_RETAIL_CBASICSTRING_COMPARE_BODY (Compare 0x00411570) + FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO (the visible, called
//   CBasicString<char>::operator==(const char*), retail 0x004115A0, whose pick-any copy is then the owner's 88-byte body); the call binds the
//   Ego_r.pdb 0x0040C910 / FableWin.pdb 0x01899D90 ??8?$CBasicString@D@@QBE_NPBD@Z (was ??8CBasicStringChar@@QBE_NPBD@Z).
#include <string.h>
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CWorld { public: static long __fastcall GetFrame(); };               // 0x0049D870
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
#include "fable_string.h"
struct CBoast {                                    // CQuestManager::CBoast (PDB, 32 bytes)
    long        ID;                                // +0x00
    long        Cost;                              // +0x04
    long        Reward;                            // +0x08
    CCharString ScriptName;                        // +0x0c
    CWideString Text;                              // +0x10
    CWideString Title;                             // +0x14
    bool        Evil;                              // +0x18
    bool        Failed;                            // +0x19
    bool        Completed;                         // +0x1a
    long        GroupIndex;                        // +0x1c
};
struct CBoastNode {                                // STLport list node
    CBoastNode* next;
    CBoastNode* prev;
    CBoast      value;                             // +0x08
};
inline CBoastNode* EraseBoast(CBoastNode* node)    // STLport list::erase(iterator)
{
    CBoastNode* next = node->next;
    CBoastNode* prev = node->prev;
    prev->next = next;
    next->prev = prev;
    node->value.~CBoast();
    free(node);
    return next;
}
class CQuestManager { public:
    unsigned char _pad_0x00[0x64];
    unsigned long FrameBoastsLastUpdated;          // +0x64 (PDB +0xa0)
    CBoastNode*   Boasts;                          // +0x68 list<CBoast> (head node; PDB +0xa4)
    CBoastNode*   AcceptedBoasts;                  // +0x6c
    void RemoveBoast(long id, CCharString scriptName);
};
void CQuestManager::RemoveBoast(long id, CCharString scriptName)
{
    for (CBoastNode* node = Boasts->next; node != Boasts; node = node->next) {
        if (node->value.ID != id)
            continue;
        if (scriptName != "" && scriptName != node->value.ScriptName)
            continue;
        long group = node->value.GroupIndex;
        EraseBoast(node);
        if (group) {
            for (CBoastNode* other = Boasts->next; other != Boasts; ) {
                if ((other->value.GroupIndex & group) && (scriptName == "" || scriptName == other->value.ScriptName))
                    other = EraseBoast(other);
                else
                    other = other->next;
            }
        }
        break;
    }
    FrameBoastsLastUpdated = CWorld::GetFrame();
}
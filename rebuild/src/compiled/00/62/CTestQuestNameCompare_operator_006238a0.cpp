// CTestQuestNameCompare::operator()(const CTestQuest* a, const CTestQuest* b) -- orders test quests by display name (falling back to the quest name when the display name is empty), case-insensitively
// Leaf: ??RCTestQuestNameCompare@@QBE_NPBVCTestQuest@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006238A0, 225 bytes. `ret 8`, bool in al.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30; CBasicString<char>::operator==(const char*) 0x004115A0
//          (on the data block, inside the inline CCharString::operator==; "" @ 0x0122D70E); CCharString::operator= 0x0099EFB0; _stricmp 0x00BFEBA8
//          (`__cdecl`, `add esp,8`); CCharString::~CCharString 0x0099EAE0. The empty-text global @ 0x0129AAF4.
// Shape: each name is a CCharString copy of DisplayName (+0x0c) in a frame word (the second in the dead `a` slot); the
//        natural `if (name == "") name = quest->QuestName;` with the inline CCharString ==("") whose null arm is
//        `if (strcmp(text, "") == 0) return true; return false;`. 0x004115A0 and Compare get their landed-equivalent
//        bodies as same-TU stand-ins, so VC knows the compare writes no memory. nameB's block then stays in esi across it,
//        and its C string is threaded on every path as retail. The C strings are the null-first `PStringData ? Data : ""`
//        reads (b first, then a); the compare result rides in bl across the two destructors.
// Name fix (NF215, 2026-11-21, q12262): ported onto fable_string.h: the local CBasicStringChar / CCharStringData / CCharString models and the
//   same-TU Compare_00411570 / CBasicStringChar::operator== stand-ins are replaced by FABLE_RETAIL_CCHARSTRING with the header bodies
//   FABLE_RETAIL_CBASICSTRING_COMPARE_BODY (Compare 0x00411570) + FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO (the visible, called
//   CBasicString<char>::operator==(const char*), retail 0x004115A0); the call binds the Ego_r.pdb 0x0040C910 / FableWin.pdb 0x01899D90
//   ??8?$CBasicString@D@@QBE_NPBD@Z (was ??8CBasicStringChar@@QBE_NPBD@Z); GetPString -> the header GetAsCharArray (NullChar 0x0129AAF4).
// Layout: Ego_r.pdb CTestQuest (0x1c): QuestName +0x00, HSPName +0x04, ColourIndex +0x08, DisplayName +0x0c.
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
#include "fable_string.h"
class CTestQuest { public:
    CCharString QuestName;                      // +0x00
    CCharString HSPName;                        // +0x04
    long        ColourIndex;                    // +0x08
    CCharString DisplayName;                    // +0x0c
};
class CTestQuestNameCompare { public:
    bool operator()(const CTestQuest* a, const CTestQuest* b) const;
};
bool CTestQuestNameCompare::operator()(const CTestQuest* a, const CTestQuest* b) const
{
    CCharString nameA(a->DisplayName);
    if (nameA == "")
        nameA = a->QuestName;
    CCharString nameB(b->DisplayName);
    if (nameB == "")
        nameB = b->QuestName;
    return _stricmp(nameA.GetAsCharArray(), nameB.GetAsCharArray()) < 0;
}
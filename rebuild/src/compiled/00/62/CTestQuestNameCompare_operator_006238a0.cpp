// CTestQuestNameCompare::operator()(const CTestQuest* a, const CTestQuest* b) -- orders test quests by display name (falling back to the quest name when the display name is empty), case-insensitively
// Leaf: operator()
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
// Layout: Ego_r.pdb CTestQuest (0x1c): QuestName +0x00, HSPName +0x04, ColourIndex +0x08, DisplayName +0x0c.
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body, same-TU stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
bool CBasicStringChar::operator==(const char* text) const   // 0x004115A0 (same-TU stand-in: reads only)
{
    return Compare_00411570(PData, text) ? false : true;
}
#pragma auto_inline(on)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
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
    return _stricmp(nameA.GetPString(), nameB.GetPString()) < 0;
}
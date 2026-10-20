// CTestQuestNameCompare::operator()(const CTestQuest* a, const CTestQuest* b) -- orders test quests by display name (falling back to the quest name when the display name is empty), case-insensitively
// Grade: DIFFER(ours v retail) 218v225 -- (1) the operator= `this` lea is scheduled before `push ebx` (retail after); (2) retail threads nameB's data pointer: the c_str of nameB is formed in esi right after its fixup (folded to "" on the null path, `mov esi,[esi]` on the loaded path), ours re-reads the slot into ecx at the call
// Leaf: operator()
// Retail: Fable.exe (TLC, VC7.1) @ 0x006238A0, 225 bytes. `ret 8`, bool in al.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30; CBasicString<char>::operator==(const char*) 0x004115A0
//          (on the data block, inside the inline CCharString::operator==; "" @ 0x0122D70E); CCharString::operator= 0x0099EFB0; _stricmp 0x00BFEBA8
//          (`__cdecl`, `add esp,8`); CCharString::~CCharString 0x0099EAE0. The empty-text global @ 0x0129AAF4.
// Shape: each name is a CCharString copy of DisplayName (+0x0c) in a frame word -- the second one homed in the dead `a`
//        argument slot; the inline `== ""` is written as two separate tests (a 1-byte `repe cmpsb` strcmp of the literal
//        against the null block's "", the out-of-line compare otherwise) and on equality the QuestName (+0x00) is assigned.
//        The C strings are the null-first `PStringData ? Data : ""` reads (b first, then a), the compare result rides in bl
//        across the two destructors.
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
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
};
struct CTestQuest {
    CCharString QuestName;                      // +0x00
    CCharString HSPName;                        // +0x04
    long        ColourIndex;                    // +0x08
    CCharString DisplayName;                    // +0x0c
};
struct CTestQuestNameCompare {
    bool operator()(const CTestQuest* a, const CTestQuest* b) const;
};
bool CTestQuestNameCompare::operator()(const CTestQuest* a, const CTestQuest* b) const
{
    CCharString nameA(a->DisplayName);
    if (nameA.PStringData == 0) {
        if (strcmp("", "") == 0)
            nameA = a->QuestName;
    } else if (nameA.PStringData->Data == "")
        nameA = a->QuestName;
    CCharString nameB(b->DisplayName);
    if (nameB.PStringData == 0) {
        if (strcmp("", "") == 0)
            nameB = b->QuestName;
    } else if (nameB.PStringData->Data == "")
        nameB = b->QuestName;
    const char* textB = nameB.GetPString();
    bool result = _stricmp(nameA.GetPString(), textB) < 0 ? true : false;
    return result;
}

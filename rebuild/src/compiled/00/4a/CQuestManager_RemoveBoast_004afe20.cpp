// CQuestManager::RemoveBoast(long id, CCharString scriptName) -- remove the boast with this id and script name from the boast list
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AFE20, 345 bytes, `ret 8` (by-value CCharString destroyed at the end).
// Callees: CCharString::operator!=(const char*) 0x0099E960 / (const CCharString&) 0x0099E900 (first pass), CBasicString<char>::
//   operator==(const char*) 0x004115A0 and Compare 0x00411570 (both given their landed-equivalent bodies as same-TU
//   stand-ins), ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, free 0x00BFEA14, GFGetCurrentFrame 0x0049D870.
// Layout: CQuestManager FrameBoastsLastUpdated +0x64, Boasts +0x68 (retail 0x3c below Ego_r.pdb); CBoast from the PDB.
// Shape: the group pass uses the inline CCharString == ("" and CCharString). The same-TU 0x004115A0 body tells VC7.1 the
//   call writes no memory, so scriptName's block stays in ebp across it (an extern declaration reloads it). The inline
//   ==("") null arm is `if (strcmp(text, "") == 0) return true; return false;`, which branches off the repe cmpsb flags.
#include <string.h>
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
long __fastcall GFGetCurrentFrame();               // 0x0049D870
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
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                          // CBasicString<char>, 13 bytes
    char*         PData;                           // +0x00
    unsigned long StringLength;                    // +0x04
    unsigned long DataLength : 31;                 // +0x08
    bool          UseFastExtend : 1;               // +0x0c
    bool operator==(const char* text) const;       // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;                         // +0x00
    long             NoRefs;                       // +0x0d
};
#pragma pack(pop)
#pragma auto_inline(off)
bool CBasicStringChar::operator==(const char* text) const   // 0x004115A0 (same-TU stand-in: reads only)
{
    return Compare_00411570(PData, text) ? false : true;
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                  // 0x0099EAE0
    bool operator!=(const char* text) const;         // 0x0099E960
    bool operator!=(const CCharString& other) const; // 0x0099E900
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        if (PStringData->Data.StringLength != other.PStringData->Data.StringLength)
            return false;
        return Compare_00411570(PStringData->Data.PData, other.PStringData->Data.PData) ? false : true;
    }
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                  // 0x0099B510
};
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
    FrameBoastsLastUpdated = GFGetCurrentFrame();
}
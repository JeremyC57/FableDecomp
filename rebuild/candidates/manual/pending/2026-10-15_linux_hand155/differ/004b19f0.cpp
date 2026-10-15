// CQuestManager::GetBoastsForScript(list<CBoast>& boasts, const CCharString& scriptName) const -- fill a list with copies of every offered boast that belongs to the script
// Grade: DIFFER(141v141) -- lane gap (callee-saved push delayed into a branch): retail pushes edi at entry, ours after the empty-list test, so every frame offset in the loop differs by 4; everything else matches. Tried: SameString(boast, name), CCharString::operator==, a prev-node local in the STLport insert (fixed the link stores), the boast name block in a named local (fixed the eax/ecx swap).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B19F0, length 141 bytes (`ret 8`).
// Label: the catalog name "CQuestManager::PopulateTreePane" is descriptive only; the body clears the output list
// and copies the matching boasts. The method name here is descriptive.
// Callees: list<CBoast>::clear @ 0x004B8C00 (catalog "CGuiControlTreePane::SortTreeRecursively", propagated),
// malloc @ 0x00BFEA0E (the node), CBoast::CBoast(const CBoast&) @ 0x004B73A0, Compare(const char*, const char*)
// @ 0x00411570 (`__fastcall`, the inlined CCharString equality).
// Layout: Boasts (list<CBoast>, head pointer) at +0x68 (PDB +0xa4), as in the landed RemoveAcceptedBoastsForScript
// 0x004B0030; node {next, prev, CBoast +8}, CBoast ScriptName at node +0x14; the node is 0x28 bytes.
// Shape: STLport push_back = insert(end(), x): end() read before the node is allocated, the previous node read
// once into a local (before the first link store), the placement copy
// null-checks the value address, then the four link stores; `this` lives in the `push ecx` frame word; the
// head is re-read for every end test and the name's string block every pass (Compare is only declared).
// Leaf: GetBoastsForScript
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);      // 0x00BFEA0E

struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

inline bool SameString(CCharStringData* a, CCharStringData* b)
{
    if (a == b)
        return true;
    if (a && b && a->m_length == b->m_length)
        return Compare_00411570(a->m_pData, b->m_pData) == 0;
    return false;
}

struct CCharString {
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const { return SameString(PStringData, other.PStringData); }
};
struct CWideString { void* PStringData; };

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
    CBoast(const CBoast& other);                   // 0x004B73A0
};

struct CBoastNode {                                // STLport list node
    CBoastNode* next;
    CBoastNode* prev;
    CBoast      value;                             // +0x08
};

struct CBoastList {                                // STLport list<CBoast>
    CBoastNode* m_node;                            // header
    void clear();                                  // 0x004B8C00
    CBoastNode* end() { return m_node; }
    void insert(CBoastNode* pos, const CBoast& x)
    {
        CBoastNode* tmp = static_cast<CBoastNode*>(malloc(sizeof(CBoastNode)));
        new (&tmp->value) CBoast(x);
        CBoastNode* prev = pos->prev;
        tmp->next = pos;
        tmp->prev = prev;
        prev->next = tmp;
        pos->prev = tmp;
    }
    void push_back(const CBoast& x) { insert(end(), x); }
};

struct CQuestManager {
    unsigned char _pad00[0x68];
    CBoastList    Boasts;                          // +0x68
    void GetBoastsForScript(CBoastList& boasts, const CCharString& scriptName);
};

void CQuestManager::GetBoastsForScript(CBoastList& boasts, const CCharString& scriptName)
{
    boasts.clear();
    for (CBoastNode* node = Boasts.m_node->next; node != Boasts.m_node; node = node->next) {
        CCharStringData* name = node->value.ScriptName.PStringData;
        if (SameString(name, scriptName.PStringData))
            boasts.push_back(node->value);
    }
}

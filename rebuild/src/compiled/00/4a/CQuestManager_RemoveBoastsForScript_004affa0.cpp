// CQuestManager::RemoveBoastsForScript(CCharString scriptName) -- erase every offered (not yet accepted) boast that belongs to this script
// Leaf: RemoveBoastsForScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFFA0, length 131 bytes (padded to 144), `ret 4`.
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString equality),
// CWideString::~CWideString @ 0x0099B510 (Title, Text), CCharString::~CCharString @ 0x0099EAE0 (the boast's ScriptName and
// the by-value parameter), free @ 0x00BFEA14 (the list node).
// Label correction: the catalog calls this RemoveAcceptedBoastsForScript too; it walks Boasts (+0x68), so it is
// FableWin.pdb's ?RemoveBoastsForScript@CQuestManager@@QAEXVCCharString@@@Z; the AcceptedBoasts twin is 0x004B0030.
// Layout: Boasts (list<CBoast>) at +0x68 (PDB +0xa4); node {next, prev, CBoast +8} (landed AreAllBoastsProcessed
// 0x004AF9C0 model); CBoast's ScriptName at node +0x14, Text +0x18, Title +0x1c.
// Shape: `it = (match) ? erase(it) : next` as an if/else; STLport's inline erase unlinks, runs the implicit ~CBoast
// (Title, Text, ScriptName) and frees the node; the head is re-read for every end test. The parameter's string block
// stays in edi across the Compare call and is re-read only after an erase: that needs Compare's real body (landed
// 0x00411570, 15 lines) defined above under `#pragma auto_inline(off)` so VC7.1 knows it writes no memory; with a bare
// declaration the block is re-read every pass.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct CCharStringData {                           // the shared string block
    const char*   m_pData;                         // +0x00
    unsigned long m_length;                        // +0x04
};
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body, global_global_00411570.cpp):
{                                                                 // defined above the caller so VC7.1 knows it writes no memory
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
    CCharString(const CCharString& other);           // (the by-value parameter is built by the caller)
    ~CCharString();                                  // 0x0099EAE0
};
struct CWideString {
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
inline CBoastNode* Erase(CBoastNode* pos)          // STLport list::erase(iterator)
{
    CBoastNode* next = pos->next;
    CBoastNode* prev = pos->prev;
    prev->next = next;
    next->prev = prev;
    pos->value.~CBoast();
    free(pos);
    return next;
}
struct CQuestManager {
    unsigned char _pad_0x00[0x68];
    CBoastNode*   Boasts;                          // +0x68 list<CBoast> (head node; PDB +0xa4)
    CBoastNode*   AcceptedBoasts;                  // +0x6c list<CBoast> (head node; PDB +0xb0)
    void RemoveBoastsForScript(CCharString scriptName);
};
void CQuestManager::RemoveBoastsForScript(CCharString scriptName)
{
    CBoastNode* node = Boasts->next;
    while (node != Boasts) {
        if (SameString(node->value.ScriptName.PStringData, scriptName.PStringData))
            node = Erase(node);
        else
            node = node->next;
    }
}
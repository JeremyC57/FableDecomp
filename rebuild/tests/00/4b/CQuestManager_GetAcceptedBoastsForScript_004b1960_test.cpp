// Behaviour test for 004b1960: src/004b1960.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CQuestManager::GetAcceptedBoastsForScript(const CCharString& scriptName, std::list<CBoast>& out) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQuestManager::GetAcceptedBoastsForScript(const CCharString& scriptName, std::list<CBoast>& out) const -- refill `out` with copies of every accepted boast of this script (catalog label TransferBoastToUI is wrong)
// Leaf: GetAcceptedBoastsForScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1960, length 141 bytes (padded to 144), `ret 8`.
// Callees: list<CBoast>::clear() @ 0x004B8C00 (catalog label "CGuiControlTreePane::SortTreeRecursively" is propagated:
// the body destroys Title/Text/ScriptName of every node, frees it and re-links the head), Compare @ 0x00411570 (the
// inlined CCharString equality), malloc @ 0x00BFEA0E (0x28-byte node), CBoast::CBoast(const CBoast&) @ 0x004B73A0.
// Layout: AcceptedBoasts (list<CBoast>) at +0x6c; node {next, prev, CBoast +8} (0x28 bytes).
// Shape: the comparison is the inline CCharString::operator== (node string on the left); the name's string block is re-read from the argument every pass (malloc and the copy ctor may write memory);
// push_back is STLport's inline insert(end(), x): end() read before the malloc, placement new null-tested, then the four
// link stores; `this` is spilled to the push-ecx frame word because esi carries the new node.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }
struct CCharStringData {                           // the shared string block
    const char*   m_pData;                         // +0x00
    unsigned long m_length;                        // +0x04
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
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare_00411570(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
struct CWideString {
    void* PStringData;
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
    CBoast(const CBoast& other);                   // 0x004B73A0
};
struct CBoastNode {                                // STLport list node (0x28 bytes)
    CBoastNode* next;
    CBoastNode* prev;
    CBoast      value;                             // +0x08
};
struct CBoastList {                                // STLport list<CBoast>
    struct iterator {
        CBoastNode* _M_node;
        iterator(CBoastNode* n) : _M_node(n) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    CBoastNode* _M_node;
    void clear();                                  // 0x004B8C00
    iterator end() const { return iterator(_M_node); }
    CBoastNode* _M_create_node(const CBoast& x)
    {
        CBoastNode* tmp = (CBoastNode*)malloc(sizeof(CBoastNode));
        new (&tmp->value) CBoast(x);
        return tmp;
    }
    void insert(iterator position, const CBoast& x)
    {
        CBoastNode* tmp = _M_create_node(x);
        CBoastNode* pos = position._M_node;
        CBoastNode* prev = pos->prev;
        tmp->next = pos;
        tmp->prev = prev;
        prev->next = tmp;
        pos->prev = tmp;
    }
    void push_back(const CBoast& x) { insert(end(), x); }
};
struct CQuestManager {
    unsigned char _pad_0x00[0x6c];
    CBoastNode*   AcceptedBoasts;                  // +0x6c list<CBoast> (head node; PDB +0xb0)
    void GetAcceptedBoastsForScript(const CCharString& scriptName, CBoastList& out) const;
};
void CQuestManager::GetAcceptedBoastsForScript(const CCharString& scriptName, CBoastList& out) const
{
    out.clear();
    for (CBoastNode* node = AcceptedBoasts->next; node != AcceptedBoasts; node = node->next) {
        if (node->value.ScriptName == scriptName)
            out.push_back(node->value);
    }
}

// ---- stubs and driver ----

long __fastcall Compare_00411570(const char* a, const char* b) { return strcmp(a, b); }
static int g_copies = 0;
CBoast::CBoast(const CBoast& other) : ID(other.ID), Cost(other.Cost), Reward(other.Reward), ScriptName(other.ScriptName), Text(other.Text), Title(other.Title),
    Evil(other.Evil), Failed(other.Failed), Completed(other.Completed), GroupIndex(other.GroupIndex) { ++g_copies; }
static int g_clears = 0;
void CBoastList::clear() { ++g_clears; _M_node->next = _M_node; _M_node->prev = _M_node; }
static unsigned char g_nodes[4][sizeof(CBoastNode)];
int main()
{
    CCharStringData a = { "QS_A", 4 }, a2 = { "QS_A", 4 }, b = { "QS_B", 4 }, c = { "QS_AB", 5 };
    CCharStringData* names[4] = { &a, &b, &a2, &c };
    static unsigned char headStore[sizeof(CBoastNode)]; CBoastNode* head = (CBoastNode*)headStore; head->next = head; head->prev = head;
    for (int i = 0; i < 4; ++i) {
        CBoastNode* n = (CBoastNode*)g_nodes[i]; memset(n, 0, sizeof *n); n->value.ID = i; n->value.ScriptName.PStringData = names[i];
        n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n;
    }
    static unsigned char qmStore[sizeof(CQuestManager)]; CQuestManager* qm = (CQuestManager*)qmStore; qm->AcceptedBoasts = head;
    static unsigned char outStore[sizeof(CBoastNode)]; CBoastNode* outHead = (CBoastNode*)outStore; outHead->next = outHead; outHead->prev = outHead;
    CBoastList out; out._M_node = outHead;
    CCharString wanted; wanted.PStringData = &a;
    qm->GetAcceptedBoastsForScript(wanted, out);
    CHECK(g_clears == 1); CHECK(g_copies == 2);
    CHECK(outHead->next->value.ID == 0); CHECK(outHead->next->next->value.ID == 2); CHECK(outHead->next->next->next == outHead); CHECK(outHead->prev->value.ID == 2);
    CCharString none; none.PStringData = 0;
    qm->GetAcceptedBoastsForScript(none, out);
    CHECK(g_clears == 2); CHECK(g_copies == 2); CHECK(outHead->next == outHead);
    if (fails == 0) printf("004b1960_TEST PASS\n"); else printf("004b1960_TEST FAIL\n");
    return fails;
}
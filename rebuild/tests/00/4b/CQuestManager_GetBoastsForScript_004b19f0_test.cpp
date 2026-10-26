// Behaviour test for 004b19f0: src/004b19f0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CQuestManager::GetBoastsForScript(const CCharString& scriptName, list<CBoast>& boasts) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQuestManager::GetBoastsForScript(const CCharString& scriptName, list<CBoast>& boasts) const -- clear the out list, then fill it with copies of every offered boast that belongs to the script
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

class CCharStringData { public:
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

class CCharString { public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const { return SameString(PStringData, other.PStringData); }
};
class CWideString { public: void* PStringData; };

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
    struct iterator {
        CBoastNode* _M_node;
        iterator(CBoastNode* n) : _M_node(n) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    CBoastNode* m_node;                            // header
    void clear();                                  // 0x004B8C00
    iterator end() { return iterator(m_node); }
    CBoastNode* _M_create_node(const CBoast& x)
    {
        CBoastNode* tmp = static_cast<CBoastNode*>(malloc(sizeof(CBoastNode)));
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

class CQuestManager { public:
    unsigned char _pad00[0x68];
    CBoastList    Boasts;                          // +0x68
    void GetBoastsForScript(const CCharString& scriptName, CBoastList& boasts) const;
};

void CQuestManager::GetBoastsForScript(const CCharString& scriptName, CBoastList& boasts) const
{
    boasts.clear();
    for (CBoastNode* node = Boasts.m_node->next; node != Boasts.m_node; node = node->next) {
        CCharStringData* name = node->value.ScriptName.PStringData;
        if (SameString(name, scriptName.PStringData))
            boasts.push_back(node->value);
    }
}

// ---- stubs and driver ----

long __fastcall Compare_00411570(const char* a, const char* b) { return strcmp(a, b); }
static int g_copies = 0;
CBoast::CBoast(const CBoast& other) : ID(other.ID), Cost(other.Cost), Reward(other.Reward), ScriptName(other.ScriptName), Text(other.Text), Title(other.Title),
    Evil(other.Evil), Failed(other.Failed), Completed(other.Completed), GroupIndex(other.GroupIndex) { ++g_copies; }
static int g_clears = 0;
void CBoastList::clear() { ++g_clears; m_node->next = m_node; m_node->prev = m_node; }
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
    static unsigned char qmStore[sizeof(CQuestManager)]; CQuestManager* qm = (CQuestManager*)qmStore; qm->Boasts.m_node = head;
    static unsigned char outStore[sizeof(CBoastNode)]; CBoastNode* outHead = (CBoastNode*)outStore; outHead->next = outHead; outHead->prev = outHead;
    CBoastList out; out.m_node = outHead;
    CCharString wanted; wanted.PStringData = &a;
    qm->GetBoastsForScript(wanted, out);
    CHECK(g_clears == 1); CHECK(g_copies == 2);
    CHECK(outHead->next->value.ID == 0); CHECK(outHead->next->next->value.ID == 2); CHECK(outHead->next->next->next == outHead); CHECK(outHead->prev->value.ID == 2);
    CCharString none; none.PStringData = 0;
    qm->GetBoastsForScript(none, out);
    CHECK(g_clears == 2); CHECK(g_copies == 2); CHECK(outHead->next == outHead);
    if (fails == 0) printf("004b19f0_TEST PASS\n"); else printf("004b19f0_TEST FAIL\n");
    return fails;
}
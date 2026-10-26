// Behaviour test for 004b1670: src/004b1670.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CManager::AddQuestCardEntry(const CCharString& name, const CCharString& value, bool flag)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CManager::AddQuestCardEntry(const CCharString& name, const CCharString& value, bool flag) -- append {name, value,
// flag, -1} to the entry list at +0x58 unless an entry with that name is already there, and mark the list changed
// Leaf: AddQuestCardEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1670, length 176 bytes, `ret 0Ch`.
// Identity: the catalog files the row under CGameScriptInterface, but `this` holds the 16-byte-entry list at +0x58
// and the changed flag at +0x8F of the class of the adjacent landed 0x004B14F0 (same TU); the class name is kept as
// that row's descriptive CManager.
// Callees: STLport `__find` over list<entry> by name @ 0x004B91F0 (`__fastcall`, iterator by hidden pointer, the
// empty input_iterator_tag by reference), CCharString::CCharString() @ 0x0099E4B0 (x2), CCharString::operator= @
// 0x0099EFB0 (x2), the list insert(iterator, const T&) @ 0x004B99E0, ~CCharString @ 0x0099EAE0 (x2).
// Shape: the landed 0x004B14F0 entry/list model; find(begin(), end(), name) through the out-of-line STLport __find with
// both iterators built in their argument slots (user copy ctor); push_back = insert(end(), x).

class CCharString { public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SEntry {
    CCharString   m_name;
    CCharString   m_value;
    bool          m_flag;
    long          m_uid;
};
struct SEntryNode {
    SEntryNode* _M_next;
    SEntryNode* _M_prev;
    SEntry      _M_data;
};
struct SEntryIterator {
    SEntryNode* _M_node;
    SEntryIterator(SEntryNode* n) : _M_node(n) { }
    SEntryIterator(const SEntryIterator& o) : _M_node(o._M_node) { }
};
struct input_iterator_tag { input_iterator_tag() {} };
SEntryIterator __fastcall __find(SEntryIterator first, SEntryIterator last, const input_iterator_tag& tag, const CCharString& name);   // 0x004B91F0
bool __fastcall SameString_s7(const void* a, const void* b);   // stand-in: CCharString equality on the string data
#pragma auto_inline(off)
// Same-TU body of STLport's input-iterator __find instantiation 0x004B91F0: walks the list comparing each entry name with
// the one read through its const reference; the empty tag is never touched.
__declspec(noinline) SEntryIterator __fastcall __find(SEntryIterator first, SEntryIterator last, const input_iterator_tag& tag, const CCharString& name)
{
    for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next)
        if (SameString_s7(first._M_node->_M_data.m_name.m_pStringData, name.m_pStringData))
            return first;
    return last;
}
#pragma auto_inline(on)

struct CEntryList {
    SEntryNode* _M_node;
    SEntryIterator begin() { return SEntryIterator(_M_node->_M_next); }
    SEntryIterator end() { return SEntryIterator(_M_node); }
    SEntryIterator insert(SEntryIterator pos, const SEntry& x);   // 0x004B99E0
    void push_back(const SEntry& x) { insert(SEntryIterator(_M_node), x); }
    __forceinline bool Contains(const CCharString& name)
    {
        SEntryNode* end = _M_node;
        SEntryNode* first = end->_M_next;
        SEntryIterator it = __find(SEntryIterator(first), SEntryIterator(end), input_iterator_tag(), name);
        return it._M_node != end;
    }
};
void* NewListNode_s7(unsigned long size);   // stand-in: the list node allocation (the node's strings start empty)
#pragma auto_inline(off)
// Same-TU body of list::insert 0x004B99E0: a new node holding a copy of the entry read through its const reference,
// linked in before the position.
__declspec(noinline) SEntryIterator CEntryList::insert(SEntryIterator pos, const SEntry& x)
{
    SEntryNode* node = (SEntryNode*)NewListNode_s7(sizeof(SEntryNode));
    node->_M_data.m_name = x.m_name;
    node->_M_data.m_value = x.m_value;
    node->_M_data.m_flag = x.m_flag;
    node->_M_data.m_uid = x.m_uid;
    node->_M_next = pos._M_node;
    node->_M_prev = pos._M_node->_M_prev;
    pos._M_node->_M_prev->_M_next = node;
    pos._M_node->_M_prev = node;
    return SEntryIterator(node);
}
#pragma auto_inline(on)
struct CManager {
    unsigned char _pad00[0x58];
    CEntryList    m_entries;                       // +0x58
    unsigned char _pad5c[0x8f - 0x5c];
    bool          m_entriesChanged;                // +0x8f
    void AddQuestCardEntry(const CCharString& name, const CCharString& value, bool flag);
};

void CManager::AddQuestCardEntry(const CCharString& name, const CCharString& value, bool flag)
{
    if (!m_entries.Contains(name)) {
        SEntry entry;
        entry.m_name = name;
        entry.m_value = value;
        entry.m_flag = flag;
        entry.m_uid = -1;
        m_entries.push_back(entry);
        m_entriesChanged = true;
    }
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_ctor = 0, g_dtor = 0, g_assign = 0, g_alloc = 0;
CCharString::CCharString() { ++g_ctor; m_pStringData = 0; }
CCharString::~CCharString() { ++g_dtor; }
CCharString& CCharString::operator=(const CCharString& o) { ++g_assign; m_pStringData = o.m_pStringData; return *this; }
bool __fastcall SameString_s7(const void* a, const void* b) { return a == b; }
void* NewListNode_s7(unsigned long size) { ++g_alloc; void* p = malloc(size); memset(p, 0, size); return p; }
int main()
{
    static SEntryNode head; head._M_next = &head; head._M_prev = &head;
    static unsigned char raw[sizeof(CManager)]; CManager& m = *(CManager*)raw; m.m_entries._M_node = &head; m.m_entriesChanged = false;
    CCharString n1, n2, v1; n1.m_pStringData = (void*)0x10; n2.m_pStringData = (void*)0x20; v1.m_pStringData = (void*)0x99;
    m.AddQuestCardEntry(n1, v1, true);
    CHECK(g_alloc == 1 && head._M_next != &head && head._M_next->_M_data.m_name.m_pStringData == (void*)0x10 && head._M_next->_M_data.m_value.m_pStringData == (void*)0x99);
    CHECK(head._M_next->_M_data.m_flag == true && head._M_next->_M_data.m_uid == -1 && m.m_entriesChanged);
    m.m_entriesChanged = false; m.AddQuestCardEntry(n1, v1, false); CHECK(g_alloc == 1 && !m.m_entriesChanged);   // already listed
    m.AddQuestCardEntry(n2, v1, false); CHECK(g_alloc == 2 && head._M_prev->_M_data.m_name.m_pStringData == (void*)0x20 && head._M_prev->_M_data.m_flag == false && m.m_entriesChanged);
    CHECK(head._M_next->_M_next == head._M_prev && head._M_prev->_M_next == &head);
    if (fails == 0) printf("004b1670_TEST PASS\n"); else printf("004b1670_TEST FAIL\n");
    return fails;
}
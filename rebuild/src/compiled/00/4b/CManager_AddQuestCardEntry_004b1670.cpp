// CManager::AddQuestCardEntry(const CCharString& name, const CCharString& value, bool flag) -- append {name, value,
// flag, -1} to the entry list at +0x58 unless an entry with that name is already there, and mark the list changed
// Leaf: AddQuestCardEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1670, length 176 bytes, `ret 0Ch`.
// Identity: the catalog files the row under CGameScriptInterface, but `this` holds the 16-byte-entry list at +0x58
// and the changed flag at +0x8F of the class of the adjacent landed 0x004B14F0 (same TU); the class name is kept as
// that row's descriptive CManager.
// Callees: STLport `__find_if(first, last, pred, const input_iterator_tag&)` over list<entry> @ 0x004B91F0 (`__fastcall`,
// iterator by hidden pointer, the 4-byte name predicate by value on the stack, the empty tag by reference in edx;
// landed as std::__find_if), CCharString::CCharString() @ 0x0099E4B0 (x2), CCharString::operator= @
// 0x0099EFB0 (x2), the list insert(iterator, const T&) @ 0x004B99E0, ~CCharString @ 0x0099EAE0 (x2).
// Shape: the landed 0x004B14F0 entry/list model; find_if(begin(), end(), name-is) through the out-of-line STLport
// __find_if with both iterators built in their argument slots (user copy ctor); push_back = insert(end(), x).
// Fixed (rename1, 2026-10-24): the callee was declared as `__find(first, last, const input_iterator_tag&, const
//   CCharString&)`; 0x004B91F0 is the landed std::__find_if whose third argument is a predicate class (its other caller,
//   STLport find_if 0x004B9E70, passes its own predicate word on the stack with edx unused). Bytes unchanged.

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
bool __fastcall SameString_s7(const void* a, const void* b);   // stand-in: CCharString equality on the string data
// "is this entry's name the wanted one?" -- the 4-byte predicate (a pointer to the wanted name, passed by value)
struct CEntryNameIs {
    const CCharString* m_pWanted;                   // +0x00
    CEntryNameIs(const CCharString& wanted) : m_pWanted(&wanted) { }
    bool operator()(const SEntry& e) const { return SameString_s7(e.m_name.m_pStringData, m_pWanted->m_pStringData); }
};
namespace std {
#pragma auto_inline(off)
// Same-TU body of STLport's input-iterator __find_if instantiation 0x004B91F0: walks the list asking the predicate
// about each entry; the empty tag (by reference, edx) is never touched.
template <class InputIter, class Predicate>
__declspec(noinline) InputIter __fastcall __find_if(InputIter first, InputIter last, Predicate pred, const input_iterator_tag&)
{
    for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next)
        if (pred(first._M_node->_M_data))
            return first;
    return last;
}
#pragma auto_inline(on)
}

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
        SEntryIterator it = std::__find_if(SEntryIterator(first), SEntryIterator(end), CEntryNameIs(name), input_iterator_tag());
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
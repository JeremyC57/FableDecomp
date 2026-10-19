// CManager::AddQuestCardEntry(const CCharString& name, const CCharString& value, bool flag) -- append {name, value,
// Grade: DIFFER(ours v retail) -- 168v168: the __find result iterator gets its own frame word (sub esp,14h) where retail homes it in the dead name argument slot; everything else is the same code shifted by 4
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

struct CCharString {
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

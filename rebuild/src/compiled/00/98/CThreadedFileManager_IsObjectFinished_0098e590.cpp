// CThreadedFileManager::IsObjectFinished(unsigned long) const -- under the manager lock, true unless the object is known and its load has not reached status 4
// Leaf: ?IsObjectFinished@CThreadedFileManager@@QBE_NK@Z
// Retail: Fable.exe @ 0x0098E590, 95 bytes, thiscall `ret 4`, bool in al (kept in bl across LeaveCriticalSection).
// Identity (proven): FableWin.pdb ?IsObjectFinished@CThreadedFileManager@@QBE_NK@Z and ?ObjectExists@...QBE_NK@Z are the
// manager's two const (unsigned long) -> bool queries; the sibling 0x0098E5F0 (lock, Objects.find, unlock, `it != end`) is
// ObjectExists, and this one additionally tests the found CFileObject's LoadingStatus. Both lock Ego_r.pdb's CriticalSection
// and search the Objects map in the order Objects, Initialised, ExitThread, ThreadFinished, CriticalSection (retail +0x13C ..
// +0x14C: STLport map 0xC + three bools + pad). Its only reference is the static forwarder 0x0098E630
// (`mov ecx, 0x013BC870 ; jmp`, the manager singleton). Catalog "_gapscan::sub_0098e590".
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10), the map's const find @ 0x00990320
// (STLport _Rb_tree::find, iterator through the hidden pointer; declared only).
// Layout: local model (no engine header for CThreadedFileManager); CFileObject (Ego_r.pdb) LoadingStatus at +0x1C; the map
// node's value at +0x10, so the CFileObject* is at +0x14. Status 4 is the finished state (EFileLoadingStatus enumerators
// are not in the PDB dump).
// Shape: a `finished` local (bl) set per branch; VC7.1 duplicates the unlock into both arms. STLport iterators have user
// constructors, so find returns through the hidden pointer (the frame slot).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(struct _RTL_CRITICAL_SECTION* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(struct _RTL_CRITICAL_SECTION* section);

struct _RTL_CRITICAL_SECTION {
    void* DebugInfo;
    long LockCount;
    long RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};

enum EFileLoadingStatus { };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> class allocator { };
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Tp> struct _Const_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &**this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    _Rb_tree_node<_Value>* _M_header;                       // +0x00 (alloc proxy)
    unsigned int _M_node_count;                             // +0x04
    _Compare _M_key_compare;                                // +0x08
    const_iterator find(const _Key& __x) const;             // 0x00990320
    const_iterator end() const { const_iterator __it; __it._M_node = _M_header; return __it; }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    const_iterator find(const _Key& __x) const { return _M_t.find(__x); }
    const_iterator end() const { return _M_t.end(); }
};
}

class CThreadedFileManager {
public:
    class CFileObject {
    public:
        unsigned char Desc[0x18];                           // +0x00 CThreadedFileObjectDesc
        unsigned long UID;                                  // +0x18
        EFileLoadingStatus LoadingStatus;                   // +0x1C
    };

    bool IsObjectFinished(unsigned long uid) const;

    unsigned char _pad000[0x13c];
    std::map<unsigned long, CFileObject*> Objects;          // +0x13C
    bool Initialised;                                       // +0x148
    bool ExitThread;                                        // +0x149
    bool ThreadFinished;                                    // +0x14A
    mutable _RTL_CRITICAL_SECTION CriticalSection;          // +0x14C
};

bool CThreadedFileManager::IsObjectFinished(unsigned long uid) const
{
    EnterCriticalSection(&CriticalSection);
    std::map<unsigned long, CFileObject*>::const_iterator it = Objects.find(uid);
    bool finished;
    if (it != Objects.end() && it->second->LoadingStatus != 4)
        finished = false;
    else
        finished = true;
    LeaveCriticalSection(&CriticalSection);
    return finished;
}
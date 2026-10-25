// _STL::list::erase(iterator position) -- STLport list<CQuestManager::CBoast>: unlink one node, destroy the boast (title, text, script name), free the node, return the next position
// Leaf: ?erase@?$list@VCBoast@CQuestManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B87F0, 60 bytes, __fastcall `ret 8` (hidden iterator result, position); `this` unused.
// Identity: STLport 4.6 list::erase(iterator) out of line; the node payload at +8 is Ego_r.pdb CQuestManager::CBoast
//   (32 bytes): ID, Cost, Reward, ScriptName +0xC (CCharString), Text +0x10 and Title +0x14 (CWideString), Evil, Failed,
//   Completed, GroupIndex -- node +0x14 / +0x18 / +0x1C as retail destroys them.
// Callees: CWideString::~CWideString @ 0x0099B510 (Title, Text), CCharString::~CCharString @ 0x0099EAE0 (ScriptName),
//   free @ 0x00BFEA14 (the node).
// Shape: CBoast declares no destructor (its implicit one runs Title, Text, ScriptName); the iterator (user ctors) comes back
//   through the hidden pointer.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class CCharString {
public:
    void* PData;
    ~CCharString();                                     // 0x0099EAE0
};

class CWideString {
public:
    void* PData;
    ~CWideString();                                     // 0x0099B510
};

class CQuestManager {
public:
    class CBoast {                                      // 32 bytes (Ego_r.pdb)
    public:
        long        ID;                                 // +0x00
        long        Cost;                               // +0x04
        long        Reward;                             // +0x08
        CCharString ScriptName;                         // +0x0C
        CWideString Text;                               // +0x10
        CWideString Title;                              // +0x14
        bool        Evil;                               // +0x18
        bool        Failed;                             // +0x19
        bool        Completed;                          // +0x1A
        long        GroupIndex;                         // +0x1C
    };
};

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x0
    _List_node_base* _M_prev;                           // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) {}
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    void _M_deallocate(_Node* __p) { if (__p != 0) free(__p); }

    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_deallocate(__n);
        return iterator(__next_node);
    }
};

template list<CQuestManager::CBoast>::iterator list<CQuestManager::CBoast>::erase(iterator);

} // namespace _STL
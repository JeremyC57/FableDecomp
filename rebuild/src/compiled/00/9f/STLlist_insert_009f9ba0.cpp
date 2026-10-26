// std::list::insert(iterator, const CProfileTreeEntry&) -- STLport list insert for CProfileManager::ProfileTree: malloc a 0x68-byte node, copy-construct the tree entry into it (CProfile copy, ID, Children vector copy, Parent) and link it before the position
// Leaf: ?insert@?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9BA0, 96 bytes (+ int3 padding), thiscall, `ret 0Ch` (hidden iterator
//   result, position, value).
// Identity: called by list<CProfileTreeEntry>::push_back @ 0x009F9C40 (the ProfileTree of Ego_r.pdb CProfileManager, +0x78).
//   The chunk label "ProfileEntry_Insert" is descriptive.
// Layout: Ego_r.pdb CProfileManager::CProfileTreeEntry (retail 0x60 bytes: the PDB's 16-byte VC8 vector is STLport's 12):
//   Profile CProfile +0x00 (0x48), ID +0x48, Children vector<CProfileTreeEntry*> +0x4C, Parent +0x58.
// Callees: 0x00BFEA0E malloc, CProfile::CProfile(const CProfile&) @ 0x009F9450 (landed), vector<CProfileTreeEntry*> copy
//   constructor @ 0x009F9930.
// Shape: STLport 4.x `insert(position, x)` with `_M_create_node` inlined; `_Construct` keeps the placement-new null test around
//   the implicit CProfileTreeEntry copy constructor (members in declaration order).
extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }

namespace std {

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    vector(const vector<_Tp, _Alloc>& __x);                         // 0x009F9930
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __tmp = _M_node.allocate(1);
        _Construct(&__tmp->_M_data, __x);
        return __tmp;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
};

}

class CProfileManager;
class CWideStringData;
class CWideString { public:
    CWideStringData* m_pStringData;
};

class CProfile {                                    // 0x48 bytes
public:
    CProfile(const CProfile& other);                // 0x009F9450
    CProfileManager* Manager;                       // +0x00
    double StartTime;                               // +0x08
    double TotalTime;                               // +0x10
    double LastTime;                                // +0x18
    bool Running;                                   // +0x20
    double Percentage;                              // +0x28
    CWideString Name;                               // +0x30
    long TreeDepth;                                 // +0x34
    bool NewNode;                                   // +0x38
    long Visible;                                   // +0x3C
    long CallCounter;                               // +0x40
    long UpdatedCallCounter;                        // +0x44
};

class CProfileManager {
public:
    struct CProfileTreeEntry {                      // 0x60 bytes
        CProfile Profile;                           // +0x00
        long ID;                                    // +0x48
        std::vector<CProfileTreeEntry*> Children;  // +0x4C
        CProfileTreeEntry* Parent;                  // +0x58
    };
};

template class std::list<CProfileManager::CProfileTreeEntry>;
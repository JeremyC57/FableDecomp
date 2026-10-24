// _STL::list::insert(iterator) -- STLport single-argument insert for CProfileManager::ProfileTree: insert a default-constructed CProfileTreeEntry before the position through insert(position, x) and destroy the temporary
// Leaf: ?insert@?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9C40, 98 bytes, thiscall, `ret 8` (hidden iterator result, position).
// Identity: the 0x60-byte local is a CProfileManager::CProfileTreeEntry (Ego_r.pdb; retail 0x60 with STLport's 12-byte
//   vector): CProfile::Manager = 0 at +0, CWideString Name default-constructed at +0x30 (0x0099AED0), Children zeroed at
//   +0x4C; ID and Parent are left alone. The chunk label "ProfileList_AddEntry" is descriptive.
// Callees: CWideString::CWideString() @ 0x0099AED0, list<CProfileTreeEntry>::insert(iterator, const CProfileTreeEntry&) @
//   0x009F9BA0 (hidden result forwarded from ebx), 0x00BFEA14 free (the Children buffer), CWideString::~CWideString() @ 0x0099B510.
// Shape: STLport 4.x `iterator insert(iterator __position) { return insert(__position, _Tp()); }`; no EH frame in this TU.
//   The by-value position is built in place (`push ecx ; mov eax,esp ; mov [eax],ecx`) by the iterator's user copy constructor.
extern "C" void __cdecl free(void* block);                          // 0x00BFEA14

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) {}
    vector(const vector<_Tp, _Alloc>& __x);                         // 0x009F9930
    ~vector() {}
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
    _List_iterator(const _List_iterator<_Tp>& __x) : _List_iterator_base(__x._M_node) {}
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;

    iterator insert(iterator __position, const _Tp& __x);           // 0x009F9BA0
    iterator insert(iterator __position) { return insert(__position, _Tp()); }
};

}

class CProfileManager;

struct CWideStringData;
struct CWideString {
    CWideStringData* m_pStringData;
    CWideString();                                  // 0x0099AED0
    CWideString(const CWideString& other);          // 0x0099B720
    ~CWideString();                                 // 0x0099B510
};

class CProfile {                                    // 0x48 bytes
public:
    CProfile() : Manager(0) {}
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
        _STL::vector<CProfileTreeEntry*> Children;  // +0x4C
        CProfileTreeEntry* Parent;                  // +0x58
    };
};

template _STL::list<CProfileManager::CProfileTreeEntry>::iterator
    _STL::list<CProfileManager::CProfileTreeEntry>::insert(_STL::list<CProfileManager::CProfileTreeEntry>::iterator);
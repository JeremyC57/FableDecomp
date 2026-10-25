// CProfileManager::AddProfile(CProfile&) -- push the profile onto the front of the manager's ProfileList (STLport list<CProfile*>, 12-byte malloc'd node) and point the profile's Manager back at this manager
// Leaf: ?AddProfile@CProfileManager@@QAEXAAVCProfile@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F8FC0, 52 bytes, thiscall, `ret 4`. No direct callers in the image.
// Name: Ego_d.pdb / Ego_r.pdb ?AddProfile@CProfileManager@@QAEXAAVCProfile@@@Z; layouts from Ego_r.pdb: CProfileManager::ProfileList
//   list<CProfile*> at +0x4C, CProfile::Manager at +0x00. The chunk label "DoublyLinkedList_InsertNode" is descriptive.
// Callees: 0x00BFEA0E malloc (STLport's malloc allocator, `n != 0` folded for the constant 1).
// Shape: STLport `insert(begin(), __x)` -- the begin() node (`mov esi,[head]`) is read before the node is allocated, the
//   _Construct placement new keeps its null test (`test ecx,ecx`), then the four link stores and `Manager = this`.
extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }

namespace std {

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

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class list {
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
    iterator begin() { return iterator((_Node*)(_M_node._M_data->_M_next)); }
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
    void push_front(const _Tp& __x) { insert(begin(), __x); }
};

}

class CProfileManager;

class CProfile {
public:
    CProfileManager* Manager;                       // +0x00
};

class CProfileManager {
public:
    unsigned char m_before[0x4C];                   // UpPressed .. NewProfilesRegistered
    std::list<CProfile*> ProfileList;              // +0x4C
    void AddProfile(CProfile& profile);
};

void CProfileManager::AddProfile(CProfile& profile)
{
    ProfileList.push_front(&profile);
    profile.Manager = this;
}
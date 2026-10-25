// CThreadedFileManager::CloseAllStreamsForFile(CThreadedFile* file) -- under the lock (pending processing blocked by the ref count), collects the UIDs of every object read from the file, then for each one waits while it is being read (unlock, kick the reader thread, SleepEx(0, TRUE), relock, look it up again), takes a still-pending object off its device queue (its callback's OnFileReadCompletion(false) runs outside the lock), frees the OVERLAPPED block, deletes the object and erases the map entry
// Grade: DIFFER(883v840) -- 4 grades; the UID loop now has retail's pointer + down-counter shape (named data pointer and count); still differs: retail keeps this in ebx (homed at [esp+14h]) and &CriticalSection in edi with a memory `inc [ebx+16Ch]`, and leaves the third Objects.find (after the callback) out of line (call 0x009902D0, hidden iterator result) where VC7.1 here inlines it
// Leaf: ?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098E7D0, length 840 bytes, thiscall, `ret 4`.
// Callees: EnterCriticalSection / LeaveCriticalSection / SetEvent / SleepEx / GlobalFree (IAT 0x0143FE0C / 0x0143FE10 /
//   0x0143FDE0 / 0x0143FDDC / 0x0143FDF8), vector<unsigned long>::_M_insert_overflow @ 0x00464ACE, the pending tree's
//   _M_find @ 0x0098FE70, _M_upper_bound @ 0x0098FCA0, _M_lower_bound @ 0x009901D0, erase(first, last) @ 0x009905D0, the
//   Objects tree's find @ 0x009902D0 (third lookup, not inlined), _Rb_global<bool>::_Rebalance_for_erase @ 0x0042A34E,
//   operator delete @ 0x00BFE9BC, free @ 0x00BFEA14.
// Layout (Ego_r.pdb, retail offsets): DeviceQueue CPendingObjectMap[2][7] +0x04 (16 bytes each: the 12-byte STLport map,
//   then NextObject), Objects map<unsigned long, CFileObject*> +0x13C, CriticalSection +0x14C, Event +0x164,
//   NextObjectUID +0x168, BlockPendingObjectProcessingRefCount +0x16C. CFileObject: Desc.PCallback +0x0C, Desc.Priority
//   +0x14, UID +0x18, LoadingStatus +0x1C, Overlapped +0x20, PFile +0x28; CThreadedFile::DeviceID +0x04.
// Label: catalog "CThreadedFileManager_ProcessPendingRequests" is descriptive; the PDB name set holds
//   ?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z, the only CThreadedFile* member.
#include <stddef.h>

extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);             // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);             // IAT 0x0143FE10
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                            // IAT 0x0143FDE0
extern "C" __declspec(dllimport) unsigned long __stdcall SleepEx(unsigned long ms, int alertable); // IAT 0x0143FDDC
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* mem);                          // IAT 0x0143FDF8
extern "C" void __cdecl free(void* p);                                                           // 0x00BFEA14
void operator delete(void* p);                                                                   // 0x00BFE9BC
inline void* operator new(size_t, void* p) { return p; }

struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };

namespace std {
struct __true_type { __true_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, size_t) { if (__p != 0) free(__p); }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef size_t size_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend);   // 0x00464ACE
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root,
                                                  _Base_ptr& __leftmost, _Base_ptr& __rightmost);   // 0x0042A34E
    static _Base_ptr __cdecl _M_increment(_Base_ptr __x);
};
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    _Self& operator++() { _M_increment(); return *this; }
    _Self operator++(int) { _Self __tmp = *this; _M_increment(); return __tmp; }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    size_t   _M_node_count;                                        // +0x04
    _Compare _M_key_compare;                                       // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }

    _Link_type _M_find(const key_type& __k) const;
    _Link_type _M_lower_bound(const key_type& __k) const;
    _Link_type _M_upper_bound(const key_type& __k) const;
    iterator find(const key_type& __x) { return iterator(_M_find(__x)); }
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }

    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                          this->_M_header._M_data->_M_parent,
                                                                          this->_M_header._M_data->_M_left,
                                                                          this->_M_header._M_data->_M_right);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
    void erase(iterator __first, iterator __last);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const key_type& __k) const
{
    _Link_type __y = _M_header._M_data;                 // last node which is not less than k
    _Link_type __x = _M_root();                         // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == _M_header._M_data || _M_key_compare(__k, _S_key(__y)))
        __y = _M_header._M_data;
    return __y;
}

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    iterator upper_bound(const _Key& __x) { return _M_t.upper_bound(__x); }
    void erase(iterator __position) { _M_t.erase(__position); }
    void erase(iterator __first, iterator __last) { _M_t.erase(__first, __last); }
};
}   // namespace std

class CThreadedFileCallback {
public:
    virtual ~CThreadedFileCallback();
    virtual bool OnFileReadBegin(void** dst, int priority);
    virtual void OnFileReadCompletion(bool success);            // vtable slot 2
};

class CThreadedFile {
public:
    void* m_vtable;                                              // +0x00
    long  DeviceID;                                              // +0x04 EThreadedFileDeviceID
};

class CThreadedFileObjectDesc {
public:
    void*                  PDst;                                 // +0x00
    unsigned long          FilePos;                              // +0x04
    unsigned long          Size;                                 // +0x08
    CThreadedFileCallback* PCallback;                            // +0x0C
    bool                   QueueWhenFinished;                    // +0x10
    long                   Priority;                             // +0x14 EThreadedFilePriority
};

class CThreadedFileManager {
public:
    class CFileObject {
    public:
        CThreadedFileObjectDesc Desc;                            // +0x00
        unsigned long  UID;                                      // +0x18
        long           LoadingStatus;                            // +0x1C EFileLoadingStatus
        void*          Overlapped;                               // +0x20 _OVERLAPPED*
        CThreadedFileManager* PManager;                          // +0x24
        CThreadedFile* PFile;                                    // +0x28
    };
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;
        CPendingObjectSortKey(CFileObject* object) : Object(object) { }
        bool operator<(const CPendingObjectSortKey& o) const;
    };
    class CPendingObjectMap {
    public:
        std::map<CPendingObjectSortKey, CFileObject*> Objects;   // +0x00
        std::map<CPendingObjectSortKey, CFileObject*>::iterator NextObject;   // +0x0C
    };

    void*             m_vtable;                                  // +0x000
    CPendingObjectMap DeviceQueue[2][7];                         // +0x004
    long              CurrentDeviceAtPriorityLevel[7];           // +0x0E4
    double            CurrentDeviceEndOfTimeSlice[7];            // +0x100
    CFileObject*      CurrentlyReadingObject;                    // +0x138
    std::map<unsigned long, CFileObject*> Objects;               // +0x13C
    bool              Initialised;                               // +0x148
    bool              ExitThread;                                // +0x149
    bool              ThreadFinished;                            // +0x14A
    _RTL_CRITICAL_SECTION CriticalSection;                       // +0x14C
    void*             Event;                                     // +0x164
    unsigned long     NextObjectUID;                             // +0x168
    long              BlockPendingObjectProcessingRefCount;      // +0x16C

    void CloseAllStreamsForFile(CThreadedFile* file);
};

typedef std::map<unsigned long, CThreadedFileManager::CFileObject*> TObjectMap;
typedef std::map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> TPendingMap;
typedef std::map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>::_Rep_type TPendingTree;
namespace std {
template <> TPendingTree::_Link_type TPendingTree::_M_find(const CThreadedFileManager::CPendingObjectSortKey& __k) const;   // 0x0098FE70
}

void CThreadedFileManager::CloseAllStreamsForFile(CThreadedFile* file)
{
    EnterCriticalSection(&CriticalSection);
    ++BlockPendingObjectProcessingRefCount;

    std::vector<unsigned long> uids;
    for (TObjectMap::iterator it = Objects.begin(); it != Objects.end(); ++it)
        if ((*it).second->PFile == file)
            uids.push_back((*it).first);

    unsigned long* ids = uids.begin();
    unsigned int count = uids.size();
    for (unsigned int i = 0; i < count; ++i)
    {
        TObjectMap::iterator it = Objects.find(ids[i]);
        while (it != Objects.end() && ((*it).second->LoadingStatus == 1 || (*it).second->LoadingStatus == 2 ||
                                       (*it).second->LoadingStatus == 3))
        {
            LeaveCriticalSection(&CriticalSection);
            SetEvent(Event);
            SleepEx(0, 1);
            EnterCriticalSection(&CriticalSection);
            it = Objects.find(ids[i]);
        }
        if (it != Objects.end())
        {
            CFileObject* object = (*it).second;
            if (object->LoadingStatus == 0)
            {
                long device = object->PFile->DeviceID;
                CPendingObjectSortKey key(object);
                CPendingObjectMap& queue = DeviceQueue[device][object->Desc.Priority];
                if (queue.NextObject == queue.Objects.find(key))
                    queue.NextObject++;
                TPendingMap& pending = DeviceQueue[device][object->Desc.Priority].Objects;
                pending.erase(pending.lower_bound(key), pending.upper_bound(key));
                if (object->Desc.PCallback)
                {
                    LeaveCriticalSection(&CriticalSection);
                    object->Desc.PCallback->OnFileReadCompletion(false);
                    EnterCriticalSection(&CriticalSection);
                    it = Objects.find(ids[i]);
                }
            }
            if (it != Objects.end())
            {
                CFileObject* doomed = (*it).second;
                if (doomed)
                {
                    if (doomed->Overlapped)
                        GlobalFree(doomed->Overlapped);
                    delete doomed;
                }
                Objects.erase(it);
            }
        }
    }

    --BlockPendingObjectProcessingRefCount;
    LeaveCriticalSection(&CriticalSection);
}

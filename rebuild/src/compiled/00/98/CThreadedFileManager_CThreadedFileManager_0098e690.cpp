// CThreadedFileManager::~CThreadedFileManager() -- threaded file manager destructor: Uninitialise(), then the Objects map and the 2x7 DeviceQueue pending-object maps are destroyed, then ~CBaseClassNonCopyable
// Leaf: ??1CThreadedFileManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098E690, 214 bytes, tail jump to ~CBaseClassNonCopyable 0x0099A300.
// Identity (proven): callers 0x00991330, slot 0 (scalar deleting dtor) of vtable 0x0129A164, RTTI `.?AVCThreadedFileManager@@`
//   (CThreadedFileManager : CBaseClassNonCopyable), and the atexit thunk 0x0122A4D0 for the global instance 0x013BC870; the
//   body stores that vtable first (user dtor) and calls ?Uninitialise@CThreadedFileManager@@QAEXXZ 0x0098E0D0. Ego_r.pdb /
//   FableWin.pdb ??1CThreadedFileManager@@UAE@XZ.
// Members (Ego_r.pdb, retail STLport sizes): DeviceQueue CPendingObjectMap[2][7] +0x04 (16 bytes each: Objects map<CPendingObject
//   SortKey, CFileObject*> + CurrentPosition iterator; landed ctor 0x00991240), CurrentDeviceAtPriorityLevel +0xe4,
//   CurrentDeviceEndOfTimeSlice double[7] +0x100, CurrentlyReadingObject +0x138, Objects map<unsigned long, CFileObject*>
//   +0x13c (Ego_r +0x174 less the 14 x 4 container difference), Initialised/ExitThread/ThreadFinished +0x148, CriticalSection
//   +0x14c, Event +0x164, NextObjectUID +0x168, BlockPendingObjectProcessingRefCount +0x16c, DeviceTimeSlice +0x170.
// Callees: Uninitialise 0x0098E0D0, the Objects tree's _M_erase 0x0098FD80 (called), the pending-object tree's _M_erase
//   0x0098FD40 (VC7.1 expands its top level in place: right subtree call, free 0x00BFEA14, walk left), free 0x00BFEA14.
// Shape: STLport 4.6 _tree.h/_tree.c text with the real _M_erase body: VC7.1 inlines one level of it into the implicit
//   ~CPendingObjectMap array loop and keeps the call for the Objects map, exactly as retail. _RTL_CRITICAL_SECTION is the
//   winnt.h struct written out (no windows.h in this TU model).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
#include "engine/CBaseClassNonCopyable.h"
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_pair.h, stl/_function_base.h, stl/_tree.h (the members this row needs;
// the malloc-backed allocator of this build: deallocate = free 0x00BFEA14)
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Value> struct _Rb_tree_iterator {                     // (traits parameter omitted: unused here)
    _Rb_tree_node_base* _M_node;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<_Value> iterator;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type& __cdecl _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& __cdecl _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
private:
    void _M_erase(_Link_type __x);
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
// stl/_tree.c
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Rb_tree_node<_Value>* __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
};
}

struct _RTL_CRITICAL_SECTION_DEBUG;
struct _RTL_CRITICAL_SECTION {                                           // winnt.h, 0x18 (no windows.h in this model)
    _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;
    long          LockCount;
    long          RecursionCount;
    void*         OwningThread;
    void*         LockSemaphore;
    unsigned long SpinCount;
};
enum EThreadedFileDeviceID { };
class CThreadedFileManager : public CBaseClassNonCopyable {              // vtable 0x0129A164
public:
    class CFileObject;
    class CPendingObjectSortKey {
    public:
        bool operator<(const CPendingObjectSortKey&) const;
        CFileObject* Object;                                             // +0x00
    };
    class CPendingObjectMap {
    public:
        CPendingObjectMap();
        std::map<CPendingObjectSortKey, CFileObject*>           Objects;          // +0x00
        std::map<CPendingObjectSortKey, CFileObject*>::iterator CurrentPosition;  // +0x0c
    };
    CThreadedFileManager();
    virtual ~CThreadedFileManager();
    void Uninitialise();                                                 // 0x0098E0D0
    CPendingObjectMap              DeviceQueue[2][7];                    // +0x04
    EThreadedFileDeviceID          CurrentDeviceAtPriorityLevel[7];      // +0xe4
    double                         CurrentDeviceEndOfTimeSlice[7];       // +0x100
    CFileObject*                   CurrentlyReadingObject;               // +0x138
    std::map<unsigned long, CFileObject*> Objects;                       // +0x13c
    bool                           Initialised;                          // +0x148
    bool                           ExitThread;                           // +0x149
    bool                           ThreadFinished;                       // +0x14a
    _RTL_CRITICAL_SECTION          CriticalSection;                      // +0x14c
    void*                          Event;                                // +0x164
    unsigned long                  NextObjectUID;                        // +0x168
    long                           BlockPendingObjectProcessingRefCount; // +0x16c
    float                          DeviceTimeSlice[2];                   // +0x170
};
CThreadedFileManager::~CThreadedFileManager()
{
    Uninitialise();
}
// CThreadedFileManager::GetTotalObjectsPendingCount() const -- under the manager lock, walk the UID -> file-object map and count the file objects not yet finished (status != 4)
// Leaf: ?GetTotalObjectsPendingCount@CThreadedFileManager@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098E390, length 128 bytes (117 + int3 padding), plain `ret`.
// PDB (FableWin, donor 0x03010910; also Ego_r): ?GetTotalObjectsPendingCount@CThreadedFileManager@@QBEJXZ.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10).
// Label: the manifest's "CFileInstaller_CountNonCompleteItems" is propagated (wrong class); the PDB places the four counters on CThreadedFileManager.
// Layout (retail, as the landed 0x0098E0D0 / 0x0098EB20): Objects (STLport map<unsigned long, CFileObject*>, 12 bytes)
// +0x13C, CriticalSection +0x14C; node value at +0x10 (object at +0x14); CFileObject Desc.Size +0x08, LoadingStatus +0x1C.
// Shape: STLport const_iterator loop with the static _Rb_global::_M_increment(node) inlined (right-subtree descent / parent
// climb); the header (end) and the status constant 4 are hoisted; the lock is a mutable member (const method).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(struct _RTL_CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(struct _RTL_CRITICAL_SECTION* section);   // IAT 0x0143FE10
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };

namespace _STL {

template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> struct less { };
template <class T> class allocator { };

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                       // +0x10
};

struct _Rb_global {                            // _Rb_global<bool>: the out-of-line STLport helpers, inlined here
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global::_M_increment(_M_node); }
};

template <class Value> struct _Rb_tree_const_iterator : _Rb_tree_base_iterator {
    _Rb_tree_const_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    const Value& operator*() const { return static_cast<_Rb_tree_node<Value>*>(_M_node)->_M_value_field; }
    const Value* operator->() const { return &**this; }
    _Rb_tree_const_iterator& operator++() { _M_increment(); return *this; }
    bool operator!=(const _Rb_tree_const_iterator& other) const { return _M_node != other._M_node; }
};

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    unsigned int _M_node_count;                 // +0x04
    Compare _M_key_compare;                     // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};

}   // namespace _STL

enum EFileLoadingStatus { FILE_LOADING_PENDING, FILE_LOADING_READING, FILE_LOADING_READ_ISSUED, FILE_LOADING_READ_COMPLETE, FILE_LOADING_FINISHED };

class CThreadedFileManager {
public:
    class CFileObject {
    public:
        void* PDst;                             // +0x00 Desc.PDst
        unsigned long FilePos;                  // +0x04 Desc.FilePos
        unsigned long Size;                     // +0x08 Desc.Size
        void* PCallback;                        // +0x0c Desc.PCallback
        bool QueueWhenFinished;                 // +0x10 Desc.QueueWhenFinished
        long Priority;                          // +0x14 Desc.Priority
        unsigned long UID;                      // +0x18
        EFileLoadingStatus LoadingStatus;       // +0x1c
    };
    unsigned char _pad00[0x13c];                // vtable, DeviceQueue, device levels, time slices, current object
    _STL::map<unsigned long, CFileObject*> Objects;   // +0x13c
    bool Initialised;                           // +0x148
    bool ExitThread;                            // +0x149
    bool ThreadFinished;                        // +0x14a
    mutable _RTL_CRITICAL_SECTION CriticalSection;    // +0x14c
    void* Event;                                // +0x164
    long GetTotalObjectsPendingCount() const;
    long GetTotalBytesPendingCount() const;
    long GetTotalObjectsFinishedCount() const;
    long GetTotalBytesFinishedCount() const;
};

long CThreadedFileManager::GetTotalObjectsPendingCount() const
{
    EnterCriticalSection(&CriticalSection);
    long total = 0;
    for (_STL::map<unsigned long, CFileObject*>::const_iterator it = Objects.begin(); it != Objects.end(); ++it)
        if (it->second->LoadingStatus != FILE_LOADING_FINISHED)
            ++total;
    LeaveCriticalSection(&CriticalSection);
    return total;
}
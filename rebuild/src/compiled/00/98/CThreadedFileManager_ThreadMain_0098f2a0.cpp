// CThreadedFileManager::ThreadMain() -- the file-loading thread body: process objects until ExitThread is raised, then free every remaining file object (its OVERLAPPED block first) and report ThreadFinished under the lock
// Leaf: ?ThreadMain@CThreadedFileManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098F2A0, length 176 bytes, plain `ret`.
// PDB (FableWin/Ego_r): ?ThreadMain@CThreadedFileManager@@AAEXXZ (private; run from the static BeginThread).
// Callees: CThreadedFileManager::ProcessObjects @ 0x0098ECD0, GlobalFree (IAT 0x0143FDF8, cached in ebp for the loop),
// operator delete @ 0x00BFE9BC, EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10).
// Label: the manifest's "CThreadedFileManager_Shutdown" is descriptive; the PDB name is ThreadMain.
// Layout (retail, as the landed 0x0098E0D0 / 0x0098EB20): Objects +0x13C, ExitThread +0x149, ThreadFinished +0x14A,
// CriticalSection +0x14C; CFileObject Overlapped +0x20.
// Shape: the polling loop is rotated (the `mov edi,edi` is VC's loop-alignment filler); the object walk is the STLport
// iterator loop with STLport's static _Rb_global::_M_increment(node) inlined (the node copy in eax, then stored back to
// the iterator in esi -- the in-place member form keeps the node in esi, 4 bytes shorter), end() re-read after the opaque
// calls; the CFileObject destructor is trivial (delete is a bare operator delete call).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(struct _RTL_CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(struct _RTL_CRITICAL_SECTION* section);   // IAT 0x0143FE10
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void* hMem);   // IAT 0x0143FDF8
void operator delete(void* p);                 // 0x00BFE9BC
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };

namespace std {

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

template <class Value> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    Value& operator*() const { return static_cast<_Rb_tree_node<Value>*>(_M_node)->_M_value_field; }
    Value* operator->() const { return &**this; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    bool operator!=(const _Rb_tree_iterator& other) const { return _M_node != other._M_node; }
};

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    typedef _Rb_tree_iterator<value_type> iterator;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    _Rb_tree_node_base* _M_header;              // +0x00
    unsigned int _M_node_count;                 // +0x04
    Compare _M_key_compare;                     // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};

}   // namespace std

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
        void* Overlapped;                       // +0x20 (GlobalAlloc'd _OVERLAPPED)
    };
    unsigned char _pad00[0x13c];                // vtable, DeviceQueue, device levels, time slices, current object
    std::map<unsigned long, CFileObject*> Objects;   // +0x13c
    bool Initialised;                           // +0x148
    bool ExitThread;                            // +0x149
    bool ThreadFinished;                        // +0x14a
    mutable _RTL_CRITICAL_SECTION CriticalSection;    // +0x14c
    void* Event;                                // +0x164
private:
    void ProcessObjects();                      // 0x0098ECD0
    void ThreadMain();
};

void CThreadedFileManager::ThreadMain()
{
    while (!ExitThread)
        ProcessObjects();
    for (std::map<unsigned long, CFileObject*>::iterator it = Objects.begin(); it != Objects.end(); ++it) {
        CFileObject* object = it->second;
        if (object) {
            if (object->Overlapped)
                GlobalFree(object->Overlapped);
            delete object;
        }
    }
    EnterCriticalSection(&CriticalSection);
    ThreadFinished = true;
    LeaveCriticalSection(&CriticalSection);
}
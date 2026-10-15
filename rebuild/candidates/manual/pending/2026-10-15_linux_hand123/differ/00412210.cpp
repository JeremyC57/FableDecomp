// CRetailDeque<T*>::~CRetailDeque() -- SGI-style deque destructor for a deque of pointers: run the (trivial) element destroy walk over [start, finish), then free the node buffers and the node map
// Grade: DIFFER(150v150) -- register allocation only: retail keeps the finish copy in edx/edi/ebx/ebp and walks the start copy through ecx; this build uses ecx/edx/edi/ebx and ebp (tried: plain call (116v150), inline by-value layer, memberwise user copy ctor (146v150), this POD-body copy ctor). Next: copy order inside the copy constructor / a `const It&` inline layer.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412210, 150 bytes, __thiscall, no arguments. Callers 0x00408A0A and the
// tail jump at 0x00412870.
// Leaf: ??1?$CRetailDeque
// The catalog label (CBankFileAsyncData::FreeBuffer) is propagated -- the landed FreeBuffer rows are other
// bodies; this one is a deque teardown: two 16-byte iterators {cur, first, last, node} at +0 / +0x10, the
// node map at +0x20 and its size at +0x24.
// Callees: the destroy walk 0x00410D80 (__thiscall on an empty helper, both iterators by value, `ret 20h`; it
// steps through every 0x80-byte node buffer and does nothing per element), _M_destroy_nodes 0x0040E130
// (__thiscall on the deque: first node, one-past-last node), free 0x00BFEA14 (the map).
// Shape: both iterators are copied before the pushes by an inline by-value `_Destroy` layer -- the finish
// iterator into four registers, the start iterator into a frame copy, each read through a pointer to the
// member (`lea eax,[esi+10h]` / `mov eax,esi`) -- which needs an iterator with a user copy constructor
// copying a POD body (a memberwise user copy constructor folds the offsets, a POD iterator elides the
// copies); the empty helper object is a frame byte (`lea ecx,[esp+33h]`, user default constructor). The map is tested, the
// nodes released, then the map pointer re-read and freed when non-null.
#include <stdlib.h>

template <class T>
struct SRetailDequeIteratorData {                   // 16 bytes, POD
    T* _M_cur;
    T* _M_first;
    T* _M_last;
    T** _M_node;
};

template <class T>
struct CRetailDequeIterator {
    SRetailDequeIteratorData<T> d;
    CRetailDequeIterator(const CRetailDequeIterator& other) : d(other.d) {}
};

struct CDestroyRange {                              // empty helper
    CDestroyRange() {}
    template <class It> void Destroy(It first, It last);   // <iterator of void*>: 0x00410D80
};

template <class It>
inline void _Destroy(It first, It last)            // the inline by-value layer: its parameters are the copies
{
    CDestroyRange().Destroy(first, last);
}

template <class T>
struct CRetailDeque {
    CRetailDequeIterator<T> _M_start;               // +0x00
    CRetailDequeIterator<T> _M_finish;              // +0x10
    T** _M_map;                                     // +0x20
    unsigned long _M_map_size;                      // +0x24
    void _M_destroy_nodes(T** first, T** last);     // 0x0040E130
    void _M_deallocate_map(T** map)
    {
        if (map)
            free(map);
    }
    ~CRetailDeque()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_map) {
            _M_destroy_nodes(_M_start.d._M_node, _M_finish.d._M_node + 1);
            _M_deallocate_map(_M_map);
        }
    }
};

template struct CRetailDeque<void*>;

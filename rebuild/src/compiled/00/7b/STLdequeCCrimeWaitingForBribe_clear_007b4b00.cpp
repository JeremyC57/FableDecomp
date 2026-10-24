// _STL::deque<CCrimeWaitingForBribe>::clear() -- STLport deque clear for 8-byte POD bribe records: free every interior node buffer, free the finish buffer when the deque spans more than one node, then collapse the finish iterator onto the start
// Leaf: ?clear@?$deque@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B4B00, 94 bytes, thiscall, plain `ret`.
// Callees: free 0x00BFEA14 (STLport allocator deallocate: null-tested).
// Identity: Ego_r.pdb CTCGuard::PendingCrimes (+0x14) is a deque<CCrimeWaitingForBribe>; the only caller (0x007B2FBE, in
//   the CTCGuard routine catalogued as CTCVillage_ProcessDeedReports) calls it with ecx = guard + 0x14. The sibling
//   push_back 0x007B4F70 allocates 0x80-byte node buffers (16 records). Catalog label "CharacterBuffer_Reset" is
//   descriptive.
// Shape: STLport 4.x `deque::clear()`: `for (node = _M_start._M_node + 1; node < _M_finish._M_node; ++node) {
//   _Destroy(*node, *node + buffer_size()); deallocate(*node); }` (the finish node re-read after every free), then if
//   the start and finish nodes differ the two partial ranges are destroyed and the finish buffer freed, else the single
//   range is destroyed (all no-ops for a POD), and `_M_finish = _M_start` (a four-word iterator copy through `lea`).
//   Retail STLport deque layout: _M_start {cur, first, last, node} +0, _M_finish +0x10, _M_map +0x20, _M_map_size +0x24.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CCrimeWaitingForBribe {                               // 8 bytes, POD
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace _STL {

template <class T> inline void _Destroy(T*, T*) { }        // trivial destructor: nothing to do

template <class T> class allocator {
public:
    allocator() { }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template <class T> struct _Deque_iterator {
    T*  _M_cur;                                             // +0x00
    T*  _M_first;                                           // +0x04
    T*  _M_last;                                            // +0x08
    T** _M_node;                                            // +0x0c
};

template <class T> class deque {
public:
    _Deque_iterator<T> _M_start;                            // +0x00
    _Deque_iterator<T> _M_finish;                           // +0x10
    T**                _M_map;                              // +0x20
    unsigned int       _M_map_size;                         // +0x24
    static unsigned int buffer_size() { return 128 / sizeof(T); }
    void _M_deallocate_node(T* p) { allocator<T>().deallocate(p, buffer_size()); }
    void clear();
};

template <class T> void deque<T>::clear()
{
    for (T** __node = this->_M_start._M_node + 1; __node < this->_M_finish._M_node; ++__node) {
        _Destroy(*__node, *__node + buffer_size());
        _M_deallocate_node(*__node);
    }
    if (this->_M_start._M_node != this->_M_finish._M_node) {
        _Destroy(this->_M_start._M_cur, this->_M_start._M_last);
        _Destroy(this->_M_finish._M_first, this->_M_finish._M_cur);
        _M_deallocate_node(this->_M_finish._M_first);
    }
    else
        _Destroy(this->_M_start._M_cur, this->_M_finish._M_cur);
    this->_M_finish = this->_M_start;
}

template class deque<CCrimeWaitingForBribe>;
}
#pragma optimize("y", off)
#pragma optimize("s", on)
// CDwordDeque::back() -- STLport deque<4-byte T>::back(): copies the finish iterator, steps it back one element and returns a reference to that element
// Leaf: back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B039, length 33 bytes. Frame-pointer size TU (`push ebp ; mov ebp,esp`,
// the 16-byte iterator copied with four `movsd`, `leave`): `optimize("y",off)` + `("s",on)`.
// Callees: _Deque_iterator_base::_M_decrement() @ 0x0059AC41 (`cur == first` -> previous node, first/last reset,
// cur = last; then cur -= 4; manifest "lua_getglobals" is a propagated label).
// Label: the catalog name "CTCTrophy::OnShowTrophyInitialise" is a propagated label: the body is the inline
// `back()` of an STLport deque whose finish iterator sits at +0x10 (start iterator +0x00, then map / map size), the
// element 4 bytes and the node buffer 0x80 bytes (32 elements); 26 call sites in 0x00595xxx..0x0059Axxx use it
// (the module of the twin push_back helper 0x0059B576). The element type is not identified, so a local model.
// Shape: `iterator tmp = _M_finish; --tmp; return *tmp;` -- operator-- is the inline `_M_decrement(); return *this;`
// and the reference result is tmp._M_cur (`mov eax,[ebp-10h]`).
struct SDequeIterator {                     // STLport _Deque_iterator<T, ...>, 16 bytes
    unsigned long*  _M_cur;
    unsigned long*  _M_first;
    unsigned long*  _M_last;
    unsigned long** _M_node;
    void _M_decrement();                    // 0x0059AC41
    SDequeIterator& operator--() { _M_decrement(); return *this; }
    unsigned long& operator*() const { return *_M_cur; }
};

struct CDwordDeque {                        // STLport deque<T> (4-byte element)
    SDequeIterator  _M_start;               // +0x00
    SDequeIterator  _M_finish;              // +0x10
    unsigned long** _M_map;                 // +0x20
    unsigned long   _M_map_size;            // +0x24
    unsigned long& back();
};

unsigned long& CDwordDeque::back()
{
    SDequeIterator tmp = _M_finish;
    --tmp;
    return *tmp;
}
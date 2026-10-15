#pragma optimize("s", on)
// CDwordDeque::_M_push_back_aux_v(const unsigned long&) -- STLport deque<4-byte T> push_back slow path: reserve a map slot, allocate the next 0x80-byte node, construct the value at the old finish and move finish to the new node
// Leaf: _M_push_back_aux_v
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B576, length 76 bytes. Size TU (`pop ecx` after the allocation
// call even though its result is used): `optimize("s",on)`.
// Callees: deque::_M_reserve_map_at_back(size_type) @ 0x0059B4D8 (manifest "CTCUserActionPointBase::
// GetNumberOfActionPointsInUse" is propagated), malloc @ 0x00BFEA0E (the node allocator, 0x80 bytes = 32 elements).
// Label: the catalog name "CTCUserActionPointBase::AllocateBuffer" is propagated: the body is STLport's
// `_M_push_back_aux_v` of the same deque as 0x0059B039 (finish iterator at +0x10, node pointer at +0x1C).
// Shape: the value is copied into a local first (`mov edi,[eax]`, `value_type __t_copy = __t;`), `_Construct`
// is a placement new (the null test on _M_cur), `_M_set_node` stores node, first, last in that order and cur is
// re-read from first.
#include <stddef.h>

inline void* operator new(size_t, void* where) { return where; }

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct SDequeIterator {                     // STLport _Deque_iterator<T, ...>, 16 bytes
    unsigned long*  _M_cur;
    unsigned long*  _M_first;
    unsigned long*  _M_last;
    unsigned long** _M_node;
    void _M_set_node(unsigned long** newNode)
    {
        _M_node = newNode;
        _M_first = *newNode;
        _M_last = _M_first + 0x80 / sizeof(unsigned long);
    }
};

struct CDwordDeque {                        // STLport deque<T> (4-byte element)
    SDequeIterator  _M_start;               // +0x00
    SDequeIterator  _M_finish;              // +0x10
    unsigned long** _M_map;                 // +0x20
    unsigned long   _M_map_size;            // +0x24
    void _M_reserve_map_at_back(unsigned long nodesToAdd);   // 0x0059B4D8
    void _M_push_back_aux_v(const unsigned long& value);
};

void CDwordDeque::_M_push_back_aux_v(const unsigned long& value)
{
    unsigned long copy = value;
    _M_reserve_map_at_back(1);
    *(_M_finish._M_node + 1) = (unsigned long*)malloc(0x80);
    new (_M_finish._M_cur) unsigned long(copy);
    _M_finish._M_set_node(_M_finish._M_node + 1);
    _M_finish._M_cur = _M_finish._M_first;
}

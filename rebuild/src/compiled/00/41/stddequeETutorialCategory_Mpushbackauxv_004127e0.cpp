// std::deque<ETutorialCategory>::_M_push_back_aux_v(const ETutorialCategory& value) -- STLport deque push_back slow path: grow the map if no slot is left past the finish node, allocate the next 0x80-byte node, construct the value at the old finish and move finish to the new node
// Leaf: ?_M_push_back_aux_v@?$CRetailDeque@W4ETutorialCategory@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004127E0, 101 bytes. Callees: deque::_M_reallocate_map(1,
// false) 0x00411630 (manifest Std_Deque_Resize_WithGap), malloc 0x00BFEA0E (the node allocator:
// 0x80 bytes = 32 four-byte elements).
//
// Retail bytes:  mov eax,[value] ; push esi ; mov esi,ecx ; mov ecx,[esi+0x1c] ; mov edx,[esi+0x24] ;
//   push edi ; mov edi,[eax] ; sub ecx,[esi+0x20] ; sar ecx,2 ; sub edx,ecx ; cmp edx,2 ; jae K ;
//   push 0 ; push 1 ; mov ecx,esi ; call _M_reallocate_map ; K: push 0x80 ; call malloc ;
//   mov ecx,[esi+0x1c] ; mov [ecx+4],eax ; mov eax,[esi+0x10] ; add esp,4 ; test eax,eax ; je N ;
//   mov [eax],edi ; N: <_M_set_node(node + 1): node, first, last = first + 0x80> ;
//   mov edx,[esi+0x14] ; pop edi ; mov [esi+0x10],edx ; pop esi ; ret 4
// STLport 4.x `_M_push_back_aux_v`: `value_type t_copy = t; _M_reserve_map_at_back(); *(_M_finish._M_node
// + 1) = _M_allocate_node(); _Construct(_M_finish._M_cur, t_copy); _M_finish._M_set_node(_M_finish._M_node
// + 1); _M_finish._M_cur = _M_finish._M_first;` (no EH frame: the TU has no /EHsc, so the
// _STLP_TRY/_STLP_UNWIND pair is empty). _M_reserve_map_at_back(1) is inlined here (`nodes + 1 >
// _M_map_size - (_M_finish._M_node - _M_map)` -> `cmp edx,2 ; jae`), unlike the size-TU twin
// CDwordDeque::_M_push_back_aux_v 0x0059B576 which calls it. The element is ETutorialCategory: the
// only direct caller is CUserProfileManager::GiveTutorialForCategory (0x0040BB16), whose inline
// push_back fast path writes the deque at CUserProfileManager+0x98 (PendingTutorialCategories);
// 0x004129E0 / 0x00412C00 tail-jump here. The manifest name Std_Deque_PushBack_ChunkAlloc is descriptive.
#include <stddef.h>

inline void* operator new(size_t, void* where) { return where; }

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

enum ETutorialCategory { TUTORIAL_CATEGORY_NONE = 0 };

template <class T>
struct CRetailDequeIterator {                     // STLport _Deque_iterator<T, ...>, 16 bytes
    T* _M_cur;
    T* _M_first;
    T* _M_last;
    T** _M_node;
    void _M_set_node(T** newNode)
    {
        _M_last = (_M_first = *(_M_node = newNode)) + 0x80 / sizeof(T);
    }
};

template <class T>
struct CRetailDeque {                             // STLport deque<T>, 40 bytes
    CRetailDequeIterator<T> _M_start;             // +0x00
    CRetailDequeIterator<T> _M_finish;            // +0x10
    T** _M_map;                                   // +0x20
    size_t _M_map_size;                           // +0x24
    void _M_reallocate_map(size_t nodesToAdd, bool addAtFront);   // 0x00411630
    void _M_reserve_map_at_back(size_t nodesToAdd = 1)
    {
        if (nodesToAdd + 1 > _M_map_size - (_M_finish._M_node - _M_map))
            _M_reallocate_map(nodesToAdd, false);
    }
    T* _M_allocate_node() { return (T*)malloc(0x80); }
    void _M_push_back_aux_v(const T& value);
};

template <class T>
void CRetailDeque<T>::_M_push_back_aux_v(const T& value)
{
    T copy = value;
    _M_reserve_map_at_back();
    *(_M_finish._M_node + 1) = _M_allocate_node();
    new (_M_finish._M_cur) T(copy);
    _M_finish._M_set_node(_M_finish._M_node + 1);
    _M_finish._M_cur = _M_finish._M_first;
}

template struct CRetailDeque<ETutorialCategory>;
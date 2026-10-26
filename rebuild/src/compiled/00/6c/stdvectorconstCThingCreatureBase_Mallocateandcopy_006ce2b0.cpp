// std::vector<const CThingCreatureBase*>::_M_allocate_and_copy(unsigned int n, const CThingCreatureBase* const* first, const CThingCreatureBase* const* last) -- STLport: malloc n pointers (none for zero), trivially copy [first, last) into them, return the new block
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CE2B0, length 65 bytes (+ int3 padding), thiscall `ret 0Ch` (this unused:
// the allocator is empty). Single caller: vector::reserve 0x006CE230 (this chunk's row), which is reached from
// CGroundNavigatorExternalInterface::IsMovementBlockedAt (0x006CDF5C, `reserve(8)` on a local result vector that is
// then filled by GetAllThingsInRoughArea 0x006CE300 -- the landed model of that caller names the element
// `const CThingCreatureBase*`; the bytes only show a 4-byte POD element). The manifest label
// "CActiveFile_AllocStringBuffer" is propagated/descriptive.
// Callees: malloc @ 0x00BFEA0E (STLport allocator), __copy_trivial(first, last, result) @ 0x00405BA0 (out-of-line
// `__fastcall` COMDAT: memmove + end pointer; manifest "Std_Move_Backward").
// Shape: STLport 4.x `_M_allocate_and_copy` = `allocate(n)` (`n != 0 ? malloc(n * 4) : 0`) + `__uninitialized_copy(first,
// last, result, __true_type())` -> `__copy_trivial`; VC duplicates the copy call and the return into both arms of the
// allocate ternary. It is a member template (the iterator type is a template parameter in STLport), hence `??$`.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

class CThingCreatureBase;

namespace std {
struct __true_type { };

void* __fastcall __copy_trivial(const void* first, const void* last, void* result);   // 0x00405BA0

template <class InputIter, class ForwardIter>
inline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __true_type&)
{
    return (ForwardIter)__copy_trivial(first, last, result);
}

template <class T> struct allocator {
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    template <class ForwardIter> T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = _M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __true_type());
        return result;
    }
};

template const CThingCreatureBase** vector<const CThingCreatureBase*>::_M_allocate_and_copy(unsigned int,
    const CThingCreatureBase* const*, const CThingCreatureBase* const*);
}
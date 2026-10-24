// std::vector<CAppearanceModifierGraphics::CEntry>::_M_allocate_and_copy(unsigned int n, const CEntry* first, const CEntry* last) -- STLport: malloc n 24-byte graphic entries (none for zero), placement-copy [first, last) into them, return the new block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045CD9B, length 71 bytes, `ret 0Ch` (thiscall: n, first, last).
// Callees: malloc @ 0x00BFEA0E.
// Label: the catalog "CActiveFile_AssignVector24" is behavioural. The single caller is the vector's operator= @ 0x0045CC79
// (landed as Vector<Record24>::Assign), which CAppearanceModifierDef::operator= (landed 0x0045CC04) applies to MorphGraphics,
// a CArray<CAppearanceModifierGraphics::CEntry>; the PDB CEntry is 24 bytes (Type, MorphType, BankIndex, RenderSizeX,
// StartMorphStrength, EndMorphStrength).
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type) with
// _Construct inlined (`new (p) T(value)`: the null test, then the implicit copy as a 6-dword `rep movsd` block).
// Shape: `push 6 ; pop ecx` and `pop ecx` after malloc = size TU (#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CAppearanceModifierGraphics {
    struct CEntry {                                // PDB 24 bytes
        long  Type;                                // +0x00 EEngineGraphicType
        long  MorphType;                           // +0x04 EHeroMorphType
        long  BankIndex;                           // +0x08
        float RenderSizeX;                         // +0x0C
        float StartMorphStrength;                  // +0x10
        float EndMorphStrength;                    // +0x14
    };
};

namespace std {
struct __false_type { };

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last);
};

template <class T, class A> T* vector<T, A>::_M_allocate_and_copy(unsigned int n, const T* first, const T* last)
{
    T* result = _M_end_of_storage.allocate(n);
    __uninitialized_copy(first, last, result, __false_type());
    return result;
}

template class vector<CAppearanceModifierGraphics::CEntry, allocator<CAppearanceModifierGraphics::CEntry> >;
}
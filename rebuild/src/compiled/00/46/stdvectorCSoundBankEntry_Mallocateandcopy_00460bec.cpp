// std::vector<CSoundBankEntry>::_M_allocate_and_copy(unsigned int n, const CSoundBankEntry* first, const CSoundBankEntry* last) -- STLport: malloc n 20-byte sound bank entries (none for zero), placement-copy [first, last) into them, return the new block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460BEC, length 71 bytes, `ret 0Ch` (thiscall: n, first, last).
// Callees: malloc @ 0x00BFEA0E.
// Label: the catalog "CActiveFile_AssignVector20" is behavioural. The single caller is vector<CSoundBankEntry>::operator=
// @ 0x00460ACA, the first vector CSoundDef::operator= (landed 0x00460A61) assigns; PDB CSoundBankEntry is 20 bytes (ID,
// Filename, SymbolFilename, LipSyncBankHandle -- CDefString string-table indices -- and DVD).
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type) with
// _Construct inlined (`new (p) T(value)`: the null test, then the implicit copy as a 5-dword `rep movsd` block, so
// CDefString is trivially copyable here and modelled as its index).
// Shape: `push 5 ; pop ecx` and `pop ecx` after malloc = size TU (#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CSoundBankEntry { public:                           // PDB 20 bytes
    unsigned long ID;                              // +0x00
    long          Filename;                        // +0x04 CDefString (string-table index)
    long          SymbolFilename;                  // +0x08 CDefString
    long          LipSyncBankHandle;               // +0x0C CDefString
    bool          DVD;                             // +0x10
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

template class vector<CSoundBankEntry, allocator<CSoundBankEntry> >;
}
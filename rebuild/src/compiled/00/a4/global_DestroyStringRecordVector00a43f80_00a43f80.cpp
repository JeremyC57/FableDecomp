// _global::Destroy_StringRecordVector_00a43f80(SStringRecordVector_00a43f80* first, SStringRecordVector_00a43f80* last, const __false_type&) -- STLport __destroy_aux<vector<T>*> (T = 12-byte record led by a CCharString): run each vector's destructor on [first, last) (destroy its elements, free the storage)
// Leaf: Destroy_StringRecordVector_00a43f80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43F80, length 75 bytes (+5 int3 pad), `ret 4`.
// Callers: twelve sites -- the vector<vector<T> > destructors / erase / resize paths at 0x00A43FF0.., 0x00A44130,
//   0x00A495F0, 0x00A4AE00, 0x00A4AF00, 0x00A4B160 (`lea eax,[esp+N] ; push eax` = the empty tag by reference).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (out of line: UnassignString + the instance counter), free @ 0x00BFEA14.
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated (`ret 4` with no
//   destination is the STLport destroy helper, GOTCHAS chunk 358). The outer element is a retail 12-byte STLport vector
//   (stride 0x0C); its 12-byte element has a CCharString at +0 and trivially destructible words after it. The element
//   type is not named by a landed relative (the body is shared by several callers) -- descriptive names.
// Shape: `__fastcall` (first in ecx -> edi, last in edx -> ebp, the tag reference on the stack). Each vector's destructor
//   is inlined: `_Destroy(_M_start, _M_finish)` (finish in ebx, start in esi; the record's implicit destructor is the
//   CCharString call on `this`), then ~_Vector_base's deallocate as a null-tested free of the re-read start.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct CCharString {
    void* m_pStringData;                        // +0x00
    ~CCharString();                             // 0x0099EAE0
};

struct SStringRecord12_00a43f80 {               // 12 bytes
    CCharString m_name;                         // +0x00
    long        m_4;                            // +0x04
    long        m_8;                            // +0x08
};

template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        _Destroy(first);
}

template <class T> struct _Vector_base {        // retail 12-byte STLport vector base
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start) free(_M_start); }   // _M_end_of_storage.deallocate(_M_start, n)
};

struct SStringRecordVector_00a43f80 : _Vector_base<SStringRecord12_00a43f80> {   // _STL::vector<SStringRecord12>
    ~SStringRecordVector_00a43f80() { _Destroy(this->_M_start, this->_M_finish); }
};

void __fastcall Destroy_StringRecordVector_00a43f80(SStringRecordVector_00a43f80* first, SStringRecordVector_00a43f80* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(first);
}
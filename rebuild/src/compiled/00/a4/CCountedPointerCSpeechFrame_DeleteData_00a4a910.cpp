// CCountedPointer<CSpeechFrame>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the speech frame (free its frequency array storage) and free the object
// Leaf: ?DeleteData@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4A910, length 33 bytes, `ret` (static `__fastcall`, data in ecx).
// Referenced (absolute) at 0x00A4ABB0 by CCountedPointer<CSpeechFrame>::Reset 0x00A4AB60 (landed), which stores it as the
//   count block's DeleteFunc; that Reset is called from 0x00A4958B with a fresh 0x1C-byte CSpeechFrame (three zero words =
//   the array) whose ReadFromFile 0x00A49240 follows. Ego_r.pdb names the template member
//   ?DeleteData@?$CCountedPointer@...@@SIXPAX@Z (public static __fastcall void (void*)).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate of the array block), operator delete 0x00BFE9BC.
// Label correction: catalogued as `FreeListNode_Destroy`.
// Layout (retail, as the landed CSpeechFrame::ReadFromFile): Frequencies CArray<CComplexNumber> +0 (STLport vector: start
//   +0, finish +4, end of storage +8), Amplitude +0x0C, MedianFrequency +0x10, BaseFrequency +0x14, FrequencyVariance
//   +0x18 (0x1C bytes; the PDB's 16-byte CArray puts it at 0x20).
// Shape: `delete (T*)data` -- the null test, the inline (forced) destructor chain of a POD-element vector (`_Destroy` of
//   trivially destructible elements is empty; `_Vector_base`'s destructor frees a non-null start), then operator delete.
extern "C" void __cdecl free(void*);                 // 0x00BFEA14
void __cdecl operator delete(void* p);               // 0x00BFE9BC

namespace std {
template <class _Tp>
class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    __forceinline ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    __forceinline ~vector() { }                      // _Destroy over trivially destructible elements: nothing
};
}

class CComplexNumber {                               // 8 bytes, trivially destructible
public:
    float Real, Imaginary;
};

template <class T>
class CArray : public std::vector<T> {
};

class CSpeechFrame {                                 // 0x1C bytes in retail
public:
    CArray<CComplexNumber> Frequencies;              // +0x00
    float Amplitude;                                 // +0x0C
    float MedianFrequency;                           // +0x10
    float BaseFrequency;                             // +0x14
    float FrequencyVariance;                         // +0x18
};

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                            // +0x00
    CCPPointerInfo* Info;                            // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<CSpeechFrame>;
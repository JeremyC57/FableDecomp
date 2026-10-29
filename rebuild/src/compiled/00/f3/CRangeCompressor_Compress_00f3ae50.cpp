// CRangeCompressor::Compress(const void* data, long count, long stride, unsigned char* dest, long destSize) -- compress count stride-byte records: build the compression script and, when the scripted stream (plus its 2 header bytes) beats the raw size + 1, write flag byte 1 and the range-compressed stream, else flag byte 0 and a raw copy; add both sizes to the compression statistics, decompress the result into a scratch byte array as a check, and return the bytes written (1 with flag 0 for an empty or out-of-range input)
// Leaf: ?Compress@CRangeCompressor@@QAEJPBXJJPAEJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3AE50, 353 bytes, thiscall `ret 14h`.
// Identity: Ego_r.pdb CRangeCompressor donor order ... CalcCompressionScript 0x00BDCDE0, Compress(const void*, long, long,
//   unsigned char*, long) 0x00BDD0A0 (0x170 bytes), Compress(const void*, long, long, CArray<unsigned char>&) 0x00BDD210;
//   retail CalcCompressionScript 0x00F3ABF0 (the call here, 5 arguments as its PDB signature) then this body. Five stack
//   arguments (`ret 14h`) with the last (destSize) never read, as the public overload's PBXJJPAEJ; the callees are the
//   bound Decompress 0x00F39ED0 and the landed private Compress 0x00F3A2E0.
// Callees: CalcCompressionScript 0x00F3ABF0, Compress(const unsigned char*, long, long, unsigned char*, const CArray<CScriptEntry>&)
//   0x00F3A2E0, the STLport vector<unsigned char>(n, val, alloc) fill constructor 0x009D2A40, Decompress 0x00F39ED0, free
//   0x00BFEA14 (the two arrays' inline destructors); memcpy is the /Oi inline `rep movsd ; rep movsb`.
// Globals (descriptive): 0x0143E980 bytes given to Compress, 0x0143E97C bytes it wrote; they follow the landed Decompress
//   statistics 0x0143E970 (time) and 0x0143E978 (bytes).
// Shape: the 64-bit range test of count * stride (`cdq`, signed compare against 0 and 0x7FFFFFFF) as in the landed
//   Decompress; the check buffer pointer is CArray::GetAsCArray() (FableWin.pdb: the first element when size() > 0, else
//   GetZeroSizeArray() = the array object itself).
#include <string.h>
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
inline void fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val)
{
    unsigned char __tmp = __val;
    memset(__first, __tmp, __last - __first);
}
inline unsigned char* uninitialized_fill_n(unsigned char* __first, unsigned int __n, const unsigned char& __val)
{
    fill(__first, __first + __n, __val);
    return __first + __n;
}
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    vector(size_type __n, const _Tp& __val, const _Alloc& __a = _Alloc());     // 0x009D2A40 for unsigned char
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
};

// Same-TU body of the fill constructor retail compiles out of line at 0x009D2A40 (the landed STLport 4.6 source): with it
// visible VC7.1 homes the value and allocator temporaries in one dead argument slot, as retail does.
template <class _Tp, class _Alloc>
__declspec(noinline) vector<_Tp, _Alloc>::vector(size_type __n, const _Tp& __val, const _Alloc& __a)
    : _Vector_base<_Tp, _Alloc>(__n, __a)
{
    this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
}
}

template <class T> class CArray : public std::vector<T> {
public:
    CArray() { }
    CArray(unsigned int n, const T& val) : std::vector<T>(n, val) { }
    T* GetAsCArray() { return this->size() > 0 ? this->_M_start : const_cast<T*>(GetZeroSizeArray()); }
private:
    const T* GetZeroSizeArray() const { return reinterpret_cast<const T*>(this); }
};

extern long g_RangeCompressBytesIn;                            // 0x0143E980 (descriptive)
extern long g_RangeCompressBytesOut;                           // 0x0143E97C (descriptive)

class CRangeCompressor {
public:
    struct CScriptEntry {
        unsigned long Flags;        // +0x00
        long BitsNeeded;            // +0x04
        unsigned long Bias;         // +0x08
        long GapStart;              // +0x0C
        unsigned long KeptMask;     // +0x10
        unsigned long SetMask;      // +0x14
    };
    void Decompress(const unsigned char* data, long count, long stride, void* dest);                    // 0x00F39ED0
    long Compress(const void* data, long count, long stride, unsigned char* dest, long destSize);
private:
    long Compress(const unsigned char* data, long count, long stride, unsigned char* dest,
                  const CArray<CScriptEntry>& script);                                                  // 0x00F3A2E0
    long CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count, long stride,
                               long blockSize);                                                         // 0x00F3ABF0
};

long CRangeCompressor::Compress(const void* data, long count, long stride, unsigned char* dest, long destSize)
{
    if (count == 0) {
        dest[0] = 0;
        return 1;
    }
    long size = count * stride;
    __int64 size64 = size;
    if (size64 < 0 || size64 > 0x7fffffff) {
        dest[0] = 0;
        return 1;
    }
    CArray<CScriptEntry> script;
    long written;
    if (CalcCompressionScript(script, (const unsigned char*)data, count, stride, stride) + 2 >= size + 1) {
        dest[0] = 0;
        memcpy(dest + 1, data, size);
        written = size + 1;
    } else {
        dest[0] = 1;
        written = Compress((const unsigned char*)data, count, stride, dest + 1, script) + 1;
    }
    g_RangeCompressBytesIn += size;
    g_RangeCompressBytesOut += written;
    CArray<unsigned char> check(size, 0);
    Decompress(dest, count, stride, check.GetAsCArray());
    return written;
}
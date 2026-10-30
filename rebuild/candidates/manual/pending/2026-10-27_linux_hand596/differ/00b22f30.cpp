// NParticleEngine::GFInterpolateArray(const CArray<unsigned long>& a, const CArray<unsigned long>& b, float t, const unsigned long& defaultValue) -- blend two word arrays: the result length interpolates the two lengths, and each element interpolates a[i] and b[i] (an array that is too short supplies the default)
// Grade: DIFFER(354v340) -- register allocation: retail spills the hidden result pointer ([esp+1Ch]) and keeps a/b in esi/edi, sizes in ebx/ebp, places the i < sizeA value-select arm after the epilogue and shares the b[i] load; ours keeps the result pointer in ebx, b on the stack, and lays the arms out in source order (the inline STLport copy at the end then reloads finish/start around malloc)
// Leaf: ??$GFInterpolateArray@K@NParticleEngine@@YI?AV?$CArray@K@@ABV1@0MABK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B22F30, 340 bytes, __fastcall: hidden result pointer in ecx (returned in eax), a in
//   edx, b / t / defaultValue on the stack (`ret 0Ch`).
// Identity (proven): Ego_r.pdb name set ??$GFInterpolateArray@K@NParticleEngine@@YI?AV?$CArray@K@@ABV1@0MABK@Z (the
//   unsigned long instantiation; its C3DVector sibling is ??$GFInterpolateArray@VC3DVector@@...); the one caller is the
//   landed NParticleEngine::CSplinePosParamData::InterpolateTo 0x00B217D0, which passes the CRC of "" as the default.
//   The landed caller binds it as a non-template `?GFInterpolateArray@NParticleEngine@@YI?AU?$CArray@K@1@ABU21@0MABK@Z`
//   over an invented NParticleEngine::CArray struct (NAME_FIXES.tsv).
// Callees: std::vector<unsigned long>::vector(unsigned int) 0x00A96100 (the result array, out of line), malloc 0x00BFEA0E /
//   memmove 0x00BFEAE6 (the inline STLport copy of the returned array: no NRVO in VC7.1), free 0x00BFEA14 (the local's
//   inline destructor). Floats: 1.0f 0x0122DED8, 4294967296.0f 0x0122DCB4 (the unsigned-to-float correction).
// Retail-asm: 0x00B22F7F `fld dword ptr [esp+14h]` + 0x00B22F83 `fistp dword ptr [esp+30h]` (and again 0x00B22FE5 /
//   0x00B22FE9 `fistp dword ptr [esp+18h]`) -- the engine's inline-assembly float-to-long helper GFFastFloatToLong (Ego_r.pdb
//   ?GFFastFloatToLong@@YIJM@Z) inlined through GFInterpolate: the blended value is stored to the helper's float parameter
//   and reloaded for a bare dword fistp; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` without the reload.
// Shape: GFInterpolate(long, long, float) (?GFInterpolate@NParticleEngine@@YIJJJM@Z: signed fild) gives the length, and
//   GFInterpolate(unsigned long, unsigned long, float) (?..@YIKKKM@Z: fild plus the 2^32 correction) each element; the
//   default stands in for the missing side (three arms that VC7.1 cross-jumps into one blend).
// Layout: CArray<T> is the engine's 12-byte STLport vector (no engine header for CArray).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);    // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp& reference;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    explicit vector(size_type __n);                     // 0x00A96100 (out of line)
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __true_type());
    }
    ~vector() { }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    CArray(unsigned int n) : std::vector<T>(n) { }
    T& operator[](unsigned int i) { return *(this->begin() + i); }
    const T& operator[](unsigned int i) const { return *(this->begin() + i); }
};

__forceinline long __fastcall GFFastFloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

namespace NParticleEngine {
__forceinline long __fastcall GFInterpolate(long a, long b, float t)
{
    return GFFastFloatToLong(a * (1.0f - t) + b * t);
}
__forceinline unsigned long __fastcall GFInterpolate(unsigned long a, unsigned long b, float t)
{
    return GFFastFloatToLong(a * (1.0f - t) + b * t);
}

template <class T>
CArray<T> __fastcall GFInterpolateArray(const CArray<T>& a, const CArray<T>& b, float t, const T& defaultValue)
{
    long sizeA = a.size();
    long sizeB = b.size();
    long count = GFInterpolate(sizeA, sizeB, t);
    CArray<T> result(count);
    for (long i = 0; i < count; ++i) {
        T valueA;
        T valueB;
        if (i < sizeA) {
            valueA = a[i];
            if (i < sizeB)
                valueB = b[i];
            else
                valueB = defaultValue;
        }
        else {
            valueA = defaultValue;
            valueB = b[i];
        }
        result[i] = GFInterpolate(valueA, valueB, t);
    }
    return result;
}

template CArray<unsigned long> __fastcall GFInterpolateArray<unsigned long>(const CArray<unsigned long>&, const CArray<unsigned long>&, float, const unsigned long&);
}

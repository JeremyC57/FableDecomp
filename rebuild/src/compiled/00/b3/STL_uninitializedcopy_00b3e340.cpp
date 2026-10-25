// std::__uninitialized_copy(CTextureManagerPoolInfo* first, CTextureManagerPoolInfo* last, CTextureManagerPoolInfo* result, const __false_type&) -- STLport placement copy-construct [first, last) of texture-pool records into raw storage, return the advanced result
// Leaf: ??$__uninitialized_copy@PAVCTextureManagerPoolInfo@@PAV1@@std@@YIPAVCTextureManagerPoolInfo@@PAV1@00ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3E340, length 68 bytes, `__fastcall` (first ecx, last edx, result + the
//   tag reference on the stack), `ret 8`.
// Identity: the only caller is the landed std::vector<CTextureManagerPoolInfo>::_M_insert_overflow 0x00B3EA70 (two calls:
//   the head and tail moves), whose model already carries this body as its same-TU __uninitialized_copy; element =
//   CTextureManagerPoolInfo {Width, Height, CPixelFormat PixelFormat, bool UseMipmaps, Count} (20 bytes, the divide by 20
//   there; Ego_r CTextureManagerPoolInfo). The catalog `_Move<NThingMap::CMapwhoLevelInfo*>` (also 20 bytes) and the
//   landed SRecord20 name were a propagation and a shape name.
// Shape: STLport 4.6 `for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first);`: the placement new
//   keeps the `test eax,eax` null test on the destination. The implicit (bitwise) copy constructor of the 5-word record is
//   copied through reference registers: `mov esi,ecx ; mov edi,eax` and five ebx moves.
inline void* operator new(unsigned int, void* p) throw() { return p; }   // placement new (the null test on the destination)

class CPixelFormat {
public:
    long FormatIndex;                                   // +0x00
    CPixelFormat() : FormatIndex(-1) { }
};

class CTextureManagerPoolInfo {                         // 20 bytes
public:
    unsigned long Width;                                // +0x00
    unsigned long Height;                               // +0x04
    CPixelFormat  PixelFormat;                          // +0x08
    bool          UseMipmaps;                           // +0x0C
    unsigned long Count;                                // +0x10
};

struct __false_type { __false_type() { } };            // STLport empty tag; passed by const reference (one stack slot)

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

template CTextureManagerPoolInfo* __fastcall std::__uninitialized_copy<CTextureManagerPoolInfo*, CTextureManagerPoolInfo*>(
    CTextureManagerPoolInfo*, CTextureManagerPoolInfo*, CTextureManagerPoolInfo*, const __false_type&);

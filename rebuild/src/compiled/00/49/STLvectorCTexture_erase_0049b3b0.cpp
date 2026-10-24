// _STL::vector<CTexture>::erase(CTexture*, CTexture*) -- STLport range erase of 8-byte textures: assign the tail [last, finish) down onto first, destroy the vacated tail and pull finish back; returns first
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B3B0, 104 bytes, `ret 8` (__fastcall: this -> ebp, first and last on
//   the stack; the iterators are raw pointers).
// Callees: 0x009FA1C0 CTexture::operator= (out of line, `push src ; mov ecx,dst`), 0x009F9F70 ~CTexture (byte twin of
//   CTexture::Uninitialise: release the D3D texture, clear the pointer, keep the 28-bit ByteLength).
// Callers: nine sites in CProgressDisplay (0x00499F00..0x0049B818) clearing its texture vectors.
// Identity: FableWin.pdb names `vector<CTexture>::erase` / `??1CTexture@@QAE@XZ`; the 8-byte element is Ego_r.pdb
//   CTexture { IDirect3DTexture9* PD3DTexture; ulong ByteLength:28, AllocationSource:4; } (the generated engine header
//   has the layout but no member functions, so the class is modelled locally like the landed CTexture users).
// Shape: STLport 4.x `pointer i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i; return first;`
//   with the random-access `__copy` (`for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`,
//   one counted `dec ebx` loop, the source addressed as dst + (last - first)) and `_Destroy` over the tail; the two
//   exits are tail-duplicated (empty tail: `mov eax,ecx`).
// Label correction: catalogued as `Vector_EraseRange` (right idea); the element is CTexture.
struct IDirect3DTexture9;

struct CTexture {
    IDirect3DTexture9* PD3DTexture;                     // +0x00
    unsigned long      ByteLength : 28;                 // +0x04
    unsigned long      AllocationSource : 4;
    CTexture& operator=(const CTexture& other);         // 0x009FA1C0
    ~CTexture();                                        // 0x009F9F70
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                          _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

inline CTexture* copy(CTexture* __first, CTexture* __last, CTexture* __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

inline void _Destroy(CTexture* __pointer) { __pointer->~CTexture(); }

inline void _Destroy(CTexture* __first, CTexture* __last)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}

struct CTextureVector {
    CTexture* _M_start;                                 // +0x00
    CTexture* _M_finish;                                // +0x04
    CTexture* _M_end_of_storage;                        // +0x08
    CTexture* erase(CTexture* __first, CTexture* __last);
};

CTexture* CTextureVector::erase(CTexture* __first, CTexture* __last)
{
    CTexture* __i = copy(__last, _M_finish, __first);
    _Destroy(__i, _M_finish);
    _M_finish = __i;
    return __first;
}
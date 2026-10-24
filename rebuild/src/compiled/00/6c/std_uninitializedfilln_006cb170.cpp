// std::__uninitialized_fill_n(CEngineTextureMorph* first, unsigned int n, const CEngineTextureMorph& x, const __false_type&) -- STLport fill of n raw texture morph records with copies of x; returns the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB170, length 135 bytes (+ int3 padding), `__fastcall` (first in ecx,
// n in edx, x and the empty tag reference on the stack), `ret 8`. Single caller: vector<CEngineTextureMorph>::
// _M_insert_overflow 0x006CB390 (the `fill_len != 1` arm). The manifest label "Std_UninitCopyArray" is descriptive.
// Callees: malloc @ 0x00BFEA0E (the inline allocator), __uninitialized_copy(const CCompositeBlendDesc* first, last,
// result, const __false_type&) @ 0x005BAF28 (`__fastcall`; the vector copy constructor's const_pointer instantiation,
// distinct from the pointer one at 0x006CAC20).
// Layout: CEngineTextureMorph (Ego_r.pdb: OriginalTextureIndex +0, MorphID +4, vector<CCompositeBlendDesc> Blends +8;
// 20 bytes in retail); STLport vector {_M_start, _M_finish, _M_end_of_storage}.
// Shape: STLport 4.x verbatim, `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` from a /Gr TU: the placement new
// null-tests the slot, the implicit record copy is two dwords and the inline STLport vector copy constructor
// (_Vector_base(n, a) zeroes the three words, allocates, sets start/finish/end, then __uninitialized_copy of the
// blends); the count lives in the `push ecx` frame word and the inner tag is homed over the dead `x` argument slot.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CRGBColour {                                   // D3DCOLOR byte order
public:
    unsigned char B, G, R, A;
};

class CCompositeBlendDesc {                          // Ego_r.pdb, 16 bytes
public:
    unsigned long TextureIndex;                       // +0x00
    long          BlendType;                          // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                      // +0x08
    CRGBColour    SpecularColour;                     // +0x0C
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

// Same-TU body of the blends copy (0x005BAF28), noinline.
#pragma auto_inline(off)
__declspec(noinline) CCompositeBlendDesc* __fastcall __uninitialized_copy(const CCompositeBlendDesc* first,
    const CCompositeBlendDesc* last, CCompositeBlendDesc* result, const __false_type&)   // 0x005BAF28
{
    CCompositeBlendDesc* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
#pragma auto_inline(on)

template <class T> struct allocator {
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template <class T, class A> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    _Vector_base(unsigned int n) : _M_start(0), _M_finish(0)
    {
        _M_end_of_storage._M_data = 0;
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    vector(const vector& x) : _Vector_base<T, A>(x.size())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start,
                                               __false_type());
    }
};
}

class CEngineTextureMorph {                          // Ego_r.pdb; 20 bytes in retail
public:
    long                                  OriginalTextureIndex;   // +0x00
    long                                  MorphID;                // +0x04
    std::vector<CCompositeBlendDesc>      Blends;                 // +0x08
};

namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template CEngineTextureMorph* __fastcall __uninitialized_fill_n<CEngineTextureMorph*, unsigned int, CEngineTextureMorph>(
    CEngineTextureMorph*, unsigned int, const CEngineTextureMorph&, const __false_type&);
}
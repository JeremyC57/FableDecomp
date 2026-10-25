// std::vector::_M_insert_overflow(pair* position, const pair& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD growth of the hero-morph texture-layer map's vector<pair<long, CTCHeroMorph::CTextureLayer> >: allocate old + max(old, n), copy the head, place the n copies, copy the tail, free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721060, length 226 bytes (+ int3 padding), thiscall, `ret 14h`.
// Callers: 0x0071D76B / 0x0071DD05 (CTCHeroMorph::AddCompositeTexture 0x0071DBD0, landed), 0x00721782, and the landed
//   CVectorMap::Add 0x00721A50 (0x00721ABD) -- push_back's growth arm of CVectorMap<long, CTCHeroMorph::CTextureLayer>.
// Callees: malloc @ 0x00BFEA0E (inline allocate: `n != 0 ? malloc(n * 24) : 0`), __uninitialized_copy @ 0x00720270 (landed,
//   twice) and __uninitialized_fill_n @ 0x00720310 (landed; both __fastcall, first/n in ecx/edx), the blend descriptor's
//   copy constructor @ 0x005BA48A (the fill_len == 1 arm's inline pair copy), free @ 0x00BFEA14 (inline _M_clear).
// Label: catalog "std::vector_InsertRangeComplex" is descriptive.
// Element: pair<long, CTCHeroMorph::CTextureLayer {long TextureID; CCompositeBlendDesc Blend;}> (24 bytes, as the landed
//   0x00721A50 / 0x0071DBD0 model it); trivially destructible, so _Destroy is empty and _M_clear is only the free.
// Shape: STLport 4.6 verbatim (as the landed 0x00BEF830): the size by the 0x2AAAAAAB multiply, max by const reference into
//   the `push ecx` local, both __false_type temporaries homed in the atend slot (needs same-TU stand-in bodies of the
//   two helpers, below), _M_clear and _M_set inline.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CRGBColour {                                          // D3DCOLOR byte order
public:
    unsigned char B, G, R, A;
};
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
class CCompositeBlendDesc {                                 // Ego_r.pdb, 16 bytes
public:
    unsigned long       TextureIndex;                       // +0x00
    ECompositeBlendType BlendType;                          // +0x04
    CRGBColour          DiffuseColour;                      // +0x08
    CRGBColour          SpecularColour;                     // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);  // 0x005BA48A
};
class CTCHeroMorph {
public:
    class CTextureLayer {                                   // Ego_r.pdb, 20 bytes
    public:
        long                TextureID;                      // +0x00
        CCompositeBlendDesc Blend;                          // +0x04
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

// Same-TU stand-in bodies of the two out-of-line helpers (the STLport text of 0x00720270 / 0x00720310): VC7.1 learns
// that the empty tag is never read and homes both __false_type temporaries in the atend slot, as retail; with
// declarations only it gives them a byte of their own (`sub esp,8`, 230 v 226 bytes).
#pragma auto_inline(off)
template <class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class ForwardIter, class Size, class T>
__declspec(noinline) ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
#pragma auto_inline(on)

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline void _Destroy(T*, T*) { }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};
template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;      // +0x08
};
template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template <class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = this->_M_end_of_storage.allocate(len);
    T* new_finish = new_start;
    new_finish = __uninitialized_copy(this->_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
}

typedef std::pair<long, CTCHeroMorph::CTextureLayer> TTextureLayerEntry;
template void std::vector<TTextureLayerEntry>::_M_insert_overflow(TTextureLayerEntry*, const TTextureLayerEntry&,
    const std::__false_type&, unsigned int, bool);
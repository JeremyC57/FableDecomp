// std::vector<CCompositeBlendDesc>::push_back(const CCompositeBlendDesc& x) -- STLport push_back of a composite blend: copy it to the end, or grow the buffer (size + max(size, 1)) and copy everything over
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB010, length 179 bytes (+ int3 padding), thiscall `ret 4`. Callers:
// CTextureMorphs::AddBlend 0x006CA950 (landed; its model declares `void push_back(const CCompositeBlendDesc&); //
// 0x006CB010`) and CEngineInternalPrimitive::RebuildDecals 0x0070A570. The manifest label
// "CCompositeBlendDesc_VectorGrow" is descriptive.
// Callees: CCompositeBlendDesc copy constructor @ 0x005BA48A (out of line, both arms), malloc @ 0x00BFEA0E, STLport
// __uninitialized_copy(first, last, result, const __false_type&) @ 0x006CAC20 (`__fastcall`, landed), free @ 0x00BFEA14.
// Layout: CCompositeBlendDesc (Ego_r.pdb, 16 bytes: TextureIndex, BlendType, DiffuseColour, SpecularColour); STLport
// vector {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x verbatim -- push_back with _M_insert_overflow(_M_finish, x, __false_type(), 1, true) inlined: the
// fill length 1 and the old size are homed in the two frame words for `(max)(old_size, fill_len)` by reference, the
// `fill_len == 1` and `!atend` tests fold away, the trivially destructible _M_clear is the null-tested free, and
// _M_set stores end, start, finish. The empty tag has a user constructor and the out-of-line copy loop's body sits in
// the TU (noinline, under auto_inline(off)) so VC7.1 homes the tag over the live `x` argument slot (`lea edx,[esp+1Ch]`).
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

// Same-TU body of the out-of-line copy loop (the landed 0x006CAC20), noinline.
#pragma auto_inline(off)
__declspec(noinline) CCompositeBlendDesc* __fastcall __uninitialized_copy(CCompositeBlendDesc* first,
    CCompositeBlendDesc* last, CCompositeBlendDesc* result, const __false_type&)   // 0x006CAC20
{
    CCompositeBlendDesc* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CCompositeBlendDesc(*first);
    return cur;
}
#pragma auto_inline(on)

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> struct allocator {
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    typedef unsigned int size_type;
    size_type size() const { return size_type(_M_finish - _M_start); }
    void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_type fill_len, bool atend = false)
    {
        const size_type old_size = size();
        const size_type len = old_size + (max)(old_size, fill_len);
        T* new_start = _M_end_of_storage.allocate(len);
        T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) {
            _Construct(new_finish, x);
            ++new_finish;
        }
        if (!atend)
            new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
        _M_clear();
        _M_set(new_start, new_finish, new_start + len);
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

template void vector<CCompositeBlendDesc>::push_back(const CCompositeBlendDesc&);
}
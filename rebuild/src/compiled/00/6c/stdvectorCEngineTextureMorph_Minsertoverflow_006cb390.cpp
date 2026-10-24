// std::vector<CEngineTextureMorph>::_M_insert_overflow(CEngineTextureMorph* position, const CEngineTextureMorph& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport grow-and-insert for the 20-byte texture morph records
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB390, length 218 bytes (+ int3 padding), thiscall `ret 14h`. Callers:
// CTextureMorphs::AddMorph 0x006CA8C0 (push_back, catalog "Map_InsertEntry") and 0x006CB470 (catalog
// "vector<pair<int,CDefPtr>>::emplace_back", another push_back of the same vector). The manifest label
// "Array_Insert_range" is descriptive.
// Callees: malloc @ 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) @ 0x006CB0D0
// (`__fastcall`, x2), vector<CCompositeBlendDesc>'s copy constructor @ 0x005BB346 (out of line), __uninitialized_fill_n(
// first, n, x, const __false_type&) @ 0x006CB170 (`__fastcall`), _M_clear() @ 0x006CB340 (landed as
// CRetailVector_006cb340::Destroy: frees each record's Blends buffer, then the old buffer).
// Layout: CEngineTextureMorph (Ego_r.pdb: OriginalTextureIndex +0, MorphID +4, vector<CCompositeBlendDesc> Blends +8;
// 20 bytes in retail, the STLport vector being 12); STLport vector {_M_start, _M_finish, _M_end_of_storage}.
// Shape: STLport 4.x _vector.c verbatim: len = old_size + (max)(old_size, fill_len) with the old size homed in the
// `push ecx` frame word (the /20 is the 0x66666667 multiply), allocate, copy the prefix, construct one copy (the
// implicit record copy: two dwords and the out-of-line vector copy) or fill n, copy the suffix unless atend, _M_clear,
// _M_set. The fresh `__false_type()` tags have a user constructor and, with the two loop bodies in the TU (noinline,
// under auto_inline(off)), are homed over the live `atend` slot.
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
};

namespace std {
struct __false_type { __false_type() { } };

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
    vector(const vector& x);                          // 0x005BB346 for vector<CCompositeBlendDesc>
    size_type size() const { return size_type(_M_finish - _M_start); }
    void _M_clear();                                  // 0x006CB340 for vector<CEngineTextureMorph>
    void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_type fill_len, bool atend = false);
};
}

class CEngineTextureMorph {                          // Ego_r.pdb; 20 bytes in retail
public:
    long                                  OriginalTextureIndex;   // +0x00
    long                                  MorphID;                // +0x04
    std::vector<CCompositeBlendDesc>      Blends;                 // +0x08
};

namespace std {
// Same-TU bodies of the two out-of-line loops, noinline: only then does VC7.1 see that the empty tags are never read
// and home them over the live `atend` slot (`lea ecx,[esp+28h]`) instead of a frame word.
#pragma auto_inline(off)
__declspec(noinline) CEngineTextureMorph* __fastcall __uninitialized_copy(CEngineTextureMorph* first,
    CEngineTextureMorph* last, CEngineTextureMorph* result, const __false_type&)          // 0x006CB0D0
{
    CEngineTextureMorph* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}

__declspec(noinline) CEngineTextureMorph* __fastcall __uninitialized_fill_n(CEngineTextureMorph* first,
    unsigned int n, const CEngineTextureMorph& x, const __false_type&)                     // 0x006CB170
{
    CEngineTextureMorph* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(cur, x);
    return cur;
}
#pragma auto_inline(on)

template <class T, class A>
void vector<T, A>::_M_insert_overflow(T* position, const T& x, const __false_type&, size_type fill_len, bool atend)
{
    const size_type old_size = size();
    const size_type len = old_size + (max)(old_size, fill_len);
    T* new_start = _M_end_of_storage.allocate(len);
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    } else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template void vector<CEngineTextureMorph>::_M_insert_overflow(CEngineTextureMorph*, const CEngineTextureMorph&,
    const __false_type&, unsigned int, bool);
}
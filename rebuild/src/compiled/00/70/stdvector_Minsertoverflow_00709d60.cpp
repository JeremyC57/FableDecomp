// std::vector::_M_insert_overflow(SMeshMorphPair* position, const SMeshMorphPair& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the (mesh info, texture morph set) pair vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00709D60, 249 bytes, `ret 0x14`.
// Label correction: catalog "Vector_Reallocate_CMusicSetEntry" -> STLport 4.x
//   vector<pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> >::_M_insert_overflow, reached from the inline
//   push_back in CTCHeroAttachableAppearanceModifiers::GetMeshesAndMorphSets (the callers of the landed
//   __uninitialized_copy 0x00709C90 name the element; 24 bytes: the 0x2AAAAAAB / `sar edx,2` divide).
// Element: CMeshInfo (Index, SizeX, PrimaryMesh:1; three dwords) + CEngineTextureMorphSet (a CArray<CEngineTextureMorph>
//   = retail 12-byte STLport vector with an out-of-line copy constructor 0x005BBDF1).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x00709C90, __uninitialized_fill_n 0x00709CE0,
//   CArray copy constructor 0x005BBDF1, the range destroy 0x005BC29E (an identical-COMDAT-folded copy from the /Os
//   module).
// Shape: STLport `_M_insert_overflow` (see 0x006FC6A0): len = size + max(size, fill_len) via the const-ref `max`,
//   allocate, copy the head, construct one element in place (pair's memberwise copy: three dwords, then the morph array
//   copy constructor) or fill_n, copy the tail unless atend, _M_clear, _M_set. The empty `__false_type` tags are homed
//   in the high argument slot; the tag-taking callees are same-TU noinline stand-ins.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace NGraphicAppearance {
class CMeshInfo {                              // Ego_r.pdb, 12 bytes
public:
    long  Index;                               // +0x00
    float SizeX;                               // +0x04
    unsigned char PrimaryMesh : 1;             // +0x08
};
}
class CEngineTextureMorph;
class CEngineTextureMorphArray {               // CArray<CEngineTextureMorph> (retail STLport vector, 12 bytes)
public:
    CEngineTextureMorph* _M_start;
    CEngineTextureMorph* _M_finish;
    CEngineTextureMorph* _M_end_of_storage;
    CEngineTextureMorphArray(const CEngineTextureMorphArray& other);   // 0x005BBDF1
    ~CEngineTextureMorphArray() { if (_M_start) free(_M_start); }
};
class CEngineTextureMorphSet {                 // Ego_r.pdb: Morphs +0
public:
    CEngineTextureMorphArray Morphs;
};

namespace std {

struct __false_type { __false_type() {} };

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) {}
};

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x00709C90, 0x00709CE0, 0x005BC29E).
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    for (; n > 0; --n, ++first)
        _Construct(first, x);
    return first;
}
template<class T> __declspec(noinline) void __fastcall __destroy_aux(T* first, T* last, const __false_type&)
{
    for (; first != last; ++first)
        first->~T();
}
#pragma auto_inline(on)

template<class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    Alloc _M_alloc() const { return Alloc(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_alloc().deallocate(_M_start, (unsigned int)(_M_end_of_storage - _M_start));
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template<class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    T* new_start = _M_alloc().allocate(len);
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> >;

}
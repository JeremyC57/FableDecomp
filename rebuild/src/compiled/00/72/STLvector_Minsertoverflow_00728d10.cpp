// _STL::vector::_M_insert_overflow(CMeshMorphSetParts* position, const CMeshMorphSetParts& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD growth of a mesh morph set's part list (vector<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts>): allocate old + max(old, n), copy the head, place the n copies, copy the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728D10, length 216 bytes (+ int3 padding), thiscall, `ret 14h`.
// Callers: the same vector's _M_fill_insert 0x00728DF0 (0x00728EFE, the no-room arm; reached from resize 0x00728F20).
// Callees: malloc @ 0x00BFEA0E (inline allocate: `n != 0 ? malloc(n * 16) : 0`), __uninitialized_copy @ 0x00728B10
//   (landed, twice), __uninitialized_fill_n @ 0x00728A40 (landed), CArray<CEngineTextureMorph>::CArray(const CArray&)
//   @ 0x005BBDF1 (landed; the fill_len == 1 arm's inline element copy), __destroy_aux @ 0x00722130 (landed; _M_clear's
//   _Destroy of the old range), free @ 0x00BFEA14. The three helpers are __fastcall (first/n in ecx, last in edx).
// Label: catalog "std::vector_InsertAndGrow_Pair" is descriptive.
// Element (16 bytes, `sar eax,4`): {unsigned long key; CArray of 20-byte records} with the record {long, long, vector of
//   16-byte PODs} (the landed destroy 0x00722130 and copy 0x005BBDF1). That is Ego_r.pdb's CTCAppearanceMorphBase::
//   CMeshMorphSet::CMeshMorphSetParts {ulong MeshIndex; CEngineTextureMorphSet TextureMorphs;} with CEngineTextureMorphSet
//   {CArray<CEngineTextureMorph> Morphs;} and CEngineTextureMorph {long OriginalTextureIndex; long MorphID;
//   vector<CCompositeBlendDesc> Blends;} at the retail 12-byte vector (the PDB's 20 / 16 / 24 bytes); the bodies sit in
//   the CTCRandomAppearanceMorph (a CTCAppearanceMorphBase) neighbourhood.
// Shape: STLport 4.6 verbatim, as 0x00721060 of this chunk (max by const reference into the `push ecx` local, the tag
//   temporaries homed in the atend slot through same-TU stand-in bodies of the out-of-line helpers, _M_clear / _M_set
//   inline), here with a non-trivial _Destroy: the out-of-line __destroy_aux(first, last, __false_type) before the free.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CEngineTextureMorph;
template <class T> class CArray {                           // retail: an STLport vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray(const CArray<T>& other);                         // 0x005BBDF1 for T = CEngineTextureMorph
    ~CArray();                                              // 0x005BC0D3
};
class CEngineTextureMorphSet {                              // Ego_r.pdb (retail 12 bytes)
public:
    CArray<CEngineTextureMorph> Morphs;                     // +0x00
};
class CTCAppearanceMorphBase {
public:
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts {                          // Ego_r.pdb (retail 16 bytes)
        public:
            unsigned long          MeshIndex;               // +0x00
            CEngineTextureMorphSet TextureMorphs;           // +0x04
        };
    };
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

// Same-TU stand-in bodies of the out-of-line helpers (the STLport text of 0x00728B10 / 0x00728A40 / 0x00722130): VC7.1
// learns that the empty tags are never read and homes the __false_type temporaries in the atend slot, as retail.
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
template <class ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
#pragma auto_inline(on)
template <class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

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

typedef CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts TMeshMorphSetParts;
template void _STL::vector<TMeshMorphSetParts>::_M_insert_overflow(TMeshMorphSetParts*, const TMeshMorphSetParts&,
    const _STL::__false_type&, unsigned int, bool);
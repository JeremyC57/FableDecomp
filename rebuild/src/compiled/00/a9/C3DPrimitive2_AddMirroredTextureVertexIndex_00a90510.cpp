// C3DPrimitive2::AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count) -- append the given vertex indices to the primitive's mirrored-texture index list
// Leaf: ?AddMirroredTextureVertexIndex@C3DPrimitive2@@AAEXPBKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A90510, 75 bytes, `ret 8`. Ego_r.pdb
//   `?AddMirroredTextureVertexIndex@C3DPrimitive2@@AAEXPBKK@Z` (private); its caller 0x00A92476 (the C3DPrimitive2
//   loader) passes CMirroredTextureHighlightsChunk::GetList() (0x00AC4F30) and GetCount() (0x00AC4F20).
// Callee: std::vector<unsigned long>::_M_insert_overflow(unsigned long*, const unsigned long&, const __true_type&,
//   unsigned int, bool) @ 0x00464ACE (the binding of the landed callers' std spelling).
// Layout: Ego_r.pdb C3DPrimitive2 with retail 12-byte CArrays (ClothPrimitives +0x20, PartitionedMeshSourceIndexBuffer
//   +0x80): MirroredTextureVertexIndexList at +0x94 (Ego_r +156). engine has no C3DPrimitive2 header.
// Shape: STLport 4.6 push_back inline (placement copy with its null test, else the overflow with a __true_type tag homed in
//   the dead `count` slot); the indexed loop becomes a down-counter (ebx) over a stepping pointer (edi).
inline void* operator new(unsigned int, void* __p) { return __p; }

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
template <class _Tp> struct __type_traits { typedef __true_type is_POD_type; };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef typename __type_traits<_Tp>::is_POD_type _IsPODType;
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend);   // 0x00464ACE
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};
}

template <class T> class CArray : public std::vector<T> { };

class C3DPrimitive2 {
public:
    unsigned char         _pad00[0x94];                 // vptr (CMovableResource), MaterialIndex .. PartitionTree
    CArray<unsigned long> MirroredTextureVertexIndexList;   // +0x94
private:
    void AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count);
};

void C3DPrimitive2::AddMirroredTextureVertexIndex(const unsigned long* indices, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        MirroredTextureVertexIndexList.push_back(indices[i]);
}
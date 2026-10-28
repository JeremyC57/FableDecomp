// std::vector::push_back(const CBodyPartMesh& x) -- (vector<CRandomAppearanceMorph::CBodyPartMesh>) STLport push_back of a 24-byte body-part mesh (mesh index, priority, texture-morph vector map)
// Leaf: ?push_back@?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086E880, 87 bytes, thiscall, `ret 4`.
// Identity: the manifest's "PushBack_CEntry_24byte" is descriptive. Ego_r.pdb CRandomAppearanceMorph::CBodyPartMesh is
//   { long MeshIndex; long Priority; CVectorMap<unsigned long, CTextureMorph> TextureMorphs; } (24 bytes in retail:
//   the CVectorMap is its 12-byte vector base, the empty Compare +0xC and Dirty +0xD); CRandomAppearanceMorph::BodyParts
//   is CArray<CArray<CBodyPartMesh> >, and the only caller 0x0086BE16 (landed 0x0086BDE0) pushes
//   CBodyPartMesh(mesh, priority) onto BodyParts[layer].
// Callees: vector<pair<unsigned long, CTextureMorph> >::vector(const vector&) 0x00673E10 (the map's base copy),
//   vector<CBodyPartMesh>::_M_insert_overflow(pos, x, __false_type, 1, true) 0x0086E3B0.
// Shape: STLport 4.6 push_back; the implicit CBodyPartMesh copy constructor copies the two longs, then the CVectorMap's
//   implicit one: the out-of-line vector copy (source/destination +8 kept in esi/edi), nothing for the empty
//   comparator, then the Dirty byte. The overflow's same-TU noinline stand-in homes the tag in x's argument slot.
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector(const vector<_Tp, _Alloc>& __x);                 // out of line (0x00673E10 for the texture-morph map)
    protected:
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                 unsigned int __fill_len, bool __atend);
    public:
    void push_back(const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
}

// Same-TU stand-in for the out-of-line overflow 0x0086E3B0 (its own row): never reads the tag.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (&__x != 0);
}
#pragma auto_inline(on)

} // namespace std

template <class K, class V> class CKeyPairCompareLess {
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                           // +0x0C (empty)
    bool Dirty;                                             // +0x0D
};

class CRandomAppearanceMorph {
public:
    class CTextureMorph {
    public:
        unsigned long NewTexture;                           // +0x00
        unsigned long GroupID;                              // +0x04
    };
    class CBodyPartMesh {
    public:
        long                                       MeshIndex;       // +0x00
        long                                       Priority;        // +0x04
        CVectorMap<unsigned long, CTextureMorph>   TextureMorphs;   // +0x08
    };
};

template void std::vector<CRandomAppearanceMorph::CBodyPartMesh>::push_back(const CRandomAppearanceMorph::CBodyPartMesh&);
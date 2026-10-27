// CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc::CSeaBodyDesc() -- the record's default constructor: an empty name, then the empty array
// Leaf: ??0CSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00515620, 23 bytes, thiscall, plain `ret`.
// Identity (probable): unreferenced (inlined at its uses); Ego_r.pdb/FableWin.pdb ??0CSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@QAE@XZ is the only {CCharString, CArray<...>} record with a default constructor symbol in the CWorldMap static-map code this sits in (0x00515620, among CWorldMap's STLport instantiations; the manifest's propagated labels name CSeaBodyDesc in this TU); the other such records (CStringParserComment, C3DSkeletalMorphTarget 0x00A44E60) live in the engine.
// Callees: CCharString::CCharString() 0x0099E4B0.
// Shape (family M109, representative 0x00515620): CCharString's default constructor out of line, then the CArray (an STLport
//   vector, the landed `CArray<T> : public std::vector<T>` model) zeroes its three pointers at +0x4 (one `xor eax,eax`).
class CCharString {
public:
    CCharString();                                        // 0x0099E4B0
    void* PStringData;
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}
template <class T> class CArray : public std::vector<T> { };
class CStaticMapGenerationVisibiilityInfo { public:
    struct CSeaBodyDesc {                                 // Ego_r.pdb, 20 bytes (Dinkumware CArray)
        CSeaBodyDesc() { }
        CCharString Identifier;                           // +0x0
        CArray<long> Maps;                                // +0x4
    };
};
// the implicit-in-retail constructor, forced out of line for this object (not part of the retail body)
CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc* ConstructOne(void* memory);
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc* ConstructOne(void* memory) { return new (memory) CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc; }
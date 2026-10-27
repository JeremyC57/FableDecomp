// C3DSkeletalMorphTarget::C3DSkeletalMorphTarget() -- the record's default constructor: an empty name, then the empty array
// Leaf: ??0C3DSkeletalMorphTarget@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A44E60, 23 bytes, thiscall, plain `ret`.
// Identity (proven): bound ??0C3DSkeletalMorphTarget@@QAE@XZ (Ego_r.pdb/FableWin.pdb symbol) from its callers CSkeletalMorphResourceManager::GetMorphTarget 0x006C3620 and PreloadBoneConfigs 0x006C37D0; Ego_r.pdb C3DSkeletalMorphTarget is {CCharString Name, CArray<C3DSkeletalBoneMorph> BoneMorphs}.
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
class C3DSkeletalBoneMorph;
class C3DSkeletalMorphTarget {                           // Ego_r.pdb, 20 bytes (Dinkumware CArray)
public:
    C3DSkeletalMorphTarget() { }
    CCharString Name;                                     // +0x0
    CArray<C3DSkeletalBoneMorph> BoneMorphs;              // +0x4
};
// the implicit-in-retail constructor, forced out of line for this object (not part of the retail body)
C3DSkeletalMorphTarget* ConstructOne(void* memory);
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
C3DSkeletalMorphTarget* ConstructOne(void* memory) { return new (memory) C3DSkeletalMorphTarget; }
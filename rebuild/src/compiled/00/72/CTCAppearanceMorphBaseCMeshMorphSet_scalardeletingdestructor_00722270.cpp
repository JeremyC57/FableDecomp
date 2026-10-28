// CTCAppearanceMorphBase::CMeshMorphSet::`scalar deleting destructor'(unsigned int flags) -- destroys the mesh morph set (original texture morphs, then the mesh-part array) and frees it when bit 0 of flags is set
// Leaf: ??_GCMeshMorphSet@CTCAppearanceMorphBase@@QAEPAXI@Z
// Retail: Fable.exe @ 0x00722270, 63 bytes, thiscall `ret 4`; no vtable store (a non-polymorphic class).
// Identity (probable): the body is member-for-member the CMeshMorphSet destruction the landed
//   CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>::DeleteData 0x007222B0 (next row) inlines: ~CEngineTextureMorphSet
//   0x005BC0D3 on OriginalMeshTextureMorphs (+0x0c), then the MeshParts vector (+0x00) through the out-of-line
//   __destroy_aux 0x00722130 over CMeshMorphSetParts (that row's landed binding) and free 0x00BFEA14, then
//   operator delete 0x00BFE9BC. Ego_r.pdb has ??1CMeshMorphSet@CTCAppearanceMorphBase@@QAE@XZ (a user destructor); VC7.1
//   emits the non-virtual ??_G on a `delete` (GOTCHAS wave 24m). No caller or reference survives in retail, so the class
//   rests on the layout and the callees alone. The catalog label "CActiveFile::~CActiveFile" is wrong.
// Shape: the __false_type tag temporary of _Destroy is homed in the dead flags argument slot (`lea eax,[esp+8]`); that
//   needs the same-TU noinline __destroy_aux body below (declaration-only gives a new frame slot, `push ecx ; lea [esp+7]`).
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

namespace std {
struct __false_type { __false_type() {} };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00722130
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    __forceinline ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

template <class T> class CArray : public std::vector<T> { };
class CEngineTextureMorph;
class CEngineTextureMorphSet { public:                    // retail 12 bytes: CArray<CEngineTextureMorph> Morphs
    CEngineTextureMorph* _M_start;
    CEngineTextureMorph* _M_finish;
    CEngineTextureMorph* _M_end_of_storage;
    ~CEngineTextureMorphSet();                             // 0x005BC0D3
};

class CTCAppearanceMorphBase { public:
    class CMeshMorphSet { public:                          // retail 0x18 (Ego_r.pdb 0x20)
        class CMeshMorphSetParts { public:                 // retail 0x10
            unsigned long          MeshIndex;              // +0x00
            CEngineTextureMorphSet TextureMorphs;          // +0x04
        };
        CArray<CMeshMorphSetParts> MeshParts;              // +0x00
        CEngineTextureMorphSet     OriginalMeshTextureMorphs;   // +0x0C
        __forceinline ~CMeshMorphSet() { }
    };
};

// Same-TU stand-in body for the out-of-line __destroy_aux (GOTCHAS chunk 512: a tag-taking helper with a noinline body
// that never reads its tag lets VC7.1 home the tag temporary in the dead flags argument slot, as retail does).
namespace std {
#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        __first->~CMeshMorphSetParts();
}
#pragma auto_inline(on)
}

// A `delete` of the class makes VC7.1 emit its scalar deleting destructor (the graded leaf).
void DeleteMeshMorphSet_00722270(CTCAppearanceMorphBase::CMeshMorphSet* set) { delete set; }
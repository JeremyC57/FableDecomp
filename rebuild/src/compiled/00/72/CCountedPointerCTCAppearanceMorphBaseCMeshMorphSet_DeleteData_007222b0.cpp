// CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the mesh morph set (original texture morphs, mesh parts) and free it
// Leaf: ?DeleteData@?$CCountedPointer@VCMeshMorphSet@CTCAppearanceMorphBase@@@@SIXPAX@Z
// Retail: Fable.exe @ 0x007222B0, 58 bytes, static __fastcall (data in ecx), `ret`.
// Identity (probable): Ego_r.pdb names `?DeleteData@?$CCountedPointer@VCMeshMorphSet@CTCAppearanceMorphBase@@@@SIXPAX@Z`
//   (with its Reset); the body is the `delete (T*)data` deleter shape (landed CCountedPointer<CCardinalSpline>::DeleteData
//   0x0075F830) stored as the count block's deleter at 0x00722340 (the Reset beside it), and its element destructor is
//   the out-of-line __destroy_aux over CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts 0x00722130 (that row's
//   own Ego_r reloc binding). Layout (Ego_r 0x20 with 16-byte vectors; retail 0x18): CArray<CMeshMorphSetParts>
//   MeshParts +0 (16-byte retail parts: MeshIndex + CEngineTextureMorphSet), CEngineTextureMorphSet
//   OriginalMeshTextureMorphs +0xC (its out-of-line destructor 0x005BC0D3). Members destroy in reverse order.
// Callees: ~CEngineTextureMorphSet 0x005BC0D3, __destroy_aux 0x00722130, free 0x00BFEA14, operator delete 0x00BFE9BC.
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
    };
};

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>;
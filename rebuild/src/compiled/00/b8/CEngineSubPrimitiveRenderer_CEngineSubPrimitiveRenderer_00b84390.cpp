// CEngineSubPrimitiveRenderer::~CEngineSubPrimitiveRenderer() -- sub-primitive renderer destructor: restore both vtables, clear the global frame-memory-pool pointer, then the members (SortBuffer, the 15 sub-primitive lists in reverse, each freeing its element buffer) and the CEngineComponent / CBaseClassNonCopyable bases
// Leaf: ??1CEngineSubPrimitiveRenderer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84390, 104 bytes, thiscall, tail `jmp ??1CBaseClassNonCopyable@@MAE@XZ` (0x0099A300).
// Identity (proven): stores vtables 0x012A3A60 / 0x012A3A28 (RTTI .?AVCEngineSubPrimitiveRenderer@@ COL offsets 0 / 4: bases
//   CBaseClassNonCopyable : CBaseClass, CEngineComponent at mdisp 4); sole caller is the slot-0 scalar deleting destructor 0x00B866A0
//   (call at 0x00B866A3); Ego_r.pdb ??1CEngineSubPrimitiveRenderer@@UAE@XZ, whose donor order RenderListTwoPass / ??1 / DeleteList /
//   ??0 equals the retail order 0x00B84320 / 0x00B84390 / 0x00B84400 / 0x00B84470. The catalog label
//   CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch is propagated from a masked twin.
// Callees: free 0x00BFEA14 (STLport allocator, 16 buffers), ~CEngineComponent 0x00B59730 (this+4), ~CBaseClassNonCopyable 0x0099A300.
// Globals: 0x0143BAC4, a CSimpleMemoryPool* the constructor 0x00B84470 sets to &GEngine->FrameMemoryPool (Ego_r CEngine +0x138,
//   retail +0x130) and the fastcall allocator 0x00BEB540 (39 sub-primitive callers) passes to CSimpleMemoryPool::Allocate 0x00A5C910;
//   its data symbol is not in the PDB name lists, so it carries a descriptive name.
// Layout: CEngineComponent re-declared over its real vtable (engine/CEngineComponent.h is the flattened donor layout with a
//   `void* __vftable` word; same members GlobalPointerAddress +0x04, RenderEnabled +0x08, KeepAlive +0x09).
// Layout: Ego_r CEngineSubPrimitiveRenderer at STLport sizes (engine/CEngineSubPrimitiveRenderer.h is the flattened donor layout with a
//   `void* __vftable` word and byte-array members, sizeof 0x190): CEngineComponent +0x04, MaxSortDistance +0x10, SortDistanceMultiplier
//   +0x14, SubPrimitiveLists CSubPrimitiveList[15] +0x18 (0x18 each: CArray<CSortElement> Elements, RenderPass, PreRenderPass, four
//   bools; Ego_r ??0/??1CSubPrimitiveList@CEngineSubPrimitiveRenderer@@), SortBuffer +0x180, EnableSolidPass +0x18C, EnableAlphaPass +0x18D.
// Shape: the user destructor clears the global; the members' implicit destructors follow (SortBuffer's STLport ~vector = `if
//   (_M_start) free`, then VC7.1's counted reverse loop over the array -- ebp = 15, esi stepping back 0x18 -- freeing each
//   list's buffer), then the CEngineComponent base through ebx (= this+4, computed for the vtable store) and the primary base.
#include "engine/CBaseClassNonCopyable.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector
class CEngineComponent {
public:
    CEngineComponent();                                      // 0x00B59710
    virtual ~CEngineComponent();                             // 0x00B59730 ??1CEngineComponent@@UAE@XZ
    void**        GlobalPointerAddress;                      // +0x04
    bool          RenderEnabled;                             // +0x08
    unsigned char KeepAlive;                                 // +0x09
};
class CEngineSubPrimitiveBase;
class CSimpleMemoryPool;
enum ERenderPassType { RENDER_PASS_TYPE_00b84390 };
extern CSimpleMemoryPool* PFrameMemoryPool_0143bac4;         // 0x0143BAC4 (= &GEngine->FrameMemoryPool, set by the constructor)
class CEngineSubPrimitiveRenderer : public CBaseClassNonCopyable, public CEngineComponent {
public:
    class CSortElement {
    public:
        unsigned long            SortValue;                  // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;               // +0x04
    };
    class CSubPrimitiveList {
    public:
        CArray<CSortElement> Elements;                       // +0x00
        ERenderPassType      RenderPass;                     // +0x0c
        ERenderPassType      PreRenderPass;                  // +0x10
        bool                 BoundingVolumePass;             // +0x14
        bool                 RequiresStableSort;             // +0x15
        bool                 Sorted;                         // +0x16
        bool                 Enable;                         // +0x17
    };
    virtual ~CEngineSubPrimitiveRenderer();
    float                MaxSortDistance;                    // +0x10
    float                SortDistanceMultiplier;             // +0x14
    CSubPrimitiveList    SubPrimitiveLists[15];              // +0x18
    CArray<CSortElement> SortBuffer;                         // +0x180
    bool                 EnableSolidPass;                    // +0x18c
    bool                 EnableAlphaPass;                    // +0x18d
};
CEngineSubPrimitiveRenderer::~CEngineSubPrimitiveRenderer()
{
    PFrameMemoryPool_0143bac4 = 0;
}
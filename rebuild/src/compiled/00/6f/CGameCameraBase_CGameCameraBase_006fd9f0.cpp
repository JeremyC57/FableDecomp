// CGameCameraBase::~CGameCameraBase() -- destroy the game camera base: own vtable, the two STLport coordinate lists (LocalThingPosList +0xac, then DrawCoordList +0xa8) out of line, then the CAIGameCameraBase base's vtable
// Leaf: ??1CGameCameraBase@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FD9F0, 39 bytes (+9 int3), __fastcall, `ret`.
// Identity: FableWin.pdb ??1CGameCameraBase@@UAE@XZ (and ??1CAIGameCameraBase@@UAE@XZ: both destructors are virtual).
//   Vtable 0x01264A8C is RTTI CGameCameraBase and 0x0125C1F4 CAIGameCameraBase (rtti-vtables.tsv; the landed ctor
//   0x006FD8C0 and ??_G 0x006FE780 store the same pair -- the ??_G inlines this body). Label correction: the catalog's
//   "~CPredicate_And<...CIsThingAlive...>" is propagated.
// Callees: std::list<C3DVector>::~list @ 0x0048CA60 (out of line, x2: the folded POD-list body that frees every node and
//   then the sentinel; the catalog's "CGuiControlTreePane::SortTreeRecursively" is propagated).
// Layout: engine/CGameCameraBase.h is the Ego_r layout; retail matches it up to +0xa8 and then has 4-byte STLport lists
//   (DrawCoordList +0xa8, LocalThingPosList +0xac, see the landed ctor), so `this` is a local model with the camera
//   block before the lists left opaque (only the lists are destroyed here).
// Shape: a user-declared out-of-line virtual destructor: own vtable store, members in reverse declaration order, then the
//   base's inline user destructor stores its vtable.
// Name fix (agentNF6 2026-11-10): 0x0048CA60 is STLport's ??1?$_List_base@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@XZ (STLport 4.6 list has no user dtor; its implicit dtor calls the _List_base dtor).
#include "engine/C3DVector.h"

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc> class _List_base {    // STLport 4.6 _list.h
public:
    void* _M_node;                                       // _STLP_alloc_proxy over the sentinel node pointer
    ~_List_base();                                       // 0x0048CA60 for list<C3DVector>
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };   // STLport: no user dtor
}

class CAIGameCameraBase {                                // vtable 0x0125C1F4
public:
    virtual ~CAIGameCameraBase() { }
};

class CGameCameraBase : public CAIGameCameraBase {       // vtable 0x01264A8C
public:
    unsigned char          _camera[0xa4];                // +0x04 InternalCam, LastInternalCam, PointingAt, World, orbit
    std::list<C3DVector>  DrawCoordList;                // +0xa8
    std::list<C3DVector>  LocalThingPosList;            // +0xac
    virtual ~CGameCameraBase();
};

CGameCameraBase::~CGameCameraBase()
{
}
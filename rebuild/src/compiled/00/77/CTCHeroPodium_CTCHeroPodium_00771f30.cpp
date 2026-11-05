// CTCHeroPodium::~CTCHeroPodium() -- implicit component destructor: destroy the CrowdOccupation vector<long> (free its block), then ~CTCBase
// Leaf: ??1CTCHeroPodium@@UAE@XZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00771F30, 27 bytes: `push esi ; mov esi,ecx ; mov eax,[esi+10h] ; test ; je ;
//   push eax ; call free ; add esp,4 ; mov ecx,esi ; pop esi ; jmp ~CTCBase`.
// Identity: the only caller is CTCHeroPodium's scalar deleting destructor 0x00771F10 (call at 0x00771F13), slot 0 of the
//   CTCHeroPodium vtable 0x0126A89C (RTTI .?AVCTCHeroPodium@@; slot 8 0x00771F00, slot 21 GetInterfaceType 0x00771ED0,
//   slot 22 GetClassTypeName 0x00771EE0, all landed as CTCHeroPodium members). Ego_r.pdb lists CTCHeroPodium's Alloc, ctor,
//   OnInitialActivate, GetPositionForMember, GetCrowdPos, FrameUpdate, GetInterfaceType, GetClassTypeName and NO
//   destructor: the destructor is compiler-generated, hence no vtable store (VC7.1 drops it in an implicit dtor) and a
//   straight tail jump to CTCBase::~CTCBase 0x00686830. The manifest label "CTCInventoryItem::OnDie" is propagated from
//   the byte-identical 0x00529CC0 (which tail-jumps to ~CBaseClass 0x0099A2E0, a different class).
// Callees: free 0x00BFEA14 (STLport allocator<long>::deallocate under the malloc allocator), CTCBase::~CTCBase 0x00686830.
// Layout: Ego_r.pdb CTCHeroPodium : CTCBase (0x0C) with NumberOfCrowdOccupants +0x0C, vector<long> CrowdOccupation +0x10,
//   C3DVector E_X / E_Y; retail's STLport vector is 12 bytes, so E_X / E_Y sit at +0x1C / +0x28 (the landed ctor 0x00771DB0).
//   engine/CTCBase.h flattens the vptr into a data member, so the polymorphic base chain is the landed CTCChest model
//   (0x00750850): CBaseClass (engine header) -> CBaseClassNonCopyable -> CTCBase.
// Shape: the class declares no destructor; ~vector<long> (trivial _Destroy) and ~_Vector_base (`if (_M_start)
//   deallocate`) inline into it. The constructor is defined here (as the CTCHeroPodium TU does) so the vftable, the scalar
//   deleting destructor and with them the implicit destructor are emitted.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CThing;

class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
};

#include "engine/C3DVector.h"

class CTCHeroPodium : public CTCBase { public:              // vtable 0x0126A89C
    long              NumberOfCrowdOccupants;               // +0x0C
    std::vector<long> CrowdOccupation;                      // +0x10
    C3DVector         E_X;                                  // +0x1C
    C3DVector         E_Y;                                  // +0x28
    CTCHeroPodium(CThing& parent);                          // 0x00771DB0
};

CTCHeroPodium::CTCHeroPodium(CThing& parent)
    : CTCBase(parent), NumberOfCrowdOccupants(0)
{
}
// CTCHitOnCollision::~CTCHitOnCollision() -- compiler-generated destructor: free the CollidedList vector block, then ~CTCBase
// Leaf: ??1CTCHitOnCollision@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C8560, 27 bytes, thiscall, tail jump to CTCBase::~CTCBase 0x00686830.
// Identity (proven): the sole caller 0x007C8543 is the scalar deleting destructor 0x007C8540, slot 0 of vtable 0x0126E7A4
//   (rtti-vtables.tsv, RTTI .?AVCTCHitOnCollision@@ : CTCBase : CBaseClassNonCopyable : CBaseClass). No vtable store inside:
//   the destructor is compiler-generated (Ego_r.pdb ??1CTCHitOnCollision@@UAE@XZ). The member freed (+0x0C) is Ego_r
//   CTCHitOnCollision::CollidedList (vector<CThingGameObject*>). The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate), CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: Ego_r.pdb CTCHitOnCollision (0x24 donor, retail 0x20 with the 12-byte STLport vector): CollidedList +0x0C,
//   DontHitWhenCombatAction +0x18, Blockable +0x19, PassThroughIfBlocked +0x1A, PushSideways +0x1B, Damage +0x1C.
//   engine/CTCBase.h is a flattened record (vptr as data), so the polymorphic base is a local model.
// TU: the PDB constructor ??0CTCHitOnCollision@@AAE@AAVCThing@@@Z is defined with an empty body only to emit the vtable, the
//   ??_G and this implicit ??1 (not graded; retail's body is not reconstructed).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CThingGameObject;

class CTCBase : public CBaseClassNonCopyable {              // retail 0x0C bytes
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCHitOnCollision : public CTCBase {                  // vtable 0x0126E7A4
    CTCHitOnCollision(CThing& thing);
public:
    std::vector<CThingGameObject*>  CollidedList;           // +0x0C
    bool                            DontHitWhenCombatAction;// +0x18
    bool                            Blockable;              // +0x19
    bool                            PassThroughIfBlocked;   // +0x1A
    bool                            PushSideways;           // +0x1B
    float                           Damage;                 // +0x1C
};

CTCHitOnCollision::CTCHitOnCollision(CThing& thing)
    : CTCBase(thing)
{
}
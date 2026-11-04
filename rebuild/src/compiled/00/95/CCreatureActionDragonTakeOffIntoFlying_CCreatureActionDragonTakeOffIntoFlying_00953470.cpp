// CCreatureAction_DragonTakeOffIntoFlying::~CCreatureAction_DragonTakeOffIntoFlying() -- implicit destructor: release PEndAction (+0x148), free the MovementPoints block (+0x138), then ~CCombatActionBase (tail jump)
// Leaf: ??1CCreatureAction_DragonTakeOffIntoFlying@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00953470, 94 bytes (int3 padding 0x009534CE..CF): push esi ; mov esi,ecx ; the inline
//   ~CCountedPointer on +0x148 (Info +0x14C: dec / cmp / DeleteFunc / delete Info ; Data = 0, Info = 0) ; the inline STLport
//   ~vector<C3DVector> on +0x138 (`if (_M_start) free(_M_start)`) ; mov ecx,esi ; pop esi ; jmp ~CCombatActionBase 0x0062E900.
//   No vtable store: the destructor is compiler-generated.
// Identity (proven, two sources): (1) callers: the scalar deleting dtor 0x00953960, slot 0 of vtable 0x01293A7C whose RTTI is
//   .?AVCCreatureAction_DragonTakeOffIntoFlying@@ : CCombatActionBase : CCreatureActionBase (rtti-vtables.tsv / rtti_bases.tsv), and
//   CAIStateGroup_JackDragon::FlyOutOfGround 0x00951B20 (a local action destroyed at 0x00951CF8; the landed caller already binds
//   this address to the class's destructor, as non-virtual QAE); (2) Ego_r.pdb ??1CCreatureAction_DragonTakeOffIntoFlying@@UAE@XZ
//   (0x00977EB0, beside its ctor ??0..@@QAE@AAVCThingCreatureBase@@AAV?$vector@VC3DVector@@...@std@@@Z 0x00977E30) and the Ego_r
//   members Dest / StartPos / PreviousPoint / MovementPoints / UsePreviousPoint / PEndAction, released here in reverse order.
//   The catalog label `_Dest_val<...C3DSkeletalMorphTarget...>` is propagated (NAME_FIXES.tsv: label + the caller's QAE binding).
// Callees: the count block's DeleteFunc (`mov ecx,[eax+8] ; call [eax+4]`), operator delete 0x00BFE9BC (??3@YAXPAX@Z), free 0x00BFEA14
//   (allocator<C3DVector>::deallocate), CCombatActionBase::~CCombatActionBase 0x0062E900 (??1CCombatActionBase@@UAE@XZ, tail jump).
// Layout: retail CCombatActionBase is 0x114 (the landed creature-action rows; Ego_r 0x12C less its 12-byte STLport vectors), so the
//   Ego_r members sit 0x18 lower: Dest +0x114, StartPos +0x120, PreviousPoint +0x12C, MovementPoints +0x138, UsePreviousPoint +0x144,
//   PEndAction +0x148; sizeof 0x150 (the generated engine/CCreatureAction_DragonTakeOffIntoFlying.h agrees, but it flattens the vptr
//   and the bases into pads, so the chain is restated: CCreatureActionBase / CCombatActionBase members as opaque storage at their
//   retail sizes). C3DVector is three floats (12 bytes, natural alignment). CCountedPointer is engine/CCountedPointer.h; the vector is STLport 4.6
//   (_Vector_base + _STLP_alloc_proxy, as the landed 0x00B18780).
// Shape: the class declares no destructor; the harness below (a local action destroyed by name, as retail's FlyOutOfGround does;
//   ungraded) makes VC7.1 emit the out-of-line implicit ??1.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std
#include <stddef.h>
#include "engine/CCountedPointer.h"
class CThingCreatureBase;

#include "engine/C3DVector.h"

class CCreatureActionBase {                             // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                     // 0x00693EF0
private:
    unsigned char CCreatureActionBaseMembers_[0xA8 - 4];    // own members (opaque, retail +0x04..+0xA8)
};
class CCombatActionBase : public CCreatureActionBase {  // retail sizeof 0x114
public:
    virtual ~CCombatActionBase();                       // 0x0062E900
private:
    unsigned char CCombatActionBaseMembers_[0x114 - 0xA8];  // own members (opaque, retail +0xA8..+0x114)
};

class CCreatureAction_DragonTakeOffIntoFlying : public CCombatActionBase {   // vtable 0x01293A7C
public:
    CCreatureAction_DragonTakeOffIntoFlying(CThingCreatureBase& creature, std::vector<C3DVector>& movementPoints);   // Ego_r ??0
    C3DVector                            Dest;              // +0x114
    C3DVector                            StartPos;          // +0x120
    C3DVector                            PreviousPoint;     // +0x12C
    std::vector<C3DVector>               MovementPoints;    // +0x138
    bool                                 UsePreviousPoint;  // +0x144
    CCountedPointer<CCreatureActionBase> PEndAction;        // +0x148
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_DragonTakeOffIntoFlying) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CCreatureAction_DragonTakeOffIntoFlying, MovementPoints) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CCreatureAction_DragonTakeOffIntoFlying, PEndAction) == 0x148);

// TU harness (not a retail function): a local action destroyed by name, as CAIStateGroup_JackDragon::FlyOutOfGround 0x00951B20 does.
void TUHarness_DragonTakeOffIntoFlying(CThingCreatureBase& creature, std::vector<C3DVector>& movementPoints)
{
    CCreatureAction_DragonTakeOffIntoFlying action(creature, movementPoints);
}
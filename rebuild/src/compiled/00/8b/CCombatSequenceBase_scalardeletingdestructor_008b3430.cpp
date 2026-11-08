// CCombatSequenceBase::`scalar deleting destructor'(unsigned int flags) -- reset the vtable, destroy the CombatActions vector (release every action's counted pointer, free the block), then free the sequence for flag bit 0
// Leaf: ??_GCCombatSequenceBase@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B3430, length 63 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x01278FEC, whose RTTI descriptor is `.?AVCCombatSequenceBase@@` (slots 1 and 2 are
//   _purecall 0x00BFEA08). The catalog label "PairContainer_Destructor" is descriptive.
// Callees: the STLport __destroy_aux<CCombatAction*> @ 0x008B3010 (landed as DestroyRange_CountedPointer; `__fastcall`,
//   the empty __false_type tag by reference), free @ 0x00BFEA14 (the vector block), operator delete @ 0x00BFE9BC.
// Layout (Ego_r.pdb CCombatSequenceBase, retail 12-byte STLport vector): vptr +0, Running +4, NoAvailableActions +8,
//   CurrentActionIndex +0xC, CombatActions vector<CCombatAction> +0x10 (CCombatAction = CCountedPointer<
//   CCombatActionBase> CreatureAction, 8 bytes), Creature +0x1C, Target +0x20, CreateCalled +0x24.
// Shape: the inline virtual destructor (vtable store, ~vector: _Destroy(start, finish) -> out-of-line __destroy_aux,
//   ~_Vector_base: `if (start) free(start)`), then the flag-tested operator delete. The empty tag is homed in the live
//   flags argument slot (`lea eax,[esp+8]`): VC7.1 does that only with __destroy_aux's real body visible in the TU
//   (GOTCHAS 24l: same-TU auto_inline(off) + noinline stand-in below). The virtual destructor and both vector
//   destructors must be `__forceinline` or ??_G calls them out of line; the out-of-line constructor stand-in only
//   anchors the vtable (and with it this ??_G) in the TU.
#include "fable_compiler.h"
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

#include "engine/CCountedPointer.h"


class CCombatActionBase;
class CThingCreatureBase;

class CCombatAction {                                   // Ego_r.pdb, 8 bytes
public:
    CCountedPointer<CCombatActionBase> CreatureAction;  // +0x00
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// Same-TU body of the out-of-line __destroy_aux 0x008B3010 (GOTCHAS 24l).
#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    FABLE_FORCEINLINE ~_Vector_base() { if (_M_start != 0) free(_M_start); }
};

template <class _Tp>
class vector : public _Vector_base<_Tp> {
public:
    FABLE_FORCEINLINE ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

class CCombatSequenceBase {                             // vtable 0x01278FEC
public:
    CCombatSequenceBase();
    virtual FABLE_FORCEINLINE ~CCombatSequenceBase() { }
    virtual void PureSlot1() = 0;                       // _purecall in the retail vtable
    virtual void PureSlot2() = 0;                       // _purecall in the retail vtable

    bool                         Running;               // +0x04
    long                         NoAvailableActions;    // +0x08
    long                         CurrentActionIndex;    // +0x0C
    std::vector<CCombatAction>  CombatActions;         // +0x10
    CThingCreatureBase*          Creature;              // +0x1C
    CThingCreatureBase*          Target;                // +0x20
    bool                         CreateCalled;          // +0x24
};

// Vtable anchor (stand-in constructor; not graded).
CCombatSequenceBase::CCombatSequenceBase()
{
}
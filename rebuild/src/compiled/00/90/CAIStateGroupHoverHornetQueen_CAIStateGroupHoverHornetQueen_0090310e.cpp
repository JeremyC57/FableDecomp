// CAIStateGroup_HoverHornetQueen::CAIStateGroup_HoverHornetQueen() -- base CAIStateGroup_EnemyCreatureBase() (HoverMovementBase inlined), own vtable, then the empty Things / CircleMarkers vectors
// Leaf: ??0CAIStateGroup_HoverHornetQueen@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090310E, 38 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D484 (RTTI .?AVCAIStateGroup_HoverHornetQueen@@, base chain CAIStateGroup_HoverMovementBase : CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase : CBaseClass); Ego_r.pdb / FableWin.pdb ??0CAIStateGroup_HoverHornetQueen@@QAE@XZ.
// Callees: CAIStateGroup_EnemyCreatureBase::CAIStateGroup_EnemyCreatureBase 0x008FF720 (??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ).
// Layout: engine/CAIStateGroupBase.h's retail branch; the zeroed words are the STLport vector<CIntelligentPointer<CThing> > Things +0x40 / CircleMarkers +0x50
//   (three pointers each, 12 bytes; Ego_r.pdb's Dinkumware vectors are 16), whose inline default ctors run after the own vtable store.
// History: generated in a chunk batch over CBase_008ff720 with an invented `SZeroed` member; re-authored on the PDB members in naming
//   wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // Ego_r.pdb: vptr + PData (8 bytes), 1-aligned (chunk 703 0x00900B41)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    virtual ~CIntelligentPointer() { }
};
#pragma pack(pop)
class CThing;

namespace std {                                         // STLport 4.6 _vector.h (the default ctor zeroes the three pointers)
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector();
};
}

class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {   // RTTI CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase
public:
    CAIStateGroup_EnemyCreatureBase();                  // 0x008FF720 ??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
    unsigned char _unmodeled14[0x1c - 0x14];            // Ego_r.pdb: PTargetThing +0x14
};

class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase {   // RTTI : CAIStateGroup_EnemyCreatureBase
public:                                                 // ctor inlined into this row (0x00900BEF is its out-of-line copy)
};

class C3DVector {
public:
    float X, Y, Z;
};

class CAIStateGroup_HoverHornetQueen : public CAIStateGroup_HoverMovementBase {   // vtable 0x0128D484
public:
    C3DVector StartPos;                                 // +0x1C
    C3DVector ArenaCentrePos;                           // +0x28
    bool      bOneTime;                                 // +0x34
    CThing*   PEmitter;                                 // +0x38
    CThing*   PEmitter2;                                // +0x3C
    std::vector<CIntelligentPointer<CThing> > Things;   // +0x40
    long      CurrentCirclePoint;                       // +0x4C
    std::vector<CIntelligentPointer<CThing> > CircleMarkers;   // +0x50
    CAIStateGroup_HoverHornetQueen();
};

CAIStateGroup_HoverHornetQueen::CAIStateGroup_HoverHornetQueen()
{
}

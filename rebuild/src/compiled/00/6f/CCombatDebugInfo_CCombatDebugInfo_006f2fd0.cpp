// CCombatDebugInfo::~CCombatDebugInfo() -- the compiler-generated destructor: free the AttackerPos and PlacementVectors blocks, then destroy CurrentSequenceName
// Leaf: ??1CCombatDebugInfo@@QAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F2FD0, 44 bytes, thiscall, tail `jmp ~CCharString` on this+0x10.
// Identity (proven): FableWin.pdb spells ??1CCombatDebugInfo@@QAE@XZ (0x01D8EAF0) and, in CCombatManager's TU after
//   GetCombatDebugInfo, ??0CCombatDebugInfo@@QAE@ABV0@@Z (0x0241C5C0); retail's copy constructor is the next function,
//   0x006F3000 (landed as "CRecord_006f3000": long +0, byte +4, long +8, bytes +0xc/+0xd, CCharString +0x10, two vectors
//   +0x14/+0x20), and the landed CCombatManager::GetCombatDebugInfo 0x006EAC50 builds the same record (its CCombatDebugInfo
//   model: Zone, Attacker, AttackerNumber, Defender, MovingByAction, CurrentSequenceName, PlacementVectors, AttackerPos).
//   Ego_r.pdb CCombatDebugInfo has exactly those members (two vector<C3DVector>). This body frees +0x20 then +0x14 and ends
//   in ~CCharString(+0x10): the destructor of that record. No callers (unreferenced, like its copy constructor). The catalog
//   label "~CPatchTesselationEdgeStrip" is propagated.
// Callees: free 0x00BFEA14, CCharString::~CCharString 0x0099EAE0.
// Shape: members in reverse order (AttackerPos, PlacementVectors: trivial _Destroy + ~_Vector_base's null-tested free; then
//   CurrentSequenceName, out of line, as the tail jump). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this
//   out-of-line implicit `??1` (chunks 656 / 657 / 696 / 697).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CCharString {
public:
    ~CCharString();                                         // 0x0099EAE0
    void* PStringData;
};

#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
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
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

class CCombatDebugInfo {                                    // retail 0x2c bytes
public:
    long Zone;                                              // +0x00
    bool Attacker;                                          // +0x04
    long AttackerNumber;                                    // +0x08
    bool Defender;                                          // +0x0c
    bool MovingByAction;                                    // +0x0d
    CCharString CurrentSequenceName;                        // +0x10
    std::vector<C3DVector> PlacementVectors;                // +0x14
    std::vector<C3DVector> AttackerPos;                     // +0x20
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CCombatDebugInfo* info)
{
    info->CCombatDebugInfo::~CCombatDebugInfo();
}
// NCreatureMode::CModeSummonerSummon::~CModeSummonerSummon() -- the compiler-generated destructor: free the CreatePositions block, PTarget, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeSummonerSummon@NCreatureMode@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CC330, 50 bytes, thiscall, tail `jmp ~CModeStandBase`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x008CC310 of vtable 0x01282BA0 (RTTI
//   .?AVCModeSummonerSummon@NCreatureMode@@ : CModeStandBase : CTCCreatureModeManager::CModeBase); Ego_r.pdb spells
//   ??1CModeSummonerSummon@NCreatureMode@@UAE@XZ (donor 0x00903940, between GetType 0x00903930 and the ??_G 0x009039A0). The
//   destroyed members are Ego_r's CreatePositions (vector<C3DVector>, Ego +0x7C), PTarget (CIntelligentPointer<CThing>, +0x74)
//   and Filter (CAnimFilterOfGroup, +0x68: Group CCharString +0x6C, then the CAnimFilterBase vtable 0x0122FFCC). No 0x01282BA0
//   store: the destructor is the implicit one. The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14, CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CCharString::~CCharString 0x0099EAE0,
//   CModeStandBase::~CModeStandBase 0x0073D860 (??1CModeStandBase@@UAE@XZ).
// Shape: the landed sibling CModeJackOfBladesScream::~CModeJackOfBladesScream 0x00740540 plus the STLport 4.6 ~vector
//   (trivial _Destroy of 1-aligned C3DVectors + ~_Vector_base's null-tested free). DestroyOne (a destruction by name,
//   ungraded) makes VC7.1 emit this out-of-line implicit `??1` (chunks 656 / 657 / 696-698).
// Layout: local -- engine/CModeStandBase.h flattens the vptr into a data member; the base is sized to Ego_r's 0x68 (retail
//   offsets of the members equal Ego_r's up to CreatePositions; Timer +0x88 after the 12-byte STLport vector).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CThing;
class CCharStringData;

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
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
    typedef _Alloc allocator_type;
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator_type> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                               // 0x0099EAE0
};

#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#pragma pack(pop)

class CAnimFilterBase {                           // vtable 0x0122FFCC
public:
    CAnimFilterBase();
    virtual ~CAnimFilterBase() { }
};

class CAnimFilterOfGroup : public CAnimFilterBase {
public:
    CAnimFilterOfGroup();
    CCharString   Group;                          // +0x04
    unsigned long GroupCRC;                       // +0x08
};

class CModeStandBase {
public:
    CModeStandBase();
    virtual ~CModeStandBase();                    // 0x0073D860
    unsigned char _base[0x64];
};

namespace NCreatureMode {
class CModeSummonerSummon : public CModeStandBase {   // vtable 0x01282BA0
public:
    CModeSummonerSummon();
    CAnimFilterOfGroup          Filter;           // +0x68
    CIntelligentPointer<CThing> PTarget;          // +0x74
    std::vector<C3DVector>      CreatePositions;  // +0x7C
    long                        Timer;            // +0x88
};
}

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(NCreatureMode::CModeSummonerSummon* mode)
{
    mode->NCreatureMode::CModeSummonerSummon::~CModeSummonerSummon();
}
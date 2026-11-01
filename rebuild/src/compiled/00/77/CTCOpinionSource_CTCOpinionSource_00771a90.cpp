// CTCOpinionSource::~CTCOpinionSource() -- compiler-generated destructor: release the produced opinion set, the restrict-to-faction def pointer and the def-stack vector block, then ~CTCBase
// Leaf: ??1CTCOpinionSource@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00771A90, 100 bytes, tail jump to the base destructor.
// Identity (proven): no vtable store (compiler-generated destructor); the sole caller 0x00771A73 is the scalar deleting dtor
//   0x00771A70, slot 0 of vtable 0x0126A81C, RTTI `.?AVCTCOpinionSource@@` (CTCOpinionSource : CTCBase); the members it releases
//   are Ego_r.pdb CTCOpinionSource's (POpinionProduced +0x78, RestrictToFactionAndAllies +0x20, DefStack +0x0C). Ego_r.pdb has
//   ??1CTCOpinionSource@@UAE@XZ. Catalog label "CSharedData::~CSharedData" is propagated and wrong.
// Callees: operator delete 0x00BFE9BC (the counted pointer's info block), the info block's __fastcall DeleteFunc, the faction
//   def's virtual Delete (slot 1), free 0x00BFEA14 (the vector block), CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: Ego_r.pdb CTCOpinionSource with the 12-byte STLport vector (as the landed ctor 0x00771360): DefStack +0x0C ..
//   FrameToClearVirtualDeed +0x88. CCountedPointer / CDefPointer / CBaseClassNonCopyable from the shared headers; engine/CTCBase.h
//   is flattened, so CTCBase is the local model of the landed 0x007710A0. No engine/CTCOpinionSource.h.
// Emission: as retail's TU, the PDB constructor ??0CTCOpinionSource@@QAE@AAVCThing@@@Z (0x00771360, its initialisers from the
//   landed row) emits the vtable, whose ??_G calls this implicit ??1 out of line.
// Shape: implicit member destruction in reverse order: ~CCountedPointer (Data then Info zeroed), CDefPointer::Release, the
//   STLport vector<long> (trivial _Destroy, `if (_M_start) free`).
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
inline void _Destroy(long*, long*) { }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
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
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CFactionDef;
class COpinionSet;
enum EOpinionDeedType { };                                   // enumerator names not needed here

class CTCBase : public CBaseClassNonCopyable {              // retail 0x0C bytes
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCOpinionSource : public CTCBase {                   // vtable 0x0126A81C
public:
    CTCOpinionSource(CThing& thing);                        // 0x00771360
    std::vector<long>                 DefStack;                         // +0x0C
    bool                              UseNavLineOfSight;                // +0x18
    bool                              WitnessesWillNotAttack;           // +0x19
    bool                              WitnessesWillOverrideFactionEnemy;// +0x1A
    bool                              BypassesPersonality;              // +0x1B
    bool                              RelativeEffect;                   // +0x1C
    CDefPointer<const CFactionDef>    RestrictToFactionAndAllies;       // +0x20
    bool                              RestrictToFactionIsReversed;      // +0x24
    bool                              ReactionFlags[79];                // +0x25
    bool                              ReactToSelfPermitted;             // +0x74
    bool                              AlwaysAwareOf;                    // +0x75
    bool                              UnknownThreatSource;              // +0x76
    bool                              ReactNonstopFlag;                 // +0x77
    CCountedPointer<COpinionSet>      POpinionProduced;                 // +0x78
    EOpinionDeedType                  VirtualDeed;                      // +0x80
    long                              VirtualDeedID;                    // +0x84
    unsigned long                     FrameToClearVirtualDeed;          // +0x88
};

CTCOpinionSource::CTCOpinionSource(CThing& thing)
    : CTCBase(thing), UseNavLineOfSight(false), WitnessesWillNotAttack(false),
      WitnessesWillOverrideFactionEnemy(false), BypassesPersonality(false), RelativeEffect(false),
      RestrictToFactionIsReversed(false), ReactToSelfPermitted(false), AlwaysAwareOf(false),
      UnknownThreatSource(false), ReactNonstopFlag(false), VirtualDeed((EOpinionDeedType)0), VirtualDeedID(-1)
{
}
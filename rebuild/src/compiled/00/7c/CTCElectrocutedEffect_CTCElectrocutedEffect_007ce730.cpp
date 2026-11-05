// CTCElectrocutedEffect::~CTCElectrocutedEffect() -- electrocuted-effect component dtor: fade or kill the effect thing, then the members in reverse order (the tint pointer vector, the effect pointer, the def pointer) and ~CTCBase
// Leaf: ??1CTCElectrocutedEffect@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CE730, 105 bytes, tail jump to the base destructor.
// Identity (proven): first store = vtable 0x0126F044, RTTI `.?AVCTCElectrocutedEffect@@` (CTCElectrocutedEffect : CTCBase); the
//   sole caller 0x007CED43 is the scalar deleting dtor 0x007CED40, slot 0 of that vtable. Ego_r.pdb
//   ??1CTCElectrocutedEffect@@UAE@XZ; the walked members (+0x20 intelligent pointer, +0x28 vector, +0x0C def) are its PDB layout.
//   Catalog label "CTCActionUseBuyHouse::~CTCActionUseBuyHouse" is propagated and wrong.
// Callees: CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&) 0x0072ABD0 (static __fastcall, Ego_r.pdb
//   ?FadeOrKillEffect@CTCSpecialAbilities@@SIXAAV?$CIntelligentPointer@VCThing@@@@@Z), each TintList element's virtual dtor
//   (slot 0, flag 0), free 0x00BFEA14, CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, the def's Delete (slot 1),
//   CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: Ego_r.pdb CTCElectrocutedEffect less the vector growth (retail 0x34): PDef +0x0C, CurTime +0x10, FadeInTime +0x14,
//   FadeOutTimeStart +0x18, FadeOutTime +0x1C, PEffect +0x20, TintList +0x28. Same family as the landed ~CTCBurntEffect 0x007CDC20.
// Shape: the body is the one unconditional FadeOrKillEffect(PEffect) call; &PEffect stays in ebp for its member dtor.
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

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CSpecialAbilitiesElectrocutedEffectDef;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCBase : public CBaseClassNonCopyable {              // retail 0x0C bytes
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCSpecialAbilities {
public:
    static void __fastcall FadeOrKillEffect(CIntelligentPointer<CThing>& effect);   // 0x0072ABD0
};

class CTCElectrocutedEffect : public CTCBase {              // vtable 0x0126F044
public:
    virtual ~CTCElectrocutedEffect();
    CDefPointer<const CSpecialAbilitiesElectrocutedEffectDef>   PDef;       // +0x0C
    long                                        CurTime;                    // +0x10
    long                                        FadeInTime;                 // +0x14
    long                                        FadeOutTimeStart;           // +0x18
    long                                        FadeOutTime;                // +0x1C
    CIntelligentPointer<CThing>                 PEffect;                    // +0x20
    std::vector<CIntelligentPointer<CThing> >   TintList;                   // +0x28
};

CTCElectrocutedEffect::~CTCElectrocutedEffect()
{
    CTCSpecialAbilities::FadeOrKillEffect(PEffect);
}
// CTCBoss::~CTCBoss() -- implicit destructor of the boss thing component: destroy the Effects and HelperCreatures intelligent-pointer vectors, release the boss def, then ~CTCBase
// Leaf: ??1CTCBoss@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AFF70, 129 bytes, tail jump to ~CTCBase 0x00686830.
// Identity (proven): sole caller 0x007AFF50, slot 0 (scalar deleting dtor) of vtable 0x0126D494, RTTI `.?AVCTCBoss@@`
//   (CTCBoss : CTCBase : CBaseClassNonCopyable : CBaseClass). Ego_r.pdb / FableWin.pdb ??1CTCBoss@@UAE@XZ. No vtable store in
//   the body: the dtor is compiler-generated (no user ~CTCBoss). The manifest label
//   (CChunkedFileChunk_Destructor_WithVectorCleanup) is propagated and wrong.
// Members (Ego_r.pdb CTCBoss, 0x50 there, 0x48 in retail with 12-byte STLport vectors): PBossDef CDefPointer<const CBossDef>
//   +0x0c (released last: `dec [ecx+4]; jne; call [eax+4]`, then zeroed), HelperCreatures +0x30 /
//   Effects +0x3c std::vector<CIntelligentPointer<...> > (8-byte elements, virtual dtor with flag 0).
//   Ego_r puts PhaseStartPos (C3DVector) at +0x22, i.e. a packed C3DVector; not modelled (no new packing pragma) -- the member is
//   not touched here and the vectors sit at +0x30 / +0x3c either way.
// Layout: engine/CTCBase.h is the flattened Ego_r layout (no vtable, no dtor), so CTCBase is a local polymorphic model with
//   the header's members (CBaseClassNonCopyable base from engine/CBaseClassNonCopyable.h, ~CTCBase 0x00686830 out of line), as
//   the landed CTC* dtors do (0x007C7960 CTCTurncoat, 0x007CBF60 CTCMultiArrow); CDefPointer<T> from engine/CDefPointer.h; the
//   CBaseIntelligentPointer / CIntelligentPointer<T> model (vtable 0x0129C95C, PData +0x04, ~ 0x00A01C10) is the landed one.
//   STLport 4.6 vector text (_Destroy loop over the element's virtual dtor, ~_Vector_base's null-tested free 0x00BFEA14).
// TU: the ctor ??0CTCBoss@@QAE@AAVCThing@@@Z is defined (empty here; it emits the vtable, the ??_G and this implicit dtor).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_vector.h (the members this row needs; the malloc-backed allocator of
// this build: deallocate = free 0x00BFEA14)
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
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;                                               // +0x00
    _Tp* _M_finish;                                              // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CThing;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                                          // vtable 0x0129C95C, 8 bytes
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();                                  // 0x00A01C10
    CBaseObject* GetPItem() const;                                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
};

class CTCBase : public CBaseClassNonCopyable {                           // Ego_r.pdb CTCBase, 0xc
public:
    CTCBase(CThing& thing);
    virtual ~CTCBase();                                                  // 0x00686830
    CThing* ParentThing;                                                 // +0x04
    bool    ToBeRemoved;                                                 // +0x08
    bool    IsInGlobalUpdateTCs;                                         // +0x09
};
class CBossDef;
class CThingAICreature;
class C3DVector { public: float X; float Y; float Z; };
class CTCBoss : public CTCBase {                                         // vtable 0x0126D494
public:
    CTCBoss(CThing& thing);
    CDefPointer<const CBossDef>                         PBossDef;          // +0x0c
    long                                                GenericCounter;    // +0x10
    long                                                StrikesSinceHit;   // +0x14
    long                                                currentBossPhase;  // +0x18
    long                                                NextWeapon;        // +0x1c
    bool                                                BCountStrikes;     // +0x20
    bool                                                EnablePhaseChanges;// +0x21
    C3DVector                                           PhaseStartPos;     // +0x24 (Ego_r +0x22: see the Layout note)
    std::vector<CIntelligentPointer<CThingAICreature> > HelperCreatures;   // +0x30
    std::vector<CIntelligentPointer<CThing> >           Effects;           // +0x3c
};
CTCBoss::CTCBoss(CThing& thing) : CTCBase(thing)
{
}
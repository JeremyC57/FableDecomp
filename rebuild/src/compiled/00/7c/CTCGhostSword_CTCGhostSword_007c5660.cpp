// CTCGhostSword::~CTCGhostSword() -- ghost-sword spell component destructor (empty user body): destroy PItemRemoved, the CurFadeInTimer / MinionEffects / Minions vectors and the def pointer, then ~CTCBase
// Leaf: ??1CTCGhostSword@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C5660, 161 bytes, tail jump to ~CTCBase 0x00686830.
// Identity (proven): sole caller 0x007C66B0, slot 0 (scalar deleting dtor) of vtable 0x0126E494, RTTI `.?AVCTCGhostSword@@`
//   (CTCGhostSword : CTCBase); the body's first store is that vtable, i.e. a user-written dtor (an implicit one drops the store).
//   Ego_r.pdb / FableWin.pdb ??1CTCGhostSword@@UAE@XZ. The manifest label (Destroy_CChunkedFileChunk) is propagated and wrong.
// Members (Ego_r.pdb CTCGhostSword with 12-byte STLport vectors): PDef +0x0c, Minions +0x10, MinionEffects +0x1c,
//   MinionAliveTimer +0x28, FadeInTimer +0x2c, CurFadeInTimer std::vector<long> +0x30 (trivial elements: free only),
//   SpellLevel +0x3c, PItemRemoved CIntelligentPointer<CThing> +0x40 (~CBaseIntelligentPointer 0x00A01C10 called first),
//   CutSceneCancelled +0x48. The user body is empty: everything here is member destruction.
// Layout: engine/CTCBase.h is the flattened Ego_r layout (no vtable, no dtor), so CTCBase is a local polymorphic model with
//   the header's members (CBaseClassNonCopyable base from engine/CBaseClassNonCopyable.h, ~CTCBase 0x00686830 out of line), as
//   the landed CTC* dtors do (0x007C7960 CTCTurncoat, 0x007CBF60 CTCMultiArrow); CDefPointer<T> from engine/CDefPointer.h; the
//   CBaseIntelligentPointer / CIntelligentPointer<T> model (vtable 0x0129C95C, PData +0x04, ~ 0x00A01C10) is the landed one.
//   STLport 4.6 vector text (_Destroy loop over the element's virtual dtor, ~_Vector_base's null-tested free 0x00BFEA14).
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
class CSpecialAbilitiesGhostSwordDef;
class CThingAICreature;
class CTCGhostSword : public CTCBase {                                   // vtable 0x0126E494
public:
    CTCGhostSword(CThing& thing);
    virtual ~CTCGhostSword();
    CDefPointer<const CSpecialAbilitiesGhostSwordDef>   PDef;              // +0x0c
    std::vector<CIntelligentPointer<CThingAICreature> > Minions;           // +0x10
    std::vector<CIntelligentPointer<CThing> >           MinionEffects;     // +0x1c
    long                                                MinionAliveTimer;  // +0x28
    long                                                FadeInTimer;       // +0x2c
    std::vector<long>                                   CurFadeInTimer;    // +0x30
    long                                                SpellLevel;        // +0x3c
    CIntelligentPointer<CThing>                         PItemRemoved;      // +0x40
    bool                                                CutSceneCancelled; // +0x48
};
CTCGhostSword::~CTCGhostSword()
{
}
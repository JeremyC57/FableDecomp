// CTCCreatureTintEffect::~CTCCreatureTintEffect() -- the creature tint-effect component's destructor: its vtable, the TintList intelligent pointers destroyed in place and the vector block freed, the tint def pointer released, then ~CTCBase
// Leaf: ??1CTCCreatureTintEffect@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CF0B0, 86 bytes, thiscall, tail `jmp ~CTCBase` 0x00686830.
// Identity (proven, two sources): it stores vtable 0x0126F13C first (rtti-vtables.tsv: RTTI .?AVCTCCreatureTintEffect@@ : CTCBase :
//   CBaseClassNonCopyable : CBaseClass); its only caller is the scalar deleting dtor 0x007CF390, slot 0 of 0x0126F13C; the ctor
//   0x007CF110 (reached from the Alloc's `push 40h` at 0x004D5927) stores the same vtable. Ego_r.pdb ??1CTCCreatureTintEffect@@UAE@XZ
//   (0x007CD760, right before ??0 0x007CD7A0) and FableWin.pdb (0x02737540). It stores its own vtable, so the destructor is
//   user-declared (empty body: FinalCleanup is reached from OnKill / OnRemove, not from here). The catalog label
//   (~CTCGraphicAppearanceReplaceableMesh) is propagated and wrong.
// Callees: each TintList element's virtual slot 0 with flag 0 (CIntelligentPointer<CThing>'s deleting dtor run in place: STLport
//   `_Destroy` -> `p->~T()`), _free 0x00BFEA14 (the vector block, malloc allocator), the def's virtual slot 1 Delete on its last
//   release (CDefPointer<T> inline Release), ??1CTCBase@@UAE@XZ 0x00686830 (tail).
// Layout: Ego_r.pdb CTCCreatureTintEffect (0x44 with a 16-byte Dinkumware vector; retail 0x40 = the Alloc's `push 40h`): PDef
//   (CDefPointer<const CSpecialAbilitiesCreatureTintDef>) +0xC, CurTime +0x10, FadeInTime +0x14, StartFadeOut +0x18,
//   FadeOutTimeStart +0x1C, FadeOutTime +0x20, TintList (vector<CIntelligentPointer<CThing>>, 8-byte elements) +0x24, Color long[4]
//   +0x30. engine/CTCBase.h (FABLE_RETAIL_CTCBASE), engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE); CIntelligentPointer has no
//   shared header (local model, vtable 0x01238C6C).
// Shape: members die in reverse order: the STLport vector (elements via `_Destroy`, the end pointer kept in ebx, then the
//   null-tested `_Vector_base` free), then the def pointer's inline release.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (vtable 0x0125BE0C)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                             // CDefPointer<T> (inline Release)

extern "C" void __cdecl free(void* block);                  // 0x00BFEA14 _free (MSVCR71 import thunk)

namespace std {                                             // STLport 4.6 (_alloc.h / _construct.h / _vector.h), the members this body expands
template <class _Tp> class allocator { public:
    typedef unsigned int size_type;
    void deallocate(_Tp* __p, size_type) { if (__p != 0) free((void*)__p); }   // __sgi_alloc = the malloc allocator
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

class CThing;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CSpecialAbilitiesCreatureTintDef;

class CTCCreatureTintEffect : public CTCBase {              // vtable 0x0126F13C
public:
    CTCCreatureTintEffect(CThing& thing);                   // ??0CTCCreatureTintEffect@@QAE@AAVCThing@@@Z (0x007CF110)
    virtual ~CTCCreatureTintEffect();
    virtual void FrameUpdate();
    virtual void OnKill();
    virtual void OnRemove();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    CDefPointer<const CSpecialAbilitiesCreatureTintDef> PDef;              // +0x0C
    long                                     CurTime;                      // +0x10
    long                                     FadeInTime;                   // +0x14
    bool                                     StartFadeOut;                 // +0x18
    long                                     FadeOutTimeStart;             // +0x1C
    long                                     FadeOutTime;                  // +0x20
    std::vector<CIntelligentPointer<CThing> > TintList;                    // +0x24
    long                                     Color[4];                     // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureTintEffect) == 0x40);

CTCCreatureTintEffect::~CTCCreatureTintEffect()
{
}
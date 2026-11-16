// CTCMultiArrow::CTCMultiArrow(CThing&) -- CTCBase(owner), vtable, counters and the ammo-effect vector cleared, BowEffect = 0, then Init()
// Leaf: ??0CTCMultiArrow@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model (CTCBase_007cc7d0, which bound a made-up constructor name to
//   0x00686800) is gone; the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC7D0, length 92 bytes (vtable 0x0126ED7C).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem(CBaseObject*) @ 0x00A01B90, CTCMultiArrow::Init() @ 0x007CC4D0.
// Retail CTCMultiArrow (PDB, 12-byte retail vector): PDef +0x0c, NumberOfShots/NumberInVolley/NumOfVolleys +0x10..+0x18
// (left uninitialised), SpellLevel +0x1c, RemovedEffect +0x20, CreateAmmoEffectCount/Delay/Timer +0x24..+0x2c,
// AmmoEffects +0x30, BowEffect (CIntelligentPointer<CThing>, vtable 0x01238C6C) +0x3c, CutSceneCancelled +0x44.
// Shape: every scalar through the ebx zero register after the vtable; AmmoEffects is the real STLport vector
// (`_Vector_base(const allocator&)` nulling start/finish and an `_STLP_alloc_proxy` end pointer built from an
// `allocator_type()` temporary) -- with three plain zeroing words the `mov ecx,edi` for BowEffect is scheduled before
// the vector stores; BowEffect built from a null pointer (the base constructor, its vtable, SetPItem(0) with `push ebx`);
// Init is a body call.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct SAllocator { SAllocator() {} };          // empty allocator (user constructor: no zero fill of the temporary)

template<class T> struct SAllocProxy : SAllocator { // STLport _STLP_alloc_proxy: the end-of-storage pointer over the allocator
    T* m_data;
    SAllocProxy(const SAllocator& a, T* data) : SAllocator(a), m_data(data) {}
};

template<class T> struct CVectorBase {              // STLport _Vector_base
    T*             m_pFirst;
    T*             m_pLast;
    SAllocProxy<T> m_end;
    CVectorBase(const SAllocator& a) : m_pFirst(0), m_pLast(0), m_end(a, 0) {}
};

template<class T> struct CRetailVector : CVectorBase<T> {
    CRetailVector() : CVectorBase<T>(SAllocator()) {}
};

struct CDefPointerMultiArrow {                      // CDefPointer<const CSpecialAbilitiesMultiArrowDef>
    const void* m_pDef;
    CDefPointerMultiArrow() : m_pDef(0) {}
};

class CTCMultiArrow : public CTCBase { public:
    CDefPointerMultiArrow                      PDef;   // +0x0c
    long                                       NumberOfShots;   // +0x10
    long                                       NumberInVolley;  // +0x14
    long                                       NumOfVolleys;    // +0x18
    long                                       SpellLevel;      // +0x1c
    bool                                       RemovedEffect;   // +0x20
    long                                       CreateAmmoEffectCount;   // +0x24
    long                                       CreateAmmoEffectDelay;   // +0x28
    long                                       CreateAmmoEffectTimer;   // +0x2c
    CRetailVector<CIntelligentPointer<CThing> > AmmoEffects;    // +0x30
    CIntelligentPointer<CThing>                BowEffect;       // +0x3c
    bool                                       CutSceneCancelled;   // +0x44
    CTCMultiArrow(CThing& owner);
    void Init();                                    // 0x007CC4D0
};

CTCMultiArrow::CTCMultiArrow(CThing& owner)
    : CTCBase(owner), SpellLevel(0), RemovedEffect(false), CreateAmmoEffectCount(0),
      CreateAmmoEffectDelay(0), CreateAmmoEffectTimer(0), BowEffect(0), CutSceneCancelled(false)
{
    Init();
}
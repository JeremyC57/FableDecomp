#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::CDrawQuickAccessIcons(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, charge state cleared (index -1), flash fade and charge-up handle built, empty handle/effect/position vectors, then thirteen effect slots and thirteen effect positions
// Retry 10: RELOCATION_MATCH -- every 12-byte vector modelled as STLport _Vector_base (m_pEnd an _STLP_alloc_proxy with its own ctor); with three plain pointers the resize `push 0Dh` lands before EffectPositions' first zero store.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00652F0A, length 152 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65, CFadeInFadeOutBase::CFadeInFadeOutBase() @ 0x00436560 (on +0x40),
// CBaseClass::CBaseClass @ 0x0099A2D0 (the handle's root), vector<CIntelligentPointer<CParticleEmitter>>::Resize(long) @ 0x00657037 (Effects, 13),
// vector<C2DCoordI>::resize(unsigned long, const C2DCoordI&) @ 0x00656FAB (EffectPositions, 13). Vtables 0x0125A704
// (CDrawQuickAccessIcons), 0x0122F598 (CEnginePrimitiveHandle).
// PDB CDrawQuickAccessIcons (168 bytes): ChargingSoundID +0x2c, ChargingButtonIndex +0x30, ChargingButtonTime +0x34, Charging +0x38,
// FullyCharged +0x39, FullyChargedTime +0x3c, FlashButton +0x40, FlashFade +0x44, ChargeUpPrimitiveHandle +0x60,
// SpecialPrimitiveHandles +0x68, QuickAccessSpellCrossHandles +0x78, Effects +0x88, EffectPositions +0x98 -- retail 4 lower up to
// the first vector, then 4 more per 12-byte vector in the shipped build.
// Shape: frame-pointer size TU (`optimize("y",off)` + `("s",on)`, pass-through base arguments); the -1 is `or dword ptr [..],-1`,
// ebx is the shared zero; the handle is built through edi (as CDrawAbilityButtons 0x00651A7E: `__forceinline` constructor and a
// counted Data member object); Effects is zeroed through ecx and EffectPositions through edi, each pointer reused as `this` of
// its resize; the resize fill value is a default-constructed C2DCoordI (user constructor that leaves it uninitialised) in the
// 8-byte frame slot.
class CPlayerGui;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CIVCountedPointeeBase;

struct CIVCountedPointer {                         // embedded member with an inline constructor
    CIVCountedPointeeBase* Object;
    __forceinline CIVCountedPointer() : Object(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass { public:            // vtable 0x0122F598
    CIVCountedPointer Data;                        // +4
    __forceinline CEnginePrimitiveHandle() { }
};

class C2DCoordI { public:
    long X;
    long Y;
    C2DCoordI() { }                                // leaves the coordinates uninitialised
};

namespace NPlayerGui {

template <class P> struct CAllocProxy {      // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    P p;
    CAllocProxy(P v) : p(v) { }
    operator P() const { return p; }
    CAllocProxy& operator=(P v) { p = v; return *this; }
};
struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    CAllocProxy<void*> m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CRetailEmitterVector {                      // VC6 Dinkumware vector<CIntelligentPointer<CParticleEmitter>>
    void* m_pFirst;
    void* m_pLast;
    CAllocProxy<void*> m_pEnd;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void Resize(long count);                       // 0x00657037
};

struct CRetailCoordVector {                        // VC6 Dinkumware vector<C2DCoordI>
    C2DCoordI* m_pFirst;
    C2DCoordI* m_pLast;
    CAllocProxy<C2DCoordI*> m_pEnd;
    CRetailCoordVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void resize(unsigned long count, const C2DCoordI& value);   // 0x00656FAB
};

struct CFadeInFadeOutBase {
    unsigned char _body[0x1c];
    CFadeInFadeOutBase();                          // 0x00436560
};

class CDrawBase { public:
    virtual void v00();
    unsigned char _body[0x28 - 4];                 // +0x04 .. +0x28 (GUI pointers, Drawing, PrimitiveHandles)
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

class CDrawQuickAccessIcons : public CDrawBase { public:         // vtable 0x0125A704
    virtual void v00();
    unsigned long          ChargingSoundID;               // +0x28
    long                   ChargingButtonIndex;           // +0x2c
    float                  ChargingButtonTime;            // +0x30
    bool                   Charging;                      // +0x34
    bool                   FullyCharged;                  // +0x35
    float                  FullyChargedTime;              // +0x38
    long                   FlashButton;                   // +0x3c (EXboxControllerButton)
    CFadeInFadeOutBase     FlashFade;                     // +0x40
    CEnginePrimitiveHandle ChargeUpPrimitiveHandle;       // +0x5c
    CRetailHandleVector    SpecialPrimitiveHandles;       // +0x64
    CRetailHandleVector    QuickAccessSpellCrossHandles;  // +0x70
    CRetailEmitterVector   Effects;                       // +0x7c
    CRetailCoordVector     EffectPositions;               // +0x88
    CDrawQuickAccessIcons(CPlayerGui* gui, void* drawList);
};

CDrawQuickAccessIcons::CDrawQuickAccessIcons(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), ChargingSoundID(0), ChargingButtonIndex(-1), ChargingButtonTime(0.0f), Charging(false),
      FullyCharged(false), FullyChargedTime(0.0f), FlashButton(0), FlashFade(), ChargeUpPrimitiveHandle(),
      SpecialPrimitiveHandles(), QuickAccessSpellCrossHandles(), Effects(), EffectPositions()
{
    Effects.Resize(13);
    EffectPositions.resize(13, C2DCoordI());
}

}
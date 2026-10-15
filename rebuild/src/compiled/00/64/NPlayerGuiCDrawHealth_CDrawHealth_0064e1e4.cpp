#pragma optimize("s", on)
// NPlayerGui::CDrawHealth::CDrawHealth(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, an empty emitter vector, the previous health at -1, the increase fade built, the reduction/flash values zeroed, the pulse at 1, two effect slots and twelve primitive handles reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E1E4, length 107 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65 (pass-through pushes: a size TU), CFadeInFadeOutBase::CFadeInFadeOutBase()
// @ 0x00436560 (on +0x38), vector<CIntelligentPointer<CParticleEmitter>>::Resize(long) @ 0x00657037 (Effects +0x28, 2),
// vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90 (PrimitiveHandles +0x1c, 12). Vtable 0x0125A60C.
// PDB CDrawHealth (116 bytes): Effects +0x2c, PreviousHealth +0x3c, HealthIncreaseFade +0x40, HealthReduction +0x5c,
// HealthReductionAlpha +0x60, HealthFlashTimer +0x64, LowHealthPulseScale +0x68, LowHealthPulseDirection +0x6c, PreviousEffect +0x70
// -- retail 4 lower for Effects and 8 lower after it (the CDrawBase vector and Effects are 12 bytes in the shipped build).
// Shape: as the landed CDrawStamina 0x0064E24F: Effects is zeroed through its address in edi (inline member constructor, the
// pointer reused as `this` of its Resize) with `and dword ptr [..],0` (size TU), the -1.0f is an immediate store, the float zeros
// and ones come from fldz/fld1, all members in the initialiser list; Effects is resized before the primitive handles.
struct CPlayerGui;

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    void Resize(long count);                       // 0x00443F90
};

struct CRetailEmitterVector {                      // VC6 Dinkumware vector<CIntelligentPointer<CParticleEmitter>>
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void Resize(long count);                       // 0x00657037
};

struct CFadeInFadeOutBase {
    unsigned char _body[0x1c];                     // Active, FadeInTime, FadeOutTime, Closing, Opening, OpenTimer, CloseTimer, ToColour
    CFadeInFadeOutBase();                          // 0x00436560
};

struct CDrawBase {
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct CDrawHealth : CDrawBase {                   // vtable 0x0125A60C
    virtual void v00();
    CRetailEmitterVector Effects;                  // +0x28
    float                PreviousHealth;           // +0x34
    CFadeInFadeOutBase   HealthIncreaseFade;       // +0x38
    float                HealthReduction;          // +0x54
    float                HealthReductionAlpha;     // +0x58
    float                HealthFlashTimer;         // +0x5c
    float                LowHealthPulseScale;      // +0x60
    float                LowHealthPulseDirection;  // +0x64
    long                 PreviousEffect;           // +0x68
    CDrawHealth(CPlayerGui* gui, void* drawList);
};

CDrawHealth::CDrawHealth(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), Effects(), PreviousHealth(-1.0f), HealthIncreaseFade(), HealthReduction(0.0f),
      HealthReductionAlpha(0.0f), HealthFlashTimer(0.0f), LowHealthPulseScale(1.0f), LowHealthPulseDirection(1.0f), PreviousEffect(0)
{
    Effects.Resize(2);
    PrimitiveHandles.Resize(12);
}

}
#pragma optimize("s", on)
// NPlayerGui::CDrawStamina::CDrawStamina(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, two empty emitter vectors, the previous stamina values at -1, the increase fade built, ten primitive handles and two effect slots reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E24F, length 87 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65 (pass-through pushes: a size TU), CFadeInFadeOutBase::CFadeInFadeOutBase() @ 0x00436560 (on +0x48; the
// manifest label "CTCHeroStats::SoberUp" is propagated -- the body zeroes the 0x1c-byte fade record and stores an opaque black colour),
// vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90 (PrimitiveHandles +0x1c, 10), vector<CIntelligentPointer<CParticleEmitter>>::Resize(long)
// @ 0x00657037 (Effects +0x28, 2; manifest "_Dest_val<...>" is propagated). Vtable 0x0125A618 (chunk label CDrawStamina; the PDB layout fits).
// PDB CDrawStamina (112 bytes): Effects +0x2c, IncreaseEffects +0x3c, PreviousStamina +0x4c, PreviousMaxStamina +0x50, StaminaIncreaseFade +0x54 -- retail
// 4 lower for the first vector and 8/12 lower after it (the CDrawBase vector and these vectors are 12 bytes in the shipped build).
// Shape: Effects is zeroed through its address in edi (an inline member constructor whose pointer is reused as `this` of the last Resize),
// IncreaseEffects directly, the -1 values are `or dword ptr [..],-1` and the longs share the eax zero register.
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

struct CDrawStamina : CDrawBase {                  // vtable 0x0125A618
    virtual void v00();
    CRetailEmitterVector Effects;                  // +0x28
    CRetailEmitterVector IncreaseEffects;          // +0x34
    long                 PreviousStamina;          // +0x40
    long                 PreviousMaxStamina;       // +0x44
    CFadeInFadeOutBase   StaminaIncreaseFade;      // +0x48
    CDrawStamina(CPlayerGui* gui, void* drawList);
};

CDrawStamina::CDrawStamina(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), Effects(), IncreaseEffects(), PreviousStamina(-1), PreviousMaxStamina(-1), StaminaIncreaseFade()
{
    PrimitiveHandles.Resize(10);
    Effects.Resize(2);
}

}

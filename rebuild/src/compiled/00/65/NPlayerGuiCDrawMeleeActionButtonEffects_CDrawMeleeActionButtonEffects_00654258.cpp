#pragma optimize("s", on)
// NPlayerGui::CDrawMeleeActionButtonEffects::CDrawMeleeActionButtonEffects(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, an empty emitter vector, three timers at -1, a zero effect and a null ability def, three emitter slots and one primitive handle reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00654258, length 76 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65 (pass-through pushes), the emitter vector's Reserve @ 0x00657037 (on +0x28, 3; manifest "_Dest_val<...>" is
// propagated), the primitive-handle vector's Reserve @ 0x00443F90 (on +0x1c, 1). Vtable 0x0125A714 = NPlayerGui::CDrawMeleeActionButtonEffects (rtti-vtables.tsv).
// Size TU: the -1 stores are `or dword ptr [..],-1`, the zeroes share eax, and the emitter vector's three words are zeroed through its address in ecx
// (an inline constructor on the member; ecx then serves as `this` of the Reserve call). PDB (80 bytes): the emitter vector +0x2c, FlashTimer +0x3c,
// ComboLoadingIconTimer +0x40, FlourishCounter +0x44, FlourishEffect +0x48, PMeleeAbilityDef +0x4c -- retail 4 lower for the vector, 8 lower after it.
class CPlayerGui;

namespace NPlayerGui {

struct CRetailHandleVector {
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    void Reserve(long count);                      // 0x00443F90
};

struct CRetailEmitterVector {                      // VC6 Dinkumware vector<CIntelligentPointer<CParticleEmitter>>
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void Reserve(long count);                      // 0x00657037
};

class CDrawBase { public:
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct CDefPointer_Melee {
    void* m_p;
    CDefPointer_Melee() : m_p(0) { }
};

class CDrawMeleeActionButtonEffects : public CDrawBase { public:   // vtable 0x0125A714
    virtual void v00();
    CRetailEmitterVector Emitters;                 // +0x28
    long  FlashTimer;                              // +0x34
    long  ComboLoadingIconTimer;                   // +0x38
    long  FlourishCounter;                         // +0x3c
    long  FlourishEffect;                          // +0x40
    CDefPointer_Melee PMeleeAbilityDef;            // +0x44 (CDefPointer<const CMeleeCombatAbilityDef>: an inline null constructor)
    CDrawMeleeActionButtonEffects(CPlayerGui* gui, void* drawList);
};

CDrawMeleeActionButtonEffects::CDrawMeleeActionButtonEffects(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), Emitters(), FlashTimer(-1), ComboLoadingIconTimer(-1), FlourishCounter(-1), FlourishEffect(0), PMeleeAbilityDef()
{
    Emitters.Reserve(3);
    PrimitiveHandles.Reserve(1);
}

}
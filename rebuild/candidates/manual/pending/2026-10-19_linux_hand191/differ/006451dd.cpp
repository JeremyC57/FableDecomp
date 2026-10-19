#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawAbilityButtons::DrawEffects(long button, const C2DVector& position, const CHeroAbilityDef* def) -- if the ability has an icon effect and the button has no live effect emitter yet, create a 2D particle emitter at the button position plus the def's offset and remember it
// Grade: DIFFER(169v173) -- retail keeps `this` in ebx and button*8 in edi (pushing ebx before esi: one more callee-saved register in the prologue) and loads position.X before the offset X (`fld [eax] ; fadd [esi+50h]`); the lane swaps ebx/edi and loads the offset first
// Leaf: DrawEffects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006451DD, length 173 bytes, `ret 0Ch`.
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50, CRightHandedSet::CRightHandedSet(const C3DVector&,
// const C3DVector&) @ 0x00487050, the particle emitter manager getter @ 0x00AEAAF0 (returns the global @ 0x013D2E1C;
// catalog "GFGetPreMainMemoryUsed"), CParticleEmitterManager::Create2DEmitter(long, const C2DVector&, EEngine2DLayer,
// bool, const CRightHandedSet&) @ 0x00AEB790, CIntelligentPointer<T>::CIntelligentPointer(T*) @ 0x00655318 and
// operator= @ 0x0065533C (shared template bodies; catalog names other instantiations), ~CBaseIntelligentPointer
// @ 0x00A01C10.
// Layout: the button effect emitters are an array of 8-byte CIntelligentPointer at [this+0x48]; CHeroAbilityDef at retail
// PDB-4: IconEffectIndex +0x48, IconEffectOffset (C2DVector) +0x50.
// Shape (best of 4): a frame-pointer size TU (`optimize("y", off)` + `("s", on)`, fldz/fld1 stores); the two orientation vectors
// are argument temporaries of the CRightHandedSet temporary (the second built first), the position is a named local,
// the new emitter is assigned through a CIntelligentPointer temporary.
struct C2DVector {
    float X, Y;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRightHandedSet {
    unsigned char _body[0x18];
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};
enum EEngine2DLayer { LAYER_ABILITY_EFFECTS = 0x43 };
struct CParticleEmitter;
struct CParticleEmitterManager {
    CParticleEmitter* Create2DEmitter(long type, const C2DVector& position, EEngine2DLayer layer, bool start, const CRightHandedSet& orientation);   // 0x00AEB790
};
CParticleEmitterManager* GetParticleEmitterManager();   // 0x00AEAAF0

struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    CBaseObject* Item;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item);                  // 0x00655318
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0065533C
};

struct CHeroAbilityDef {
    unsigned char _pad[0x48];
    long IconEffectIndex;                          // +0x48
    long IconActiveEffectIndex;                    // +0x4c
    C2DVector IconEffectOffset;                    // +0x50
};

namespace NPlayerGui {
struct CDrawAbilityButtons {
    unsigned char _pad[0x48];
    CIntelligentPointer<CParticleEmitter>* Effects;   // +0x48
    void DrawEffects(long button, const C2DVector& position, const CHeroAbilityDef* def);
};

void CDrawAbilityButtons::DrawEffects(long button, const C2DVector& position, const CHeroAbilityDef* def)
{
    if (def->IconEffectIndex > 0) {
        if (!Effects[button].GetPItem()) {
            C3DVector forward(0.0f, 1.0f, 0.0f);
            C3DVector up(0.0f, 0.0f, 1.0f);
            C2DVector at;
            at.X = position.X + def->IconEffectOffset.X;
            at.Y = position.Y + def->IconEffectOffset.Y;
            Effects[button] = CIntelligentPointer<CParticleEmitter>(GetParticleEmitterManager()->Create2DEmitter(
                def->IconEffectIndex, at, LAYER_ABILITY_EFFECTS, true, CRightHandedSet(up, forward)));
        }
    }
}
}

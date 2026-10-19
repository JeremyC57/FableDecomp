#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawAbilityButtons::DrawEffects(long index, const C2DVector& position, const CAbilityButtonEffectDef& effect) -- when the button's effect has a type and its emitter slot is empty, create the 2D particle emitter at the button position plus the effect offset (layer 0x43, upright axes) and keep it in the slot
// Grade: DIFFER(172v173) -- register swap only: retail keeps `this` in ebx and index*8 in edi (`lea ecx,[edi+eax]`), ours the reverse (`add ecx,ebx`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006451DD, length 173 bytes (ret 0Ch; ebp frame closed with `leave`).
// Pragmas: `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (the HUD TU).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CRightHandedSet::CRightHandedSet(const C3DVector&, const C3DVector&)
// 0x00487050, CParticleEmitterManager::Get() 0x00AEAAF0, CParticleEmitterManager::Create2DEmitter(long, const C2DVector&,
// EEngine2DLayer, bool, const CRightHandedSet&) 0x00AEB790, CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) 0x00655318
// and operator= 0x0065533C (manifest labels name other instantiations; propagated), ~CBaseIntelligentPointer 0x00A01C10.
// Layout: CDrawAbilityButtons Effects +0x48 (vector<CIntelligentPointer<CParticleEmitter> > begin; PDB +0x50); the effect record
// (0x5C bytes, the GUI def's +0x7FC vector) EffectType +0x48, Offset +0x50 (C2DVector).
// Shape: one expression: the emitter call's arguments are evaluated right to left (the two C3DVector temporaries of the
// axes, the offset position, then the axes constructor), the manager is fetched last; the emitter is wrapped in a named
// CIntelligentPointer local (re-addressed with `lea` for the assignment) that is destroyed at the end of its block; the
// two tests are nested so the index register is pushed only inside the first.
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
inline C2DVector operator+(const C2DVector& a, const C2DVector& b) { return C2DVector(a.X + b.X, a.Y + b.Y); }

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CRightHandedSet {                        // PDB 0x18
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};

namespace NParticleEngine { class CParticleEmitter; }
using NParticleEngine::CParticleEmitter;

enum EEngine2DLayer { ENGINE_2D_LAYER_HUD_EFFECTS = 0x43 };

class CParticleEmitterManager {
public:
    static CParticleEmitterManager* Get();      // 0x00AEAAF0
    CParticleEmitter* Create2DEmitter(long type, const C2DVector& position, EEngine2DLayer layer, bool persistent,
                                      const CRightHandedSet& axes);   // 0x00AEB790
};

class CBaseIntelligentPointer {
public:
    void* m_pData;
    void* GetPItem() const;                     // 0x00A01B50
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item);               // 0x00655318 (T = CParticleEmitter)
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0065533C
};

struct CAbilityButtonEffectDef {                // 0x5c bytes
    unsigned char _pad00[0x48];
    long          EffectType;                   // +0x48
    unsigned char _pad4c[4];
    C2DVector     Offset;                       // +0x50
};

namespace NPlayerGui {

class CDrawAbilityButtons {
public:
    unsigned char                          _pad00[0x48];
    CIntelligentPointer<CParticleEmitter>* Effects;   // +0x48 vector begin
    void DrawEffects(long index, const C2DVector& position, const CAbilityButtonEffectDef& effect);
};

void CDrawAbilityButtons::DrawEffects(long index, const C2DVector& position, const CAbilityButtonEffectDef& effect)
{
    if (effect.EffectType > 0) {
        if (Effects[index].GetPItem() == 0) {
            CIntelligentPointer<CParticleEmitter> emitter(CParticleEmitterManager::Get()->Create2DEmitter(
                effect.EffectType, position + effect.Offset, ENGINE_2D_LAYER_HUD_EFFECTS, true,
                CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f))));
            Effects[index] = emitter;
        }
    }
}

}

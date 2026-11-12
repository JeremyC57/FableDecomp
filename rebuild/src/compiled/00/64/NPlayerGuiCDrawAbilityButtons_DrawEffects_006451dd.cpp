// NPlayerGui::CDrawAbilityButtons::DrawEffects(long index, const C2DVector& position, const CHeroAbilityDef& ability) -- when the ability has an icon effect and the button's emitter slot is empty, create the 2D particle emitter at the button position plus the icon effect offset (layer 0x43, upright axes) and keep it in the slot
// Leaf: ?DrawEffects@CDrawAbilityButtons@NPlayerGui@@IAEXJABVC2DVector@@ABVCHeroAbilityDef@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006451DD, 173 bytes, thiscall `ret 0Ch`; ebp frame closed with `leave`.
// Pragmas: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`: the HUD TU's established mode (the landed NPlayerGui rows).
// Identity: Ego_r.pdb ?DrawEffects@CDrawAbilityButtons@NPlayerGui@@IAEXJABVC2DVector@@ABVCHeroAbilityDef@@@Z (protected).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CRightHandedSet(const C3DVector&, const C3DVector&) 0x00487050,
//   NParticleEngine::GetParticleEmitterManager() 0x00AEAAF0 (FableWin.pdb, __fastcall, by reference),
//   CParticleEmitterManager::Create2DEmitter(long, const C2DVector&, EEngine2DLayer, bool, const CRightHandedSet&) 0x00AEB790
//   (Ego_r.pdb), CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) 0x00655318 / operator= 0x0065533C,
//   ~CBaseIntelligentPointer 0x00A01C10.
// Layout: CDrawAbilityButtons::Effects (Ego_r.pdb vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >) at retail +0x48
//   (PDB +0x50, Dinkumware vector sizes before it); CHeroAbilityDef IconEffectIndex / IconEffectOffset at retail +0x48 / +0x50
//   (Ego_r.pdb +0x4C / +0x54, the retail def base 4 bytes smaller).
// Shape: the real STLport vector (stlport/fable_containers.h): `Effects[index]` is `*(begin() + n)`, which gives retail's
//   `shl edi,3 ; lea ecx,[edi+eax]` with `this` in ebx (the old raw-pointer model swapped the two registers). One emitter
//   expression (arguments right to left, the manager fetched last); the named CIntelligentPointer local dies at the block end.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
inline C2DVector operator+(const C2DVector& a, const C2DVector& b) { return C2DVector(a.X + b.X, a.Y + b.Y); }
#include "engine/C3DVector.h"
class CRightHandedSet { public:                 // PDB 0x18
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};
enum EEngine2DLayer { ENGINE_2D_LAYER_0 = 67 };   // PDB enumerator (engine/CEnginePrimitive2DSprite.h, NUISystem_CManager.h: ENGINE_2D_LAYER_0=67 = 0x43); name-fix 2026-11-12, was ENGINE_2D_LAYER_HUD_EFFECTS
class CBaseObject;
class CBaseIntelligentPointer {                 // vtable 0x0129C95C (vptr + PData)
public:
    CBaseObject* GetPItem() const;              // 0x00A01B50
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
private:
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item);               // 0x00655318 (T = NParticleEngine::CParticleEmitter)
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0065533C
};
namespace NParticleEngine {
class CParticleEmitter;
class CParticleEmitterManager {
public:
    CParticleEmitter* Create2DEmitter(long type, const C2DVector& position, EEngine2DLayer layer, bool persistent,
                                      const CRightHandedSet& axes);   // 0x00AEB790
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();      // 0x00AEAAF0
}
using NParticleEngine::CParticleEmitter;
class CHeroAbilityDef { public:                 // retail layout (Ego_r.pdb names, PDB offsets - 4)
    unsigned char _def00[0x48];
    long          IconEffectIndex;              // +0x48
    long          IconActiveEffectIndex;        // +0x4c
    C2DVector     IconEffectOffset;             // +0x50
};
namespace NPlayerGui {
class CDrawAbilityButtons {
public:
    unsigned char _pad00[0x48];                 // CDrawBase and the button state before the effects
    std::vector<CIntelligentPointer<CParticleEmitter> > Effects;   // +0x48
protected:
    void DrawEffects(long index, const C2DVector& position, const CHeroAbilityDef& ability);
};
void CDrawAbilityButtons::DrawEffects(long index, const C2DVector& position, const CHeroAbilityDef& ability)
{
    if (ability.IconEffectIndex > 0) {
        if (Effects[index].GetPItem() == 0) {
            CIntelligentPointer<CParticleEmitter> emitter(NParticleEngine::GetParticleEmitterManager().Create2DEmitter(
                ability.IconEffectIndex, position + ability.IconEffectOffset, ENGINE_2D_LAYER_0, true,
                CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f))));
            Effects[index] = emitter;
        }
    }
}
}
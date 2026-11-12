// NPlayerGui::CDrawQuickAccessIcons::DrawEffectAtPos(const C2DCoordI& pos, long effect, long index) -- keep the quick-access
// particle effect of slot `index` at the screen position `pos`: for a non-zero position, kill the slot's emitter when the position
// moved, and when the slot is empty create the 2D emitter `effect` at pos + CPlayerGuiDef::QuickAccessEffectOffset (ENGINE_2D_LAYER_0 =
// 0x43, upright axes) and remember the position; for a zero position, kill the slot's emitter and clear the remembered position.
// Leaf: ?DrawEffectAtPos@CDrawQuickAccessIcons@NPlayerGui@@AAEXABVC2DCoordI@@JJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064546F, 406 bytes, thiscall `ret 0Ch`; ebp frame (`sub esp,40h`) closed with `leave`.
// Pragmas: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`: the HUD TU's established mode (the landed NPlayerGui rows).
// Identity: Ego_r.pdb ?DrawEffectAtPos@CDrawQuickAccessIcons@NPlayerGui@@AAEXABVC2DCoordI@@JJ@Z (0x0068804A, private; between DrawIcon
//   and ClearEffects there as 0x0064546F sits between the landed DrawIcon 0x00645292 and 0x00645605 here); all five callers are in
//   CDrawQuickAccessIcons::Draw 0x00652FA4. Parameter names are descriptive (the PDB has none).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, NParticleEngine::CParticleEmitter::Kill 0x00AEE8C0,
//   CIntelligentPointer<NParticleEngine::CParticleEmitter>(CParticleEmitter*) 0x00655318 / operator= 0x0065533C,
//   ~CBaseIntelligentPointer 0x00A01C10, CPlayerGui::PeekGuiDef 0x00438570 (static __fastcall),
//   CRightHandedSet(const C3DVector&, const C3DVector&) 0x00487050, NParticleEngine::GetParticleEmitterManager() 0x00AEAAF0
//   (FableWin.pdb, __fastcall, by reference), CParticleEmitterManager::Create2DEmitter(long, const C2DVector&, EEngine2DLayer, bool,
//   const CRightHandedSet&) 0x00AEB790. Global: DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Layout: CDrawQuickAccessIcons::Effects (Ego_r.pdb vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >) at retail +0x7C
//   and EffectPositions (vector<C2DCoordI>) at +0x88 (PDB +0x88 / +0x98, Dinkumware vector sizes before them); both elements 8 bytes
//   (`shl edi,3`). CPlayerGuiDef::QuickAccessEffectOffset (C2DVector) at +0x6F0. No shared header exists for this class (pad model
//   like the landed siblings DrawIcon / ToggleCharge).
// Shape: the test is the inline `pos.ToC2DCoordF().IsNonZero()` (the float vector stays on the x87 stack, Y squared first, compared
//   with DELTA_NOTIONAL_ZERO squared; `test ah,5 ; jp`); `Effects[index]` is the real STLport `*(begin() + n)` (index*8 in edi, `this`
//   in esi, `lea ecx,[edi+eax]` hoisted above the branch for both arms' GetPItem). C2DCoordI::operator!= is inline (X then Y compare).
//   Assigning a raw pointer builds a CIntelligentPointer temporary (ctor 0x655318, operator= 0x65533C, dtor) -- `= 0` in both kill
//   paths and `= Create2DEmitter(...)` in the create path, whose temporary dies before the position is remembered. The create position
//   is a named C2DVector direct-initialised from `pos.ToC2DCoordF() + offset` through a named reference to the def's offset (operator+
//   builds Y + offset.Y first, then X + offset.X, straight into the local at [ebp-8], before the axes temporaries and the manager).
//   The zero arm clears with `C2DCoordI(0, 0)` (two zeroed registers).
// Headers: overlay opt-ins FABLE_RETAIL_C2DCOORDI_NOTEQUAL (engine/C2DCoordI.h) and FABLE_RETAIL_CPLAYERGUIDEF_QUICKACCESSEFFECTOFFSET
//   (engine/CPlayerGuiDef.h); IsNonZero expanded through FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_C2DCOORDI_TOFLOAT
#define FABLE_RETAIL_C2DCOORDI_NOTEQUAL
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_QUICKACCESSEFFECTOFFSET
#include "stlport/fable_containers.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CParticleEmitter.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/NUISystem_CManager.h"
namespace NParticleEngine {
CParticleEmitterManager& __fastcall GetParticleEmitterManager();      // 0x00AEAAF0
}
using NParticleEngine::CParticleEmitter;
namespace NPlayerGui {
class CDrawQuickAccessIcons {
public:
    unsigned char _pad00[0x7c];                                    // CDrawBase and the icon state before the effects
    std::vector<CIntelligentPointer<CParticleEmitter> > Effects;   // +0x7c
    std::vector<C2DCoordI> EffectPositions;                        // +0x88
private:
    void DrawEffectAtPos(const C2DCoordI& pos, long effect, long index);
};
void CDrawQuickAccessIcons::DrawEffectAtPos(const C2DCoordI& pos, long effect, long index)
{
    if (pos.ToC2DCoordF().IsNonZero()) {
        if (Effects[index].GetPItem() && pos != EffectPositions[index]) {
            Effects[index]->Kill();
            Effects[index] = 0;
        }
        if (!Effects[index].GetPItem()) {
            const C2DVector& offset = CPlayerGui::PeekGuiDef().QuickAccessEffectOffset;
            C2DVector position(pos.ToC2DCoordF() + offset);
            Effects[index] = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(
                effect, position, ENGINE_2D_LAYER_0, true,
                CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
            EffectPositions[index] = pos;
        }
    } else {
        if (Effects[index].GetPItem()) {
            Effects[index]->Kill();
            Effects[index] = 0;
            EffectPositions[index] = C2DCoordI(0, 0);
        }
    }
}
}
// NPlayerGui::CDrawMeleeActionButtonEffects::Draw(const CEnginePrimitivePrepareInfo& info) -- the melee action button's flourish: unless the special ability buttons show (then the flourish effect is dropped), while a flash or the combo loading icon is running and a flourish effect is set, mark the drawer as drawing, create the 2D flourish emitter once at the bottom-right anchored effect position and keep it in Effects[0], and while the emitter lives draw the flourish graphic sprite at that anchored position plus the graphic offset; with no flourish effect the emitters are cleared
// Leaf: ?Draw@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646580, 393 bytes, thiscall `ret 4`; ebp frame (`sub esp,94h`) closed with `leave`.
// Pragmas: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`: the HUD TU's established mode (the landed NPlayerGui rows).
// Identity: FableWin.pdb ?Draw@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z (public, non-virtual);
//   the CPlayerGui draw loop calls it on PDrawMeleeActionButtonEffects (+0x220) with the prepare info (0x00439769).
// Callees: CPlayerGui::PeekGuiDef 0x00438570 (static __fastcall), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   NPlayerGui::GFAnchorToBottomRight(const C2DVector&) 0x00643D0C (__fastcall, hidden result), CRightHandedSet(const C3DVector&,
//   const C3DVector&) 0x00487050, NParticleEngine::GetParticleEmitterManager() 0x00AEAAF0 (FableWin.pdb, __fastcall, by reference),
//   CParticleEmitterManager::Create2DEmitter(long, const C2DVector&, EEngine2DLayer, bool, const CRightHandedSet&) 0x00AEB790,
//   CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) 0x00655318 / operator= 0x0065533C, ~CBaseIntelligentPointer 0x00A01C10,
//   CDrawBase::ScaleAlpha(unsigned char) const 0x00643D5D (Ego_r.pdb ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z), the 16-parameter
//   CEnginePrimitive2DSprite position constructor 0x00493AC2, CIEngine::SetPersistentPrimitive (virtual slot 23, +0x5C; CEngine
//   0x00B23BC0) on info.Engine (+0x34), CDrawBase::ClearEffects(vector&) 0x006445BA (static __fastcall).
// Layout (retail offsets): CDrawBase PlayerGui +4, Drawing +0x18, PrimitiveHandles +0x1c (shared header); this class (no shared
//   header; Ego_r.pdb member names, PDB offsets less the STLport vector sizes): Effects +0x28, FlashTimer +0x34,
//   ComboLoadingIconTimer +0x38, FlourishCounter +0x3c, FlourishEffect +0x40 (PMeleeAbilityDef +0x44 follows, not used here);
//   CPlayerGui ShowSpecialAbilityButtons +0x250; CPlayerGuiDef AbilityButtonFlourishEffectPos +0x788, AbilityButtonFlourishGraphic
//   +0x790, AbilityButtonFlourishGraphicOffset +0x794.
// Shape:
//   * `if (!show) { ... } else FlourishEffect = 0;` -- retail puts the reset arm last (0x006466F2) and falls into the final test.
//   * The constant 0 lives in ebx for the whole function (cmp [..],ebx / push ebx).
//   * Emitter arm: the anchored position is a named local built before the emitter expression (GFAnchorToBottomRight is called
//     before the two C3DVector temporaries are stored); the expression itself is the landed DrawEffects 0x006451DD idiom.
//   * Sprite arm: the anchored position is a named local ([ebp-8], read in place), the sum `offset + anchored` (operand order: retail
//     loads the anchored component and adds the def's, `fld [ebp-8] ; fadd [eax]`) is kept in the x87 registers across the two uv
//     temporaries and stored into the sprite's by-reference position temporary ([ebp-10h]) only just before ScaleAlpha is called
//     (`fld st(1) ; fstp ; fst ; fstp st(0) ; fstp st(0)`): the position argument is a C2DVector copy of the named sum, as in the
//     landed DrawButton 0x00645099 (`C2DVector(position)`). INFERENCE: the copy's exact source spelling is not proven (an inline
//     pass-through returning C2DVector by value would give the same bytes); the behaviour is identical either way.
//   * GFAnchorToBottomRight's real body (landed row 0x00643D0C, defined earlier in this retail source file) is in the TU, out of
//     line (`#pragma auto_inline(off)`): with the body visible VC7.1 knows the callee does not keep the result pointer, so the
//     anchored local's frame slot is free after its last read and the second uv temporary reuses it ([ebp-8], 0x00646674). With a
//     bare declaration that temporary gets its own slot ([ebp-24h]) -- the only residue of the declaration-only spelling.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET_RIGHT_COMPONENTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CPLAYERGUIDEF_FLOURISH
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawBase.h"
extern CDisplayManager* GPDisplayManager;                             // 0x013B8390 ?GPDisplayManager@@3PAVCDisplayManager@@A
namespace NParticleEngine {
CParticleEmitterManager& __fastcall GetParticleEmitterManager();      // 0x00AEAAF0
}
using NParticleEngine::CParticleEmitter;
namespace NPlayerGui {
// 0x00643D0C, the landed row's real body (defined earlier in the same retail source file; called out of line by this row).
#pragma auto_inline(off)
C2DVector __fastcall GFAnchorToBottomRight(const C2DVector& pos)
{
    return C2DVector((float)GPDisplayManager->GetRenderTargetDimensions().X + pos.X - 640.0f,
                     (float)GPDisplayManager->GetRenderTargetDimensions().Y + pos.Y - 480.0f);
}
#pragma auto_inline(on)
class CDrawMeleeActionButtonEffects : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
protected:
    std::vector<CIntelligentPointer<CParticleEmitter> > Effects;   // +0x28
    long FlashTimer;                                               // +0x34
    long ComboLoadingIconTimer;                                    // +0x38
    long FlourishCounter;                                          // +0x3c
    long FlourishEffect;                                           // +0x40
};
void CDrawMeleeActionButtonEffects::Draw(const CEnginePrimitivePrepareInfo& info)
{
    if (!PlayerGui->IsShowSpecialAbilityButtons()) {
        if ((FlashTimer >= 0 || ComboLoadingIconTimer >= 0) && FlourishEffect > 0) {
            SetAsDrawing(true);
            const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
            if (FlashTimer >= 0 && Effects[0].GetPItem() == 0) {
                C2DVector position = GFAnchorToBottomRight(def.AbilityButtonFlourishEffectPos);
                CIntelligentPointer<CParticleEmitter> emitter(NParticleEngine::GetParticleEmitterManager().Create2DEmitter(
                    FlourishEffect, position, ENGINE_2D_LAYER_1, true,
                    CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f))));
                Effects[0] = emitter;
            }
            if (Effects[0].GetPItem() != 0) {
                C2DVector anchored = GFAnchorToBottomRight(def.AbilityButtonFlourishEffectPos);
                C2DVector position = def.AbilityButtonFlourishGraphicOffset + anchored;
                CEnginePrimitive2DSprite sprite(0, C2DVector(position), ENGINE_2D_LAYER_0, def.AbilityButtonFlourishGraphic, 0,
                    ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(0xff), 0xff, 0xff, 0xff, 0,
                    C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], sprite, 0xc0);
            }
        }
    } else {
        FlourishEffect = 0;
    }
    if (FlourishEffect == 0)
        ClearEffects(Effects);
}
}
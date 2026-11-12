// NPlayerGui::CDrawAbilityButtons::DrawButton(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index, long graphic, bool special, EEngine2DLayer layer) -- draw one ability button sprite into the primary or the special handle table: a negative alpha draws unscaled (fixed 160 buttonAlpha byte), the GUI alpha byte is forced to 255 while the special buttons show, and the sprite's alpha is the GUI-scaled buttonAlpha byte
// Grade: RELOCATION_MATCH (324v324)
// Leaf: ?DrawButton@CDrawAbilityButtons@NPlayerGui@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC2DVector@@MJJ_NW4EEngine2DLayer@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645099, length 324 bytes (frame pointer), `ret 0x1c`.
// Name: Ego_r.pdb ?DrawButton@CDrawAbilityButtons@NPlayerGui@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC2DVector@@MJJ_NW4EEngine2DLayer@@@Z
//   (protected; 7 parameters = retail `ret 1Ch`; [ebp+1Ch] is tested as the bool, [ebp+20h] pushed as the sprite layer).
// Callees: CPlayerGui::PeekGuiDef @ 0x00438570 (static `__fastcall`, result unused), __ftol2 @ 0x00BFEA70,
//   CDrawBase::ScaleAlpha(unsigned char) const @ 0x00643D5D (Ego_r ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z; body: byte * PlayerGui->Alpha
//   through __ftol2, `ret 4`; landed under the propagated GetFadedAlpha spellings -> NAME_FIXES), the 2D sprite constructor @ 0x00493AC2
//   (Ego_r ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z),
//   CIEngine::SetPersistentPrimitive, virtual slot 23 (+0x5C; engine/CIEngine.h, CEngine 0x00B23BC0 ?SetPersistentPrimitive@CEngine@@). Float pool: 0.0f @ 0x0122DEDC, 255.0f @ 0x01230014, 160.0f @ 0x0125A29C.
// Layout (retail offsets): CDrawBase PlayerGui +4, PrimitiveHandles +0x1c (STLport vector, 8-byte CEnginePrimitiveHandle);
//   CDrawAbilityButtons SpecialPrimitiveHandles +0x3c; CPlayerGui ShowSpecialAbilityButtons +0x250, Alpha +0x2a0 (engine/CPlayerGui.h:
//   PDB +0x258 / +0x2b4, the STLport vectors are 4 bytes smaller); CEnginePrimitivePrepareInfo Engine +0x34.
// Name-fix pass (2026-11-12): the local CEnginePrimitiveRenderer / Renderer / AddPrimitive stand-ins (and the local C2DVector,
//   CPlayerGui, sprite, handle and CDrawBase models) replaced by the shared headers, the 0x00646580 header set; the GUI alpha byte is
//   CPlayerGui::GetAlpha() (FableWin.pdb, inline).
// Shape: the sprite position is a by-reference C2DVector temporary copied from `position` while the arguments are evaluated;
//   the two uv vectors are temporaries; `(unsigned char)(long)buttonAlpha` feeds ScaleAlpha.
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
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
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
namespace NPlayerGui {

class CDrawAbilityButtons : public CDrawBase { public:
    long                   Columns;                        // +0x28
    CEnginePrimitiveHandle BackgroundPrimitiveHandle;      // +0x2c
    CEnginePrimitiveHandle InteractBorderPrimitiveHandle;  // +0x34
    std::vector<CEnginePrimitiveHandle> SpecialPrimitiveHandles;   // +0x3c
protected:
    void DrawButton(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index, long graphic,
                    bool special, EEngine2DLayer layer);
};

void CDrawAbilityButtons::DrawButton(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index,
                                     long graphic, bool special, EEngine2DLayer layer)
{
    CPlayerGui::PeekGuiDef();
    std::vector<CEnginePrimitiveHandle>* handles = special ? &SpecialPrimitiveHandles : &PrimitiveHandles;
    bool scaled = true;
    if (alpha < 0.0f) {
        scaled = false;
        alpha = -alpha;
    }
    unsigned char guiAlpha = PlayerGui->GetAlpha();
    if (PlayerGui->IsShowSpecialAbilityButtons() && guiAlpha < 255)
        guiAlpha = 255;
    if (graphic > 0 && alpha > 0.0f) {
        float buttonAlpha = alpha * (float)guiAlpha;
        if (!scaled)
            buttonAlpha = 160.0f;
        else if (PlayerGui->IsShowSpecialAbilityButtons() && guiAlpha < 255)
            buttonAlpha = special ? 255.0f : 0.0f;
        CEnginePrimitive2DSprite sprite(0, C2DVector(position), layer, graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0,
                                        0.0f, 0.0f, ScaleAlpha((unsigned char)(long)buttonAlpha), 0xff, 0xff, 0xff, 0,
                                        C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive((*handles)[index], sprite, 0xc0);
    }
}

}
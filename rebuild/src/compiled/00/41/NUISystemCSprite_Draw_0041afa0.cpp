// NUISystem::CSprite::Draw(engine, p_handle, layer, pprimitive_index, pparent) -- queue the sprite as a 2D sprite primitive: skip (clearing the child primitive or the sprite's own handle) on the second frame in a row that it is fully transparent or zero-zoomed; otherwise build its clip box from the override or component size times the relative zoom at the relative position (moved up-left by half when the position is the centre), snap the corners to whole pixels, pick the 2D layer and build the primitive from the sprite's own texture or from its bank graphic (starting the load of frame 0 first), as a child of p_handle or on the sprite's own handle
// Leaf: ?Draw@CSprite@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: none
// Retail-asm: 0x0041B177 `fistp dword ptr [esp+0Ch]` / 0x0041B17B `fild dword ptr [esp+0Ch]` (and 0x0041B187, 0x0041B197, 0x0041B1A7: the
//   four corner snaps) -- the engine's inline-asm float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1
//   emits no bare dword fistp from C++. No asm in this source.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041AFA0, 1455 bytes, `ret 14h`; CSprite vtable slot 2 (engine/NUISystem_CSprite.h).
// Identity: Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?Draw@CSprite@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z;
//   parameters and locals as Ego_d.pdb lists them (engine, p_handle, layer, pprimitive_index, pparent; is_independent, range
//   (C2DExtentsF), render_layer (EEngine2DLayer), clip_box, size, is_layerindependent, position, colour; move; primitive; primitive,
//   frame (CGraphicFrame), pinfo (const CGraphicInfoFrame*)), statements as its line table (lines 94..226).
// Callees: CIEngine slots 30 ClearChildPrimitive (+0x78), 28 SetChildPrimitive (+0x70), 23 SetPersistentPrimitive (+0x5C); own slots
//   106 IsPositionIsCenter, 101 IsIndependent, 104 IsLayerIndependent; CManager::GetInstance 0x0041E5F2 and its slot 36 GetLayer;
//   the two CEnginePrimitive2DSprite box constructors 0x0041BF60 (texture) / 0x0041BEB0 (bank graphic); the bank pointer's destructor
//   0x004194B5; CGraphicInfoFrame::BeginLoading 0x009FE620 and the inline GetFrame chain of the landed SetGraphicIndex 0x0041B550
//   (ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, CFrameData::BeginLoading 0x009FDE80, CFrameData ctor 0x009FE380,
//   InternalGetFrameForcedLoading 0x009FF6B0, CGraphicFrameData::Uninitialise 0x009FCBA0). Inline: CEnginePrimitiveHandle::Clear,
//   C2DBoxF(const C2DVector&, const C2DExtentsF&), C2DBoxF::Move, GFFloatToLongNear.
// Fidelity: the TU also emits the two constructor COMDATs it calls; from this file both grade MATCH against retail
//   (0x0041BF60 167 bytes -- the body is a new opt-in of engine/CEnginePrimitive2DSprite.h; 0x0041BEB0 171 bytes).
// Inferred: Handle.Clear() is expanded in place in retail (0x0041B022..0x0041B057). VC7.1 keeps the plain-inline Clear as a call here
//   (1404 bytes against 1455), and no /d2inlT window gives retail: T75 changes nothing, T80 / T88 expand the bank pointer's
//   destructor instead (retail calls 0x004194B5) and still call Clear (1438), T100 expands both (1489). Hence the existing
//   FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR opt-in (same text, forced).
// The texture test is `PTexture.Get()`: Ego_d line 156 is 25 bytes, a call whose pointer result is tested (this text at /Od: 25;
//   `!PTexture.IsNull()` is 28, with the bool widening), and FableWin's ui_sprite.obj is the object that carries
//   ?Get@?$CCountedPointer@VCTexture@@@@QBEPAVCTexture@@XZ (and ??D, the `*PTexture` of the constructor argument). Same retail bytes.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_MOVE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_BOX_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CTEXTURE
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CGraphicInfo.h"
#include "engine/GFFloatToLongNear.h"

namespace NUISystem {
void CSprite::Draw(CIEngine& engine, CEnginePrimitiveHandle* p_handle, long layer, long* pprimitive_index, CComponent* pparent)
{
    if (RenderColour.A <= 0 || (RenderZoom.X <= 0.0f && RenderZoom.Y <= 0.0f))
    {
        if (NoRenderNextFrame == 1)
        {
            if (p_handle)
            {
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            }
            else
            {
                Handle.Clear();
            }

            return;
        }
        else
        {
            NoRenderNextFrame = 1;
        }
    }
    else
    {
        NoRenderNextFrame = 0;
    }

    C2DVector size;
    size.X = (Width == 0) ? Size.X : (float)Width;
    size.Y = (Height == 0) ? Size.Y : (float)Height;

    // Inferred: the order position / range / clip_box / colour is the one the Ego_d line sizes fit (line 126 21 bytes, 132 70, 137 16,
    //   138 21) and the only one of the three orders tried that matches (colour first: 1455 bytes, 55 instructions off; colour before
    //   clip_box: 1459 bytes). The layer is a conditional over two whole GetLayer calls (line 154, 136 bytes; one GetLayer call over a
    //   conditional argument is 1513 bytes).
    C2DVector position = RelativeRenderPosition;
    C2DExtentsF range(size.X * RelativeRenderZoom.X, size.Y * RelativeRenderZoom.Y);
    C2DBoxF clip_box(position, range);
    CRGBColour colour = RenderColour;

    if (IsPositionIsCenter())
    {
        C2DVector move(position.X - range.X * 0.5f, position.Y - range.Y * 0.5f);
        clip_box.Move(move);
    }

    clip_box.TLX = (float)GFFloatToLongNear(clip_box.TLX);
    clip_box.TLY = (float)GFFloatToLongNear(clip_box.TLY);
    clip_box.BRX = (float)GFFloatToLongNear(clip_box.BRX);
    clip_box.BRY = (float)GFFloatToLongNear(clip_box.BRY);

    bool is_independent = IsIndependent();
    bool is_layerindependent = IsLayerIndependent();

    EEngine2DLayer render_layer = (!is_independent && !is_layerindependent) ? CManager::GetInstance()->GetLayer(Layer + layer) : CManager::GetInstance()->GetLayer(Layer);

    if (PTexture.Get())
    {
        CEnginePrimitive2DSprite primitive(0, clip_box, render_layer, *PTexture, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, Angle, Angle,
                                           colour.A, colour.R, colour.G, colour.B, Sprite2DFlag, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));

        if (p_handle && pprimitive_index)
        {
            engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0xc0);
        }
        else
        {
            engine.SetPersistentPrimitive(Handle, primitive, 0xc0);
        }
    }
    else
    {
        if (GraphicIndex > 0)
        {
            const CGraphicInfoFrame* pinfo = CManager::GetInstance()->GetGraphicsBank()->PeekGraphicInfoFrame(GraphicIndex, 0);
            CGraphicFrame frame;

            pinfo->BeginLoading(0, 0x10);
            pinfo->GetFrame(frame, 0, 0x11);
        }

        CEnginePrimitive2DSprite primitive(0, clip_box, render_layer, GraphicIndex, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, Angle, Angle,
                                           colour.A, colour.R, colour.G, colour.B, Sprite2DFlag, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));

        if (p_handle && pprimitive_index)
        {
            engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0xc0);
        }
        else
        {
            engine.SetPersistentPrimitive(Handle, primitive, 0xc0);
        }
    }
}
}
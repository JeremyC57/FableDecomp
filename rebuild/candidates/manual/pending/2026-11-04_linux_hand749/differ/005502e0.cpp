// NUISystem::CMesh::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent) -- draw the UI mesh as a 2D static or animating mesh primitive clipped to its screen box, or clear it when invisible or off screen
// Grade: DIFFER(1321v1288) -- residue: zoom.X read straight from the member and only zoom.Y copied to a frame slot in retail (ours keeps both on x87); size/sum temporaries and the second box re-initialisation are x87 here, integer copies in retail; the early-release Clear inlines only with a do { } while (0) wrapper (frequency lever, retail construct unproven); 126 diff lines, 11 grades
// Leaf: ?Draw@CMesh@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: none
// Retail-asm: 0x00550468 `fistp dword ptr [esp+14h]` (and the seven later box-corner roundings, 0x00550478 .. 0x0055056C) -- the
//   engine's inline-asm float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1 emits no bare dword fistp
//   from C++ (NOTES.md "C++ attempts before helper").
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DANIMATIONSTATE
#include "engine/NUISystem_CMesh.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CIEngine.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DAnimationState.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/CEnginePrimitive2DStaticMesh.h"
#include "engine/CEnginePrimitive2DAnimatingMesh.h"
#include "engine/GFFloatToLongNear.h"
namespace NUISystem {
void CMesh::Draw(CIEngine& engine, CEnginePrimitiveHandle* parentHandle, long layer, long* childIndex, CComponent* parent)
{
    if (RenderColour.A <= 0 || (RenderZoom.X <= 0.0f && RenderZoom.Y <= 0.0f)) {
        if (NoRenderNextFrame == 1) {
            do {
            if (parentHandle)
                engine.ClearChildPrimitive(*parentHandle, (*childIndex)++);
            else
                Handle.Clear();
            } while (0);
            return;
        }
        NoRenderNextFrame = 1;
    } else
        NoRenderNextFrame = 0;
    {
    C2DVector position;
    position = RelativeRenderPosition;
    C2DVector zoom(RelativeRenderZoom);
    float width = zoom.X * Width;
    position.X -= 0.5f * width;
    float height = Height * zoom.Y;
    position.Y -= 0.5f * height;
    C2DVector size(width, height);
    unsigned char alpha = RenderColour.A;
    C2DBoxF screenBox(position, size + position);
    screenBox.BRX = (float)GFFloatToLongNear(screenBox.BRX);
    screenBox.BRY = (float)GFFloatToLongNear(screenBox.BRY);
    screenBox.TLX = (float)GFFloatToLongNear(screenBox.TLX);
    screenBox.TLY = (float)GFFloatToLongNear(screenBox.TLY);
    if (screenBox.TLX >= CUIScreenUnitConverter::GetBackBufferWidth() || screenBox.TLY >= CUIScreenUnitConverter::GetBackBufferHeight()
        || screenBox.BRX <= 0.0f || screenBox.BRY <= 0.0f)
        goto release;
    C2DBoxF clipBox(position, size + position);
    clipBox.BRX = (float)GFFloatToLongNear(clipBox.BRX);
    clipBox.BRY = (float)GFFloatToLongNear(clipBox.BRY);
    clipBox.TLX = (float)GFFloatToLongNear(clipBox.TLX);
    clipBox.TLY = (float)GFFloatToLongNear(clipBox.TLY);
    if (!CManager::GetInstance()->GetMeshBank()->EntryExists(MeshIndex))
        return;
    if (CManager::GetInstance()->GetMeshBank()->PeekMesh(MeshIndex)->GetDataType() == 5) {
        C3DAnimationBlendState blend;
        blend.SetBaseAnimation(C3DAnimationState(AnimationIndex, 0.0f));
        CEnginePrimitive2DAnimatingMesh primitive(0, MeshIndex, blend, clipBox, C3DBoundingSphere(C3DVector(0.0f, 0.0f, 0.0f), 1.0f),
                                                  C3DOrientationInterpolator(Orientation, Orientation), (EEngineInterpolationSet)0,
                                                  alpha, CRGBColour(255, 255, 255, 255));
        engine.SetPersistentPrimitive(Handle, primitive, 0x40);
    } else {
        CEnginePrimitive2DStaticMesh primitive(0, MeshIndex, clipBox, C3DOrientationInterpolator(Orientation, Orientation),
                                               (EEngineInterpolationSet)0, alpha, CRGBColour(255, 255, 255, 255));
        if (parentHandle && childIndex)
            engine.SetChildPrimitive(*parentHandle, (*childIndex)++, primitive, 0x40);
        else
            engine.SetPersistentPrimitive(Handle, primitive, 0x40);
    }
    return;
    }
release:
    Handle.Clear();
}
}

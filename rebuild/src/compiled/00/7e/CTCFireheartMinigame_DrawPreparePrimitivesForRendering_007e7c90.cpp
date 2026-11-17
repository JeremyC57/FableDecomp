// CTCFireheartMinigame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- drop last frame's primitives; while the delayed quit runs, cover the render target with a full-screen sprite
// Leaf: ?DrawPreparePrimitivesForRendering@CTCFireheartMinigame@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E7C90, 230 bytes, thiscall, `ret 4`; CTCFireheartMinigame vtable slot 11; Ego_r.pdb
//   ?DrawPreparePrimitivesForRendering@CTCFireheartMinigame@@UAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Callees: vector<CEnginePrimitiveHandle>::erase(first, last) 0x00440D70 (Handles.clear()), CDisplayManager::GetRenderTargetDimensions 0x009BEDC0
//   on GPDisplayManager 0x013B8390, CEnginePrimitive2DSprite(u64, const C2DBoxF&, layer 74, 0, 0, blend 2, set 0, 0, 0, 255, 0, 0, 0, 0,
//   uv, uv) 0x0041BEB0, the argument-less CArray<CEnginePrimitiveHandle>::push_back 0x005ED940, CIEngine slot 23 SetPersistentPrimitive (+0x5C)
//   on info.Engine (+0x34) with the new back() handle and flags 0x40.
// NF62 (2026-11-17): ported onto engine/CEnginePrimitive2DSprite.h / CIEngine.h / CDisplayManager.h / C2DBoxF.h / CEnginePrimitiveHandle.h /
//   CEnginePrimitivePrepareInfo.h / CTCBase.h / CArray.h (local CEnginePrimitiveHandle, CRetailVector, C2DVector, C2DBoxF, C2DExtentsI,
//   CDisplayManager, CEnginePrimitive2DSprite, renderer and prepare-info models deleted). CTCFireheartMinigame has no shared header: the
//   local class carries the Ego_r.pdb members (retail offsets: no STL member before Handles). Handles: Ego_r.pdb types it
//   vector<CEnginePrimitiveHandle>, but retail calls an argument-less push_back (0x005ED940: `T element; push_back(element);`), which only
//   CArray<T> has in either PDB (overlay opt-in FABLE_RETAIL_CARRAY_PUSHBACK_DEFAULT_DECL); CArray<T> is a public vector<T>, so clear()
//   still binds vector<CEnginePrimitiveHandle>::erase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY_PUSHBACK_DEFAULT_DECL
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIENGINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "engine/EEnginePrimitiveType.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CTCBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CDefPointer.h"
class CFireheartMinigameDef;
extern CDisplayManager* GPDisplayManager;                         // 0x013B8390 ?GPDisplayManager@@3PAVCDisplayManager@@A (no shared declaration)

class CTCFireheartMinigame : public CTCBase {
public:
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
private:
    CThing* Markers[4][4];                                         // +0x0C
    bool InSession;                                                // +0x4C
    bool ForceQuit;                                                // +0x4D
    CThing* CurrentMarker;                                         // +0x50
    bool Initialised;                                              // +0x54
    bool BlocksActive[4][4];                                       // +0x55
    long XPos;                                                     // +0x68
    long YPos;                                                     // +0x6C
    bool Moving;                                                   // +0x70
    CTCBase* PTCOldPhysics;                                        // +0x74
    CScriptGameResourceObjectScriptedThingBase ScriptData;         // +0x78
    bool QuitWithDelay;                                            // +0x88
    CArray<CEnginePrimitiveHandle> Handles;                        // +0x8C
    CDefPointer<const CFireheartMinigameDef> PDef;                 // +0x98
};

void CTCFireheartMinigame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    Handles.clear();
    if (QuitWithDelay)
    {
        C2DExtentsI size = GPDisplayManager->GetRenderTargetDimensions();
        C2DBoxF box(0.0f, 0.0f, (float)size.X, (float)size.Y);
        CEnginePrimitive2DSprite sprite(0, box, ENGINE_2D_LAYER_6, 0, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0,
            0.0f, 0.0f, 255, 0, 0, 0, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        Handles.push_back();
        info.Engine.SetPersistentPrimitive(Handles.back(), sprite, 0x40);
    }
}

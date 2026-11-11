// CTCEmoteIcon::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCEmoteIcon@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071B0C0, thiscall, `ret`.
// Identity: slot 8 of CTCEmoteIcon's vtable 0x01265ABC (defined here: the slot holds this body in 1 vtable(s), all of CTCEmoteIcon or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCEmoteIcon@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCEmoteIcon.h (opt-ins as in the landed row CTCEmoteIcon_DrawEmoteTypeIcon_0071ad20.cpp).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBAL
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/GFWorldDistance.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CEnginePrimitiveSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
bool CTCEmoteIcon::HasFrameUpdate() const
{
    return true;
}

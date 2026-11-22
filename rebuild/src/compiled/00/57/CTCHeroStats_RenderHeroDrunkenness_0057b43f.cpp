// CTCHeroStats::RenderHeroDrunkenness -- update the fullscreen overlay and its UV motion.
// Leaf: ?RenderHeroDrunkenness@CTCHeroStats@@QAEXPAVCIEngine@@@Z
// Flags: none
// Inline-budget: use the existing shared size-TU Clear owner at both observed call sites.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_DRUNKENNESS
#define FABLE_RETAIL_CTCHEROSTATS_RENDERDRUNKENNESS_PASS26
#define FABLE_RETAIL_CSPECIALABILITIESDRUNKENNESSDEF_CLASS_PASS26
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FORCEINLINE_RELEASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CVECTORMAP_LOWERBOUND_COUNT_FIRST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ISOPEN
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_INTERFACETYPE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_EXTENTS_BODY_PASS26
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CTCHeroStats.h"
#include "engine/_quarantine/CSpecialAbilitiesDrunkennessDef.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CWorld.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/GFCos.h"
#include "engine/GFSin.h"

extern double VIRTUAL_SCREEN_WIDTH;
extern double VIRTUAL_SCREEN_HEIGHT;
float FABLE_FASTCALL GFVXToAX(float position);
float FABLE_FASTCALL GFVYToAY(float position);

#pragma optimize("s", on)
void CTCHeroStats::RenderHeroDrunkenness(CIEngine* engine)
{
    if (GetWorld().GetGameScriptInterface().IsInMovieSequence()) {
        if (IsInCutSceneFirstTime) {
            IsInCutSceneFirstTime = false;
            PrimitiveHandle.Clear();
        }
        return;
    }

    CTCInventoryExperience* ptc_inventory_experience = 0;
    if (GetParentThing().GetTC(&ptc_inventory_experience) && ptc_inventory_experience->IsInventoryOpen()) {
        PrimitiveHandle.Clear();
        return;
    }

    IsInCutSceneFirstTime = true;
    float drunk = GetDrunkenness();
    if ((drunk > 0.0f || UpdateDrunkenness) && !PDef.IsNull()) {
        UpdateDrunkenness = false;
        C2DVector pos(0, 0);
        C2DExtentsF range(GFVXToAX((float)VIRTUAL_SCREEN_WIDTH), GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT));
        C2DBoxF clip_box(pos, range);

        float old_uoffset = (float)GFCos(UCounter);
        float old_voffset = (float)(GFCos(VCounter));
        float _tmpu = (float)(old_uoffset * GFCos(RotCounter) + old_voffset * GFSin(RotCounter));
        float _tmpv = (float)(old_uoffset * -GFSin(RotCounter) + old_voffset * GFCos(RotCounter));
        old_uoffset = _tmpu;
        old_voffset = _tmpv;

        UCounter += PDef->USpeed;
        VCounter += PDef->VSpeed;
        RotCounter += PDef->RotSpeed;
        while (UCounter > 1.0f)
            UCounter -= 1.0f;
        while (VCounter > 1.0f)
            VCounter -= 1.0f;
        while (RotCounter > 1.0f)
            RotCounter -= 1.0f;

        float uoffset = (float)GFCos(UCounter);
        float voffset = (float)GFCos(VCounter);
        _tmpu = (float)(uoffset * GFCos(RotCounter) + voffset * GFSin(RotCounter));
        _tmpv = (float)(uoffset * -GFSin(RotCounter) + voffset * GFCos(RotCounter));
        uoffset = _tmpu;
        voffset = _tmpv;

        float drunk_range = drunk / GetMaxDrunkenness();
        long alpha = (long)(PDef->MaxAlpha * drunk_range);
        if (alpha > 255)
            alpha = 255;

        CEnginePrimitive2DSprite primitive(0, clip_box, ENGINE_2D_LAYER_0, PDef->DrunkenGraphic, 0,
                                           ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
                                           0.0f, 0.0f, alpha, 255, 255, 255, 2,
                                           C2DVector(old_uoffset, old_voffset), C2DVector(uoffset, voffset));
        engine->SetPersistentPrimitive(PrimitiveHandle, primitive, 0x440);
    }
}
#pragma optimize("s", off)
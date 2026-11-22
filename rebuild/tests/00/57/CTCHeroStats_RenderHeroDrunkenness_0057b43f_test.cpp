// Standalone tests of the selected drunkenness render control flow.
#include <math.h>
#include <stdio.h>

#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "rebuild_abi.h"
#include "engine/GFCos.h"
#include "engine/GFSin.h"

double VIRTUAL_SCREEN_WIDTH = 640.0;
double VIRTUAL_SCREEN_HEIGHT = 480.0;
float FABLE_FASTCALL GFVXToAX(float position) { return position * 0.5f; }
float FABLE_FASTCALL GFVYToAY(float position) { return position * 0.25f; }

class CTCInventoryExperience {
public:
    CTCInventoryExperience() : Open(false) {}
    bool IsInventoryOpen() const { return Open; }
    bool Open;
};

class CThing {
public:
    CThing() : Experience(0), Lookups(0) {}
    bool GetTC(CTCInventoryExperience** result)
    {
        ++Lookups;
        if (Experience)
            *result = Experience;
        return Experience != 0;
    }
    CTCInventoryExperience* Experience;
    int Lookups;
};

class CGameScriptInterface {
public:
    CGameScriptInterface() : Movie(false) {}
    bool IsInMovieSequence() const { return Movie; }
    bool Movie;
};

class CWorld {
public:
    CGameScriptInterface& GetGameScriptInterface() { return Script; }
    CGameScriptInterface Script;
};

class CSpecialAbilitiesDrunkennessDef {
public:
    CSpecialAbilitiesDrunkennessDef() : DrunkenGraphic(16), MaxAlpha(200),
        USpeed(0.25f), VSpeed(2.0f), RotSpeed(1.75f) {}
    long DrunkenGraphic, MaxAlpha;
    float USpeed, VSpeed, RotSpeed;
};

class DefinitionPointer {
public:
    DefinitionPointer() : Data(0) {}
    bool IsNull() const { return Data == 0; }
    const CSpecialAbilitiesDrunkennessDef* operator->() const { return Data; }
    const CSpecialAbilitiesDrunkennessDef* Data;
};

class CEnginePrimitiveHandle {
public:
    CEnginePrimitiveHandle() : Clears(0) {}
    void Clear() { ++Clears; }
    int Clears;
};

enum EEngine2DLayer { ENGINE_2D_LAYER_0 = 67 };
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_ALPHA = 2 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_DEFAULT = 0 };

class CEnginePrimitive2DSprite {
public:
    CEnginePrimitive2DSprite(unsigned long id, const C2DBoxF& clip, EEngine2DLayer layer,
                             unsigned long bank, unsigned long frame, EEnginePrimitiveBlendMode blend,
                             EEngineInterpolationSet interp, float startAngle, float endAngle,
                             unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue,
                             unsigned long flags, const C2DVector& oldUV, const C2DVector& newUV)
        : Clip(clip), Bank(bank), Alpha(alpha), OldUV(oldUV), NewUV(newUV)
    {
        FixedValues = id == 0 && layer == ENGINE_2D_LAYER_0 && frame == 0 &&
                      blend == ENGINE_PRIMITIVE_BLEND_ALPHA && interp == ENGINE_INTERPOLATION_SET_DEFAULT &&
                      startAngle == 0 && endAngle == 0 && red == 255 && green == 255 && blue == 255 && flags == 2;
    }

    C2DBoxF Clip;
    unsigned long Bank;
    unsigned char Alpha;
    C2DVector OldUV, NewUV;
    bool FixedValues;
};

class CIEngine {
public:
    CIEngine() : Calls(0), Handle(0), Flags(0), Alpha(0), SpriteOK(false), OldUV(0, 0), NewUV(0, 0) {}
    void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive2DSprite& primitive,
                                unsigned long flags)
    {
        ++Calls;
        Handle = &handle;
        Flags = flags;
        Alpha = primitive.Alpha;
        OldUV = primitive.OldUV;
        NewUV = primitive.NewUV;
        SpriteOK = primitive.FixedValues && primitive.Bank == 16 && primitive.Clip.TLX == 0 &&
                   primitive.Clip.TLY == 0 && primitive.Clip.BRX == 320 && primitive.Clip.BRY == 120;
    }

    unsigned Calls;
    CEnginePrimitiveHandle* Handle;
    unsigned long Flags;
    unsigned char Alpha;
    bool SpriteOK;
    C2DVector OldUV, NewUV;
};

class CTCHeroStats {
public:
    CTCHeroStats() : IsInCutSceneFirstTime(true), UpdateDrunkenness(false),
        Drunkenness(0), MaxDrunkenness(10), UCounter(0.75f), VCounter(0.5f), RotCounter(0.25f) {}
    void RenderHeroDrunkenness(CIEngine* engine);
    CWorld& GetWorld() { return World; }
    CThing& GetParentThing() { return Parent; }
    float GetDrunkenness() const { return Drunkenness; }
    float GetMaxDrunkenness() const { return MaxDrunkenness; }

    CWorld World;
    CThing Parent;
    DefinitionPointer PDef;
    CEnginePrimitiveHandle PrimitiveHandle;
    bool IsInCutSceneFirstTime, UpdateDrunkenness;
    float Drunkenness, MaxDrunkenness, UCounter, VCounter, RotCounter;
};

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

static bool Near(float actual, double expected)
{
    return fabs(actual - expected) < 0.0001;
}

static bool CheckUV(const C2DVector& uv, float u, float v, float r)
{
    double x = GFCos(u), y = GFCos(v);
    double cosAngle = GFCos(r), sinAngle = GFSin(r);
    return Near(uv.X, x * cosAngle + y * sinAngle) &&
           Near(uv.Y, y * cosAngle - x * sinAngle);
}

int main()
{
    CTCHeroStats hero;
    CIEngine engine;
    CTCInventoryExperience experience;
    CSpecialAbilitiesDrunkennessDef def;

    hero.World.Script.Movie = true;
    hero.RenderHeroDrunkenness(&engine);
    hero.RenderHeroDrunkenness(&engine);
    if (hero.IsInCutSceneFirstTime || hero.PrimitiveHandle.Clears != 1 ||
        hero.Parent.Lookups != 0 || engine.Calls != 0)
        return 1;

    hero.World.Script.Movie = false;
    hero.Parent.Experience = &experience;
    experience.Open = true;
    hero.RenderHeroDrunkenness(&engine);
    if (hero.IsInCutSceneFirstTime || hero.PrimitiveHandle.Clears != 2 || engine.Calls != 0)
        return 2;

    experience.Open = false;
    hero.PDef.Data = &def;
    hero.RenderHeroDrunkenness(&engine);
    if (!hero.IsInCutSceneFirstTime || hero.PrimitiveHandle.Clears != 2 || engine.Calls != 0)
        return 3;

    hero.Parent.Experience = 0;
    hero.PDef.Data = 0;
    hero.UpdateDrunkenness = true;
    hero.RenderHeroDrunkenness(&engine);
    if (!hero.UpdateDrunkenness || engine.Calls != 0)
        return 4;

    hero.PDef.Data = &def;
    hero.RenderHeroDrunkenness(&engine);
    if (hero.UpdateDrunkenness || engine.Calls != 1 || engine.Alpha != 0 ||
        !engine.SpriteOK || engine.Handle != &hero.PrimitiveHandle || engine.Flags != 0x440 ||
        hero.UCounter != 1.0f || hero.VCounter != 0.5f || hero.RotCounter != 1.0f ||
        !CheckUV(engine.OldUV, 0.75f, 0.5f, 0.25f) || !CheckUV(engine.NewUV, 1.0f, 0.5f, 1.0f))
        return 5;

    hero.Drunkenness = 6.375f;
    hero.RenderHeroDrunkenness(&engine);
    if (engine.Calls != 2 || engine.Alpha != 127 || hero.UCounter != 0.25f || hero.RotCounter != 0.75f ||
        !CheckUV(engine.OldUV, 1.0f, 0.5f, 1.0f) || !CheckUV(engine.NewUV, 0.25f, 0.5f, 0.75f))
        return 6;

    hero.Drunkenness = 20.0f;
    hero.RenderHeroDrunkenness(&engine);
    if (engine.Calls != 3 || engine.Alpha != 255)
        return 7;

    hero.Drunkenness = -0.5f;
    hero.RenderHeroDrunkenness(&engine);
    hero.UpdateDrunkenness = true;
    hero.RenderHeroDrunkenness(&engine);
    if (engine.Calls != 4 || engine.Alpha != (unsigned char)-10 || hero.UpdateDrunkenness)
        return 8;

    puts("0057b43f_TEST PASS");
}
// CTCBurntEffect::FrameUpdate() -- fade the burn tint in over FadeInTime; at FadeOutTimeStart fade out or kill every live
// effect in Effects, then fade the tint back to white over FadeOutTime; remove the component once the fade-out ends
// Leaf: ?FrameUpdate@CTCBurntEffect@@UAEXXZ
// Retail 0x007CD9A0, 0x27A bytes (plain `ret`), vtable 0x0126EFBC slot 9. Callees: __ftol2 0x00BFEA70,
// NGraphicAppearance::CTCGraphicAppearance::SetTintColour(const CRGBColour&, void*) 0x004C00B0, CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&) 0x0072ABD0 (static __fastcall),
// CThing::RemoveTC(ETCInterfaceType) 0x004C9840 (TCI_BURNT_EFFECT = 137). Constants: 1.0f 0x0122DED8, 255.0f 0x01230014.
// Layout: the landed CTCBurntEffect ctor 0x007CDFE0 / dtor 0x007CDC20 (PDef +0x0c, CurTime +0x10, FadeInTime +0x14,
// FadeOutTimeStart +0x18, FadeOutTime +0x1c, Effects +0x20, TintList +0x2c); the def's Color vector<long> at +0x48.
// Shape: the tint block appears twice (fade in / fade out) written out in place (FableWin.pdb lists no tint helper on CTCBurntEffect); colour built with the
// 4-argument CRGBColour(r,g,b,a) ctor, so VC evaluates the channel expressions right to left (c[3] first). The kill loop is
// the dtor's spelling (IS_THING_ALIVE on the element, then FadeOrKillEffect).
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_DIVINE_FINALCLEANUP
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT
#include "engine/CRGBColour.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"

class CSpecialAbilitiesBurntEffectDef : public CParentDefClassBase { public:   // no engine header; RTTI base CParentDefClassBase (packed 0x39)
    float FadeInTimeSecs;                                   // +0x3c
    float FadeOutTimeStartSecs;                             // +0x40
    float FadeOutTimeSecs;                                  // +0x44
    std::vector<long> Color;                                // +0x48
};

class CTCBurntEffect : public CTCBase { public:            // vtable 0x0126EFBC, no engine header
    CDefPointer<const CSpecialAbilitiesBurntEffectDef>  PDef;                       // +0x0C
    long                                                CurTime;                    // +0x10
    long                                                FadeInTime;                 // +0x14
    long                                                FadeOutTimeStart;           // +0x18
    long                                                FadeOutTime;                // +0x1C
    std::vector<CIntelligentPointer<CThing> >           Effects;                    // +0x20
    std::vector<CIntelligentPointer<CThing> >           TintList;                   // +0x2C
    virtual void FrameUpdate();
};

void CTCBurntEffect::FrameUpdate()
{
    CThing* parent = ParentThing;
    if (CurTime <= FadeInTime) {
        float amount = (float)CurTime / (float)FadeInTime;
        if (FadeInTime == 0)
            amount = 1.0f;
        float white = (float)(long)((1.0f - amount) * 255.0f);
        const long* c = &PDef->Color[0];
        CRGBColour colour((unsigned char)(long)(c[0] * amount + white), (unsigned char)(long)(c[1] * amount + white),
                          (unsigned char)(long)(c[2] * amount + white), (unsigned char)(long)(c[3] * amount + white));
        if (parent->GetTCGraphicAppearance())
            parent->GetTCGraphicAppearance()->SetTintColour(colour, this);
        for (std::vector<CIntelligentPointer<CThing> >::iterator it = TintList.begin(), end = TintList.end(); it != end; ++it) {
            CThing* thing = it->Get();
            if (thing && !thing->IsDead()) {
                NGraphicAppearance::CTCGraphicAppearance* appearance = thing->GetTCGraphicAppearance();
                if (appearance)
                    appearance->SetTintColour(colour, this);
            }
        }
    } else if (CurTime >= FadeOutTimeStart) {
        if (CurTime == FadeOutTimeStart) {
            for (unsigned int i = 0; i < Effects.size(); ++i) {
                if (IS_THING_ALIVE(Effects[i]))
                    CTCSpecialAbilities::FadeOrKillEffect(Effects[i]);
            }
        }
        float amount = 1.0f - (float)(CurTime - FadeOutTimeStart) / (float)FadeOutTime;
        if (FadeOutTime == 0)
            amount = 1.0f;
        float white = (float)(long)((1.0f - amount) * 255.0f);
        const long* c = &PDef->Color[0];
        CRGBColour colour((unsigned char)(long)(c[0] * amount + white), (unsigned char)(long)(c[1] * amount + white),
                          (unsigned char)(long)(c[2] * amount + white), (unsigned char)(long)(c[3] * amount + white));
        if (parent->GetTCGraphicAppearance())
            parent->GetTCGraphicAppearance()->SetTintColour(colour, this);
        for (std::vector<CIntelligentPointer<CThing> >::iterator it = TintList.begin(), end = TintList.end(); it != end; ++it) {
            CThing* thing = it->Get();
            if (thing && !thing->IsDead()) {
                NGraphicAppearance::CTCGraphicAppearance* appearance = thing->GetTCGraphicAppearance();
                if (appearance)
                    appearance->SetTintColour(colour, this);
            }
        }
    }
    ++CurTime;
    if (CurTime > FadeOutTimeStart + FadeOutTime)
        parent->RemoveTC(TCI_BURNT_EFFECT);
}
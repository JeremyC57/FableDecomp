// CTCMultiStrike::ApplyEffectOnWeapon(CThing* weapon) -- dress a live weapon with the multi-strike effect of the current spell level: for every pair of the def's attachment points found on the weapon (helper point, else dummy object), create the short and long weapon effects, stretch the short one's splines onto the long one by the pair's length, attach it to the weapon, kill the long one and keep the short one as a particle effect
// Grade: DIFFER(1088v1088) -- Q5: 9 grades logged (7 hypotheses + baseline and final regrades; 19 in all with Q3/Q4), unchanged. Same length, ratio 0.942; the only residue is the reload order at the end of the alive block (retail `mov edi,[esp+34h]` then `mov esi,[esp+88h]` at 0x007CD314; ours esi then edi). c2prio (probes): the split pieces are weapon #21 (esi), IV #45 (edi), this #52 (ebp) and stay identical under every code-neutral spelling tried -- noPairs declared at the top, one `long a = 0, b = 0;`, no inner block, frame declared first, point declared outside, begin()+point indexing, `ratio + ratio`, `x * 2.0f`, const span, one combined continue condition, explicit null/IsDead tests for weapon and for the things (all graded or probed inert); `goto next` 1072, `point++` indexing 1068, `[i * 2]` indexing changes the code. The H40i rule (ascending final piece ids) predicts esi, edi, ebp here, not ours (ebp, esi, edi), so it does not explain this row. Next: find what puts the IV piece before the weapon piece (a different split-pass membership, not declaration order); likely a numbering lane gap.
// Leaf: ?ApplyEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CCF50, 1088 bytes, thiscall, `ret 4` (with its 4-entry jump table 0x007CD380).
// Identity: Ego_r.pdb / FableWin.pdb ?ApplyEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z (engine/CTCMultiStrike.h, called by the landed
//   CTCInventoryWeapons::SetupActiveWeaponsOnCarrier 0x005CA68E).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30, CCharString()
//   0x0099E4B0 / (const CCharString&) 0x0099EC30 / operator=(const char*) 0x0099EFE0 / ~ 0x0099EAE0, CTCGraphicAppearance::GetHelperPoint
//   0x004C0560 / GetDummyObject 0x004C08C0, CThing::GetPos 0x004C73D0 / Kill 0x004C9B80, CTCDParticleEmitter::Create(long, const
//   C3DVector&, bool) 0x006E0880 / AttachToThing 0x006E0BE0, CVectorMap::LowerBound 0x0040F020 (inline GetTC), GetPItem 0x00A01B50,
//   CParticleEmitter::GetApproximateTotalSplineLength 0x00AED050 / InterpolateTo 0x00AED600, CIntelligentPointer<CThing>(CThing*)
//   0x004A96F0 / ~ 0x00A01C10, vector<CIntelligentPointer<CThing> >::push_back 0x004AD520.
// Data: "WEAPON_EFFECT_SHORT_LEVEL1..4" / "WEAPON_EFFECT_LONG_LEVEL1..4" (0x0126EE88..0x0126EF4C); spell levels 1..3 pick LEVEL2..4,
//   anything else LEVEL1 (jump table 0x007CD380: 0 -> default, 1 -> 0x007CCFBD, 2 -> 0x007CCFD2, 3 -> 0x007CCFE7).
// Headers (overlay, FABLE_HDR_OVERLAY=$A/hdr): opt-ins FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE (engine/CParticleEmitter.h) and
//   FABLE_RETAIL_CTCDPARTICLEEMITTER_GETPEMITTER_INLINE (engine/CTCDParticleEmitter.h).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCMULTISTRIKE_MEMBERS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_GETPEMITTER_INLINE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialAbilitiesMultiStrikeDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CParticleEmitter.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCMultiStrike.h"

void CTCMultiStrike::ApplyEffectOnWeapon(CThing* weapon)
{
    if (!IS_THING_ALIVE(weapon))
        return;
    long shortEffect = 0;
    long longEffect = 0;
    CDefPointer<const CSpecialEffectsDef> effectsDef;
    if (PDef->GetSubDef(&effectsDef)) {
        CCharString shortName;
        CCharString longName;
        switch (SpellLevel) {
        case 1:
            shortName = "WEAPON_EFFECT_SHORT_LEVEL2";
            longName = "WEAPON_EFFECT_LONG_LEVEL2";
            break;
        case 2:
            shortName = "WEAPON_EFFECT_SHORT_LEVEL3";
            longName = "WEAPON_EFFECT_LONG_LEVEL3";
            break;
        case 3:
            shortName = "WEAPON_EFFECT_SHORT_LEVEL4";
            longName = "WEAPON_EFFECT_LONG_LEVEL4";
            break;
        case 0:
            shortName = "WEAPON_EFFECT_SHORT_LEVEL1";
            longName = "WEAPON_EFFECT_LONG_LEVEL1";
            break;
        default:
            shortName = "WEAPON_EFFECT_SHORT_LEVEL1";
            longName = "WEAPON_EFFECT_LONG_LEVEL1";
            break;
        }
        if (!effectsDef->GetEffectFromString(shortName, shortEffect))
            return;
        if (!effectsDef->GetEffectFromString(longName, longEffect))
            return;
        // the attachment points come in pairs: the short effect is attached at the first and stretched to the second
        long noPairs = PDef->AttachmentPoint.size() / 2;
        for (long i = 0, point = 0; i < noPairs; ++i, point += 2) {
            CCharString startPoint = PDef->AttachmentPoint[point];
            CCharString endPoint = PDef->AttachmentPoint[point + 1];
            C3DVector startPos;
            C3DVector endPos;
            CRightHandedSet frame;
            if (!weapon->GetTCGraphicAppearance()->GetHelperPoint(startPoint, 0, 0, startPos, 1.0f) &&
                !weapon->GetTCGraphicAppearance()->GetDummyObject(startPoint, 0, 0, startPos, frame, 1.0f))
                continue;
            if (!weapon->GetTCGraphicAppearance()->GetHelperPoint(endPoint, 0, 0, endPos, 1.0f) &&
                !weapon->GetTCGraphicAppearance()->GetDummyObject(endPoint, 0, 0, endPos, frame, 1.0f))
                continue;
            {
                CThing* shortThing = CTCDParticleEmitter::Create(shortEffect, weapon->GetPos(), true);
                CThing* longThing = CTCDParticleEmitter::Create(longEffect, weapon->GetPos(), true);
                if (IS_THING_ALIVE(shortThing) && IS_THING_ALIVE(longThing)) {
                    CTCDParticleEmitter* shortDriver = 0;
                    CTCDParticleEmitter* longDriver = 0;
                    shortThing->GetTC(&shortDriver);
                    longThing->GetTC(&longDriver);
                    NParticleEngine::CParticleEmitter* shortEmitter = shortDriver->GetPEmitter();
                    NParticleEngine::CParticleEmitter* longEmitter = longDriver->GetPEmitter();
                    longEmitter->Sleep();
                    C3DVector span = startPos - endPos;
                    float longLength = longEmitter->GetApproximateTotalSplineLength();
                    float shortLength = shortEmitter->GetApproximateTotalSplineLength();
                    shortEmitter->InterpolateTo(*longEmitter, 2.0f * ((span.GetMagnitude() - shortLength) / (longLength - shortLength)));
                    shortEmitter->SetAlwaysUpdate(true);
                    shortDriver->AttachToThing(*weapon, 10, startPoint, 0, 0.0f);
                    longThing->Kill(true);
                    CIntelligentPointer<CThing> effect(shortThing);
                    ParticleEffects.push_back(effect);
                }
            }
        }
    }
}

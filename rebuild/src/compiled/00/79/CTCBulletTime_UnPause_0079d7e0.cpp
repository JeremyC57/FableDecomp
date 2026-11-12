// CTCBulletTime::UnPause() -- resume a paused bullet time: restart the looping sound, recreate the "SLOWTIME" 2D screen particle
// effect if the looping one has gone (and mark it always-update), fade the screen filter back in (the parent thing bypasses it) and
// let everything the parent thing carries bypass the filter too.
// Leaf: ?UnPause@CTCBulletTime@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079D7E0, 732 bytes, thiscall plain `ret`, plain /O2 (no EH frame, no pragma).
// Identity: Ego_r.pdb ?UnPause@CTCBulletTime@@QAEXXZ (0x007A3400, beside Pause / ImmediateCancel); manifest label agrees; callers in
//   CTCSpecialAbilities (0x0072C00A / 0x0072C1AC) test Paused (+0x55) and call Pause 0x0079CF30 / this.
// Callees: CDefStringTable::GetString 0x009D49B0 (CDefString conversion expanded, GDefStringTable 0x013CA828); CTCBulletTime::PlaySoundW
//   0x0079CE80; ~CCharString 0x0099EAE0 / CCharString(const char*, long) 0x0099EBF0 ("SLOWTIME"); CBaseIntelligentPointer GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / ctor 0x00A01B10 / dtor 0x00A01C10; CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580
//   (body visible in the TU, called out of line; see the note at the body); CSpecialEffectsDef::GetEffectFromString 0x00669D30 (same-TU noinline body);
//   GFVYToAY 0x009E1E40 / GFVXToAX 0x009E1E10 (__fastcall); NParticleEngine::GetParticleEmitterManager 0x00AEAAF0;
//   CParticleEmitterManager::Create2DEmitter 0x00AEB790; CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) 0x00655318 (out of
//   line); IS_THING_ALIVE<CIntelligentPointer<CParticleEmitter>> 0x0079DC20; vector<CThingGraphicArgPasser>::push_back 0x006A3C70 /
//   ~vector 0x006A3770; CTCBase::GetWorld 0x00686A70; CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo 0x006B85D0;
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (GetTC expanded); CTCCarrying::SetAllCarriedThingsAsBypassFilter 0x006A2520.
// Shape: the emitter is held in a NAMED CIntelligentPointer block local (the out-of-line ctor's result is not reused: `lea ecx`
//   after the call), the interpolator is a named reference fetched before the eight arguments (GetWorld and the +0x18/+0x1964 chain
//   come before the PDef loads), and the "SLOWTIME" CCharString is a conditional temporary of the && (flag word stored at entry).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
#define FABLE_RETAIL_CTCBULLETTIME_UNPAUSE
#define FABLE_RETAIL_CTCCARRYING_SETBYPASSFILTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CParticleEmitter.h"
#include "engine/NUISystem_CManager.h"            // the full PDB EEngine2DLayer (ENGINE_2D_LAYER_SPELLS = 82)
#include "engine/CParticleEmitterManager.h"
#include "engine/ThingAlive.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCCarrying.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"

namespace NParticleEngine {
CParticleEmitterManager& __fastcall GetParticleEmitterManager();      // 0x00AEAAF0
}
float __fastcall GFVXToAX(float x);                                   // 0x009E1E10
float __fastcall GFVYToAY(float y);                                   // 0x009E1E40
extern double VIRTUAL_SCREEN_WIDTH;                                   // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                  // 0x013961F0

// CDefClassBase::GetSubDef<T>: the template's real body, copied from the landed row 0x00669580
//   (rebuild/src/compiled/00/66/CDefClassBase_GetSubDefCSpecialEffectsDef_00669580.cpp, same FableWin.pdb member set; CDefClassBase.h
//   declares the template only). Retail's TU saw this body but still called the instantiation out of line (0x00669580, call at
//   in the `effects &&` test): with only the declaration, VC7.1 assumes the call may write the `effects` local and re-reads it, so the frame
//   grows to 0x4C, ebp is not used to cache `effects` (retail `push ebp ; mov ebp,[esp+14h]` after the call) and 45 hunks differ.
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
// CIntelligentPointer<NParticleEngine::CParticleEmitter>(CParticleEmitter*): out of line in retail at 0x00655318 (landed row); declared as
//   an explicit specialization only (no body), because retail does not reuse its result (`lea ecx` after the call).
template <> CIntelligentPointer<NParticleEngine::CParticleEmitter>::CIntelligentPointer(NParticleEngine::CParticleEmitter* pointer);
void CTCBulletTime::UnPause()
{
    if (Pausable && Paused) {
        Paused = false;
        LoopSound = PlaySoundW(PDef->LoopSound, true);
        if (!m_tpLoopingEffect.Get() || m_tpLoopingEffect.Get()->IsDead()) {
            CDefPointer<const CSpecialEffectsDef> effects;
            long effect = 0;
            if (PDef->GetSubDef(&effects) && effects->GetEffectFromString("SLOWTIME", effect)) {
                {
                    CIntelligentPointer<NParticleEngine::CParticleEmitter> emitter(NParticleEngine::GetParticleEmitterManager().Create2DEmitter(effect,
                        C2DVector(GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) * 0.5f, GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) * 0.5f),
                        ENGINE_2D_LAYER_SPELLS /* 82, `push 52h` */, true, CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f))));
                    m_tpLoopingEffect = emitter;
                }
                if (IS_THING_ALIVE(m_tpLoopingEffect))
                    m_tpLoopingEffect.Get()->SetAlwaysUpdate(true);
            }
        }
        NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers bypass;
        bypass.push_back(NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser(&GetParentThing(), true));
        CEnvironmentSpellEffectInterpolator& interpolator = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
        ScreenFilterKey = interpolator.ScreenFilterFadeTo(0.0f,
            NScreenEffect::SCREEN_EFFECT_PRIORITY_SPELL,         // 2 (the FULL branch's enumerator, added to the ARGPASSER enum)
            PDef->ScreenFilterAlpha[m_nLevel], PDef->ScreenFilterSaturation[m_nLevel], PDef->ScreenFilterContrast[m_nLevel],
            PDef->ScreenFilterBrightness[m_nLevel],
            CRGBFloatColour(PDef->ScreenFilterTintR[m_nLevel], PDef->ScreenFilterTintG[m_nLevel], PDef->ScreenFilterTintB[m_nLevel], 1.0f),
            bypass);
        CTCCarrying* carrying;
        if (GetParentThing().GetTC(TCI_CARRYING, &carrying))
            carrying->SetAllCarriedThingsAsBypassFilter(ScreenFilterKey);
    }
}
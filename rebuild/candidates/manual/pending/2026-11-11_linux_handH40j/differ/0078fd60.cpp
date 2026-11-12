// CTCDivineWrath::FrameUpdate() -- drive the Divine Wrath spell on its caster: while charging, drain stamina, mark the creatures in range and cancel on release / no stamina; on the release frame start the effects, sounds and camera shake; while the spell lasts, strike every marked creature in range at its own interval, keep the per-creature and ground effects alive, drop creatures that left the circle and handle the ones killed; when the duration ends, give every marked creature a last hit and finish the move
// Grade: SRC_FAIL -- chunk H40h first transcription; H40i (0 grades, compile probes only, >100 errors -> the first ~40 closed): hdr opt-ins FABLE_RETAIL_DIVINEWRATH_ACTIONS (the three actions are the 0x114 CCombatActionBase + a ctor; their temporaries are destroyed by ~CCombatActionBase 0x0062E900), FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED (0x0072D360 is ?SetPItemRemoved@CCreatureActionBase, Ego_r 0x00525E80, called on the ReleaseAndLoop action at 0x00790721), FABLE_RETAIL_CWILLBUTTONHELPERTIMER_STATE_INLINE (FableWin ?GetCurrentButtonHeldState, read in place at 0x0078FD91); new headers engine/CTCWillResponse.h, CThingFilter_CanBeHitByEpicSpell.h, CTCDRumble.h (0x007F1B70 = ?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@W4EQuakeStrength@@W4EQuakeLength@@_N@Z, Ego_r 0x007EB500); the row now uses FABLE_RETAIL_CTHING_GETTC_EXPANDED (0x0078FE33..0x0078FE5E is exactly that LowerBound shape) + _STATEACCESS / C2DBOXI_CTORS / NCREATUREMODE_QUERIES. HEAD layout of CTCDivineWrath.h is already retail (no layout opt-in).
// Next (remaining compile debt, H40i list): C3DVector Normalise / GetLengthSquared opt-ins (C3DVector.h has them under opt-ins); CTCHeroStats::ModifyStaminaBy 0x0057B1DB decl; CThingSearchTools GetAllThingsInRoughArea declaration branch (instantiations 0x00792E50 / 0x007930B0 / 0x007907C9 out of line; 0x0078FF76 builds the std::list head inline); GetSubDef(&effects); CTCHeroExperience::AddTrainableStatExperience (Ego_r 0x005896B0) + the experience def; CThing IsRemoved (bit 0 of +0x91) / IsCreature / GetCreatureDef / GetHit (vtable +0x104) / GetHeightZBelowAt / SetAsGameStatePersisted inline body (expanded at 0x00790630 / 0x007911DC, called at 0x00791B8D); CHitParameters 9-arg ctor (Ego_r 0x00790750) / AddDamageWithAttribute (Ego_r 0x0083EDD0) / SetEpicSpell; NEntityEvents::CEventHitBySpecialAbility(const CHitParameters&, EHeroAbility) Ego_r 0x008079A0; CTCGraphicAppearance::GetHelperPoint; interface 0x75 = TCI_VOLUME_CONTAINMENT_TRACKER (SetUpdateLightingChannels, as FinalCleanup); CTCNymph::StopFlyingAround (Ego_r 0x00806240); CTCCreatureModeManager IsModeActive / AddMode (interface 49 = TCI_ENTITY_MODE_MANAGER); std::map erase; the frame-random global 0x01375550; and the killed-creature tail (0x0079136D.., 0x00791CB6..) is still pseudo-code (HandleCreatureKilled). Next: declarations, then a first grade. Proven this pass (Ego_r / FableWin): CTCWillResponse::IsThingNotBloodyWillableAtAll / IsThingEpicSpellable / IsThingEpicPowerExplodable are `static bool __fastcall (const CThing*)` (?..@CTCWillResponse@@SI_NPBVCThing@@@Z; 0x007E45D0 / 0x007E46D0 / 0x007E4680); CCreatureAction_BuildUpDivineWrathSpell(CThingCreatureBase&, CCharString) 0x008BD170, CCreatureAction_ReleaseDivineWrathSpell(CThingCreatureBase&) 0x008BD1C0, CCreatureAction_ReleaseAndLoopDivineWrathSpell(CThingCreatureBase&) 0x008BD220 (references, not pointers); CHitParameters(CHitLocation*, CThing*, CThing*, const C3DVector&, const C3DVector&, bool, bool, bool, bool) 0x00789238, AddDamageWithAttribute(EDamageAttribute, float) 0x00868CA0, SetEpicSpell(bool) inline; CTCHeroStats::ModifyStaminaBy(long) 0x0057B1DB; NEntityEvents::CEventHitBySpecialAbility(const CHitParameters&, EHeroAbility) 0x00811830, CEventKilledCreature(const CThing&, const CThing&), CEventDied(CThing*, CThing*); CTCNymph::StopFlyingAround(long); CTCWife::Murdered(); CTCDExperienceOrb::Create(CThing&) (SI); CThingCreatureBase::GetHeightZBelowAt(const C3DVector&, bool, bool) const; the timer test is GetCurrentButtonHeldState() read in place ([ebp+30h], needs an inline body opt-in); PeekThingSearchTools is CTCBase's 0x00686D90; the FIRST-STRIKE search is std::list<CThingObject*> with CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell> (Ego_r instantiations 0x00798FE0 CThingCreatureBase / 0x00799240 CThingObject are adjacent; the draft's Sphere / CThing is wrong), that list is built and destroyed out of line (0x007926C0 / 0x00792240) where the creature lists are inline (malloc 0xC, 0x006F09D0); CThingFilter_CanBeHitByEpicSpell::operator() is 0x005A4D70 (Ego_r). Then the inline budget notes below.
// Inline budget (H40h): the HasTC tests expand as bit tests up to 0x00791676 and are CALLED (std::bitset<274>::test 0x00410DE0) from 0x00791999 on, and SetAsGameStatePersisted 0x0078DDB0 expands in the two strike loops (0x00790630, 0x007911DC) and is called in the final one (0x00791B8D): one spelling each, the budget does the rest. Missing declarations still to bind: CThingFilter_CanBeHitByEpicSpell, CreateCameraShake 0x007F1B70 (`__fastcall (const C3DVector&, EQuakeStrength, EQuakeLength, bool)`, edx = strength), the experience def read at 0x007903F1 (GetDef 0x0044C6B0 / 0x00792350; members +0x40 / +0x7C), the ReleaseAndLoop item setter 0x0072D360
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078FD60, 9268 bytes (ends 0x00792194), thiscall, `and esp,-8` / `sub esp,230h`, no EH frame.
// Identity: CTCDivineWrath vtable slot FrameUpdate; Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCDivineWrath@@UAEXXZ; callees FinishMove 0x0078F6E0,
//   UnsheatheWeapon 0x0078E9C0, SetPTarget 0x0078E820, Attackable 0x0078E7C0, ResetCreatureHitState 0x0078E900, PlaySoundW 0x0078EBF0,
//   StopSound 0x0078EC80 (manifest names; the catalog row name is right).
// Literals (not read back yet -- the addresses are the evidence): action name 0x0126BF90, release effects 0x0126BF80 / 0x0126BF68, experience
//   def 0x0126BF40, per-creature effect 0x0126BF30, helper point 0x012651BC, hit TC 0x0123D3AC, death message def 0x0126BF0C, death effect
//   0x0126BEF4; floats 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 0.5f 0x0122F160 (rounding the radius), 0x01230A08 (first-strike damage factor),
//   0x0126BF2C (range slack); the frame counter / random word 0x01375550; GDefStringTable 0x013CA828; the fast 1/sqrt table 0x013CE558.
// HEADER DEBT (differ draft only): the names marked UNVERIFIED are descriptive stand-ins for members / accessors whose header spelling was
//   not looked up in this pass; they must be replaced by the shared-header names before this can compile, let alone land.
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DIVINE_FINALCLEANUP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_DIVINEWRATH_ACTIONS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
#define FABLE_RETAIL_CWILLBUTTONHELPERTIMER_STATE_INLINE
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCDivineWrath.h"
#include "engine/CSpecialAbilitiesDivineWrathDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ThingAlive.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CThingFilter_CanBeHitByEpicSpell.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CTCWillResponse.h"
#include "engine/CTCDRumble.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CHitParameters.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CMessageEventManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CCreatureAction_BuildUpDivineWrathSpell.h"
#include "engine/CCreatureAction_ReleaseDivineWrathSpell.h"
#include "engine/CCreatureAction_ReleaseAndLoopDivineWrathSpell.h"

// The unit vector from the caster to `target` and the point one unit short of the target on that line (the strike comes from there).
// Retail expands the table-driven fast normalise (0x007904E8..0x00790587, 0x0079100D.., 0x00791A13..).
static inline void GetStrikeLine(const CThing* caster, const CThing* target, C3DVector& from, C3DVector& direction)
{
    direction = target->GetPos() - caster->GetPos();
    direction.Normalise();                              // UNVERIFIED name: the GFastOneOverSqrtTable body of engine/C3DVector.h
    from = target->GetPos() - direction;
}

void CTCDivineWrath::FrameUpdate()
{
    if (!DivineWrathActionInitialised)                      // 0x0078FD72
        return;
    Timer.FrameUpdate();                                    // 0x0078FD84
    CThingCreatureBase* caster = (CThingCreatureBase*)&GetParentThing();   // ebx, kept at [esp+6Ch]
    CWillButtonHelperTimer::EButtonHeldState state = Timer.GetCurrentButtonHeldState();                     // UNVERIFIED accessor: the timer's long at +0x14 (this+0x30), read at 0x0078FD91

    if (Finished) {                                         // 0x0078FD89
        if (NumFramesToWaitForFadeOut <= 0) {
            caster->RemoveTC(TCI_DIVINE_WRATH);             // 0x0078FDA1 `push 85h`
            return;
        }
        NumFramesToWaitForFadeOut--;
        return;
    }

    if (CurFramesForEnvChange < NumFramesForEnvChange) {    // 0x0078FDC1: still charging
        if (++CurFramesForEnvChange >= NumFramesForEnvChange)
            SetPTarget();                                   // 0x0078FDD9
        caster->SetCurrentAction(CCreatureAction_BuildUpDivineWrathSpell(*caster, CCharString("WILL_DIVINE_WRATH_BUILD_UP" /* 0x0126BF90, UNVERIFIED text */)));
        if (state != CWillButtonHelperTimer::BEING_HELD_DOWN) {                                     // 0x0078FE0E: the button went up before the spell was ready
            PlaySoundW(PDef->CancelSound[SpellLevel], false);   // 0x0078FE12
            caster->FinishCurrentAction();
            UnsheatheWeapon();
            FinishMove();
        } else {
            CTCHeroStats* stats;                            // uninitialised when the caster has none (retail reads [esp+24h], 0x0078FE63)
            caster->GetTC(TCI_HERO_STATS, &stats);         // `test byte ptr [ebx+20h],10h`, interface 4, LowerBound 0x0040F020
            long modify = StaminaModify;                    // 0x0078FE6D
            AccModifyStamina += PerFrameStaminaModify;
            while (AccModifyStamina > 1.0f) {               // 0x0078FE7F..0x0078FEA6
                AccModifyStamina -= 1.0f;
                modify++;
            }
            stats->ModifyStaminaBy(-modify);                // 0x0078FEB1
            if (stats->Stamina <= 0) {                      // 0x0078FEB6: out of stamina cancels as well
                PlaySoundW(PDef->CancelSound[SpellLevel], false);   // CDefString -> CCharString 0x00415DD0 (0x0078FECE)
                caster->FinishCurrentAction();              // retail cross-jumps this tail with the one above (0x0078FED3)
                UnsheatheWeapon();
                FinishMove();
            }
        }
        // 0x0078FEFF: mark every creature the circle reaches so it stops what it is doing
        float radius = PDef->RadiusOfEffect[SpellLevel];
        C3DVector position = caster->GetPos();
        long x = (long)position.X, y = (long)position.Y, reach = (long)(radius + 0.5f);
        C2DBoxI area(x - reach, y - reach, x + reach, y + reach);
        std::list<CThingCreatureBase*> creatures;           // 0x0078FF76 (malloc'd head)
        CThingFilter_OverlappingCircle circle(position, radius);        // [esp+90h..9Ch]
        CThingFilter_CanBeHitByEpicSpell epic;                          // the byte at [esp+13h]
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(area, CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(circle, epic), creatures);   // 0x00792E50
        for (std::list<CThingCreatureBase*>::iterator it = creatures.begin(); it != creatures.end(); ++it) {
            CThingCreatureBase* creature = *it;
            if (creature == caster) continue;                                           // 0x00790003
            if (CTCWillResponse::IsThingNotBloodyWillableAtAll(creature)) continue;     // 0x00790013
            if (!CTCWillResponse::IsThingEpicSpellable(creature)) continue;             // 0x00790022
            if (!Attackable(creature)) continue;                                        // 0x00790032
            if (NCreatureMode::IsUnderground(*creature)) continue;                      // 0x00790041
            if (LastFrameCreaturesHit.find(creature) != LastFrameCreaturesHit.end()) continue;   // 0x0079005D
            if (!IS_THING_ALIVE(creature)) continue;                                    // 0x00790072
            if (!(creature->GetHealth() > 0.0f)) continue;                              // +0xB4, 0x0079007F
            LastFrameCreaturesHit[creature].ActualCreature = CIntelligentPointer<CThingCreatureBase>(creature);   // 0x00790096
            LastFrameCreaturesHit[creature].FramesToGetHit = 0;                         // 0x007900CD
            NGraphicAppearance::CTCGraphicAppearance* appearance = creature->GetTCGraphicAppearance();   // +0x64
            if (appearance) {
                LastFrameCreaturesHit[creature].DefaultChannel = appearance->PeekLightingChannel();     // +0x40, 0x007900DF
                appearance->SetLightingChannel(SpellExclusionChannel, true, true);      // 0x00790108
            }
            if (creature->HasTC((ETCInterfaceType)0x75))                                // `test dword ptr [esi+2Ch],200000h`
                creature->GetTC<CTCBase>((ETCInterfaceType)0x75)->SetEnabledFlag(false);   // UNVERIFIED: byte +0x20 of interface 0x75 (0x0079013E)
        }
        return;
    }

    CDefPointer<const CSpecialEffectsDef> effects;          // [esp+60h], 0x0079016C
    PDef->GetSubDef(effects);                               // 0x00790173

    if (CurFramesForEnvChange == NumFramesForEnvChange) {   // 0x00790178: the release frame
        CurFramesForEnvChange++;
        if (!effects.IsNull()) {
            long effect = 0;
            if (effects->GetEffectFromString("DIVINE_WRATH_RELEASE" /* 0x0126BF80 */, effect))
                CTCSpecialAbilities::CreateParticleEffect(effect, caster->GetPos());    // 0x007901DB
            if (effects->GetEffectFromString("DIVINE_WRATH_RELEASE_GROUND" /* 0x0126BF68 */, effect))
                CTCSpecialAbilities::CreateParticleEffect(effect, caster->GetPos());    // 0x00790223
        }
        CTCDRumble::Create(caster->GetPos(), (EQuakeStrength)3, (EQuakeLength)0, false);   // 0x00790238
        PlaySoundW(PDef->ReleaseSound[SpellLevel], false);        // 0x00790261
        StopSound(LoopingSound);                                                        // 0x00790278
        LoopingSound = PlaySoundW(PDef->ReleaseLoopSound[SpellLevel], false);   // 0x007902A1
    }

    if (DurationOfSpellFrames > 0) {                        // 0x007902B5
        DurationOfSpellFrames--;
        float damage = PDef->DamagePerHit[SpellLevel];      // [esp+64h]
        float radius = PDef->RadiusOfEffect[SpellLevel];    // [esp+34h]
        C3DVector position = caster->GetPos();              // [esp+84h]
        long x = (long)position.X, y = (long)position.Y, reach = (long)(radius + 0.5f);
        C2DBoxI area(x - reach, y - reach, x + reach, y + reach);   // [esp+90h]

        if (!PerformedReleaseAction) {                      // 0x00790349
            PerformedReleaseAction = caster->SetCurrentAction(CCreatureAction_ReleaseDivineWrathSpell(*caster));   // 0x00790379
            if (PerformedReleaseAction) {
                if (caster->HasTC(TCI_HERO_EXPERIENCE)) {   // interface 0x68, `test ah,1` on [ebx+2Ch]
                    CTCHeroExperience* experience = caster->GetTC<CTCHeroExperience>(TCI_HERO_EXPERIENCE);
                    CDefPointer<const CHeroExperienceAwardDef> award;                   // UNVERIFIED def class (reads +0x40 and +0x7C)
                    CGameDefinitionManager::Get().GetDef("DIVINE_WRATH_EXPERIENCE" /* 0x0126BF40 */, award);   // 0x007903F1
                    experience->AddTrainableStatExperience(award->Category, award->Amount);   // 0x0079040D
                }
                // the first strike: everything the sphere reaches takes a reduced hit at once
                std::list<CThing*> things;                  // 0x00790427
                CThingFilter_OverlappingSphere sphere(position, radius);
                CThingFilter_CanBeHitByEpicSpell epic;
                PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(area, CFilter_Bind2<CThingFilter_OverlappingSphere, CThingFilter_CanBeHitByEpicSpell>(sphere, epic), things);   // 0x007930B0
                for (std::list<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
                    CThingCreatureBase* thing = (CThingCreatureBase*)*it;
                    if (!thing) continue;                                               // 0x007904A3
                    if (thing->IsRemoved()) continue;                                   // `test byte ptr [esi+91h],1`
                    if (CTCWillResponse::IsThingNotBloodyWillableAtAll(thing)) continue;
                    if (!CTCWillResponse::IsThingEpicSpellable(thing)) continue;
                    C3DVector from, direction;
                    GetStrikeLine(caster, thing, from, direction);
                    CHitParameters hit(0, caster, 0, from, direction, true, false, false, false);   // 0x007905CE
                    hit.AddDamageWithAttribute(9, damage * 0.5f /* 0x01230A08, UNVERIFIED value */);   // 0x007905EA
                    hit.SetMoveBackOnHit(false);                                        // [hit+23h] = 0
                    hit.SetEpicSpell(true);                                             // [hit+1Eh] = 1
                    bool explodable = false;                                            // [esp+13h]
                    if (thing->IsCreature() && !thing->GetCreatureDef()->CannotBeExploded && CTCWillResponse::IsThingEpicPowerExplodable(thing))   // 0x007905EF..0x00790621
                        explodable = true;
                    bool persisted = true;
                    if (explodable) {                                                   // keep the body in the world while it is hit
                        persisted = thing->IsGameStatePersisted();                      // bit 5 of +0x15C
                        thing->SetAsGameStatePersisted(false);                          // expanded, 0x00790630
                    }
                    thing->GetHit(hit);                                                 // vtable +0x104, 0x00790652
                    if (explodable)
                        thing->SetAsGameStatePersisted(persisted);                      // 0x00790660
                    if (thing->HasTC(TCI_ENTITY_EVENTS))                                // interface 0x42
                        thing->GetTC<CTCEntityEvents>(TCI_ENTITY_EVENTS)->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 0x12));   // 0x007906CC
                }
            }
        } else {
            CCreatureAction_ReleaseAndLoopDivineWrathSpell action(*caster);              // 0x00790707
            action.SetPItemRemoved(CIntelligentPointer<CThing>(PItemRemoved));                  // 0x0072D360, UNVERIFIED name
            caster->SetCurrentAction(action);                                           // 0x00790730
        }

        CTCDRumble::Create(caster->GetPos(), (EQuakeStrength)0, (EQuakeLength)0, false);   // 0x00790752
        std::list<CThingCreatureBase*> creatures;                                       // 0x00790759
        CThingFilter_OverlappingCircle circle(position, radius);
        CThingFilter_CanBeHitByEpicSpell epic;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(area, CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilter_CanBeHitByEpicSpell>(circle, epic), creatures);   // 0x007907C9

        // 0x007907E0: creatures that walked into the circle get an entry and a ground effect
        for (std::list<CThingCreatureBase*>::iterator it = creatures.begin(); it != creatures.end(); ++it) {
            CThingCreatureBase* creature = *it;
            if (creature == caster) continue;
            if (CTCWillResponse::IsThingNotBloodyWillableAtAll(creature)) continue;
            if (!CTCWillResponse::IsThingEpicSpellable(creature)) continue;
            if (!Attackable(creature)) continue;
            if (NCreatureMode::IsUnderground(*creature)) continue;
            MapOfCreaturesToPerCreatureInfo::iterator found = LastFrameCreaturesHit.find(creature);   // 0x0079083D
            if (found == LastFrameCreaturesHit.end()) {
                if (!IS_THING_ALIVE(creature)) continue;
                if (!(creature->GetHealth() > 0.0f)) continue;
                LastFrameCreaturesHit[creature].ActualCreature = CIntelligentPointer<CThingCreatureBase>(creature);
                LastFrameCreaturesHit[creature].FramesToGetHit = 0;
                long effect = 0;
                CCharString effectName("DIVINE_WRATH_HIT" /* 0x0126BF30 */);              // named: destroyed at 0x00790A6A
                if (!effects.IsNull() && effects->GetEffectFromString(effectName, effect)) {
                    C3DVector ground = creature->GetPos();                              // 0x007908EB
                    if (creature->GetTCGraphicAppearance())
                        creature->GetTCGraphicAppearance()->GetHelperPoint("HP_ROOT" /* 0x012651BC */, 0, 0, ground, 1.0f);   // 0x00790930
                    ground.Z = creature->GetHeightZBelowAt(ground, false, true);        // 0x00790949
                    LastFrameCreaturesHit[creature].Effect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, ground));   // 0x00790985
                    PerEffectInfo info;                                                 // 0x0079099D
                    info.Position = ground;
                    info.Effect = LastFrameCreaturesHit[creature].Effect;               // 0x007909DA
                    HitEffects.push_back(info);                                         // 0x007909ED
                }
                NGraphicAppearance::CTCGraphicAppearance* appearance = creature->GetTCGraphicAppearance();
                if (appearance) {
                    LastFrameCreaturesHit[creature].DefaultChannel = appearance->PeekLightingChannel();
                    appearance->SetLightingChannel(SpellExclusionChannel, true, true);  // 0x00790A2C
                }
                if (creature->HasTC((ETCInterfaceType)0x75))
                    creature->GetTC<CTCBase>((ETCInterfaceType)0x75)->SetEnabledFlag(false);   // 0x00790A62
            } else {
                if (!IS_THING_ALIVE(creature)) continue;                                // 0x00790A78
                CIntelligentPointer<CThing>& hitEffect = found->second.Effect;          // node +0x20
                if (IS_THING_ALIVE(hitEffect)) continue;                                // 0x00790A90
                long effect = 0;
                CCharString effectName("DIVINE_WRATH_HIT");
                if (!effects.IsNull() && effects->GetEffectFromString(effectName, effect)) {
                    C3DVector ground = creature->GetPos();
                    if (creature->GetTCGraphicAppearance())
                        creature->GetTCGraphicAppearance()->GetHelperPoint("HP_ROOT", 0, 0, ground, 1.0f);   // 0x00790B1F
                    ground.Z = creature->GetHeightZBelowAt(ground, false, true);
                    hitEffect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, ground));   // 0x00790B63
                    PerEffectInfo info;
                    info.Position = ground;
                    info.Effect = hitEffect;
                    HitEffects.push_back(info);                                         // 0x00790BB9
                }
            }
        }

        // 0x00790BE5: creatures that left the circle (or may no longer be hit) are released
        for (MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin(); it != LastFrameCreaturesHit.end(); ) {
            CIntelligentPointer<CThingCreatureBase> creature(it->second.ActualCreature);   // 0x00790BF7
            if (!creature.GetPItem() || creature->IsRemoved()) {                        // 0x00790C1A..0x00790C37
                ++it;
                continue;
            }
            float range = radius + creature->GetPhysicsDef()->Radius;                   // UNVERIFIED: [[creature+60h]+34h], 0x00790C93
            C3DVector offset = creature->GetPos() - caster->GetPos();
            if (!(range * range + 1.0f /* 0x0126BF2C */ < offset.GetLengthSquared())    // 0x00790CDE: still inside
                && !CTCWillResponse::IsThingNotBloodyWillableAtAll(creature.GetPItem())
                && CTCWillResponse::IsThingEpicSpellable(creature.GetPItem())) {
                ++it;                                                                   // 0x00790D27 (out of line)
                continue;
            }
            ResetCreatureHitState(creature);                                            // 0x00790D5A
            CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);                   // 0x00790D62
            if (creature.GetPItem() && !creature->IsRemoved()) {
                if (creature->GetTCGraphicAppearance())
                    creature->GetTCGraphicAppearance()->SetLightingChannel(it->second.DefaultChannel, true, true);   // 0x00790D9E
                if (creature->HasTC((ETCInterfaceType)0x75))
                    creature->GetTC<CTCBase>((ETCInterfaceType)0x75)->SetEnabledFlag(true);   // 0x00790DDD
                creature->SetBeingHitByDivineWrath(false);                              // UNVERIFIED: `and byte ptr [eax+15Dh],0BFh`
            }
            LastFrameCreaturesHit.erase(it++);                                          // 0x00790DF1..0x00790E37
        }

        // 0x00790E4E: a ground effect that died is started again
        for (VectorOfCreaturesToPerEffectInfo::iterator e = HitEffects.begin(); e != HitEffects.end(); ++e) {
            if (e->Effect.GetPItem() && !e->Effect->IsRemoved())
                continue;
            long effect = 0;
            CCharString effectName("DIVINE_WRATH_HIT");
            if (!effects.IsNull() && effects->GetEffectFromString(effectName, effect))
                e->Effect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, e->Position));   // 0x00790ECA
        }

        // 0x00790F21: strike every marked creature when its counter runs out; handle the dead
        for (MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin(); it != LastFrameCreaturesHit.end(); ) {
            CIntelligentPointer<CThingCreatureBase> creature(it->second.ActualCreature);
            if (creature.GetPItem() && !creature->IsRemoved()) {
                if (creature->HasTC(TCI_NYMPH))                                         // interface 0xF5
                    creature->GetTC<CTCNymph>(TCI_NYMPH)->StopFlyingAround(0);          // 0x00790FBE
                if (--it->second.FramesToGetHit <= 0) {                                 // 0x00790FC3
                    it->second.FramesToGetHit = (long)((float)GRandomFrameWord /* 0x01375550 */ * PDef->RandomFactorForNumSecsBetweenDamageHit) + NumFramesBetweenDamageHit;
                    C3DVector from, direction;
                    GetStrikeLine(caster, creature.GetPItem(), from, direction);
                    CHitParameters hit(0, caster, 0, from, direction, false, false, false, false);   // 0x00791106
                    hit.AddDamageWithAttribute(9, damage);
                    hit.SetMoveBackOnHit(false);                                        // +0x23
                    hit.SetPlayHitResponseOverride(false);                              // +0x22, UNVERIFIED
                    hit.SetEpicSpell(true);                                             // +0x1E
                    if (creature->HasTC(TCI_CREATURE_MODE_MANAGER)) {                   // interface 0x31
                        CTCCreatureModeManager* modes = creature->GetTC<CTCCreatureModeManager>(TCI_CREATURE_MODE_MANAGER);
                        if (!modes->IsModeActive((ECreatureMode)0x5f)) {                // 0x00791174
                            creature->FinishCurrentAction();
                            modes->AddMode(0x5f);                                       // 0x00791191
                        }
                    }
                    bool explodable = false;
                    if (!creature->GetCreatureDef()->CannotBeExploded && CTCWillResponse::IsThingEpicPowerExplodable(creature.GetPItem()))
                        explodable = true;
                    bool persisted = true;
                    if (explodable) {
                        persisted = creature->IsGameStatePersisted();
                        creature->SetAsGameStatePersisted(false);                       // expanded, 0x007911F1
                    }
                    creature->GetHit(hit);                                              // 0x0079120D
                    if (!creature->HasTC((ETCInterfaceType)0x8b))
                        creature->AddTC("CTCDivineWrathVictim" /* 0x0123D3AC, UNVERIFIED text */, true, 0);   // 0x00791248
                    creature->SetBeingHitByDivineWrath(true);                           // `or byte ptr [eax+15Dh],40h`
                    if (explodable)
                        creature->SetAsGameStatePersisted(persisted);                   // 0x00791272
                    if (creature->HasTC(TCI_ENTITY_EVENTS))
                        creature->GetTC<CTCEntityEvents>(TCI_ENTITY_EVENTS)->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 0x12));   // 0x007912EA
                }
            }
            if (creature.GetPItem() && !creature->IsRemoved() && creature->GetHealth() > 0.0f) {   // 0x007912FB
                ++it;
                continue;
            }
            if (creature.GetPItem()) {                                                  // 0x00791344
                if (CTCWillResponse::IsThingEpicPowerExplodable(creature.GetPItem())) {
                    HandleCreatureKilled(creature.GetPItem(), effects, true);           // 0x0079136D..0x00791860, see below
                } else {
                    ResetCreatureHitState(creature);                                    // 0x00791889
                    CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);           // 0x00791895
                }
            }
            LastFrameCreaturesHit.erase(it++);                                          // 0x0079189A..0x00791920 (expanded)
        }
    } else {
        // 0x00791947: the spell is over -- one last hit for everything still marked
        float damage = PDef->DamagePerHit[SpellLevel];
        for (MapOfCreaturesToPerCreatureInfo::iterator it = LastFrameCreaturesHit.begin(); it != LastFrameCreaturesHit.end(); ++it) {
            CIntelligentPointer<CThingCreatureBase> creature(it->second.ActualCreature);   // 0x00791978
            if (!IS_THING_ALIVE(creature))                                              // 0x00791981
                continue;
            if (creature->HasTC(TCI_NYMPH))                                             // called std::bitset<274>::test from here on
                creature->GetTC<CTCNymph>(TCI_NYMPH)->StopFlyingAround(0);
            ResetCreatureHitState(creature);                                            // 0x007919EF
            C3DVector from, direction;
            GetStrikeLine(caster, creature.GetPItem(), from, direction);
            CHitParameters hit(0, caster, 0, from, direction, true, false, false, false);   // 0x00791B09
            hit.AddDamageWithAttribute(9, damage);
            hit.SetMoveBackOnHit(false);
            hit.SetEpicSpell(true);
            bool explodable = false;
            if (!creature->GetCreatureDef()->CannotBeExploded && CTCWillResponse::IsThingEpicPowerExplodable(creature.GetPItem()))
                explodable = true;
            bool persisted = true;
            if (explodable) {
                persisted = creature->IsGameStatePersisted();
                creature->SetAsGameStatePersisted(false);                               // called, 0x00791B8D
            }
            creature->GetHit(hit);                                                      // 0x00791BA7
            if (!creature->HasTC((ETCInterfaceType)0x8b))
                creature->AddTC("CTCDivineWrathVictim", true, 0);                       // 0x00791BEB
            if (explodable)
                creature->SetAsGameStatePersisted(persisted);                           // 0x00791C0D
            if (creature->HasTC(TCI_ENTITY_EVENTS))
                creature->GetTC<CTCEntityEvents>(TCI_ENTITY_EVENTS)->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 0x12));
            if (!(creature->GetHealth() > 0.0f) && CTCWillResponse::IsThingEpicPowerExplodable(creature.GetPItem()))   // 0x00791C87
                HandleCreatureKilled(creature.GetPItem(), effects, false);              // 0x00791CB6..0x007920F9, see below
        }
        caster->FinishCurrentAction();                                                  // 0x00792134
        UnsheatheWeapon();
        FinishMove();
        PlaySoundW(PDef->FinishSound[SpellLevel], false);   // 0x0079216C
    }
}

// NOT a retail function: retail has this sequence twice in FrameUpdate (0x0079136D and 0x00791CB6). It is written once here only to
// keep the transcription readable; the real source repeats it (the second copy differs: every HasTC is a called bitset test and there
// is no creature-def effect table, only the spell's). Paste it back into both places when the headers exist.
//   world message: GetWorld()->GetMessageEventManager()->AddMessage((EMessageEventType)9, GRandomFrameWord, creature->GetPhysicsDef()->Position (+0xC),
//       PThing, creature, 0.0f, 0.0f, 0)                                                              0x007913AA / 0x00791CF3
//   if (PThing->HasTC(0x42)) PThing's CTCEntityEvents->AddEvent(new NEntityEvents::CEventKilledCreature(PThing, creature))   0x00791414
//   CTCDExperienceOrb::Create(creature)                                                               0x00791424
//   if (creature->HasTC(0x3C)) creature->GetTC(0x3C)->(vtable +0x34)()                                0x00791465
//   creature->Kill(false)                                                                             0x00791475
//   CDefPointer<const CMessageEventDef> message; CThing* killer = creature->GetKiller() (the pointer at +0xF4)
//   if (killer && killer->(vtable +0x134)() && GetDefinitionManager().GetDef(<0x0126BF0C>, message))
//       GetWorld()->GetMessageEventManager()->AddMessage(message, creature->PeekPos(), killer, creature, 0)   0x0079151E
//   CTCWife* wife = 0; if (creature->HasTC(0xD7)) wife = creature's CTCWife                           0x00791534
//   if (killer && killer->HasTC(0x51)) {                                                              CTCHeroOpinionDeedLog
//       if (creature->HasTC(0x49)) { villager = creature->GetTC(0x49) (LowerBound copy 0x004365B0); if (villager->+0x18) {
//           log->PostDeed((EOpinionDeedType)9, creature->PeekPos(), creature, 0, (EOpinionPostDeedType)2, true, false, faction, false, 0);     0x0079161B
//           log->PostDeed((EOpinionDeedType)0x3A, creature->PeekPos(), creature, 0, (EOpinionPostDeedType)7, false, false, faction, true, 0); } }   0x00791651
//       if (wife) wife->Murdered(); }                                                                 0x00791664
//   if (creature->HasTC(0x42)) creature's CTCEntityEvents->AddEvent(new NEntityEvents::CEventDied(killer, creature))   0x007916C9
//   death effect <0x0126BEF4> at the creature's position moved to helper point <global CCharString 0x013BAC48> (scale 1.0f):
//       first copy: from the creature def's own CSpecialEffectsDef sub-def when it has one that knows the name (0x007916F0..0x0079179B),
//       otherwise (and always in the second copy) from the spell's `effects` (0x007917A0 / 0x0079204E)
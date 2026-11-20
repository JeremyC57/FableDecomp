// CTCDExperienceOrb::FrameUpdate() -- experience orbs flock: gather the other orbs within 5 units, merge with one that touches once the combination immunity has run out, else drift towards their average position; then fly to the hero while the absorb button is held, and fade out when the life timer of a decaying orb runs out
// Leaf: ?FrameUpdate@CTCDExperienceOrb@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077A430, 1115 bytes, ebp frame, `ret`.
// Retail-asm: 0x0077A45B `fld dword ptr [ebp-10h]` / 0x0077A45E `fadd dword ptr [ebp-0Ch]` / 0x0077A461 `fistp dword ptr [ebp-4]` ..
//   0x0077A475 `dec dword ptr [ebp-4]` -- the engine's inline-assembly ceiling, through the shared engine/GFFloatToLongCeil.h helper
//   (the search radius 5.0f). No asm in this source.
// Source shape: Ego_d.pdb locals (area, experience_orbs, combined_with_another_orb, average_position, c1, from_me_to_average_pos,
//   phero, distance_from_hero, from_me_to_hero, acceleration, ptc_fade_out_and_in) and line table (lines 270..338), statement by
//   statement: 272 the rough-area box, 273 GetAllTCDsInRoughArea<CTCDExperienceOrb> with the bound IsNot / In2DRange filters,
//   276..277 the two flock locals, 278 the `long c1` loop over size() (an unsigned compare, `jb` 0x0077A6CF), 281..287 the touch
//   test and merge, 290 the sum, 292..299 the average / offset / IsNonZero / Normalise / acceleration, 304 the immunity count,
//   306 the two pause tests, 308..315 the hero pull, 318 DecayOverTime, 320 `--LifeTimer < 0` (46 debug bytes: one statement),
//   322 `ptc_fade_out_and_in = 0`, 323 GetTC, 325 IsFading, 327 Kill, 332 AddTC, 333 FadeOut. The two debug ASSERTs (lines 280, 312)
//   leave nothing in retail.
// Helpers in their Ego_d line-table shapes: C3DVector::Normalise = GFFastOneOverSqrt(GFSqr(X) + GFSqr(Y) + GFSqr(Z)) then three
//   products (lines 686..694: 74 / 13 / 15 / 15), IsNonZero one statement (line 565, 78 bytes), operator- / operator* one
//   constructor return, operator/= through a reciprocal local. The default IsNonZero of the shared header is a three-statement
//   running sum fitted for another row: with it the three squares come out X, Z, Y (DIFFER 1118v1115); every one-expression
//   spelling (X Y Z, Y X Z, Z Y X: c2 sorts a commutative sum's operands itself) gives retail's order.
// TU mate: the GetAllTCDsInRoughArea instance (retail 0x0077ADF0, the landed row's define set) is defined in the TU, out of line;
//   c2 then knows the orb vector does not escape and keeps its size in edi across the loop (0x0077A50A / 0x0077A6B8).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_TCDRIVER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCDSINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE
#define FABLE_RETAIL_CTCD_STATGETDRIVERTYPE
#define FABLE_RETAIL_CTCDEXPERIENCEORB_STATGETDRIVERTYPE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_PEEKPLAYERINTERFACE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER_JOYSTICKDEVICE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CTCDEXPERIENCEORB_FRAMEUPDATE
#define FABLE_RETAIL_CTCFADEOUTANDIN
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/EGameAction.h"
#include "engine/CTCDriverBase.h"
#include "engine/CTCDExperienceOrb.h"
#include "engine/CTCFadeOutAndIn.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/ThingAlive.h"
#include "engine/GFFloatToLongCeil.h"

void CTCDExperienceOrb::FrameUpdate()
{
    std::vector<CThing*> experience_orbs;
    C2DBoxI area(GetParentThing().GetPos().ToC2DCoordI(), GFFloatToLongCeil(5.0f));
    PeekThingSearchTools().GetAllTCDsInRoughArea<CTCDExperienceOrb>(area, GFFilter_Bind2(CThingFilter_IsNot(GetParentThing()), CThingFilter_In2DRange(GetParentThing().GetPos(), 5.0f)), experience_orbs);
    if (!experience_orbs.empty())
    {
        C3DVector average_position(0.0f, 0.0f, 0.0f);
        bool combined_with_another_orb = false;
        for (long c1 = 0; c1 < experience_orbs.size(); c1++)
        {
            if (CombinationImmunityTimer <= 0)
            {
                // Inferred: `<=` from retail's `test ah,41h ; jnp` (0x0077A69D); the bound is the float 0x3AD1B717 at 0x0126AC94
                // (0.0016f, which is also 0.04f squared: the debug statement's 93 bytes do not tell the two spellings apart).
                if ((GetParentThing().GetPos() - experience_orbs[c1]->GetPos()).GetSquaredMagnitude() <= 0.0016f)
                {
                    CombineWithExperienceOrb(*experience_orbs[c1]);
                    combined_with_another_orb = true;
                    break;
                }
            }
            average_position += experience_orbs[c1]->GetPos();
        }
        if (!combined_with_another_orb)
        {
            average_position /= experience_orbs.size();
            C3DVector from_me_to_average_pos = average_position - GetParentThing().GetPos();
            if (from_me_to_average_pos.IsNonZero())
            {
                from_me_to_average_pos.Normalise();
                GetParentThing().GetPhysicsTC()->ApplyAcceleration(from_me_to_average_pos * 0.006f);
            }
        }
    }

    CombinationImmunityTimer--;

    if (!GetThingManager().IsNonScriptedEntitesPauseMode() && !GetThingManager().IsAllEntitesPauseMode())
    {
        CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(phero) && PeekPlayerManager().GetMainPlayer().PeekPlayerInterface()->IsGameActionEventInQueue(GAME_ACTION_ATTRACT_EXPERIENCE_ORBS, (EInputButtonEventType)2 /* INPUT_BUTTON_TYPE_HELD (Ego_d.pdb); the shared headers only forward-declare this enum */, GetPlayerManager().GetMainPlayerJoystickDeviceNumber()))
        {
            C3DVector from_me_to_hero = phero->GetPos() - GetParentThing().GetPos();
            float distance_from_hero = from_me_to_hero.GetMagnitudeAndNormalise();
            float acceleration = 0.7f / distance_from_hero;
            GetParentThing().GetPhysicsTC()->ApplyAcceleration(from_me_to_hero * acceleration);
        }

        if (DecayOverTime)
        {
            if (--LifeTimer < 0)
            {
                CTCFadeOutAndIn* ptc_fade_out_and_in = 0;
                if (GetParentThing().GetTC(&ptc_fade_out_and_in))
                {
                    if (!ptc_fade_out_and_in->IsFading())
                    {
                        GetParentThing().Kill(true);
                    }
                }
                else
                {
                    ptc_fade_out_and_in = static_cast<CTCFadeOutAndIn*>(GetParentThing().AddTC("CTCFadeOutAndIn", false, 0));
                    ptc_fade_out_and_in->FadeOut(1.5f, true);
                }
            }
        }
    }
}
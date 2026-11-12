# Chunk Q9 header overlay (FABLE_HDR_OVERLAY=$A/hdr)

Every file under `hdr/engine/` shadows (or adds to) `rebuild/include/engine/`. Copies are the current repo header plus ONLY the
opt-ins below; every change sits behind a new `#ifdef FABLE_RETAIL_*`, so the default path is byte-identical to the repo header
(checked with `diff` against the repo copy on 2026-11-11, before hand-back). New headers carry their PDB evidence in their own
comments and need their opt-in macro (or are new classes with no previous includer).

## Opt-ins added to existing headers

| Header | Macro | Row(s) | What it does |
|---|---|---|---|
| C2DExtentsI.h | `FABLE_RETAIL_C2DEXTENTSI_INITIALISE` | 0x009F3010 (src) | `C2DExtentsI(long,long)` delegates to `Initialise(x, y)` (+ copy ctor), giving the failure arm's `xor eax,eax ; mov edx,eax` (0x009F3081). Default `:X(x),Y(y)` stays in `#else`. |
| CSurface.h | `FABLE_RETAIL_CSURFACE_READONLYLOCK` | 0x009F3010 (src) | Declares `CSurfaceLockedRect ReadOnlyLock(unsigned long) const; // 0x009F3010` (Ego_r + FableWin). Declaration only. |
| CThingPlayerCreature.h | `FABLE_RETAIL_CTHINGPLAYERCREATURE_MOVEMENTBAND` | 0x006AB430 (src) | Declares protected `EMovementBandType GetMovementBandTypeFromJoytickMagnitude(float) const; // 0x006AB430` (PDB `IBE`). Declaration only. |
| CTCHeroSpecialMovement.h | `FABLE_RETAIL_CTCHEROSPECIALMOVEMENT_ISRUNNING` | 0x006AB430 (src) | Inline `bool IsRunning() const { return Running; }` (FableWin.pdb; expanded in retail, 0x006AB504). |
| CCreatureAction_PlayAnimation.h | `FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR` | 0x0092B900 (src) | Omits the declared `virtual ~CCreatureAction_PlayAnimation()`, so a temporary is destroyed through ~CCreatureActionBase 0x00693EF0 directly (0x0092BB7B), as retail binds it. |
| C3DVector.h | `FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS` | 0x0092B900 (src) | Normalise reads X, Y, Z into locals and sums Z*Z, X*X, Y*Y (0x0092B95F..7A). Selected before the existing `_NORMALISE_SQUARE_HELPER` / `_UNPAREN` branches (the old `#ifdef _SQUARE_HELPER` became `#elif defined(...)`, same meaning). |
| C3DVector.h | `FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D` | 0x0092ACE0 (differ) | `GetMagnitude2D()` as `GFFastSqrt(X*X+Y*Y)` (and the `GFFastSqrt` helper) without the `FABLE_RETAIL_UPDATE_NEAREST_INTERESTING` opt-in's CThingCreatureBase / CTCTargetingBase / C2DBoxI changes; inert when either older opt-in is defined. |
| CAIStateGroup_ApprenticeHero.h | `FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_GOTOLIBRARY` | 0x0092ACE0 (differ) | Declares protected `bool GoToLibrary(); // 0x0092ACE0` (FableWin `IAE_NXZ`). Declaration only. |
| CTCHero.h | `FABLE_RETAIL_CTCHERO_TITLEOBJECT` | 0x0059FC17 (differ) | Ported unchanged from pending/2026-11-10_linux_hand1009b: Title +0x10, HeroTitleObjectDef +0x14, `_tail[0x20]`. |
| CTCHeroAttachableAppearanceModifiers.h | `FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_WEARING` | 0x0059FC17 (differ) | Ported from 1009b: declares IsWearingAppearanceModifier 0x00706000. |
| CTattooDef.h | `FABLE_RETAIL_CTATTOODEF_MEMBERS` | 0x0059FC17 (differ) | Ported from 1009b: members, IsConflicting 0x0059B679, static asserts. |

## New headers

| Header | Guard / macro | Row(s) | Evidence |
|---|---|---|---|
| CQ_WizardBattleScript.h | needs `FABLE_RETAIL_NSCRIPT` | 0x00E1CAE0 (src) | Ego_r.pdb layout - 0x14 (retail CLevelScriptBase 0x48); sizeof 0x80 (Alloc 0x00E1D9F0 allocates 0x80). Methods WatchForLeaving 0x00E1C4D0, MonitorMazePhases 0x00E1CAE0, PhaseTwo 0x00E1C580, CutsceneScreamers 0x00E1CD40, PhaseOne 0x00E1CFD0. |
| CPlayerMovementDef.h | (new class) | 0x006AB430 (src) | Ego_r.pdb CPlayerMovementDef : CParentDefClassBase, members JoystickMovementStationaryBand +0x3C .. LeanAngleStepPerFrame +0x78 (retail CParentDefClassBase 0x39 padded to 0x3C); sizeof 0x7C. |
| CAIStateGroup_OnHeroPush.h | (new class) | 0x0092B900 (src) | Ego_r.pdb, 32 bytes: PushDir +0x14 after the 0x14-byte CAIStateGroupBase (0x0092B942 `lea esi,[edi+14h]`). |
| CTCOnHeroPush.h | needs `FABLE_RETAIL_CTCBASE` | 0x0092B900 (src) | Ego_r.pdb, 32 bytes: DistPushed +0x0C, PushDir +0x10, FrameSincePushed +0x1C (also written by landed CThingAICreature::OnHeroPush 0x008328D0, which still has a local model). |
| NFractal_CNoise.h | (new class) | 0x00F35C60 (differ) | Ego_r.pdb NFractal::CNoise, 0x1804: P[512] +0, G2[512] +0x800, Seed +0x1800; PerlinNoise 0x00F35C60 public const, InitPerlinNoise 0x00F35E00 private. |
| CTCHeroMorph.h | needs `FABLE_RETAIL_CTCHEROMORPH` | 0x0059FC17 (differ) | Ported unchanged from pending/2026-11-10_linux_hand1009b/hdr (no promoted repo header). |

The src/ rows depend on: C2DExtentsI.h, CSurface.h (0x009F3010); CThingPlayerCreature.h, CTCHeroSpecialMovement.h,
CPlayerMovementDef.h (0x006AB430); CQ_WizardBattleScript.h (0x00E1CAE0); C3DVector.h, CCreatureAction_PlayAnimation.h,
CAIStateGroup_OnHeroPush.h, CTCOnHeroPush.h (0x0092B900). The rest serve differ/ rows only.

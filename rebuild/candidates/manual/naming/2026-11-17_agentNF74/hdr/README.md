# NF74 header overlay (copies of the repo headers at first change, 2026-11-17, with `#ifdef` opt-ins only)

Every body edit is token-identical without its opt-in; pin_asserts.py was run after every edit (results below).

| header (copy of rebuild/include/...) | opt-ins added | used by | pin_asserts |
|---|---|---|---|
| engine/CCombatAction_GenericStrikeResponseBase.h | `FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_FINISHACTION`: declares Ego_r / FableWin `virtual void FinishAction()` (0x008663D0) | 0x008C2550 | pinned 0 |
| engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h | `FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_FINISHACTION`: `virtual void FinishAction()` (0x008C2550) and the protected `bool GetValidRushPosition(C3DVector&)` (0x008C1600, Ego_r IAE spelling) | 0x008C2550 | pinned 0 |
| engine/CTCBoss.h | `FABLE_RETAIL_CTCBOSS_GETPHASESTARTPOS`: FableWin inline `const C3DVector& GetPhaseStartPos() const` | 0x008C2550 (its same-TU GetValidRushPosition body) | pinned 1: the plain `FABLE_STATIC_ASSERT(sizeof(CTCBoss)==72)` after the insertion became `FABLE_STATIC_ASSERT_AT(85, ...)` (its repo line) |
| engine/CSpecialEffectsDef.h | (SKYBEAM_ORIGINAL branch) `FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_INLINE`: includes the shared engine/CCRC.h instead of the branch's local CCRC declaration and drops `__declspec(noinline)` from GetEffectFromString (LTCG per-site inlining); nested `FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_NAMED_ARGS` -- **FITTED**: length then text as named locals before `CCRC::Calc` (Ego_d lists only `it` in GetEffectFromString; gives retail's index-first SIB at 0x008DDC00) | 0x008DDAD0 | pinned 0 |

| engine/CCreatureAction_DragonRearUpFull.h | (retail branch) `FABLE_RETAIL_CCREATUREACTION_DRAGONREARUPFULL_PERFORMACTION`: `virtual void PerformAction()` (both PDBs, 0x008F3630) | 0x008F3630 | pinned 4: the four plain `FABLE_STATIC_ASSERT`s after the insertion became `FABLE_STATIC_ASSERT_AT(<repo line>, ...)` |
| engine/NThingShot_CTCShotDefinitionBase.h | `FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ADDTHINGTOAVOID`: `void AddThingToAvoid(const CThing&)` (both PDBs; retail 0x007C0FB0, landed under the descriptive AddHitThing) | 0x008F3630 | pinned 0 |
| engine/CCreatureAction_CastPhysicalShieldSpell.h | (retail branch) `FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD_CTOR`: declares the Ego_r / FableWin ctor `(CThingCreatureBase&, long, bool, const CCharString&, long, float)` (0x008BB6F0); the destructor stays implicit | 0x008E0060 | pinned 8: the eight plain `FABLE_STATIC_ASSERT`s of the generated (#else) branch below the insertion became `FABLE_STATIC_ASSERT_AT(<repo line>, ...)` |

Tried and removed (no effect, not in the overlay): a CCRC.h Ego_d-named Calc body, a CCRC::CalcFromString opt-in, a GetCRC route in GetEffectFromString, a declared virtual destructor in the CASTPHYSICALSHIELD_CTOR block.

New headers, UNVERIFIED drafts (neither has compiled in a passing TU; no repo counterpart; laid out from ghidra_out/struct_layouts_egor.tsv / Ego_r member names; used only by the blocked drafts in work/att/):
- engine/CEnginePrimitiveManager2DStaticMeshes.h -- on CEnginePrimitiveManagerMeshBase (Ego_r sizeof 792, no own members): ctor (CArray<CEngineScene*>&), virtual CreateInternalPrimitive / GetSupportedTypes / Render2DPrimitive (0x00B9E950) / RenderSubPrimitive.
- engine/CEnginePrimitiveManager2DAnimatedMeshes.h -- the same member set on CEnginePrimitiveManagerAnimatedMeshesBase (Ego_r sizeof 1680), Render2DPrimitive 0x00BD1F70.

Default-path check: every opt-in is a new `#ifdef` block, except CSpecialEffectsDef.h, where the existing `#ifdef _MSC_VER` guard of
`__declspec(noinline)` became `#if defined(_MSC_VER) && !defined(FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_INLINE)` and the local
`class CCRC{...};` line sits in the `#else` of the new opt-in: the preprocessed text without the opt-in is unchanged.

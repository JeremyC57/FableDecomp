# NF11 header overlay

Full copies of the current repo headers with `#ifdef` opt-in additions only (diff against
`rebuild/include/engine/` removes 0 lines), plus new headers. Graded with `FABLE_HDR_OVERLAY=$D/hdr`.

## Opt-ins added to existing headers
| Header | Opt-in | Adds | Evidence |
|---|---|---|---|
| CAnimateThing.h | FABLE_RETAIL_CANIMATETHING_CLEARALLANIMS | `void ClearAllAnims();` 0x0070EC30 | Ego_r.pdb + FableWin.pdb `?ClearAllAnims@CAnimateThing@@QAEXXZ`; bound by 0070b4d0, 00835e30 |
| CAnimateThing.h | FABLE_RETAIL_CANIMATETHING_SETASFINISHED | `void SetAsFinished();` 0x0070BE80 | both PDBs; bound by 00835e30 |
| CAnimateThing.h | FABLE_RETAIL_CANIMATETHING_INIT | `void Init(const CAnimInit&);` 0x0070D920 | both PDBs; Ego_r order SetMainAnim then Init matches retail; bound by 00732850 |
| CTCAnimationComplex.h | FABLE_RETAIL_CTCANIMATIONCOMPLEX_ONCREATE (inside _FULL) | `virtual void OnCreate();` | both PDBs (UAE); vtable 0x012650A4 slot 4 |
| CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_COMBATANIM | `GetRandomCombatAnimationFromName(const CCharString&) const` 0x00667B30, `GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>&) const` 0x00666B50 | both PDBs; Ego_r address order (see RENAMES.tsv) |
| CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_COMBATANIMSET_SAMETU (needs CAppearanceDef.h first) | inline body of GetCombatAnimationSet after the class | retail auto-inlines it into 0x00667B30 (same TU); without it the call stays out of line |
| CThingDeadCreature.h | FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION | includes CCountedPointer.h; PAnimFilter typed `CCountedPointer<CAnimFilterBase>` instead of `unsigned char[8]` (same size and offset); protected `PreloadDeadAnimation()`, `SetupDeadAnimation()` | Ego_r.pdb member type; both PDBs for the methods (IAE) |
| CThingPlayerCreature.h | FABLE_RETAIL_CTHINGPLAYERCREATURE_CONSTRUCT | protected `bool ConstructFromParams(long, const C3DVector&, long, const CThingPlayerCreatureInit&)` | both PDBs (IAE, reference params) |
| CEngineMeshEffect.h | FABLE_RETAIL_CENGINEMESHEFFECT_OUTLINESMOOTH | `class CEngineMeshEffectOutlineSmooth : CEngineMeshEffectOutlineBase` with the ctor 0x004C3000 | both PDBs `??0CEngineMeshEffectOutlineSmooth@@QAE@MMABVCRGBColour@@EJJ@Z` |
| NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MESHEFFECT | `SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect>, bool, bool, bool, EAppearanceMeshEffectPriority, void*)` 0x004C0240 and the CIVCountedPointer include | both PDBs |
| CGameScriptThing.h | FABLE_RETAIL_CGAMESCRIPTTHING_QUERIES | `virtual bool IsSummonedCreature() const;` | both PDBs (UBE); vtable slot 65 |

## New headers
| Header | Contents | Evidence |
|---|---|---|
| CTCClock.h | `CTCClock : CTCBase`, virtual OnInitialActivate, PDef, LastTimeUpdated; size 0x14 | Ego_r.pdb layout (struct_layouts_egor.tsv) |
| CTCCreatureHive.h | Open(), Animating +0xc | Ego_r.pdb |
| CTCAnimatingObject.h | PlayDefaultAnimation, private PlayAnimation(const CAnimInit&, long) 0x007F8DC0, Animating, CurrentAnimationPriority, `std::list<CAnimInit> QueuedAnims` | Ego_r.pdb + FableWin.pdb |
| CDeadCreatureAnimPreloader.h | inline ctor (mesh bank), operator() decl; for_each instantiation 0x008365F0 | FableWin.pdb |
| CThingPlayerCreatureInit.h | OriginalThing, PreviousThing, PlaceAtEndOfList; 12 bytes | Ego_r.pdb |
| NProgressDisplay.h | FABLE_FASTCALL `DisplayProgress(const CCharString&, float, bool, bool)` 0x009E9F40 | both PDBs `?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z` |
| CTCSummonableCreature.h | StatGetInterfaceType TCI_SUMMONABLE_CREATURE, inline GetHasBeenSummoned, PDef, HasBeenSummoned +0x10, Summoner[8]; size 0x1c | Ego_r.pdb; used by 008d08a0 |

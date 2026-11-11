# 00839940 CThingGameObject::OnHit — in progress

Exact leaf `?OnHit@CThingGameObject@@UAEXAAVCHitParameters@@@Z`: both original PDB name sets and complete CThingGameObject records; native callers00667d44/007039b4, vtable references, donor027ec280. True extent5195bytes; assigned extent agrees.

Complete dual-PDB type records are retained privately for68 matching class records, plus additional physical/creature/player/explosion and action-use-window types. Original locals and lexical scopes are in PDB_LOCALS/PDB_SCOPES evidence. The inherited descriptive local names and local class definitions were replaced with original names and shared headers; new header declarations describe actual PDB classes, not invented stand-ins. Some declaration-only component interfaces do not expose unused private data and make no sizeof claim.

Corrections: GPInputManager global (distinct from NEngineInternal's); original singleton CPlayerGui::Get; CCreatureDef::GetStatsDef; CHitThing's private StatsDefOfHitThing reference belongs to the derived class, with no extra base word; global EHeroAbility/EOpinionPostDeedType; original emote enum; original CMaterialDef parent; NHitLocations::CHitLocation pointer; original GetVillageLimbo; original HasTC<CTCActionUseWindow>; protected GetThingManager. Exact wrong bound names found so far are recorded at hand-back root NAME_FIXES.tsv.

The HasTC template is reconstructed from donor024db630; window subtype/base and class-name literal are proven by both type streams and donor01b994cb. All liveness checks use the genuine IS_THING_ALIVE template; the inherited forced-noinline/manual-site selection was removed. The original repeated const-reference scopes are supported by donor027ed2ef/027eddc7.

The genuine visible line constructor follows donor01d06250 and improves native argument scheduling. It is still called at both native sites. Rumble type is defined after the existing outer header packing pop, preserving its original natural20-byte size without adding packing. The visible rumble-body experiment was neutral and is not selected.

Selected grade main_a13b_visible_line_constructor:5196vs5195bytes, frameA4vsA0,72.3688% strict instruction alignment,88.0801% relaxed alignment. These ratios are diagnostics, not masked byte-match percentages. Current row is NOT accepted; no claim that every final relocation has passed strict matching. An aligned partial-call review is only evidence and can misalign repeated calls.

Full unchanged production body passes49 behavior scenarios on realVC71 /Od /GR- withoutEHsc andg++03 (main_run001); TEST_BODY_PROVENANCE records exact copied-body hash. Fixture dependencies are controlled doubles; ABI/helper implementation correctness is assessed separately against native and original type evidence. Native/donor bytes remain private and must not be included in any hand-back.

A18b supersedes the earlier selected-grade paragraph: frame0xA0 exact,5174/5195bytes,stillunmatched. OriginalSTLfind dependency is now independently raw-exact71bytes and correctly member-templated; see STL_FIND_IDENTITY.json. RejectedGetSubDef/CHitThing bodies and all original-local variants remain in history.

# NF35 round 2: notes (slice land1011fj..land1113fz, 464 entries, continuing NF33)

## Method
- Probe first:
  - `callers.py` found every E8/E9 call site of every slice address.
  - Each landed caller row was compiled once in a private dir (`pS/`) and its relocation at each retail target was read: 1,214 caller rows in `work/probe.tsv`, 419 self rows in `work/selfprobe.tsv`.
  - Absolute (data or address-taken) references were listed separately in `work/dir32.tsv`.
- Every entry's wrong binders came from that worklist. Edits were grouped by target class: one edit and one grade per row.
- Every grade was a private `q.py` run under `timeout 240`, preceded by a `cpp` #error precheck (`bin/g.sh`).
- Every changed row was rechecked in one vland `--recheck-landed` run (`recheck_final.log`, under `timeout 900`).
- Claims were checked against both PDB name sets (`ghidra_out/egor_pdb_names.tsv`, `ghidra_out/fablewin_pdb_names.tsv`), plus the body, caller bindings, or Ego_r order and size.
- Before each self rename, the catalog was checked so the name was not already on another retail address.

## Statuses (APPLIED.tsv)
- **done 74, done-already 3, rejected 1, partial 3, deferred 2, blocked 46, carry 335.** The 46 blocked are 36 port, 9 authoring and 1 ICF.
- **done-already:** 0x007D7860 random_shuffle (STLport), 0x008857E0 and 0x007021F0. The self row and every landed binder already spell the claim.
- **rejected (REJECTED.tsv):** q25 FormatDateTime `PA_W1H`. Wide text is `unsigned short` in the retail build, and q161 carries the PAG spelling.
- **partial:**
  - 0x00A5D5F0 GetVertexShader const: the self row is renamed and 14 binders are respelled. 5 binders keep hand-rolled ABI views (0x00B90270, 0x00BCA770, 0x00BCC5F0, 0x00BC7E90, 0x00BCE070), and 0x00B99D90 is held by NF36.
  - 0x0097F690 AIAction_MoveToPreCalcRoute: the self row now emits the PDB spelling. Caller 0x00980770 still binds the old one, because its own leaf takes the global CAIFilterBase it models.
  - 0x0097D8B0 AIAction_PerformAction: the self row is renamed and 15 binders are respelled. 3 binders declare the method on a derived stand-in class and need to move onto CAIStateGroupBase.
- **deferred (row held by NF36):** 0x0049F180 (row 0x00416953) and 0x005D9930 (row 0x005E4430).
- Other NF36-held rows named in entry notes: 0x00595845, 0x005958F5, 0x0059899A, 0x0059DB82, 0x005A30A1, 0x00961880 and 0x00B99D90.
- **blocked / port (36):**
  - 11 are rows I opened: the round-1 partials q2-q14, q118, q236, q365, q371 and q385. Their notes give the reason and the rows.
  - 25 are rule-5 gaps: the row reaches the claim through a stand-in (a free function or an address-suffixed class), and no shared header exists for the claim's class. The header must be authored first.
- **blocked / authoring:** the GFSerialise* template selections and the other round-1 partials that need a new byte match (q16-q23).
- **carry (335):** each note gives the worklist; none of these entries was opened or graded.
  - 185 port candidates: the row models the claim's class as a stand-in, and engine/<Class>.h exists, so the note names the header.
    - I first labelled these blocked/port from the probe alone, then relabelled them carry. The brief's port needs more than 10 grades, and this pass moved several such rows in 1-3 grades: CTCBerserk and the five Alloc rows.
  - 44 self rows whose spelling differs from the claim.
  - 32 template/STL or free-function claims whose self row emits an unrelated stand-in. These need a body and caller check: STLport spelling vs ICF fold.
  - 21 NOT_MANGLED screens with no single mangled claim, plus 2 never reached.
  - 19 data globals: absolute references only, so they need their own dir32 probe.
  - 15 entries with wrong binders left.
  - 12 hot targets with many unprobed landed callers.
  - 5 reached but not changed, each with its reason: q108, q228, q292, q320, q388.

## Rows changed: 191, all MATCH (5) / RELOCATION_MATCH (186), every test PASS (recheck_final.log)
- **Caller respells (local models):**
  - CDisplayManager GetBest*TextureFormat (CPixelFormat&, unsigned long); SkipPastString; PeekMesh; GetAsUpperCase; IsPosInViewRange const.
  - GetInventoryType -> ETCInterfaceType (engine/ETCInterfaceType.h); AddMessage by reference; Add/RemovePerceivingThing(CThing&).
  - UpdateScoreForActivity (nested enum, 16 rows); STLport map with four template arguments (12 rows); GetSleepTimes; GetFreeActionPoint; AIAction_PerformAction (15); GetVertexShader const (14).
  - Others are listed per row in STATUS.tsv.
- **Self renames (RENAMES.tsv section 1, 51 rows):**
  - Batch 1: CTCBerserk, JackDragon, CUserProfileManager Clamp* (private, by reference), weather/sky renderer, and others.
  - Batch 2:
    - CTCShop::GetQuantity const
    - CRightHandedSet::Init
    - CQuestManager::OnQuestStartScreenDisplayed
    - CCharString::GetAsUpperCase
    - CAIStateGroup_ScorpionKing::UpdatePhase2
    - CCombatManager::Init
    - CRenderTarget::AttachZBuffer
    - InsertBlock (private, unsigned long&)
    - IsEventButtonPressed (protected const)
    - RedoMatrixConversion / GetChildInverseTransformStaticMesh (CMatrix3x4&)
    - GetBest{Opaque,Signed,Alpha}TextureFormat (unsigned long)
    - NUISystem::CManager::SetInput (nested EInputType)
    - five quest-script Alloc rows (CGameScriptInterfaceBase&, moved onto engine/CLevelScriptBase.h)
    - CBodyGuard::GetGoldCost
    - CThing::CreateSerialiseCreatableTCs
    - Draw2DBoxColours (array parameter)
    - CEngineSurfaceCompositionManager::GetGraphicInfo
    - ThrowRocks / AIAction_MoveToPreCalcRoute (EAIActionReturn)
- **Tests changed (10):** 004B2FE0, 004BBE10, 004CA230, 006E8300, 007726B0, 0097C580, 0099EE20, 00A10E90, 00B52090 and 00E5FC70.
  - Each is a standalone copy of the row's logic, respelled to the new method name. PassPattern lines are untouched.
  - Every self-renamed row's test was checked; no other test names the old method.
- **Leaf lines added** where a row had none and its self name changed: 0099EE20, 006E8300, 00A10E90, 00B38E30, 0041DF10, 00B2B790 and 0094DC40.

## Header opt-ins (hdr/, see hdr/README.md)
- `engine/CWStringParser.h`: `FABLE_RETAIL_CWSTRINGPARSER_INIT` declares `Init(const unsigned short*)`, 0x00A0C8E0 (Ego_r `?Init@CWStringParser@@QAEXPB_W@Z`). Row 0x0048F50C uses it.
- Existing repo opt-ins were used as shipped.

## Local-model policy
- Respells on rows that already modelled a class locally were done in that local model, following round-1 practice. The verifier still warns SHARED_HEADER_AVAILABLE on those rows.
- No new local model of a header-backed class is handed back. 0x009624D0 had renamed its local CCombatAction_StabThing stand-in to CCreatureAction_TrollGroundPound, which has engine/CCreatureAction_TrollGroundPound.h. I caught it in the final audit and reverted it: row deleted, lock released, entry q236 blocked/port.
- Edits to pre-existing local models:
  - 0x00658020: the local C2DVector got a user default ctor, so the result comes back through the hidden pointer as retail does.
  - 0x00820CC0: `friend class CMap` added in the local model for the private Clear.
  - 0x007726B0: the local same-TU map `find` was made const, because the row's own function is const.

## Names
- No made-up names.
- Descriptive-by-library: STLport instances absent from both (Dinkumware) PDBs, namely random_shuffle and sort. They are spelled from body shape and callers; no template arguments were invented.
- Stale declaration, not a binder: the landed GetMaxStock row 0x00772720 still declares an uncalled `GetStockQuantity(long)` for 0x007726B0. The shared headers (engine/CTCShop.h etc.) already say `GetQuantity(long) const`.
- Self-rename cross-check: the shared headers already declare the new names at these addresses: CTCShop.h, CRightHandedSet.h, CQuestManager.h, CAIStateGroup_ScorpionKing.h, CCombatManager.h, CRenderTarget.h and CThing.h.
- Catalog conflict noticed, not mine to fix: 0x00B38DB0 is catalogued RedoMatrixConversion, but its body and the queue evidence make it the twin UndoMatrixConversion.

## Rule slips
- One Bash call exited 1: a `grep -c` returned 0 without `; true`.
- 0x009624D0 (above) briefly held a renamed local stand-in of a header-backed class. It was reverted before hand-back.
- The first final-recheck attempt ran without the lane environment and failed at startup (no compile ran). It was rerun with `source $S/wine/env.sh`.

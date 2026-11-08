# chunk 867 — agent notes

## Incidents (report to the coordinator)
- check_extents.py (chunk_pool) was run ONCE early in this session (crashed with IndexError, no output read); coordinator says it
  rewrites repo EXTENTS.tsv -- never run again; extents verified from disassembly only (bin/ad.py reads the exe + oracle, writes nothing).
- Concurrent-grade slip: 0x00427216 v3 and v4 were graded at the same time (two G.sh jobs on one address). Both results were
  read from their own q864 dirs and are consistent with the later serial v5..v9, but it violated the "never grade one address twice
  at once" rule. Every grade after that was serial per address (max 2 jobs, distinct addresses).
- cl.exe hang on 0x0064BFBA v1 (coordinator killed it after 3.5 min). Root causes, found with preprocess-only probes
  (`bin/pp.py`, `/E` under `timeout`): (1) `#error` in engine/CDefString.h when FABLE_RETAIL_CDEFSTRINGTABLE is not defined —
  cl 13.10 under Wine spins at 100% on `#error` instead of exiting; (2) four overlay headers the TU includes were never written
  (the python script that wrote the overlay aborted on an AssertionError before reaching them: stlport/fable_algorithm.h,
  CTCBattleCharge.h, CTCTavernGameMaster.h, CTCActionUseBase.h) and a missing include also spins cl. Fix: the define added, the four
  headers written by hand; every grade since runs under `timeout 240` (bin/G.sh) and no further hang occurred.

## Row outcomes
| row | result | notes |
|---|---|---|
| 0x00427216 CManager::CreateTickSlider | MATCHED (v8, v9 RELOCATION_MATCH), test PASS | key: CRGBFloatColour->CRGBColour conversion as `__forceinline` (opt-in), the implicit-conversion temp order of CreateComponent's arguments, morality fill written as one `?:` |
| 0x005A8C66 CTCInventoryTrade::ConstructActualSpecialInventoryPC | MATCHED (v11, v12), test PASS | CIVCountedPointer dtor direct form; CCharString `?:` temporaries |
| 0x005AF64E CTCInventoryTrade::BuyItem | MATCHED (v9, v10), test PASS | vtable call through NInventory::CTCInventoryBase slot 80; GetSubDef<CHeroTitleDef>/CHairCardDef typed CDefPointers |
| 0x005D6748 CTCInventoryStats::ConstructExperienceList | MATCHED (v6, v10), test PASS | the 18 AddHeroAbility calls are the row's own out-of-line helper; fcomp flag idiom for the float threshold |
| 0x00590F0B CTCInventory::GetItemsListUIComponentPC | PARKED, 8 grades (one SRC_FAIL), best v3 DIFFER(2803v2907) md 714 | differ/00590f0b.cpp. Structurally the twin of the landed ConstructItemCategoryListPC tail; residue = (a) ours specialises the ability-skip cleanup because the "itemDef != 0" fact survives the same-TU GetSubDef call, retail jumps to the shared release chain; (b) the text-bank reference is memory-homed in retail, a register candidate in ours (same quantity as 005f59c7). Fewer than 50 attempts: not finished |
| 0x005B01FE CTCInventoryTrade::SellItemPC | PARKED, 26 grades, best v11 DIFFER(2217v2222) md 10 (v11..v23 identical) | differ/005b01fe.cpp. Two hunks: (a) retail materialises the usable-flag test as `sete al ; test al,al ; jne` (a bool reaching the test through a join or a frame slot), ours folds it into one `je`; (b) the three-field money sum is read 1E4h+1E8h+1E0h in retail, ours 1E8h+1E0h+1E4h (c2 ranks a commutative chain by the operands' first appearance). Twelve spellings of those two statements did not move the bytes; the next idea is in the differ header |
| 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList | PARKED, 5 grades + 1 diagnostic (d5), best v2 DIFFER(2682v2652) md 327; v4 DIFFER(2649v2652) md 458 is structurally identical modulo colouring | differ/005f59c7.cpp. Residue = register colouring: retail keeps the text-bank reference in memory and gives ebx to the -1 constant; d5 (the bank's address escaped to an extern) reproduced retail's colouring, so the quantity is the bank local's register-candidate status. Fewer than 50 attempts: not finished |
| 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw | PARKED, 10 grades + 24 /FA probes, best v6 = v10q DIFFER(2223v2223) md 40, 20 bytes | differ/0064bfba.cpp. Size, frame, every call, every branch and every constant match; the 20 bytes are all frame-slot numbers (see below) |
| 0x0048E78E CSubtitleRenderer::DrawButtonGraphics | untouched | judged very hard (FP-heavy layout maths, many C2DBoxF temps); no attempt budget left after 0064bfba |
| 0x0064ED4F NPlayerGui::CDrawDamage::Draw | untouched | not reached |

## 0x0064BFBA: the open compiler quantity (with probe evidence)
Every hunk of the v6 diff is a `[ebp+disp]` slot number in the targeting-border block. Retail's slot layout (from the annotated
listing tmp/0064bfba.ann.txt): key/doorDef +0x64, CIntelligentPointer temp +0x60, {dims1,dims2,dims3,vecB,vecD} +0x58, box +0x48,
{vecA,vecC} +0x40, {anchor,dims4,dims5} +0x38, left sprite -0x1c, right sprite -0x70. Our best grade folds {anchor,dims4,dims5} into
the +0x58/+0x40 groups (the compiler reuses two temp slots where retail has three), so every later slot shifts by one group.
24 /FA probes (probe/slots_*.cpp, bin/asm.py) established, for cl 13.10.3077 /Os:
- temps are freed at the end of their full-expression; hidden-return temps (the `GetRenderTargetDimensions()` C2DCoordI returns)
  and inline-constructed C2DVector temps share one free list;
- named locals (even dead ones) never share with temps and are read via `[ebp+x]`; retail's `fild [eax]` on the dims proves those
  are temps, not named locals;
- sibling/nested scopes, destructors on the sprite class, const-reference binding, inlined helpers, the QFE-4035 toolset
  (v10q = v6 byte for byte) do not change the grouping;
- only a statement that keeps the left sprite's temps alive across the SetPersistentPrimitive call (probe v11:
  `Set(S(...)), Anchor(p);` as one expression) reproduces retail's third slot, but it moves the sprite objects themselves
  (-0x1c/-0x70 change), so it is not retail's spelling either.
With a third slot present, retail's slot order by descending reference count matches the R&D rule, so the quantity is "which
statement in retail kept a dims temp live into the anchor call". Candidates not yet tried (budget): the right-sprite box built
inside the sprite constructor's argument list (no `box` reassignment statements), or GFAnchorToTopRight called inside the box
expression rather than into the named `rightPos` (but the named local is what fixed the frame size in v6).

## Placeholders and unproven names (for the coordinator)
- `EHighlightingCursorType` enumerators (NONE/USE/ATTACK/TALK/INTEREST = 0..4) are PLACEHOLDER spellings: no PDB on disk names the
  enum; the values are from the row's stores and the guiDef table indexing.
- `ETargetingType::TARGETING_TYPE_ACTION_USE = 1` and `NGraphicAppearance::EAppearanceMeshEffectPriority::..._HIGHLIGHT = 3`:
  the enum names are PDB (FableWin mangled args of IsTargetedThingOfType / SetAsHighlighted); the enumerator spellings are placeholders.
- CTCBattleCharge::GetPTarget / GetIsZTargetting, CTCTavernGameMaster::GetTavernGame, CTCActionUseBase::IsUsable are placeholder
  accessor names for inline member reads (members are Ego_r names).
- `EExpressionInventoryType`, `EHeroTrainableStatType` (005f59c7 / 005d6748 headers): PDB-named enums, placeholder enumerators.
- CThingObjectDef (new header): sizeof unproven (only the typed CDefPointer conversion is used).
- CUserProfileManager: ShowTargetingStatus +0x6C and ShowTargetingStatusNewProfile +0x134 are the Ego_r offsets minus the STLport
  size deltas of the earlier string/vector members (the -4 rule); the row reads exactly those offsets, so they are consistent with
  retail, but the intermediate members are not modelled.
- The shared NInventory_CTCInventoryBase.h comment lists the two AddItemToInventory overloads at slots 80/81 in the reversed
  order (rtti-vtables.tsv: slot 80 = (long,long,bool,bool,long,bool), slot 81 = (CThing&,...)). NAME_FIXES.tsv rows 0059c6dd/0059c710.
- Manifest mislabel 0x008300D0 (propagated byte-identical getter label CTCPhysicsStandard::CanCollideWithLandscape): it is
  CThingAICreature::GetHasInformation (NAME_FIXES.tsv).
- CDefinitionManager::GetDef(long, CDefPointer<CExpressionDef>*) is `const` in Ego_r but FableWin lists the QAE (non-const) form; the
  overlay declares it const to match the landed callers; the row calls it on a const manager so either compiles the same bytes.

## Tests
test/<addr>.cpp for the four matched rows (g++ standalone, `<ADDR>_TEST PASS`): they model the UI components / inventory items
with stub classes and check the same behaviour the retail body has (tick-slider children and morality icons; special inventory
list contents; buy: money deduction, inventory add, title/tattoo/hair paths, buy-list refresh; experience list: 18 abilities with
the threshold rule).

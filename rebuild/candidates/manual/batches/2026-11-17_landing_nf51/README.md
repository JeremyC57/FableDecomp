# 2026-11-17 landing nf51: name-fix round 7, NF51 (48 rows: CAIStateGroupBase AI actions, NUISystem, NPlayerGui)

Owner: "Continue the naming fix queue with two agents until complete".

NF51 took 16 entries from both halves: 68 binder rows from NF41's probe and NF44's binder lists. The entries are:
- `CAIStateGroupBase::AIAction_MoveToPosProximity` (25 rows) and the other AI actions;
- the NPlayerGui::CDrawBase vcall thunk;
- NUISystem CDeletion ctors, `CActionParamSLONG`, CCountedPointer ReleaseData, the FRONTEND_SCREEN map and CanStartAttractMode;
- CPlayerGui Add/RemovePerceivingThing and CDrawAbilityButtons::DrawAbilityButton.

A container restart interrupted the agent once; it resumed from its files. Brief, worklist and records: `naming/2026-11-17_agentNF51/`.

## Rows (68, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 48 (46 full; 2 partial, see below) |
| not-a-binder | 15 |
| blocked/port | 5 (unchanged, locks released) |

**Partial ports:**
- **0x0053B7F0 ~CList:** the counted-pointer and def models now use the shared headers; its local CList / CComponent / list / find
  models remain.
- **0x00599E3F CFrontEndManager::Update:** the screen map is now the shared `std::map<FRONTEND_SCREEN, NUISystem::CComponent*>`.
  Its self name is still the global `?Update@CFrontEndManager@@QAEXM@Z` (queued).

**Blocked, each with what the headers lack:**
- **0x005907AB:** out-of-line `map<long,long>` members, plus two CTCInventory declarations.
- **0x0053B321:** a whole-row port onto NUISystem_CList.h.
- **0x00596CCE / 0x00597006:** they rely on same-TU bodies (GetProfileNames 0x005955AB). On the shared headers the loop is not
  strength-reduced: DIFFER 838 v 824 and 540 v 546.
- **0x00549B20:** NUISystem_CTextSlider.h lacks Initialize, the CTextSliderDef fields are unnamed, and it needs a same-TU
  `_M_insert_overflow`.

## Entries (APPLIED.tsv)
- **10 done:** nf44:86 (25 binders), nf44:128, e187, e112, e87, e98, e133, e60, e61 and e168.
- **1 done-already:** e75.
- **1 rejected:** nf44:2, the vcall thunk. Every binder already emits `??_9@$BA@AE` to 0x0043E420, the only spelling VC7.1 RTM emits
  for a slot-0 `__fastcall` vcall thunk (REJECTED.tsv).
- **4 blocked:**
  - e36: 3 of 7 binders;
  - e37: 1 of 4;
  - e34: blocked by 0x00597006;
  - nf44:76: 5 of 6.

## Rows and checks
- **Self renames (section 1):** 17 leaf-only changes; catalog names and files are unchanged.
  - NF51 wrote `Class::Method` in the name column; the coordinator stripped the class prefix before batch_rerow.
- **Section 2:** 123 respelled bindings.
- **Recheck (`logs/recheck_part0..2.log`):** all 48 graded from the repo after landing. All 48 are RELOCATION_MATCH, every test
  PASS.
- **Checks:**
  - funclet_includes_check reports 1,355 checked, 0 broken;
  - `naming/rebind.py` removed 130 binding lines and added 72;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (8 opt-ins in 5 files; lines added only, default path unchanged)
- CAIStateGroupBase.h: `_MINIONIDLE`, `_PRECALCROUTEPROXIMITY`, `_MOVETOPOSSIBLYBLOCKED`.
- CAIStateGroup_ApprenticeHero.h: `_LOOKATLEADERBOARD`, `_LOOKATMAP`.
- CFrontEndManager.h: `_UPDATE`.
- CTCInventory.h: `_STYLEENTRY`.
- CTCInventoryAbilities.h: `_HUDLEVEL`.
  - It declares the inline `GetHUDAssignedAbilityLevel` (FableWin.pdb), with body `return CurrentAssignedAbilityLevel;`. That body
    is inferred from 0x0064DB2B's in-place read of +0x144.
  - Retail inlines it, so no symbol is bound. The coordinator accepts it as a readable accessor, but the debug body has not been
    disassembled.

## Name fixes queued (`land1117fu`)
- 0x0094BEF0 is `CAIStateGroup_WorkBase::StayInShop`; the landed file says IsInWorkHours.
- 0x00594FA9: CloseVirtualKeyboard vs the local EnableInput.
- 0x00599E3F: the CFrontEndManager::Update self name.

NF51's NOTES.md also has these findings:
- The 8-byte slot-62 objects are CDeletion.
- CManager slot 5 is `CObservable::RemoveObserver`.
- `CUIDef::PartOfListTree` is at +0x20A.
- CDrawAbilityButtons slot 5 is `GetAbilityButtonMapping`.

## Rule slips (NF51 work/slips.txt)
- Four Bash calls exited non-zero.
- One foreground `sleep` was blocked.
- Two helper runs pointed at a missing file; no compile ran.
- One grade lost its `#ifdef` guard, and it was counted.

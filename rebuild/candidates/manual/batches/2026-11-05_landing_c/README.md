# Landing 2026-11-05c: chunk 776 (2 of the last 3 asm bakes, as plain C++)

Authored by one expert agent (owner, 2026-11-05: "fix the 9 old bakes with the established rules"). The asm rules are in
`docs/pipeline/ASM_RULES.md`. Both rows are compiler residues, so the agent used plain C++ only, with no asm, helper or residue pragma.
No row source or test has `__asm`, `naked`, `_emit` or `volatile`.

## Landed in place (replacing the byte bakes)

| Address | Name | Grade | Purity | Key spelling |
|---|---|---|---|---|
| 0x0053C332 | NUISystem::CList::DoRecomputeOffsets | RELOCATION_MATCH PASS | genuine | copy `PositionOffsetToUse`, then `PositionOffset`, then negate `PositionOffsetToUse.X` (sibling GetDataFromDef 0x0053822B's order) |
| 0x00597BF2 | NUISystem::CFrontEndManager::GotoPreviousScreen | RELOCATION_MATCH PASS | genuine | `FABLE_RETAIL_STLP_MAP` + a named `current = m_stackScreens.top()` + the screen-20 early return that plays the back sound itself |

- **TU form:** both use the CList-row TU form, `#pragma optimize("s", on)` + `("y", off)`, which gives the same bytes as
  `/O1 /Oy-`. Both entries are unaligned, after a neighbour's `leave; ret`.
- **build entries:** in `build_candidates.ps1`, the `Grade = 'asm_bake'` lines are removed and the PassPatterns are now
  `<addr>_TEST PASS`. Both rows regrade RELOCATION_MATCH PASS with `--recheck-landed` (`logs/recheck_inplace.log`).
- **Attempts:** `ATTEMPTS.tsv`. 0x0053C332 needed 2 grades and 10 probes; 0x00597BF2 about 50.
- **No header changes.**
- **vland warnings on 0x00597BF2 (header debt, not rejects).**
  - SHARED_HEADER_AVAILABLE for CUserProfileManager, CVirtualKeyboard and CFrontEndManager. The shared headers lack the
    members the row uses (`agent_NOTES.md`), so it keeps chunk 774's local models.
  - RELOC_NAME on 0x0059B039 and 0x0059A8F3:
    - 0x0059B039 is labelled CScreenStack::Top, but its bytes look like a deque member.
    - 0x0059A8F3 is labelled NThingMap::CThingMapManager::GetMemoryFootprint, but its bytes look like the deque iterator `_M_subtract`.
    - Each rests on one source, so they are noted for a label_trust pass, not relabelled.

## Parked
0x009BEF80 CDisplayManager::SetViewport: DIFFER(477v479), after about 30 idea families and about 860 builds
(`pending/2026-11-05_linux_hand776/`).
- On the right edge, retail emits `inc eax; cmp eax,ebp; jge`, where ebp is the zero register that also holds top.
- VC7.1 folds `x < 0` into the inc flags and emits `jns`.
- Every spelling that avoids the fold loses the zero register.
- Next ideas are in its NOTES.

## Names
Two respellings are queued (tag `land0511c`). The catalog already has both right; the wrong spellings are in landed 0x0059A238 and
`reloc_bindings.tsv`:
- 0x0042DBEE `CNewFrontendGameComponent::ResetAttractModeTimer` (spelled RestartAttractModeTimer);
- 0x00594FA9 `NUISystem::CFrontEndManager::CloseVirtualKeyboard` (spelled EnableInput).

## The 9 old bakes
8 are now C++: 0x0042DF9E and 0x00551340 (landing a), four in landing b, and the two above. One remains: 0x009BEF80.

# NF453 hand-back (2026-11-26): census-verified stale binding lines (owner-approved speed-up A3)

NF453 compiled all 57,160 landed rows with VC7.1 and read each object's relocations to classify the 1,054 stale-line candidates from the naming-speed research. `NOTES.md` covers the method, both rounds and the resume point.

## Applied to `rebuild/oracles/reloc_bindings.tsv` (`TABLE_CHANGES.tsv`)
- **292 deletes.** For 280 of them, the witness's live spelling is already in the table at that target. The other 12 are second lines at a target that a replace in this batch already covers.
- **204 replaces** (`stale -> live`: same target, same witness), including 6 of the vtable lines that would otherwise have vanished.
- **17 rewitnesses.**
- **Closing check** (`EVCHECK.tsv`): every row that references a changed target (13,337 rows) was recompiled and its pairs re-derived with `reloc_identity.check`. All 513 lines pass. No row binds a stale spelling at its target, and every replace witness binds its live spelling.
- **Coordinator checks:**
  - the repo table was byte-equal to NF453's base snapshot;
  - apply 0 misses;
  - bindings_multi 1;
  - descriptive_check 2,016 lines, 0 FAIL.
- **Tracker** (against the NF456 state): **+72 done**, from 29 split_fixable → agree_pdb and 43 split_unknown → agree_std. Also:
  - 92 split_unknown → agree_unverified;
  - 32 split_unknown → standin (only a stand-in spelling is left);
  - 4 split_unknown → split_fixable;
  - 1 split_fixable → split_unknown: 0x012B2F60, below.

## Rulings (advisor decision, owner delegated)
- **r453b: replace, not delete, where the witness's live spelling is missing from the table.** This cut the first round's +88 to +72, because a bare delete would have made a target read agreed while a landed row spells it differently.
- **r453a:** the 8 vtable addresses that would have vanished follow the same rule.
- **r453c: the three "worse" moves.**
  - 0x004ADFF0 is an exposure: one stand-in self is shared by two rows.
  - 0x00C5F51C is an exposure: a bad funclet resolution of `??1CDspGain`; the real destructor is 0x00C69680.
  - Both are in `IDENTITY_FIXES.tsv` and queued.
  - 0x012B2F60's `??_7CGuiVarTransferFloat` was a wrong label: there is no RTTI, and the user is libpk. Its witness now binds `??_7CNodeAllocator@@6B@`.
- **r453d: held.** 79 lines (`HELD_LINES.tsv`) where a referencing row could not be re-derived, plus the first round's holds. Their 46 targets are under COORD_HOLD_NF453 locks.
- **r453e:** 23 live but wrong spellings (`NAME_FIXES.tsv`) are queued in NAME_FIXES_QUEUE, for example `g_pScriptTimerManager` against PDB `pDSTGame` at 0x0143E8F8.

## Owner item: the table misses live bindings
This count comes from NF453's full census cache and is approximate, because the graded function was picked by leaf name. Rows that grade MATCH / RELOCATION_MATCH produce 3,746 live (target, symbol) pairs that are not in the table. They come from 4,077 rows and cover 3,424 targets, 801 of which the tracker does not track today. Adding them would move about 798 targets out of an agree status: 409 agree_pdb, 137 agree_std, 136 agree_descriptive and 116 agree_unverified. So the proven figure overstates closure by up to about 800. Nothing was bulk-added. A table rebuild from all landed objects is the owner's call.

## Next
`NEXT_CANDIDATES.tsv`: from the same census, 323 more possible deletes and 167 possible rewitnesses outside the A3 list. They need the closing check and the r453b replace rule. `FALSE_POSITIVES.tsv` lists the lines that are still live, mostly spellings supplied by headers and C / CRT names.

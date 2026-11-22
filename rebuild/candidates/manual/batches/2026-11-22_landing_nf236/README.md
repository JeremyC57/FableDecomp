# Landing nf236 (Account A, 2026-11-22): nf235 follow-ups, the IsEventButton second spellings, 21 of the 54 held p27 bindings

Agent NF236. Hand-back: `pending/2026-11-22_nf236_handback/`. Entries q12663-q12667 done (q12667 partly); q12659 (0x00418DCA) not started.

- **Recheck** (`logs/recheck.log`, landed tree): 61 rows, 1 MATCH + 60 RELOCATION_MATCH, every test PASS. One RELOC_CONFLICT warning
  (0x0068D890 PushLookAround), identical at HEAD and queued as a finding. No shared header changed (equivalence not owed).
- **q12663:** 0x004189C2, 0x00412F90, 0x0042EC7C bind `?GOldGameComponentToDelete@@3PAVCGameComponent@@A`: 0x013B7D58 3 -> 1 spelling.
- **q12664:** 0x004065D0 renamed CUserProfileManager::GetDefaultProfileName (const QBE; Ego_r 0x00405230 / FableWin; body L"Default";
  0x20 before GetAutoSaveFileName in Ego_r and retail; both binders). Files, catalog and manifest renamed here; labels in
  `ghidra_out/labels_nf236_2026-11-22.tsv`. GetAutoSaveFileName's real address is 0x00406690 (L"AutoSave"), its leaf now const. Both on
  fable_string.h + the DEFAULTPROFILENAME opt-in, with header tests.
- **q12665:** seven IsEventButtonReleased QAE binders respelled to the protected const IBE form (0x00687BD0 2 -> 1); 12 of 13
  IsEventButtonPressed QAE binders too (0x00687A90 stays 2: 0x007E5230 CTCBetting is not a CInputProcessGameBase class).
- **q12666:** header tests for 0x00495620, 0x00496070, 0x00496CF0, 0x00A0A110, 0x00A0A150 (0x00496CF0 keeps its local CSystemManager model).
- **q12667:** 21 of the 54 held p27 binding lines restored with their older binders respelled (28 caller rows + 6 self-leaf renames);
  14 addresses reach one spelling. reloc_bindings: 27 deleted, 22 added (TABLE_CHANGES.tsv); the 24 touched addresses go 36 -> 31
  spellings, none grows. The coordinator also restored 4 held anonymous-namespace lines under the 2026-11-20 `?A0x` rule
  (`logs/reloc_bindings_restored_anon_namespace.tsv`: the same name once the per-TU hash is stripped). 29 held lines remain (queued).
- **Link (agent):** 0 LNK2005; +1 LNK4006 (0x0040CEC0's equal inline GetDefaultProfileName copy meets its owner); 0x00406690 net zero.

## Coordinator rulings on NF236's questions
1. The 0x004065D0 LNK4006 (equal copies): accepted, nf235 ruling 1.
2. 0x007E5230 stays on the old form until an access path is proven; no friend opt-in (no friend record in any PDB).
3. 0x004C7E30 IsLocal: the callers stand; the self row's rename (catalog GetTypeName) is queued with a body check.
4. Anonymous-namespace lines: the 2026-11-20 rule (hash stripped) applies; 4 restored here, 0x006F2C00 / 0x01296F14 queued.
5. NF235's stale locks released; NF236's rows were locked for it at landing (NF237 runs beside it).
6. 0x007D7C10 name lookup queued.

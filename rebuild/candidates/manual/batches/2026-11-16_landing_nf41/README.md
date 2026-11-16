# 2026-11-16 landing nf41: name-fix round 5, NF41 (61 landed rows, 53 renamed)

Owner: "Continue the naming fix queue with two agents until complete". Round 5 continued NF39's half: tags land1011fj ..
land1113fz, 223 entries. NF41 ran beside NF42 (landed c2e099319c) and NF44 (landed 190163133a). Its brief and records are in
`naming/2026-11-16_agentNF41/`.

## Queue (223 entries, counted from APPLIED.tsv)

| Status | Entries |
|---|---|
| done | 49 |
| done-already | 4 |
| blocked | 8 |
| partial | 4 |
| carry | 158 |

- **NF39's 14 "blocked, 0 grades" entries** were all worked: 13 done, 1 partial (0x0063F3A0, 1 of 9 binders).
- **blocked:** exact reasons are in APPLIED.tsv and NOTES.md.
  - Five are ports NF41 examined but never graded, because each row carries 14 to 25 local class models: 0x00A0AA80, 0x00B66190,
    0x00950C30, 0x0059B79A and 0x0059C0C6. They go to the port list.
  - 0x008302D0 is authoring.
  - 0x01231728 and 0x0056A680 were withdrawn after 1 and 2 grades.
- **partial:**
  - 0x013B86A0 (CMainGameComponent singleton): 1 of 35 binders;
  - 0x013B871C: 1 of 7;
  - 0x006CB6A0: 2 of 3;
  - 0x0063F3A0: 1 of 9.

## Rows (61; two tests changed)
- **RENAMES (`RENAMES_applied.tsv`):** section 1 has 53 self renames, 38 of them file moves (`naming/batch_rerow.py`); section 2
  has 75 binder respellings.
- **Coordinator spot-check:** before applying, the coordinator read the evidence for the risky renames:
  - the four CThingSearchTools::GetAllThingsInRoughArea instances have distinct template leaves, and both PDBs list them;
  - 0x00A0AAA0 GetCurrentAWindow is right after SetAWindow in PDB order; its old label CVertexBufferWin32::DoSizeof was a
    same-shape twin label;
  - 0x007F23D0 OnInitialActivate is vtable slot 6;
  - 0x00A36100 HasHelperPoint comes before GetNoHelperPoints in Ego_r;
  - the STLport `_Vector_base` pair 0x004D9C5D / 0x0074F680 is descriptive-by-library, named by its only caller.
- **Rows that had no `// Leaf:` line** (0x007A3CA0, 0x007F4020, 0x007F42A0, 0x00746560, 0x0069D900, 0x00A4FD70) now carry their PDB
  leaf. Each is graded at full retail size. NF41 departed from the brief here because these were queue entries; the coordinator
  accepts it.
- **Tests:** 0x004C7F40 and 0x00960FC0 only rename stand-ins; their PassPattern is unchanged.
- **Recheck (`logs/recheck_part0..2.log`):** all 61 graded from the repo after landing. 7 MATCH and 54 RELOCATION_MATCH, every test
  PASS.
- **Checks:**
  - funclet_includes_check reports 1,355 checked, 0 broken;
  - `naming/rebind.py` removed 97 binding lines and added 55;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (25 overlays, opt-in additions only)
`land_naming_prep.sh` diffed every overlay against HEAD: 0 lines removed in each, and the default path is unchanged in each
(hdrcheck2). None of the new opt-in macros had an existing user. `naming/2026-11-16_agentNF41/hdr/README.md` lists every
opt-in with the rows that use it. Among them:
- CRenderManager2D `_ADDTEXT` / `_ADD2DLINE` / `_ADDLINE`;
- CThingSearchTools `_ROUGHAREA_TYPEDLOCAL`;
- NCreatureAI `_AWARE_INDIRECTLY`;
- CEngineCamera `_SCREENTOHOMOGENOUS`;
- CAIStateGroup_BirdGroundWander;
- the CChunkedFile*, CDisplayManager, CTexture and CSurfaceCompositor opt-ins.

## Name fixes queued (`land1116fu`, from NF41's "Also found")
- **0x0069D520 / 0x0069CA00:** still defined under stand-ins. Their real names are the STLport `_Rb_tree<NCameraModes::EMode,...>`
  `_M_insert` and the iterator `operator--`, which the new 0x0069D900 binds.
- **0x00BF2E50:** its self leaf should be the PDB private, nested-struct spelling.
- **0x00AA80E0 / 0x00AA8190:** non-const twins of FindFirst / FindNextSubChunkOfType; check their spellings.
- **0x00A0AAA0:** retire the opt-in `FABLE_RETAIL_CRENDERMANAGER2D_VIEWPORTBOXFLOAT`, which declares the made-up
  GetViewportBoxFloat.

0x00AAF800 AddMatrix was already queued (line 1231).

## Rule slips (NF41 work/slips.txt)
- One grade overlapped the probe, so two compiles ran at once.
- Three Bash calls exited non-zero; no file changed.
- Four native g++ test compiles ran without `timeout`.
- Two unquoted heredocs expanded backticks inside comments; NF41 repaired both comments.
- NF41 overlaid the `#pragma once` fable_string.h and broke another TU, then withdrew the overlay.

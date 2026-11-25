# NF436 hand-back (2026-11-25): NF434 debt rows, the Alloc shapes

| batch | verdict | rows | what |
|---|---|---:|---|
| batches/a48 | ACCEPT | 5 | Alloc rows of the 0x48 shape on EXISTING headers (CPersonalScript_GlobalThings, CV_DemonDoors, CV_Oracle, CPersonalScript_alextest, CMarkTestScripts) |
| batches/a1 | ACCEPT | 25 | the other 54-byte Alloc rows (0x4C..0x74) on EXISTING headers |
| batches/a1t | ACCEPT if R-NF436-2 | 1 | 0x00E28CA0 CQS_GuardianTrophyDealerInfoScript::Alloc: its opt-in also `#ifndef`s the header's user-declared destructor (gated with a1; evidence copied from a1) |
| batches/n1 | ACCEPT | 8 | Alloc rows with no header: 8 NEW class headers with their own members (gen/gen436n.py) |

- 39 of NF434's 73 debt rows ported (38 ACCEPT + 1 on ruling R-NF436-2); 34 left in T3.txt by shape (5 wrong-self Alloc rows, 4 member-ctor / ctor-coupled Alloc rows, 25 ctor rows). No HOLD batch: the left rows were not put in batches and are not locked.
- Batches are independent (disjoint rows and headers); any order. Each batch's hdr/engine copies equal the master hdr/engine/ (34 headers: 26 add-only opt-ins with orig/hdr_<Name>.h at the root, equal to the repo copy at hand-back; 8 NEW).
- Header changes: an add-only `FABLE_RETAIL_<C>_ALLOC` block at the end of the outer class (only what the header lacks of: user ctor, static Alloc, FableWin.pdb's public virtual overrides; a ctor already declared under another macro is wrapped `#ifndef` that macro); trailing `FABLE_STATIC_ASSERT`s pinned with `FABLE_STATIC_ASSERT_AT(<original line>)`; one `#ifndef FABLE_RETAIL_CQS_GUARDIANTROPHYDEALERINFOSCRIPT_ALLOC` wrap around a user-declared `virtual ~C()` that FableWin.pdb marks compiler-generated.
- No table, catalog or self change (RENAMES / TABLE_CHANGES / CATALOG_CHANGES empty); no best-guess names (every name is FableWin.pdb + Ego_r.pdb / retail vftable), so no BEST_GUESS.tsv.
- RULINGS_NEEDED: R-NF436-2 (a1t), R-NF436-1 (CLevelScriptNoMasterBase ctor rows: base modelling v a /FORCE duplicate). NAME_FIXES.tsv: 8 vftable overrides landed under a non-virtual QAE or stand-in spelling (they block the wrong-self rows).
- Method, gates, numbers: NOTES.md; per row: batches/*/CHECKS.tsv; evidence/ per batch. Tools: tools/ (full436.sh, gates436.sh, definers.py, fwcls.py, hdrscan.py, checks436.py + NF434 / NF432 copies, hdrcheck434 patched for #ifndef unwraps and trailing comments), gen/ (gen436a.py, gen436n.py, meta434.py).
- T3 lines resolved (in part): the nf430 `family placeholder debt on t1 witness rows | debt` line and NF434's `family debt_header` line
  plus its groups `alloc size=0x48 header=yes` (all 5), the header=yes groups of other sizes (their 54-byte rows; the wrong-self and member-ctor rows of those groups stay open, T3.txt),
  and the header=no groups for 0x00CF0640 0x00D12C50 0x00D513C0 0x00D67740 0x00D68EA0 0x00E25970 0x00EEEE40 0x00F02460. The 39 addresses:
  a48 0x00CE19A0 0x00E75590 0x00EBEE30 0x00EEE6D0 0x00EFE380; a1 0x00CF8860 0x00D253B0 0x00D2A7C0 0x00D55D10 0x00D5A9F0 0x00D5DBF0 0x00D664D0 0x00D7AC50 0x00D8D5A0 0x00DDDC30 0x00DEC790 0x00E26780 0x00E277E0 0x00E4F550 0x00E7E0B0 0x00E906F0 0x00E93CF0 0x00E98640 0x00EC11B0 0x00EC5360 0x00ED4A40 0x00EDEB40 0x00EEF540 0x00EF9130 0x00F01330; a1t 0x00E28CA0; n1 0x00CF0640 0x00D12C50 0x00D513C0 0x00D67740 0x00D68EA0 0x00E25970 0x00EEEE40 0x00F02460. Release them with the lander's --release-t3 (REL=) for nf430 / NF434. What stays open is restated in T3.txt.
- Owner rule of 2026-11-25 (best-guess names): changes nothing here; no row was held for want of a second source.
- name_gate ran through tools/name_gate430.py, which differs from lane_tools/name_gate.py only in reading NG_TABLE; NG_TABLE was a byte copy
  of today's reloc_bindings.tsv. Coordinator hdrcheck.py: a48 / n1 / a1t clean; a1 two DIFFs that are plain-cpp artifacts
  (batches/a1/evidence/hdrcheck_coord_note.txt).

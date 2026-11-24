# NF318 (2026-11-23): header-route rejects of the alias1 / flatten-switch1 scripted routes (122 entries)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Identity per entry: ghidra_out/labels_rtti_port.tsv
names exactly the ENTRIES pdb_spelling at all 122 addresses (re-checked). **No shared header was edited** (hdr/ is empty, no equiv_check needed).

## Result (STATUS.tsv)
- **88 done**, ready to land (full proof below): 28 flatten-switch1 rows (CGameScriptInterface 23, CCreatureActionBase 4 with libcp under
  the 2026-11-23 ruling, CAIStateGroupBase 1) and 60 alias1 rows (29 with `.drectve` unchanged, 31 with exactly `/DEFAULTLIB:"libcp"`
  added: the BRIEF prefix lists that exception as allowed; STATUS marks them "libcp (alias route)", Q1 asks only for confirmation).
- **3 skipped-held**: 0x00B6CCF0 READY with full proof but held on Q2; 0x005DBDA0 (Q5) and 0x00893EC0 (Q4) held without a landing claim.
- **31 skipped-port**: reasons per line (pass-specific switch 3, in-class inline body 1, body ports / callee identities 16, DIFFER on the
  real class model 4, bool-v-int return 1, NF310 D1 pick-any duplicates 5, a same-TU stand-in constructor body 1).
- rows.txt lists the 89 changed rows (88 done + 0x00B6CCF0). rows/ holds exactly those sources plus 5 tests (0x00692460, 0x006924D0,
  0x006924F0, 0x00692500, 0x00A19AE0: the LINK_FAIL rows; their old tests called the flattened / alias names). Every rows/ file is
  byte-identical to the text vland graded last (checked). All other landed tests are unchanged (standalone stubs or their own copies;
  vland PASS with the new rows). orig/ has the HEAD copy of every entry's source and test; work/drafts/ the drafts of skipped rows.
- Locks: kept on the 89 changed rows and 0x005DBDA0; every other lock released.

## Method (per row)
- flatten-switch1: `tools/flat.py` keeps every line of the HEAD row (the script had dropped the local declarations the bodies need:
  made-up externs, local models), turns `R __fastcall X_M(P* self)` into `R' X::M() [const]` with the header's declared return type,
  defines the class-model switch and includes engine/X.h. Two rows needed their own local stand-in struct members retyped to `bool` for the
  PDB bool return (0x0088F3E0 TutMgr flags, 0x00892270 CPlayerGui::inCutscene; local models only, no extern / callee changed, bytes equal).
- alias1: start from the script draft; `tools/hoist.py` moves the switch `#define` above every include (rows that included engine/X.h
  before defining the switch compiled the flat branch); `tools/port.py` copies the define / include block of the smallest landed precedent
  (a row defining a member of X under the switch, else the smallest row defining the switch; never a `_PASS<nn>` precedent);
  `tools/retfix.py` gives the definition the header's return type (an enum return keeps its literal as `(E)N`, NF307 D3; Q3);
  `tools/chunkfix.py` (chunk classes: FABLE_RETAIL_CCHUNKEDFILECHUNK, local CCharString model dropped); `tools/tidy.py` drops the emptied
  alias class and the stale "alias route" / "Stand-in" comments and adds one provenance line after the Leaf. The CScriptThing rows were
  rewritten on the private PImp (CCountedPointer<CScriptThing>) as the landed NScript in-TU copies spell them.
  0x00AC62B0 uses the three switches its header names instead of a precedent block whose STLport includes added `/alternatename`
  lines (NF303 Q2).
- Proof, per row (evidence/libcp_b<N>_vland.txt, recheck_final.log, evidence/recheck_{head,new}_b<N>.log):
  1. precheck: clang -fsyntax-only of the new row, its HEAD source and every new test (work/pre.log); every landed test before the locks;
  2. vland --recheck-landed of the HEAD payload (catalog sources, `recheck.py`) and of the new payload (rows/ + tests): new MATCH /
     RELOCATION_MATCH + test PASS for all 89 (38 MATCH, 51 RELOCATION_MATCH);
  3. `tools/libcp.py` on the two vland objects: exactly one DEF rename, old -> the ENTRIES pdb_spelling (asserted), UND sets equal,
     DEF sets equal under the rename, no ??_7 / ??_R, every non-debug section equal in bytes, flags and relocations, `.drectve` equal
     (53 rows) or HEAD + exactly `/DEFAULTLIB:"libcp"` (36 rows: 4 flatten under the ruling + 32 alias, Q1). Planted control 0x006FF040 (graded DIFFER) -> NOT_OK
     (evidence/libcp_ctl.txt); 0x005DBDA0 is flagged NOT_OK too (Q5);
  4. graded symbol = the PDB symbol (vland result lines name it; RENAMES section 1 mangled_leaf);
  5. links: /FORCE link of all 89 HEAD objects v all 89 new objects (evidence/link_all.txt: no duplicates in either, no reference the
     HEAD set resolved left unresolved; 12 rows were re-graded after comment / empty-alias-class edits, b6 / b7, objsec ALL_EQ again, same
     DEF / UND sets, so the link result carries over); batch 1 alone (evidence/link_b1.txt); CScriptThing
     set with the 19 landed NScript rows that define CScriptThing members in-TU (evidence/link_cs.txt): GetFocalPos / GetHomePos /
     GetCurrentMapName go from unresolved (16 references) to resolved, nothing newly unresolved; the other five members duplicate.
- Binders / duplicates: exe census (evidence/census.tsv) + reloc_bindings: no landed row or table line binds the old spelling of a changed
  row (the virtuals are reached through vftables no landed row emits); evidence/defgrep.tsv + `tools/inclass.py`: no other landed source
  defines or Leafs the PDB name of a changed row (CScriptThing's five in-TU copies are why those five are T3). TABLE_CHANGES.tsv: none.
- Screens: none of the 122 addresses or the 19 control rows is in NF317's ENTRIES / rows, under another lock, or reserved elsewhere.

## Questions
- **Q1** (confirmation only; 32 alias rows: the 31 done rows marked "libcp (alias route)" plus 0x00B6CCF0): the alias route adds exactly
  `/DEFAULTLIB:"libcp"`, meeting every condition of the 2026-11-23 ruling (all other sections ALL_EQ, equal UND sets, MATCH /
  RELOCATION_MATCH PASS). The ruling text names the flatten switch route; my BRIEF prefix lists the exception as allowed without that
  limit, so they are marked done. If the coordinator reads the ruling narrowly, hold those 31.
- **Q2**: 0x00B6CCF0's switch FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR is defined by no landed row (this would be the first definer). Allowed?
- **Q3**: enum returns written as the PDB enum with the literal cast (`return (EAIStateGroupType)3;`, `(EVillageTaskType)1`,
  `(EReactionMatchType)0x3`): the enumerator names are not in the repo (EAIStateGroupType is a forward MS enum). NF307 D3 form; OK?
- **Q4**: rows whose move needs callee respells off alias / local stand-ins onto the header's PDB names (0x00893EC0: GetFrameForMessages_006e7510
  -> GetMinWorldFrameForMessages; 0x006FECA0, 0x00AA4B50..C00) are not "ALL_EQ under the one rename": held / T3. Should a respell onto a
  name the class header already declares count as part of the move?
- **Q5**: 0x005DBDA0 is byte-exact on the real class (RELOCATION_MATCH PASS) but HEAD also emitted the alias class's made-up inline
  accessor TargetOffsetVector() as a COMDAT; the real member needs no accessor, so the object loses that section. Acceptable? (draft:
  work/drafts/005dbda0.cpp, lock kept)

## Name fixes seen (pre-existing, not changed here): NAME_FIXES.tsv
- 0x013B8790 CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass is bound as ?g_gameObj@@3PAUGameObj@@A (0x00891C30) and ?g_obj@@3PAUCObj@@A
  (0x00892030) (vland RELOC_NAME, HEAD and new alike).

## T3 queue candidates (from skipped-port)
- 0x004AA8D0 / 0x004AA900 / 0x004AA930 / 0x004AAA40 / 0x004AB150 (CScriptThing): drafts in work/drafts match; queue for the nf86 / nf88
  pick-any form together with the landed in-TU inline copies (0x008A8AF0, 0x00D17470, 0x00D38000, 0x00D387A0, 0x00D75060, 0x00DD9520,
  0x00DDAE40, 0x00DDB720, 0x00DF6010, 0x00E03F70, 0x00E0DE90, 0x00E2B090, 0x00E338E0, 0x00E4E0E0, 0x00E5EBD0, 0x00E6A0B0, 0x00E9CB20,
  0x00EDBD80, 0x00EE2A80).
- 0x00692480 is a unit with its binder 0x008579D0 (binds the non-virtual QAE form).

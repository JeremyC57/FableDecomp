# NF216 notes (2026-11-21): q12378 family pass, the 14 caller rows of 0x004115A0 with a differing ==(const char*) copy

## Summary (counts from STATUS.tsv / APPLIED.tsv / recheck_final.log)
- Entries (2): q12378 blocked (8 of 14 rows ported, 6 left); q12374 blocked (no binder to move in this slice; the X_004fe630 split stays
  with blocked 0x004FE630; N006d41f0 and the 13 dead definers are out of scope).
- Rows ported (rows/, 8): 0x004A0D90, 0x009ACEE0, 0x007D2CC0, 0x00870E00, 0x00871290, 0x004FDCB0, 0x009AF1E0, 0x00870FC0. Every one
  RELOCATION_MATCH + PASS in one vland run (recheck_final.log). In each object the pick-any ??8?$CBasicString@D@@QBE_NPBD@Z copy is now
  the owner 0x004115A0's 88 bytes, bytes and relocation targets equal (tools/cmpcopy.py), and the Compare copy equals 0x00411570 (40 = 40).
- Rows blocked (6): 0x004FE630 (port DIFFER(453v440), 3 forms), 0x00CBFB7D (3 forms), 0x005A5B31 (3 forms, the class probe),
  0x005A04F7 / 0x0065A075 (no forms spent, on the 0x005A5B31 / 0x00CBFB7D measurement), and 0x005A4CDD (a passing draft held for a
  ruling, below). All six stay locked NF216.
- Header opt-ins: none (hdr/README.md). The route uses NF215's FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO +
  FABLE_RETAIL_CBASICSTRING_COMPARE_BODY from the working tree.
- One self rename (RENAMES section 1): 0x00870FC0 UAE -> MAE (both PDBs). One binder respelling (section 2): 0x009AF1E0's 0x0129AAF4
  stand-in g_emptyLiteral_0129aaf4 -> ?NullChar@CCharString@@1DB (an existing spelling; g_emptyLiteral_0129aaf4 stays in other rows).

## Ported rows, by kind
- Macro swap (TEXT_EQ_NOINLINE -> TEXT_EQ_NOINLINE_COMPARE_ZERO; COMPARE_BODY already set): 0x004A0D90, 0x009ACEE0.
- 0x007D2CC0: the swap, + COMPARE_BODY, and its local explicit (strong) specialization of Compare dropped.
- 0x00870E00 / 0x00871290: the local explicit (strong) specialization of ==(const char*) dropped; the two route macros added.
- 0x004FDCB0 / 0x009AF1E0: the local CBasicString / CCharStringData / CCharString models (and their same-TU == stand-ins under
  auto_inline(off)) dropped; fable_string.h included first with FABLE_RETAIL_CCHARSTRING + FORCEINLINE_TEXT_EQ + the route macros.
  Their other local models (CWorldMapLayout / CThingSearchToolsLayout rename-includes, CThingFilter_*, CFilter_Bind2; CStringParser,
  std::list, CDefinitionManagerLayout) are HEAD debt, unchanged (vland SHARED_HEADER_AVAILABLE warnings, as at HEAD).
- 0x00870FC0: the `#define CCharString CCharStringPdb` rename-include and the local string / CActionEventBase / CActionEventCameraShake
  models dropped; the row now uses fable_string.h + engine/NCreatureActionEvents.h (as its siblings 0x00870E00 / 0x00871290 do);
  relocations identical to HEAD (xref). A `// Leaf:` line is added (the row had none; mkpay gave `vfunc_4`).

## Checks
- Defined-symbol diff (objdump -t, HEAD object vs port; tools/symdiff.sh): 0x004A0D90, 0x009ACEE0, 0x007D2CC0 none; 0x00870E00,
  0x00871290, 0x004FDCB0 + ?Compare@?$CBasicString@D@@KIJPBD0@Z only (equal to 0x00411570); 0x009AF1E0 + Compare, + GetAsCharArray
  (equal to 0x00403A10, 15 = 15, relocs equal), + ?PeekData@?$CBasicString@D@@QBEPBDXZ (header inline, no binding line), - the
  row-local ?GetString@CCharString@@QBEPBDXZ (no binding line); 0x00870FC0 the self leaf respelled + Compare.
- Relocation bindings HEAD vs port (xref.py, every relocation of each leaf): identical for 7 rows; 0x009AF1E0 differs only at 0x0129AAF4
  (above). So no == binding moved: all 8 rows already bound ??8?$CBasicString@D@@QBE_NPBD@Z at HEAD.
- No new split: 0x004115A0 spellings unchanged (3); 0x00411570 unchanged; 0x0129AAF4 one row onto an existing spelling; 0x00870FC0
  1 spelling -> 1 (no landed referencer: callers.py none; absrefs.py one hit, the vtable 0x0125C10C, in no landed row; the sibling
  rows 0x00695480 / 0x00692170 / 0x00692190 / 0x00871250 / 0x006954D0 model the class locally without a SetDataFromString slot;
  0x006932B0 uses engine/NCreatureActionEvents.h, which declares it protected = MAE; no reloc_bindings line for either spelling).
- EH funclets: no rebuild/src/compiled/01/21 file (and no other row) includes any of the 8 changed files. 0x00870FC0's self rename
  does not change its file name; the coordinator may rename NCreatureActionCActionEventCameraShake_vfunc4_00870fc0.cpp
  (catalog name vfunc_4 -> SetDataFromString).
- Tests: unchanged for 7 rows. 0x00870FC0's test is a standalone source snapshot of the HEAD row (local models); behaviour unchanged,
  only its "exact source snapshot" marker comment reworded (rows/00870fc0_test.cpp). HEAD debt: it still models CBasicString /
  CCharString locally.

## Flags for the coordinator
- LNK2005 debt (as NF215): 5 rows newly carry a pick-any (COMDAT selection 2) ?Compare@?$CBasicString@D@@KIJPBD0@Z copy (0x00870E00,
  0x00871290, 0x004FDCB0, 0x009AF1E0, 0x00870FC0), each equal to 0x00411570, whose landed owner is an explicit specialization.
  Five no-duplicates copies go away (tools/comdat.py on the HEAD objects: COMDAT selection 1): 0x007D2CC0's explicit Compare
  specialization and the ==(const char*) specializations of 0x00870E00 / 0x00871290 / 0x004FDCB0 / 0x00870FC0; the port copies are
  selection 2 (pick-any) and equal their owners.
- 0x005A4CDD draft held for a ruling (STATUS; work/f/005a4cdd_f1.cpp, vland work/p4_005a4cdd.log: RELOCATION_MATCH PASS): an inline explicit specialization
  of CBasicString<char>::operator==(const char*) between `#pragma optimize("t", on)` and `("s", on)` in the row gives the owner's
  88-byte pick-any copy with the leaf unchanged and no other COMDAT changed. It is a row-level size-pragma device on a same-TU copy
  of a header member (plus a row-local definition of that member), which the 2026-11-20 size-pragma rule (shared-header opt-ins only)
  does not cover. If ruled in, the same form is the next one for 0x005A04F7, 0x005A5B31, 0x00CBFB7D, and for 0x0065A075 with "ty".

## Blocked rows
- 0x004FE630 (3 forms, 6 compiles incl. two SRC_FAIL retries; work/f/004fe630_f1..f3.cpp, work/004fe630_f3.diff): the full header
  port NF62 drafted (fable_string.h, engine/CAFile.h READVAR_DECL, engine/CWorldMap.h NAVIGATIONSECTION with the PDB self leaf,
  stlport set / _M_find) now with the visible, called == body: the NULL / "" tests thread with the string data in esi (NF62's
  residue gone), == copy 88 = owner. f1 (_M_find noinline body) DIFFER(455v440), f2 (_M_find declaration only) DIFFER(455v440),
  f3 (f2 + the find result in a named iterator, so end() is read after the call as retail does) DIFFER(453v440). Residue: retail
  keeps the loop counter i in ebp (`xor ebp,ebp ... inc ebp ; cmp ebp,[count]`) and reloads the sections argument from its stack
  slot at the find; the port keeps sections in ebp and i in a frame word. HEAD row unchanged (its X_004fe630 stand-ins stay; the
  PDB self leaf is in NAME_FIXES.tsv).
- Size-compiled TUs (0x00CBFB7D, 0x005A5B31 probed, 0x005A4CDD draft; 0x005A04F7, 0x0065A075 on the same measurement): FINDING below.
  0x00CBFB7D f1 (macro swap) leaf RELOCATION_MATCH, copy 19 (HEAD 20); f2 (declaration-only ==) DIFFER(74833v74778); f3 (the row-head
  optimize("s", on) moved after the fable_string.h include) copy still 19 while the non-template CCharString inline copies change
  (??8CCharString@@QBE_NPBD@Z 35->39, ??8CCharString@@QBE_NABV0@@Z 37->39, EqualsCaseInsensitive 50->68, StartsWith 65->81).
  0x005A5B31 f1 (plain route) copy 19; f2 (+ an explicit instantiation between optimize("t", on) and ("s", on)) copy 19, nothing
  changes; f3 (+ optimize("", on) at the end of the TU) copy 88 = owner and Compare 40 = owner, but the leaf DIFFER(3166v3128) and
  61 other function COMDATs change (work/005a5b31_f3_comdiff.txt). Those rows already bind the PDB spelling; their small copies are HEAD debt.

## FINDINGS for the coordinator (I cannot write FINDINGS_LOG)
- 2026-11-21 NF216: VC7.1 compiles a template member's implicit instantiation (a pick-any COMDAT) in the `#pragma optimize` mode in
  force at the END of the TU; moving the row-head optimize("s") after the header include (0x00CBFB7D f3) and an explicit
  instantiation between optimize("t") and ("s") (0x005A5B31 f2) leave the size-mode copy, and an end-of-TU optimize("", on) gives
  the owner copy but changes the leaf and 61 other COMDATs (0x005A5B31 f3). Ordinary inline functions, including a non-template
  explicit specialization, take the mode at their definition: the 0x005A4CDD draft (Flags above) is the measured exception.
- 2026-11-21 NF216: 0x004FE630's NF62 residue (PStringData reloaded after each == call) is gone with TEXT_EQ_NOINLINE_COMPARE_ZERO +
  COMPARE_BODY; what is left is a loop-counter register choice (DIFFER(453v440), above).

## Descriptive names
- None new.

## Flags lines
- None added (0x00870FC0's new `// Flags: none` line matches its catalog entry, which has no CompilerFlags).

## Rule slips
- One Bash call ran a STATUS note with backticks inside double quotes (bash tried to run `sections`); the line was rewritten through python.
- 0x004FE630 f1 and f3 each needed one recompile after a compile error (missing set opt-ins; a const_iterator typedef the header lacks);
  counted in its 6 compiles, as the same forms.

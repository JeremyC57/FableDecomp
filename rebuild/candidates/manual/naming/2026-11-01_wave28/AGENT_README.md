# Naming pass wave 28 (2026-11-01): 212 landed rows respelled, all byte-identical (212/212 RELOCATION_MATCH + PASS)

## Inputs and index
- Brief naming/briefs/BRIEF_W28.md; QUEUE_W28_input.tsv (58 rows: 25 carried from wave 27, 16 c696, 17 c697 appended mid-wave);
  DEFERRED_W27_INPUT.tsv.
- idx/full.jsonl is HEAD: the wave 27 after.jsonl plus chunk 696 (idx/delta_c696.jsonl, 10 rows) and chunk 697 (idx/delta_c697.jsonl,
  10 rows), found by modification time (no git). Before hand-back nothing under rebuild/src, tests, include, oracles or
  build_candidates.ps1 is newer than idx/.c697_marker. idx/after.jsonl is HEAD plus ov_final.

## Commands (from $S/naming/w28, S exported, $S/wine/env.sh sourced)
1. `python3 scripts/compose28.py ov_final` runs scripts/hand28/h28_<fam>.py on fresh repo copies; writes runs/<fam>.rows.txt and ov_<fam>/.
2. `bash scripts/chain28.sh`: recheck of runs/final.rows.txt (4 parts), mkindex / after.jsonl, qscan28, mkq28 (+ mkq28_notes.py), mkdef28,
   mkbind28 (+ runs/reloc_bindings.after.tsv), bindings_multi_check (0), mkren28 + fixren28, mklabels28 + fixlab28.
New helpers: scripts/try.sh (quick recheck of a few rows from an overlay), scripts/dd2.sh (normalised retail-vs-built instruction diff),
scripts/hand28/common.py (note line), scripts/hand28/h28_dtorfam.py (the sfb / esb mechanical families).

## Families (compose order; runs/<fam>.rows.txt)
- **d4 (5): 00666b50 007345d0 00856ac0 00857520 00667b30** (DECISIONS_W27 D4). 0x00666B50 / 0x007345D0 define the Ego_r / FableWin const
  members over STLport vector<const CAnimationEntry*>; 0x00669CB0 is called as GetComponent<CAnimComponentCombatHandedness>; the worlds come
  from the protected PeekWorld (CThing 0x004C7AB0 / CTCBase 0x00686A80), the seed from the bound GetWorldSeed (DECISIONS_W28 D1); 0x007345D0
  models CTCBase / CTCCombat locally (D2). The three callers hold real STLport vectors (00667b30's grow-insert is the own row 0x005E1420's
  spelling, same-TU body kept).
- **d5 (3): 00713660 00713850 00714900** (D5): CTCScriptedControl::CActionBase nested; the action list is the STLport
  list<CCountedPointer<CTCScriptedControl::CActionBase> > whose clear is the c695 _List_base spelling. NTCScriptedControl stays for the derived
  actions (FableWin spells CActionStopScripting / CActionMoveTo there).
- **d3 (3): 004125a0 0048608c 004091c0** (D3, proven from the bytes): 0x004125A0 is the __false_type overload (null-checked placement copies at
  0x00412610 / 0x00412636 / 0x00412656 / 0x00412688, no memmove), spelled as the protected STLport member with the allocator; 0x004091C0 on
  STLport vectors (D3 device: declaration-only explicit specialisation of vector<CWideString>::erase); 0x0040E350 held (folded copy, D4).
- **d6 (1): 006a5230** (D6): ?InitialActivate@CThingPhysical@@MAEXXZ -- slot 9 of vtable 0x01244E84 (RTTI .?AVCThingPhysical@@) where
  CThing's vtable has CThing::InitialActivate 0x004C9690; body = base call + AddToPhysicalList(*this); both PDBs. RENAMES.
- **clk (2): 00559600 0055a3e0**: qualified CClickable::Update 0x0055ACB0; 0x0055A3E0's own Leaf is the virtual CDraggable::Update.
- **peek (3): 009d4a20 0073e940 0073ea00**: PeekData 0x00995E70 on CDefStringTable::StringStream; GetStringCRC is the PDB const member (own
  Leaf and its two callers).
- **bard (1): 00ed7db0**: the compiler's ??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z (MakeOne idiom). RENAMES.
- **c697 (3): 00b81e20 00bdfa10 009b05f0**: CEnginePrimitiveSimulationInfo::Clear (own row, RENAMES); CLocalDetailCacheMap::CQuadTreeElement
  nested; CChecksumCacheFile ctor / Open / dtor.
- **sfb (150)**: the free ?AutoTinyCompositeDestructor@@YIXPAX@Z -> ??1CSpawnedFuncBase@@UAE@XZ called qualified (0x00CDD4C0).
- **esb (41)**: the free ?AutoTinyCompositeDestructor_00f35b40@@YIXPAX@Z -> ??1CEntityScriptBase@@UAE@XZ called qualified (0x00F35B40).
No shared header was edited, so no includer recheck was needed. The sfb / esb rows keep their existing Leaf lines (no symbol; graded on the
largest function, 191 "WARN leaf" pairs in the log, unchanged from HEAD: DECISIONS_W28 D7).

## Outputs
- QUEUE_W28.tsv: 58 rows -- 9 respelled, 5 respelled-partial, 10 done, 34 deferred (scope 3 and most c696 / c697 destructor families not reached).
- DEFERRED.tsv: 37 queue deferrals + 4 wave-28 findings (runs/deferred28_manual.tsv) + 435 carried; 5 wave-27 entries resolved
  (runs/w27_deferred_resolved.tsv).
- BINDINGS.tsv: 21 RETIRE, 9 ADD, 6 KEEP, 0 CONFLICT; runs/reloc_bindings.after.tsv; multi-binding check 0.
- RENAMES.tsv: 4 (004125a0, 006a5230, 00b81e20, 00ed7db0; 00bdfa10 dropped -- its own Leaf is a member stand-in). LABELS.tsv: 9.
- DECISIONS_W28.tsv: 7 items.

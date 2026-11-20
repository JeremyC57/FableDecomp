# NF180 notes (round 10, persist traits / counted pointer / creature action / STL container naming)

Hand-back at an entry boundary (context): 22 of 38 entries reached a terminal state, 16 are open (q5202 onward in queue order:
q5202, q5226, q5233, q5234, q5235, q5236, q6141, q6143, q11655, q11656, q11886, q11954, q11956, q11959, q12017, q12018).
Counts from APPLIED.tsv: 7 done, 2 done-already, 1 rejected, 6 blocked, 6 deferred, 16 open. STATUS.tsv: 16 rows ported, 2 not-a-binder,
15 blocked. recheck_final.log: one vland run over the 16 ported rows, 16 RELOCATION_MATCH + PASS. selfcheck.log: 0 problems.

## Descriptive names
- STLport instance spellings (descriptive-by-library, not in either PDB; the instantiation's mangled name, one address each):
  0x00CB6FD0 vector<pair<CCharString,CScriptInfo>>::_M_insert_overflow (q4077); 0x00CB7780 std::sort<pair<CCharString,CScriptInfo>*,
  CKeyPairCompareLess<CCharString,CScriptInfo>> (q4221).
- 0x00CB77E0 / 0x00CB7800 CVectorMap<CCharString,CScriptInfo,CKeyPairCompareLess<..>>::Sort / Update: FableWin.pdb exact symbols (not
  descriptive).

## Callee spellings that replace a stand-in (count does not grow)
- 0x00CB60F0 / 0x00CB6160 (q4077): the only binder 0x00CB6FD0 now binds the STLport library spellings
  ??$__uninitialized_copy@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@PAU12@@std@@YI... / ??$__uninitialized_fill_n@...@std@@YI...
  (was ScriptVector:: stand-ins). The self rows keep their own stand-in leaves (UninitializedCopy_PairCharStringScriptInfo_00cb60f0,
  and 0x00CB6160's std::__uninitialized_fill_n over a global __false_type); an existing self-vs-caller split, unchanged in size.
  Not renamed (outside my entries, not in any PDB). NAME_FIXES lists them.

## q3966 (LSocket family) -- coordinator question
- The claim is right and the self row 0x0090AA10 already defines it. The only wrong binder, 0x008F8C70 LSocket::Close, models LSocket
  in the global namespace. Its only legal port is onto engine/LSocket.h, which renames its own leaf to the PDB spelling
  ?Close@LSocket@LHNetworkLib@@QAE_NXZ (Ego_r 0x00922590, FableWin 0x02B37DE0). That is the spelling 3 of its 5 REFS rows already bind
  (0x008410D0, 0x008F8F50, 0x0090AA40); 0x00840070 and 0x008F9780 bind the global one. The brief's rule (every REFS row must end on the
  new spelling) makes it a whole-family port: 0x008F9780 (~LSocket) would rename too, dragging 0x00840070, 0x0090B080 RequestListener
  and the global LSocket Bind / Listen / SetNonBlocking rows (about 30 rows on the global LSocket model, list in APPLIED q3966).
- Question: may a self row move onto the spelling most of its callers already bind when the count stays 2 (global + LHNetworkLib)?
  If yes, 0x008F8C70 alone closes q3966 (one row, ClearData private opt-in on engine/LSocket.h).

## q4190 (0x0083EEE0 GetUntilCRLF)
- Self rename 0x0083EEE0 -> ?GetUntilCRLF@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@PADPAK@Z (both PDBs); the only REFS row
  0x0083EFE0 already binds it. Callee spellings it now binds: Receive 0x008F99A0 (self spelling), IsReadable 0x008F8210 (the
  LHNetworkLib spelling other rows bind; the old binding was a made-up `IsConnected(int)` stand-in), _M_fill_insert 0x004110D0 (the
  self row's spelling; was ?Insert@Packet_0083eee0@@). Every one of these shrinks the address's spelling set.
- One definition: the object emits the pick-any vector<char>::_M_fill_insert COMDAT from the shared header (FABLE_RETAIL_STLP_VECTOR).
  Not new (the landed 0x0055F710 / 0x00563480 already emit it from the same header), but the COMDAT contents differ across landed
  definers: measured section sizes 0x0083EEE0 266 bytes, 0x0055F710 293, self row 0x004110D0 289 (local template model). All pick-any,
  so the linker keeps one; which one and whether it matters is a COORDINATOR DECISION (pre-existing mismatch, not introduced here,
  but this row adds a third size). The pair<CCharString,CScriptInfo> copy ctor COMDAT that 0x00CB6FD0 emits is byte-identical (57 = 57
  bytes, compared) to the landed 0x00CB5F50 definition.
- Test rewritten as a portable behaviour model (the landed test embedded the old stand-in source).

## q4221 (std::sort family)
- One family of 4 rows, all in this hand-back: 0x00CB7780 (self rename, the claim's binder), its 3 REFS rows 0x00CB5D80 (Init, callee
  respelled), 0x00CB77E0 / 0x00CB7800 (self renamed to FableWin CVectorMap::Sort / Update; no landed referencers). After it 0x00CB7780 has
  one spelling (it had four: the self stand-in and three different caller stand-ins).
- 0x00CB5D80 keeps its HEAD local CDefinitionManager / CScriptDefinitionManager models and its GFSetPDefinitionManager /
  GFGetPDefinitionManager declarations (both have shared headers; pre-existing, WARN SHARED_HEADER_AVAILABLE x2; not re-based). Ego_r.pdb
  order (ErrorCallback 0x00C96F60, Get 0x00C96F70, Set 0x00C96FF0) against retail (ErrorCallback 0x00CD3EE0, 0x00CD3EF0, 0x00CD3F40)
  suggests 0x00CD3EF0 / 0x00CD3F40 are CScriptDefinitionManager::Get / Set (SI, static __fastcall); one source, so NAME_FIXES only as a
  candidate (5 landed rows reference 0x00CD3EF0). Not applied.

## q4531 / q4532 (+ q5231 / q5232): CVillageTask counted pointers
- Two transforms, proven on Patrol and PickCrops and scripted (tools/sdd.py, tools/lclear.py):
  - 0x0065E951 / 0x0065E96D: the shared free stand-in CLandscapeBackgroundPatch_ScalarDeletingDestructor (six rows defined the same
    ?CLandscapeBackgroundPatch_ScalarDeletingDestructor@@YIPAXPAXHI@Z symbol) -> the compiler-emitted non-virtual
    ??_G?$CCountedPointer@V<T>@@@@QAEPAXI@Z on engine/CCountedPointer.h (DTOR_DECL_ONLY + an anonymous-namespace delete-expression
    scaffolding, `// Fidelity: scaffolding, not retail`, the landed 0x00AD79C0 form). No landed row references them (exe scan).
    Descriptive-by-member: FableWin.pdb has this member for other T only.
  - 0x0065FE9A / 0x0065FECB: in-place respell inside the existing header-less row-local CRetailList_CValue_<addr> (STLport
    list<CCountedPointer<T> >::clear): the CValue_<addr> stand-in node value is now the shared CCountedPointer<T>. Their own leaves
    (?clear@CRetailList_CValue_0065d507@@QAEXXZ, ..0065d533..) are unchanged: renaming them to the STLport _List_base::clear would need
    their only REFS rows 0x00660628 / 0x0066063F (`@Helper_OnReadFinished@4`-style stand-ins of the ~list wrapper, themselves
    referenced by 0x0065B598 ...) moved too. Debt for a later pass.
- Not done here (later queue lines): q5233..q5236 (Torch / Horn / SecuritySweep / CarryCrate) take the same two scripts.

## q4382 / q4393 / q4394 (STLport _M_find families)
- Ported: 0x00515700 / 0x009EF360 (map::find stubs, self renamed to the spellings RunCutsceneMacro 0x00CBFB7D binds), 0x009F0570 /
  0x009F0500 / 0x009F05A0 (CCodeSectionManager, header-less local model kept, its maps now std::map<CCharString, unsigned long>; 0x009F0500
  gained a Leaf line, same symbol as HEAD).
- vland WARN STANDIN_LEAF on 0x00515700 / 0x009EF360: the catalog still names them CRegion::SetMiniMapRegionExitTextOffsetX (a propagated
  label); RENAMES section 1 gives the std::map module / find name.
- 0x009F05A0's operator[] at 0x009F1470 now binds the STLport spelling with the nested CCodeSectionManager::CSectionDesc and allocator
  argument; the self row 0x009F1470 still defines a global-CSectionDesc, allocator-less stand-in (count unchanged at 2; NAME_FIXES).

## Flags lines
- 0x0083EEE0: `// Flags: /Gr` (was `/O2 /Oy /Gr`; the catalog CompilerFlags '/O2 /Oy /W3 /Gr' are unchanged and agree).
- 0x00CB6FD0: `// Flags: none` (was `/O2 /Oy /W3`; the catalog has no CompilerFlags for it, q.py / vland grade with none).
- No row needs a flag its catalog entry lacks.

## Header opt-ins
- One: stlport/fable_containers.h FABLE_RETAIL_STLP_SORT_FINAL_INSERTION_OUTOFLINE (hdr/README.md), used by 0x00CB7780 only.
- Existing opt-ins used (no change): FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL, FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N,
  FABLE_RETAIL_STLP_SORT_FASTCALL(_BODY), FABLE_RETAIL_CVECTORMAP_SORT, FABLE_RETAIL_STLP_VECTOR, FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT,
  FABLE_RETAIL_STLP_TREE_MFIND_DECL, FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY, FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY.

## Tests
- Only 0x0083EEE0's test changed (the landed one embedded the old stand-in source snapshot): rewritten as a portable behaviour model with a
  FakeHost struct (no shared-header class modelled), PassPattern `0083eee0_TEST PASS` kept. Every other changed row keeps its landed test
  (standalone models that do not include the row), all PASS in recheck_final.log.

## Funclets
- None: no rebuild/src/compiled/01/21 funclet includes any changed row's file.

## Rule slips
- One Bash call exited non-zero (exit 1): a claim + catalog-flags grep whose last command (a `grep -i flags` on catalog blocks without
  CompilerFlags) matched nothing. No other slip known. No git, no writes under the repo, one compile at a time, every grade in `timeout`.
- q4382's census note first said reloc_bindings lists every distinct spelling; the 0x009EF360 find (q4393) showed the table is incomplete,
  and the APPLIED line was corrected.

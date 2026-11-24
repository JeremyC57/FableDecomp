# NF356 (Account A naming queue, 2026-11-24): six owner rows ported onto their landed size-body opt-ins

Scope: T3_PORT_QUEUE lines nf349 (0x0042AF3C, 0x0042B040, 0x0042B312, 0x005F81BE) and nf352 (0x00429C6F, 0x004291DE), "port (owner onto
opt-in)". All six ports keep the leaf's bytes, relocations and characteristics, grade RELOCATION_MATCH PASS before and after (vland), and
pass both pick-any checkers. Run dirs (land_scripted form), bucketed by the rulings each needs (verdict column = what holds it):
- `batches/b1/` ACCEPT: 0x004291DE (objsec ALL SAME: every section, symbol and COMDAT selection equal).
- `held/hA_rulings_3_4/` RULING_3_4: 0x0042AF3C, 0x0042B040, 0x0042B312 (nf135 address helper; the local model's inline COMDATs swapped for the
  shared header's).
- `held/hB_rulings_1_3/` RULING_1_3: 0x005F81BE (nf135 helper; one NOADDR header inline changes alignment).
- `held/hC_rulings_1_2/` RULING_1_2: 0x00429C6F (one NOADDR header inline in speed mode; .drectve + libcp).
After a ruling, flip that dir's RESULTS verdicts to ACCEPT and land it with land_scripted. The dirs are independent of each other.
No header edited; no test changed (the six landed tests are standalone and unchanged; prechecks in work/pre.log).
Tools: `tools/` (copies of agent_tools/nf352 retargeted to this folder, plus objsec356.py, pkrun356.py, pragscan356.py, helperscan356.py,
mkbatch356.py, vlandchain356.sh, droptest.py `--hleaf`).

## What each row now is
Every owner defines its opt-in, includes the shared header, carries no local class model, and sets its own optimize pragma(s) AFTER the
include (nf189 form: the opt-in block ends with `optimize("", on)`, so a row-head pragma before the include is forbidden; the pragma after
the include keeps the rest of the TU, including the template instances VC7.1 compiles at the end of the TU, in the size mode it had).
The Leaf line is the exact mangled PDB symbol, which is also the symbol every reloc_bindings.tsv line for the address carries.

| Row | Opt-in(s) | Includes | Emission |
|---|---|---|---|
| 0x0042AF3C GetDef<CThingBaseDef> | GETDEF_CTHINGBASEDEF_SIZE_BODY (+ GETDEF_GLOBAL_BODY, SETDYNAMICCAST_REREAD) | fable_string.h, CDefinitionManager.h, CThingBaseDef.h | nf135 address helper |
| 0x0042B040 GetDef<CThingObjectDef> | GETDEF_CTHINGOBJECTDEF_SIZE_BODY (+ same) | ..., CThingObjectDef.h | nf135 address helper |
| 0x0042B312 GetDef<NUISystem::CUIMiscThingsDef> | GETDEF_CUIMISCTHINGSDEF_SIZE_BODY (+ same) | ..., CDefClassBase.h | nf135 address helper |
| 0x005F81BE GetDef<CExpressionDef> | GETDEF_CEXPRESSIONDEF_SIZE_BODY (added to its existing shared-header text) | unchanged (CExpressionDef.h) | nf135 address helper (replaces its explicit instantiation) |
| 0x00429C6F Reset<CComponent> | CCOMPONENT_RESET_SIZE_BODY (+ RELEASEDATA) | engine/NUISystem_CComponent.h | explicit instantiation |
| 0x004291DE ReleaseData<CComponent> | CCOMPONENT_RELEASEDATA_SIZE_BODY (+ RELEASEDATA) | engine/CCountedPointer.h only | explicit instantiation |

- **Emission device.** `template bool CDefinitionManager::GetDef<T>(...) const;` after the header's `template<>` specialization is
  rejected by VC7.1 (C2945, "explicit instantiation does not refer to a template-class specialization"; work/pre_zs.log). For a member of
  the class template CCountedPointer<T> the explicit instantiation is accepted and emits the specialization. The GetDef owners therefore
  take the landed owner form of landing nf135 (0x008C7A70 GetSubDef<CSnowTrollDef>; GOTCHAS nf86 / nf88 / nf135): an anonymous-namespace
  function returning `&CDefinitionManager::GetDef<T>`, marked `Fidelity: scaffolding, not retail`. It adds one per-TU
  `?Reference...@?A0x<hash>@@` symbol (6 bytes, never called, never collides). Helper names census: 0 hits in rebuild/src, tests, include.
- **Includes kept light.** 0x0042B312 with its def header engine/NUISystem_CUIMiscThingsDef.h added `/DEFAULTLIB:"libcp"`; engine/CDefClassBase.h
  (the only type the body needs complete) does not (probe E4). 0x004291DE needs no class model at all: the specialization never touches the
  pointee and the opt-in block forward-declares NUISystem::CComponent. 0x00429C6F needs the complete class for DeleteData<CComponent>
  (its address is stored), so it includes the shared engine/NUISystem_CComponent.h, which adds libcp (see Reset below).
- **Trailing pragma controls (advisor).** Without the trailing pragmas (variants E3), the leaf is still EQ, so the leaf's mode comes from the
  header block, not the row. For the GetDef rows the trailing pair is what keeps the end-of-TU instances (SetDynamicCast / Release /
  ~CDefPointer) in size mode; for ReleaseData it changes nothing; for Reset it keeps DeleteData's characteristics. They stay in every row
  so the opt-in comments' condition (b) ("the owner rows set these same pragmas themselves") stays true.

## Measurements (<dir>/evidence, logs; the vland logs and the link texts cover all six rows in every dir)
- **Grade:** vland `--recheck-landed` HEAD v new: 6 / 6 RELOCATION_MATCH PASS on both sides (logs/recheck_head.log, recheck_new.log). The
  HEAD run warns SHARED_HEADER_AVAILABLE on 0x0042AF3C / 0x0042B040 / 0x0042B312, STANDIN_LEAF on 0x0042AF3C and "leaf matches no symbol"
  (no Leaf line); the new run has no warning.
- **Leaf:** objsec356 (stable section keys + leaf rename) and leafrel: for all six the leaf COMDAT's bytes, relocations and section
  characteristics are equal (0x0042AF3C under the rename stand-in -> PDB). COMDAT selection: 0x0042AF3C and 0x0042B040 go from 1
  (NODUPLICATES, a strong definition) to 2 (ANY, pick-any); the other four were already 2.
- **Other sections / symbols** (evidence/objsec/<a>.txt; UND sets equal in all six):
  - 0x0042AF3C, 0x0042B040, 0x0042B312: the local model's inline COMDATs are dropped (operator= / converting operator=, CRefCounted::DecRef /
    GetRefCount, CDefBase::Release / ReleaseTemporary: 5 each) and the shared header's are added (SetDynamicCast<const CDefClassBase>,
    CDefPointer<const CDefClassBase>::Release / Get / InternalGetObject, CDefPointer<const T>::Release, CIVCountedPointeeBase::AddRef);
    ~CDefPointer<const CDefClassBase> changes 21 -> 24 B (local model v header). All of these are NOADDR in pk_all (no retail address, no table
    line, not leaf-called). The shared ones equal 0x005F81BE's HEAD copies (the shared-header size-mode baseline) in bytes, relocations and
    characteristics, except AddRef's alignment (the header-inline mode effect described for 0x005F81BE below). Plus the helper symbol.
    No .drectve change.
  - 0x005F81BE: + the helper; CIVCountedPointeeBase::AddRef (NOADDR) keeps its bytes but its characteristics change (alignment): it is a
    plain inline defined in a header above the pragmas, so it now takes the command-line mode where HEAD's row-head pragmas gave it size mode.
  - 0x00429C6F: ??0CCPPointerInfo (NOADDR, no table line, not leaf-called) is the same kind of header inline: 25 B both sides, but
    speed-mode bytes and alignment 16 instead of size-mode / 1. .drectve gains exactly `/DEFAULTLIB:"libcp"` (from
    engine/NUISystem_CComponent.h's STLport includes). ReleaseData / DeleteData copies unchanged and EQ.
  - 0x004291DE: every section equal (ALL SAME).
- **Pick-any gate:** pk_all (`--leaf` per side) and pickany_vs_retail, HEAD v new: SAME for every row, GATE PASS; no NEQ copy anywhere;
  reach-flip scan: 0 flips in 6 pairs. Reversed control also PASS (nothing changes, so it cannot discriminate); a planted control
  (ReleaseData flipped to NEQ in the new 0x00429C6F files) FAILS as it must (pk_gate_control_planted.txt).
- **Drop test** (evidence/droptest_<a>.tsv, drop_refscan.txt): the dropped external symbols are 0x0042AF3C's own stand-in and the local
  model's inline members (5 per GetDef row). None has a reloc_bindings.tsv line, a retail address, or a pk verdict other than NOADDR. By
  construction they are inline members of a per-row local model, and each of the ~260 landed sources that carry that model defines its own
  copy. Scratch-corpus scan (helperscan356, 70,487 objects found under the scratchpad, 17 unreadable): und=0 for all 11 symbols, 0 table
  lines; the 143 `refs` hits are in objects that define the symbol themselves (same local model), so no object is left undefined.
- **Link (LINK cleared, /FORCE; evidence/link/):** HEAD 0x0042B040 + 0x005E4BE0: one LNK2005 on ??$GetDef@VCThingObjectDef... (the
  strong owner v the pick-any copy); new: none. All six + the four emitters (0x005E4BE0, 0x00744ED0, 0x006AC430, 0x00605570): HEAD 1
  duplicate, new 0. The watched unresolved names are identical HEAD v new in every set.
- **Pragma order (condition c):** pragscan356 on the preprocessed TUs: C_OK for all six (no optimize pragma before the block; one
  specialization reached per opt-in); planted control (pragmas moved back to the row head) C_FAIL.

## Rulings needed
1. **Header-inline mode changes (hB 0x005F81BE, hC 0x00429C6F).** Moving the row-head pragma below the include (the opt-in rule) puts every
   plain inline defined in the headers above the opt-in block in command-line mode. Two COMDATs present at HEAD change: ??0CCPPointerInfo
   bytes (size -> speed, same 25 B, align 1 -> 16) in 0x00429C6F, and CIVCountedPointeeBase::AddRef alignment (bytes equal) in 0x005F81BE.
   Neither has a retail address, a table line or a leaf call. Accept them as listed changes? The only way to keep them is a pragma before
   the include, which the opt-in comment forbids.
2. **libcp on hC 0x00429C6F.** The move onto the shared class header engine/NUISystem_CComponent.h adds exactly `/DEFAULTLIB:"libcp"`, with
   the UND set equal and the grade RELOCATION_MATCH PASS. "Every other section ALL_EQ" (the NF318 Q1 condition) is not met because of item 1.
   The alternatives fail:
   - DELETEDATA_DECL_ONLY (no class header) leaves ?DeleteData@?$CCountedPointer@VCComponent... UND. Its owner 0x00428B10 defines only the
     stand-in `@DeleteIfSet@4` (NAME_FIXES line), so the nf347 decl-only condition fails.
   - Keeping the local CComponent model contradicts the brief.
3. **The nf135 address helper (hA, hB).** VC7.1 rejects `template bool CDefinitionManager::GetDef<T>(...) const;` after the header's explicit
   specialization (C2945). GOTCHAS nf135 / landing nf135 (0x008C7A70) makes the anonymous-namespace address helper the owner form for a member
   template a shared header defines, so it is not a new device. It still adds one strong `?Reference...@?A0x<hash>@@` symbol (6 B,
   per-TU name, never called) per row. Book the four helpers as scaffolding debt (NF326 Q1 style)?
4. **COMDAT-set change on an owner port (hA; hB for its one added helper).** Both sides of the question:
   - **For:** the brief gates drops by the drop test (passed). The 2026-11-24 NF340 scope ruling gates a deliberate COMDAT-set change by the
     drop test, not by "COMDAT sets SAME".
   - **Against:** the same scope ruling says NF318 Q5 / NF320 Q2 ("COMDAT sets SAME") gate MOVES, and a row moving onto its shared header is
     a move. These ports drop the local model's 5 inline COMDATs and ADD the shared header's 7 unreferenced inline COMDATs plus the helper
     (NF320 Q2: adding unreferenced pick-any COMDATs holds a move). NF348 D1 says a sibling landed before NF320 is no precedent; that covers
     both 0x005F81BE's HEAD COMDAT set and nf135 (landed 2026-11-20).
   - **Mitigating:** every added and dropped COMDAT is NOADDR (no retail address, no table line, not leaf-called). The added ones equal
     0x005F81BE's landed HEAD copies in bytes and relocations (AddRef alignment aside). There is no form without them: the header's
     inline bodies are what the shared specialization expands.

## Queue / coordinator follow-ups
- T3: each dir closes its rows' nf349 / nf352 lines. land_scripted holds an address while ANY T3 line mentioning it is unreleased:
  - b1 (0x004291DE): `--release-t3 nf352`.
  - hB / hC: `--release-t3 nf349` / `nf352`.
  - hA additionally meets nf340 lines 3505 / 3506 (0x0042B312 / 0x0042B040 in column 2, "pick-any debt (copy)", emitter 0x005E4BE0). Their own
    reason already records those copies EQ since nf349, so hA needs `--release-t3 nf349,nf340`, which also closes those two stale lines, or
    the coordinator closes them first.
- NAME_FIXES: 0x0042AF3C's stand-in ?GetDef_0042af3c@... is retired (no landed binder). rebuild/oracles/self_symbols.tsv line 327 becomes
  the PDB symbol at the next self_symbols.py pass.
- TABLE_CHANGES: none (every reloc_bindings.tsv line for the six addresses already carries the PDB symbol; witnesses unchanged).
- RENAMES (hA): the 0x0042AF3C self symbol, plus a file-base DECISION, not applied: `CDefinitionManager_GetDefT_0042af3c` ->
  `CDefinitionManager_GetDefCThingBaseDef_0042af3c` (the 0x0042B040 pattern). engine/CDefinitionManager.h's NF349 comment cites the old path,
  so the rename needs that comment respelled in a header pass.
- NAME_FIXES (hC and top level): 0x00428B10 ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z, whose landed self row defines
  extern "C" `@DeleteIfSet@4` (queue line 703 lists only the older stand-in Destroy_00428b10). Seen, not applied (out of scope).
- Scaffolding debt (if hA / hB land): the four `Reference GetDef<T>` helpers (0x0042AF3C, 0x0042B040, 0x0042B312, 0x005F81BE). They go away if a
  later header pass gives the GetDef size bodies a non-inline owner form.
- The opt-in comments in engine/CDefinitionManager.h ("this template + REREAD" for 0x005F81BE, the owner paths) and engine/CCountedPointer.h
  ("+ FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA") still describe the owners' HEAD form; a later header pass may say "ported (NF356)".

## Process deviations (disclosed; results unaffected)
- The six final texts were prechecked (clang, PASS) in the same Bash call that launched their compile, after the check succeeded
  (`ok=1` gate in that call), not in an earlier call. The same holds for 0x0042B312's E4 probe (`pre ... && launch`).
- The four emitter sources (0x005E4BE0, 0x00744ED0, 0x006AC430, 0x00605570) were prechecked before their locks were claimed, not after.
  Each was then locked, compiled once (HEAD, link control only) and released.

## Grade budget (pass.py grades this pass, all at one path work/one/<addr>/; plus one vland HEAD and one vland new run)
0x0042AF3C 5, 0x0042B040 5, 0x0042B312 6, 0x005F81BE 5, 0x00429C6F 7, 0x004291DE 7 (two duplicate compiles from a glob overlap). Controls:
HEAD of 0x005E4BE0 / 0x00744ED0 / 0x006AC430 / 0x00605570 once each (link emitters).

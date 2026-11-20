# NF151 notes (round 10, PRIORITY pick list: CShaderRenderManager and CEngineLightingManager, 18 entries)

Counts (from APPLIED.tsv / STATUS.tsv): entries 10 done, 6 done-already, 1 deferred (q4541: row 0x00BB8090 held by NF150), 1 blocked
(q4669: row 0x009BEF80 is a naked asm bake, authoring), 0 rejected, 0 open. Rows: 23 changed (ported), 14 not-a-binder, 1 deferred (0x00BB8090),
1 blocked (0x009BEF80). Final recheck: recheck_final.log, one vland --recheck-landed run over all 23 changed rows: 4 MATCH + 19 RELOCATION_MATCH, every test PASS. selfcheck.log: 0 problems.

## Census (measured)
`work/probe_all.tsv`: every one of the 115 distinct REFS rows compiled once at HEAD and every relocation at the 16 entry targets dumped
(binds.py / reloc_identity.check). 0x00BE6F70 / 0x00BF4570 are July candidates-header rows (DIFFER at HEAD, 198v2654 / 316v4431); their objects
reference none of the entry targets (undefined symbols are the row's own Apply/SetupLandscape* helpers), so they are not-a-binder.

## Per entry
- q11717 0x00988020: the View stand-ins replaced by the shared headers; MATCH at the first grade. The +0x2908 read is FableWin's inline
  CRenderStateManager::IsSoftwareProcessingEnabled (new overlay opt-in). `force` is unused in retail. Test rewritten on the shared headers.
- q11738 0x0098B420, q4500 0x00B44CE0: access-only self fixes (`private:` in the row's existing local model, in place).
- q4499 0x00B44FE0: in place in the existing local models: `private:`, CManagerLocalLight nested in the local CEngineLightingManager model (same
  class token, namespace change only; the object defines no symbol naming it), the centre parameter `const C3DVector&` (both PDBs; both callers
  pass a C3DVector). The local C2DVector (used only by this parameter) is gone. The nested light's slot-0 virtual is renamed `v00` -> `Delete`
  (Ego_r name, as engine/CEngineLightingManager.h; declaration only, emits nothing).
- q4565 0x00B44F20 + 0x00BBC0B0: handed back together (no split): self PBV -> ABV in place, binder PAV -> ABV in place.
- q4506 0x00B47FD0: ported onto engine/CEngineLightingManager.h (0x00B47BC0 form) with two new overlay opt-ins; RELOCATION_MATCH at the first
  grade. The heap 0x01436FB0 moves from `g_LocalLightHeap` (U) to the header's `LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight` and
  CreateMoreBlocks 0x00B48F90 from QAE to AAE: both already bound by 0x00B47BC0, so those existing splits shrink (no new spelling). The row's
  made-up same-TU overflow body (calling `GrowVectorStorage_s7`) is gone. Test rewritten: the first version used `#define private public`, which
  respelled CreateMoreBlocks in the test object (LNK2019); the final test keeps the header's access.
- q4541 0x00988140: 9 binders respelled; 7 in place (local-model declaration -> `const CPixelShader&`, call dereferences; scripted with
  tools/aps.py after proving it on 0x00B54310 / 0x00B63260), 2 by the 0x00B61690 / 0x00B88BC0 ports. 0x00BB8090 is held by NF150 (NAME_FIXES).
- q4649 0x009E1280: in place, three callees moved under `private:` (UpdateObjectSpaceLighting, plus UpdateWorldSpaceTextureTransform 0x009894B0 /
  UpdateObjectSpaceFogTransform 0x00989540, both PDBs private). Those two self rows still defined the public spelling, so their access was
  fixed in place too (leaf-only self renames outside the entries; both PDBs agree; all 4 referencers 009e1280 00ba1b00 00ba5260 00bb2540 now
  bind AAE).
- q4651 0x00B99670: ported onto the header; the private SetVSConstants is reached through the existing public inline
  SetVertexShaderConstantsDirect opt-in (as 0x00B99300; its object now also carries that inline's COMDAT, as 0x00B99300's does).
- q4669 0x013BC470: 0x01223D00 now defines `CShaderRenderManager::SingletonInstance` on the shared header (NF92 0x0121A440 form; leaf `_$E1`; the
  WARN RELOC_CONFLICT `_$E2` against 0x01219F00 is the known TU-local collision). 0x00B61690 / 0x00B88BC0 / 0x00BBC130 / 0x00B6B0B0 reach it through
  the inline Get(). Left: 0x009BEF80 (`__declspec(naked)` asm bake, blocked: authoring) still declares `g_CShaderRenderManager_013BC470`.
- q11731 / q11732: 0x00B61690 / 0x00B88BC0 ported onto engine/CShaderRenderManager.h + CScopedShadersEnable.h (0x00B5E700 form): the CShaderEnableState
  stand-in is gone from the tree. q11730 / q11733 (0x00B5E700 / 0x00BCB800) were already clean (probe).

## Acceptance checks
- objdump defined-symbol diff, new object vs HEAD object (tools/symdiff.sh), for every port: only own leaves, shared-header inline COMDATs and
  STLport COMDATs change (listed per row in STATUS.tsv notes).
- No EH funclet row includes any changed row (grep of rebuild/src/compiled/01/2*).
- No self rename moves a row's file (the catalog name stays; RENAMES section 1 repeats it), except the module/name of 0x01223D00
  (`_static_init::g_CShaderRenderManager_013bc470` -> CShaderRenderManager `dynamic initializer for 'SingletonInstance'`); the coordinator
  may rename its file.

## Full binding diff (work/pairdiff.log)
Every changed row compiled twice (HEAD source in pH, hand-back source with the overlay in pS) and ALL relocation pairs diffed (tools/pairdiff.sh).
Changes beyond the entry targets, each now a RENAMES section-2 line, each moving to a PDB spelling another landed row already binds
(reloc_bindings.tsv witness in brackets), so no address gains a spelling:
- 0x00B61690 / 0x00B88BC0: EnablePixelShaders 0x00988110 / DisablePixelShaders 0x00988190 `CShaderEnableState` -> `CShaderRenderManager` QAE
  (both PDBs; [009d8ad0]); ResetWorldTransform 0x00988290 AAE -> QAE (both PDBs public; [009e1280]).
- 0x00B47FD0: CManagerLocalLight vtable 0x012A2228 -> `??_7CManagerLocalLight@CEngineLightingManager@@6B@` [00b47bc0]; heap 0x01436FB0 ->
  `LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight` [01224730]; CreateMoreBlocks 0x00B48F90 QAE -> AAE [00b47bc0].
- 0x009E1280: 0x009894B0 / 0x00989540 QAE -> AAE, measured: bindchk.sh shows 00ba1b00, 00ba5260, 00bb2540 bind AAE for both.
No other pair changed in any of the 23 rows. gate.py needs the coordinator's landing layout (gate_targets.json, pair dumps), so it was not run.

## One definition (0x00B47FD0)
Its object carries the STLport `vector<CManagerLocalLight*>::_M_insert_overflow` COMDAT (0x00B49DF0's own leaf, noinline under
FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE). Precedent: the landed HEAD objects of 0x00B47BC0 and 0x00B49DF0 both already define that symbol
(objdump -t), so this adds a third copy of the same instantiation, not a new clash.

## Recheck warnings (recheck_final.log, other than SHARED_HEADER_AVAILABLE)
All pre-existing label differences on callees these hand-backs do not touch, plus the known TU-local `_$E2` collision for 0x01223D00:
RELOC_NAME on 0x0098B420 (CLightVector ctor / LightCopy stand-ins, unchanged), 0x00B54310 (GPCamera vs catalog g_pEngineCamera),
0x00B75CA0 / 0x00B77E20 / 0x00B76BC0 (0x01396DC8; 0x00B76BC0 binds a descriptive extern there: NAME_FIXES line), 0x01223D00 (`_$E2` vs the
0x0122A4C0 catalog label: NAME_FIXES line). No HARD line.

## Header opt-ins (hdr/README.md)
FABLE_RETAIL_CRENDERSTATEMANAGER_ISSOFTWAREPROCESSINGENABLED_INLINE, FABLE_RETAIL_CENGINELIGHTINGMANAGER_ADDLIGHTPRIMITIVE,
FABLE_RETAIL_CENGINEINTERNALPRIMITIVELIGHT_TYPED. No FABLE_FORCEINLINE added. No header rename (no "needs header claim" items).

## Descriptive names
None new. All names are PDB spellings (two sources each).

## Tests
Rewritten on the shared headers: 0x00988020, 0x00B47FD0. The 0x00988020 test keeps `#define private public` while linking the
row object: safe there because the only symbol that crosses the two objects is the public leaf ApplyVertexShader (IsSoftwareProcessingEnabled is
public and a COMDAT; no private member is called), unlike 0x00B47FD0, whose first test link failed on a private callee. 0x0098B420's self-contained test gained `#define private public` before its verbatim
copy (AllocateLights is private now). The other tests are self-contained host models and compile unchanged.

## Flags lines
None added.

## For the coordinator
- 0x009BEF80 (asm bake) and 0x00BB8090 (NF150) keep the old spellings: NAME_FIXES.tsv.
- Catalog label of 0x0122A4C0 (NAME_FIXES.tsv): CShaderRenderManager `dynamic atexit destructor for 'SingletonInstance'`.
- 0x00BE6F70 / 0x00BF4570 grade DIFFER at HEAD with the q.py leaf pick (July candidates-header rows, no Leaf line); untouched.

## Rule slips
- Two Bash calls failed: one exited 1 (a relative path in the first probe run), one was refused (a foreground `sleep`).
- Order: I worked q11717 and q11738 before q4541..q4669 (the specifics order puts them after); each was taken to a terminal state before the next.
- grades_used counts q.py grades plus vland runs; the HEAD-object compiles behind the symbol diffs (one per port) are extra and not counted.

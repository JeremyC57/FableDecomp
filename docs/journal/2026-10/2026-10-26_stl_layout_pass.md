# STL layout pass — NOTES (2026-10-26)

Worktree: `$S/w22/layoutpass/repo` (no git run; one stray `git diff --no-index` early in the pass, read-only).
Retail container sizes used (STLport 4.6, VC7.1): list 4, vector/map/set/multimap/multiset/string/CArray 0xC,
deque 0x28, CVectorMap 0x10. Found during the pass: vector<bool> 0xC (byte vector), bitset<N> 4*ceil(N/32),
CDefPointer 4, CIntelligentPointer / CCountedPointer 8. libpk (Dinkumware) untouched.

## Counts
| item | count |
|---|---|
| engine headers changed (generator-patched + 2 hand) | **714** (712 patched, CBankFile/CMemoryDataOutputStream hand; CDefinitionManager/CCombatTypeDef reviewed, no layout change needed) + INDEX.tsv (713 sizes) |
| headers with sizeof change | 713 (old/new size and evidence per header: `LAYOUT_CHANGES.tsv`) |
| evidenced | **322**: alloc (operator new) agree 146, GetSizeofClass agree 121, landed models 47, regrade rebinds 8 |
| no evidence | **389** (`NO_EVIDENCE.tsv`) |
| contradicted | **3**: CEngineResourceManager (hdr 0x3410 vs new 0x31e0), and two `_quarantine` headers CEngineLandscapeMap (0x74 vs 0x60), CEngineLocalDetailGenerator (0xa4 vs 0x7c). This is TLC drift that the PDB does not describe; see Decisions. |
| base-class source | Fable.exe RTTI ClassHierarchyDescriptors (`ghidra_out/rtti_bases.tsv`, 2529 classes, `tools/decomp_pipeline/rtti_bases.py`). Template names are now demangled (`msvc_demangle.py`, Ego_r spelling): 89 template classes gained bases |
| sources/tests edited | 200 (186 src + 14 tests). Originals are in `work/orig_src/` |
| includers migrated to shared headers | 110 kept (`work/mig_kept.txt`) and 5 reverted to their local model (`work/mig_reverted.txt`). The remaining 90 edits are offset rebinds, pin re-pins, alias renames and model corrections |
| rows regraded (final, all affected) | **1296**: 199 MATCH + 1094 RELOCATION_MATCH, all tests PASS; 3 not passing, and all 3 were already in that state before the pass (below) |
| regressions from the pass | **0** |
| rows parked | **0** |
| RELOC_IDENTITY | 0 |
| leafscan | 59 mangled leaves checked, 0 bad. One pre-existing miss, 004a2d70 (public vs protected), was fixed and regraded (RELOCATION_MATCH PASS) |
| purity | no new `__asm` / naked / `_emit` / `volatile` / `_STL` in any changed source, test or header |

How "affected" was decided: rows whose source or test changed, or whose source or test includes a changed header (directly or
transitively); this was unioned with every row edited by hand. Results: `REGRADE_FINAL.tsv`, logs in
`rg/final/chunk.*.d/out.log`. Earlier rounds: r1 (1166 rows), r3 (62 fixes), m1 (115 migrations), b1/b2 (baselines on
`base/`, the pre-pass tree).

### Not passing before or after the pass (same status in baseline b1)
- 009cdf50 WriteRecoveryFile: DIFFER(246v246) PASS, the same in b1.
- 00ab6ad0 GetXAxis and 00ab6ae0 GetYAxis: MATCH, test FAIL, the same in b1.
- 11 rows that the catalog lists as MATCH grade RELOCATION_MATCH in this lane. Baseline b2 on the pre-pass tree gives the
  same result, so the pass did not cause it (`work/m2r.txt`).

## Decisions
1. **Two-pass layout engine** (`tools/decomp_pipeline/stl_layout.py`):
   - pass D must reproduce every donor offset; pass N is the retail layout;
   - headers are patched by shift / resize / drop atoms;
   - `pack(1)` headers keep explicit pads;
   - the tree compare byte stays inside the opaque [0xC];
   - stateful templates are refused (left opaque, with a flag).
2. **Evidence-backed TLC drift** (retail is not Ego_r):
   - DROP `CBankFile::{Dependencies, CurrentEntryDependencies}` and `CLipSyncDataBank::SpeechAnalyser`;
   - RESIZE `CEngineLandscapeRenderer::MeshBuilder` to 0x484. Evidence: operator new 0x6B4, plus 5 landed models that put
     every later member 0x83C lower.
3. **Template bases** (found in this session):
   - The RTTI base table had kept template names mangled. Every `CWaterPatchTesselatorBase<...>` therefore lost its
     `CLandscapeTesselatorBase`, and that base's `CArray`.
   - Fixed in the generator: a demangler, plus normalised template lookups.
   - The only header that changed as a result is CEngineWaterRenderer: 0x5cc became 0x5c4. Three sources now agree:
     operator new 0x5C4, and 5 landed models that put WaterBumpFactor at +0x5c0 and StaticMapBankFile at +0x5a8.
   - Generator re-validated: `gentest/` produced the same headers (semcmp SAME) for CEngineWaterRenderer,
     CSpecialAbilitiesPhysicalShieldDef, CAIStateGroup_HoverHornetQueen and CWorld.
4. **Model triage** (`MODEL_EVIDENCE.tsv`, members actually used in landed code):
   - Totals: 3547 AGREE_RETAIL (moved members that the code confirms), 1847 AGREE_BOTH, 0 AGREE_EGOR_ONLY, 24 DISAGREE.
   - Every model that contradicted the header was a misnamed local model, not a header error. 14 such files were
     rebound to the retail-header names; each is byte-neutral and was regraded:
     - BoastingCrowd: +0x30 is CurrentEncouragementState (EnterCrowdState 0x784260 compares CrowdState at +0x2c);
     - quest manager: +0x68 is Boasts and +0x6c is AcceptedBoasts (as GetBoast 0x4AF7A0 and IsBoastTaken 0x4AF810);
     - CWorld: +0xe0 is GuildMasterMessagesEnabled; +0x104 is RegionLoadStatus; +0xf8 is SaveGameLoadStatus; +0x12f is
       DisplayMiniMapOnRegionLoad (six different hero-move functions set it, including the door exit);
     - CTCInventory: +0x148 is QuickAccessItems and +0x154 is ContextSensitiveItems (agrees with 20+ landed models);
     - CTCInventoryTrade: +0x220 is PTop_Level_List (as the constructor 0x59EB0C);
     - CTCTavernGame: +0xc4 is PreviousTavernGameState;
     - CMemoryFile: +0x20 is Pathname;
     - HoverHornetQueen: the clears are CurrentCirclePoint +0x4c and RouteDirection +0x64;
     - PhysicalShieldDef: LoopingSound is at +0x50 and SpellLevel is 0-based;
     - CEngineSimulationManager::PausedWorldFrameUpdate: `this` is the CEngineComponent subobject at +4, so the header is
       right;
     - CEngine::OnPreDeviceReset: the +0x140 vector lies inside the Ego_r FrameMemoryPool (a TLC difference).
       ScratchZBuffersD24S8 is +0x14c, as GetScratchZBufferD24S8 and Uninitialise read it.
   - The remaining 24 DISAGREE rows are a different class that shares the name:
     - CHandleBasedMemoryPool at 0xC28xxx is a second, third-party pool; the 0xA02xxx one agrees;
     - CWorld::SetAsPaused belongs to the steering library;
     - nested CSubtitleRenderer::CSetTextInfo, CLocalDetailCacheMap::CObjectCacheGroupCollection, CTCEntityEvents' `sub`
       and CTCDPhysicalObstruction's ImmuneThings;
     - a mislabelled `CGameScriptInterface::GiveHeroGold` at 0x435000;
     - CEngineResourceManager (contradicted, below).
5. **CEngineResourceManager**: the header is STL-corrected, but operator new is 0x230 smaller (a TLC rework of the
   bank/pool area; CMeshDataBank 0x460 alone is confirmed by its own operator new). No member mapping can be proven, so
   the header is left as is and listed as contradicted. Its landed users keep their local models.
6. **5 migrations reverted** to their pre-migration local models: 0047dc69, 0048f50c, 006d0180, 007d7ba0 (DIFFER) and
   00b44f20. No header was reverted to donor offsets.
7. **Superseded retail-only members** became aliases. The CText_GetAlignment local model was kept.
8. **Still unresolved**:
   - `RECONCILE.tsv` stays stale until the generator reruns.
   - GOTCHAS:158 still has its "VC6-era" mention.
   - 16 structs have a leading base region with no RTTI (non-polymorphic or unnamed bases), so a container inside such a
     base cannot be seen. They are CBankFileDependencies, CBase#0xc8 and 14 tool/script classes (`layout` flag
     `nortti_leading_padding`).
9. **Generator**: `class_struct.py` takes ROOT from `FABLE_ROOT`. `gen_class_headers.py` uses `stl_layout.remap_emitted`
   and keeps the `class X { public:` keys. New provenance lines avoid the RETAIL_MARK prefixes.
10. **No untracked inputs were symlinked** into the worktree. Fable.exe was read from `$S/dl/game/...` and never copied in.

## Proposals
**GOTCHAS one-liner:**
- Retail containers are STLport 4.6:
  - list 4; vector, map, set, string and CArray 0xC; deque 0x28; CVectorMap 0x10; vector<bool> 0xC; bitset<N> takes
    4*ceil(N/32) bytes.
  - Engine headers carry these offsets (`stl_layout.py`, with bases from RTTI `rtti_bases.tsv`, templates demangled).
- A local model whose name disagrees with the shared header almost always misnames the member; the offset is right.
  Rebind it to the header name.
- A `this` taken through a CEngineComponent virtual is the subobject at +4.

**FINDINGS_LOG paragraph:**
- **STL layout pass (2026-10-26).** 714 engine headers were re-laid out from Ego_r (Dinkumware) to retail STLport 4.6
  container sizes, with bases from Fable.exe RTTI. 322 class sizes are confirmed: 146 by operator-new sizes, 121 by
  GetSizeofClass, 55 by landed models or rebinds. 3 are contradicted (CEngineResourceManager and two quarantined headers).
- The PDB does not describe some TLC drift, so the engine models it with explicit DROP/RESIZE:
  - CBankFile loses both dependency vectors;
  - CLipSyncDataBank loses SpeechAnalyser;
  - CEngineLandscapeRenderer::MeshBuilder is 0x484.
- Template RTTI names must be demangled. Otherwise `CWaterPatchTesselatorBase<T>` loses its base's CArray, and
  CEngineWaterRenderer comes out 8 bytes too large.
- 110 landed local models moved onto the shared headers through the `#define X XDonor` pattern. 14 misnamed local
  models were rebound to the header names, e.g. CWorld +0xe0 GuildMasterMessagesEnabled, CQuestManager +0x68 Boasts,
  CTCInventory +0x154 ContextSensitiveItems.
- The final regrade covered 1296 rows: 199 MATCH, 1094 RELOCATION_MATCH, 0 regressions. 3 rows did not pass, but they
  were already in that state before the pass.

## Files
- `LAYOUT_CHANGES.tsv`: per header, donor and retail size, alloc and GetSizeofClass evidence, landed agreement, verdict.
- `NO_EVIDENCE.tsv`, `MODEL_EVIDENCE.tsv`, `REGRADE_FINAL.tsv`, `CHANGED_FILES.txt` (923 worktree paths).
- Regrade logs: `rg/{r1,r3,m1,final,fix2}/chunk.*.d/out.log`, baselines `rg/b1`, `rg/b2`.
- Tools: `tools/` (`layout.py` = `stl_layout.py`, `msvc_demangle.py`, `patch_headers.py`, `evidence_table.py`,
  `evid_models.py`, `affected.py`, `rg_run.sh`, `rg_base.sh`, `edit.py`, `rename.py`, `rdis.py`, `worldscan.py`).

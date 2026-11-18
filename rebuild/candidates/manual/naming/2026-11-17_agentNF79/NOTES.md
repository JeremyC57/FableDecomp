# NF79 notes (round 7: the land1117fu follow-ups q10956..q10974)

Work dir: $S/w22/agentNF79. Tools copied from NF76 and repointed (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh, xref2.py, mkren.sh,
ndiff.py); new: info.py (catalog / source / leaf per row), probe.sh (grade the landed row untouched -> work/xo_<addr>.txt), r2.py (RENAMES
section 2 from work/xo_ vs work/xn_ dumps), fcheck.sh (sources that include a row's file; none found for any changed row, so no EH
funclet regrades), rdis.py (retail disassembly). Grades: grades.log; per-row vland logs work/vl_*.log; final run recheck_final.log.
APPLIED.tsv lines for q10966 / q10969 come from overrides.tsv (they carry the 0x0059A238 deferral).

## Rule slips
- Three Bash calls exited non-zero: a helper named dis.py shadowed the stdlib `dis` module (capstone import failed); an `ls` of a header that
  does not exist (NUISystem_CActionParamFLOAT.h; the header is NUISystem_CActionParamFloat.h); an `ls overrides.tsv` before the file existed.
- Order: the q10970 port (0x0057770A) was done before the q10971..q10974 respellings; the brief asked for it after them.
- 0x00894AA0 took 4 logged grades (two SRC_FAIL macro-dependency rounds of the same header form, then DIFFER, then RM) plus one direct q.py
  compile to disassemble the DIFFER; I counted the two compile-error rounds as one form.
- 0x00A25E70 went past the three-failed-forms limit: a SRC_FAIL and five DIFFER / failed forms (6 grades in STATUS.tsv, plus one direct
  q.py compile to disassemble) before I blocked it.
- Two compiles ran through q.py directly (to keep the object for objdump), outside g.sh and so outside grades.log: 0x00A25E70 and 0x00894AA0.
- I appended three claimed extra rows (0x0054C050, 0x00558EC0, 0x00970C10) to my rows.txt so that STATUS.tsv carries them.

## Deferred
- 0x0059A238 (CFrontEndManager::Action) is held by NF78 (HELD.txt): it binds back 0x0059B039 as ?top@CScreenStack@NUISystem@@... and
  0x013CA818 as ?PInstance@...; both listed in NAME_FIXES.tsv for the coordinator.

## Leads (not proven, not applied)
- 0x0122A740 (catalog _global::Release_013ca818, the same 57-byte body as 0x00404490) also binds 0x013CA818 as g_pointer_013ca818 and is not
  in reloc_bindings. It sits among the static atexit destructors (0x0122xxxx), so it is most likely the compiler's atexit destructor of
  CTBaseSingleton<CFileInstaller>::PClass (??__F...); no PDB names it, so its own name is left to the coordinator.
- 0x00A25E70 next form to try: the mesh null tests as `.Data` reads (the old stand-in read the field) instead of the inline Get().

## In-place edits of pre-existing local models of shared-header classes (listed for review)
No new local model was added. Where a full port was out of reach, I changed an existing local model only as far as the PDB spelling needed:
- 0x00B76BC0 CEngineWaterRenderer::RenderBackground: the local CShaderRenderManager indexes PSConstTable with operator[] and reads
  ReservedPresets through GetAsCArray() / size() (the local CArray model gave way to engine/CArray.h); the local CEngineWaterRenderer declares
  RenderBackground / RenderBackgroundList private.
- 0x00BE1BA0 CEnginePrimitive2DViewportManager::Render2DPrimitive: operator new moved from the local derived CEngineSubPrimitive2DViewport to
  the local CEngineSubPrimitiveBase model (the shared base is not novtable; retail's vptr stores need the local novtable base).
- 0x00A256F0 C3DMeshInfo::BeginLoading: the local C3DMeshInfo model's BeginLoading made const (MeshData mutable), the local C3DMeshLODInfo
  inline ctor takes const C3DMeshInfo* as the PDB ctor does.
- 0x0093AD70 CAIStateGroup_BanditSitRoundCampfire::SitMain: the local model (no shared header for this class) declares the five activity
  members private.

## Rows still carrying pre-existing local models of shared-header classes (vland SHARED_HEADER_AVAILABLE)
- 0x0095D590, 0x0092C8A0, 0x0095D3A0, 0x00969AE0: CAIBrain / CAIStateGroupBase (and CThing, CCombatManager, CTCCreatureModeManager,
  CRightHandedSet, CTCPhysicsBase, CTCVillage, CTCVillageMember, CSoundPair as listed by vland).
- 0x00BCC700: CEngineSubPrimitiveRenderer. 0x00A5F010: CSystemManager. 0x00490927 / 0x00582944 / 0x0089F4A0: their older local models.
- 0x00B76BC0: CShaderRenderManager, CRenderManagerCore, CRenderStateManager, CTexture, CEngineWaterRenderer.
- 0x00A256F0: CResource, CResourceList, CResourceBank, CMeshDataBank, CBankFileAsyncEntry, CIVCountedPointer, C3DMeshLODInfo, C3DMeshInfo.
- 0x00BE1BA0: CBaseClassNonCopyable, CEngineSubPrimitiveBase, CEngineInternalPrimitiveBase, CEnginePrimitive2DViewportManager.
- 0x00894AA0: the stand-ins CDefinitionManagerMethods / CGameScriptInterfaceMethods (its own leaf is still the stand-in
  ?BeginLoadingMesh@CGameScriptInterfaceMethods@@QBEXABVCCharString@@@Z; the catalog name CGameScriptInterface::BeginLoadingMesh is right).

## Tests rewritten (each keeps its catalog PassPattern)
- rows/005952c3_test.cpp, rows/008916a0_test.cpp, rows/00beb540_test.cpp: the old tests linked the old stand-in symbols (LINK_FAIL). The new
  ones link the row object under its new names; 0x008916A0's and 0x00BEB540's supply the imports as link stand-ins with the PDB mangled names
  (local classes in the test only), 0x005952C3's includes the shared headers.

## Descriptive / one-source names (flag for the coordinator)
- 0x00A00D90 CGraphicInfoFrame::GetData: FableWin.pdb-only (?GetData@CGraphicInfoFrame@@ABEPAVCFrameData@1@XZ); the body is exactly that
  accessor (create the mutable Data on first use, return it) but no Ego_r copy, no retail caller and no donor-order check exist.
- 0x013CA818 ?PClass@?$CTBaseSingleton@VCFileInstaller@@@@0V?$CCountedPointer@VCFileInstaller@@@@A: one PDB source (Ego_d.pdb PUB32,
  ghidra_out/egod_carved_names.tsv); the second is the shared header's member name (which was itself spelt from the sibling template), backed
  by the Ego_d PUB32 PClass statics of four other CTBaseSingleton<T> instances.
- 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A: Ego_d.pdb PUB32 only among the PDBs; the landed 2D / 3D mesh,
  particle and cloth rows use the same spelling.
- 0x0143BAC4 ?MemoryPool@CEngineSubPrimitiveBase@@0PAVCSimpleMemoryPool@@A: Ego_d.pdb PUB32 + the shared header's private static.
- 0x0059AC41 _M_decrement / 0x0059AD35 _M_pop_back_aux / 0x0059A8F3 _M_subtract: STLport 4.6 helper names (the PDBs carry Dinkumware deque
  members only); the class placement (_Deque_iterator_base) and the protected access of _M_pop_back_aux follow STLport 4.6 _deque.h, which is
  not on disk here (checked with find): from the library source as I know it, cross-checked with the landed 0x0059AFD7 binding
  ?_M_increment / ?_M_subtract@?$_Deque_iterator_base@... and the retail bodies.

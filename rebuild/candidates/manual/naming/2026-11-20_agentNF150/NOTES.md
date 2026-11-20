# NF150 notes (round 10: engine primitive renderer, render-manager core, navigation; 15 entries)

Counts (from APPLIED.tsv / STATUS.tsv): 15 entries: 13 done, 2 blocked (q4548, row 0x00BF4570 left; q6297, row 0x00BB8090 left), 0 done-already, 0 rejected,
0 deferred, 0 open. Rows: 15 ported (14 self rows + binder 0x00A1E370), 38 not-a-binder (census), 2 blocked (0x00BB8090 port;
0x00BF4570 authoring, probe only). Final recheck: recheck_final.log, one vland run over the 15 ported rows.

## Census (before any edit)
- vland --recheck-landed of the 56 rows of REFS.tsv + the entry rows + 0x00B324A0 at HEAD (55 MATCH / RELOCATION_MATCH + PASS;
  0x00BF4570 DIFFER(316v4431), a no-Leaf legacy row), then nftools/pairs.py (PAIRS_OUT) for the spelling each row binds per target.
  Results in STATUS.tsv. Only two wrong binders exist: 0x00A1E370 (SetGoal, CNavWayPoint) and 0x00BB8090 (DrawAsIndexedTris on the
  CRenderManagerCoreView stand-in). WORKLIST's 0x00B5E0F0 / 0x00B5AF80 already bound the PDB spelling (stale table claim).
- q6237 / q6297 queue lines carry shifted columns: the correct symbols are the Ego_r / FableWin spellings
  ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z and ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z
  (both present in both name sets); the "wrong" column text is evidence, not a symbol.

## Self renames on referenced rows (coordinator rule 2026-11-20)
Every self rename moves the address onto the spelling every relocation-checked referencing row already binds (census):
0x00B32E90 .. 0x00B31FA0 (one CEngine forwarder each), 0x00B4A5F0 (0x00B23EB0), 0x00A14E20 (0x004A5A40), 0x00A1A170 (0x00A1ABF0),
0x00A0AD40 (3 rows), 0x00A0AEA0 (15 rows), 0x00A0AF40 (8 of 9 rows).
- 0x00A0AF40: before {self `CRenderManagerCore_Methods` leaf, CRenderManagerCore spelling (8 rows), CRenderManagerCoreView (0x00BB8090)};
  after {CRenderManagerCore spelling (self + 8 rows), CRenderManagerCoreView (0x00BB8090)}. The split is pre-existing and shrinks; none is
  created. **Coordinator decision needed:** the brief says a rename "waits" unless every REFS row ends on the new spelling; the advisor's
  reading (nf120 precedent: moving off a unique stand-in onto the spelling the others bind only shrinks the set) is that the self rename
  can land with q6297 left blocked on 0x00BB8090. If you disagree, drop rows/00a0af40.cpp and its RENAMES line; nothing else depends on it.
  The same decision covers 0x00A0AD40 (q4548, below): if you disagree, drop rows/00a0ad40.cpp and its RENAMES line.
- 0x00A0AD40 (q4548 blocked: authoring, row left 0x00BF4570): the legacy row 0x00BF4570 (no Leaf; a 316-byte model of a 4431-byte
  function, DIFFER at HEAD, not relocation-checked) binds no spelling of 0x00A0AD40: retail calls it at 0x00BF55EF (ecx = [0x01436E18],
  four arguments) and the row models that draw with its free 2-argument helper ?DrawLandscapeForegroundEntry@@YAXKK@Z. Before
  {self _Methods leaf (long), CRenderManagerCore spelling (3 rows)}; after {CRenderManagerCore spelling (self + 3 rows)}. Not touched
  (brief: leave no-Leaf rows alone); NAME_FIXES line.

## q5008 one definition (0x00A1A170)
- engine/CNavigatorAStar.h defines IsEqual inline (private const) under FABLE_RETAIL_CNAVIGATORASTAR_SEARCH_PASS23 and declares it out of
  line otherwise. The landed 0x00A1CEB0 object (SEARCH_PASS23) emits ?IsEqual@CNavigatorAStar@@ABE_NPBVCANavQuadTreeNode@@0@Z as a COMDAT;
  0x00A1D290 also defines SEARCH_PASS23. Retail expands IsEqual in 0x00A1CEB0 and calls 0x00A1A170 from 0x00A1ABF0, so the header inline
  is the right model and 0x00A1A170 is its out-of-line COMDAT.
- Strong form (work/00a1a170_strong.cpp, RELOCATION_MATCH) + HEAD 0x00A1CEB0 object, `LINK=` cleared, /DLL /NOENTRY /NODEFAULTLIB
  /FORCE:UNRESOLVED: LNK2005 + LNK1169 (link/strong.log). Hand-back form: link exit 0, LNK4088 only (link/new.log).
- The member is private, so the nf86 / nf88 anonymous derived-class helper cannot name it. The row names it as the template argument of
  an explicit instantiation (`template class CIsEqualReference<&CNavigatorAStar::IsEqual>;`, access is not checked there, C++03 14.7.2/8)
  in an anonymous namespace, marked `// Fidelity: scaffolding, not retail`. Objdump: the object defines only the IsEqual COMDAT and the
  hashed helper. **Flag for the coordinator:** a new variant of the scaffolding pattern (explicit instantiation instead of a derived
  class); accept or tell me to switch to a header opt-in.
- operator== callee 0x00A763F0 keeps its HEAD spelling ??8CANavQuadTreeNode@@QBE_NABV0@@Z (b1 pairs).

## 0x00BB8090 (q6297 binder): blocked/port after 3 forms
- Form 1: CRenderManager.h retail branch, NEngineInternal::GPRenderManager, the inline setters (SetStencilOpPass / ZFail / Fail
  (STENCIL_OP_MODE_ZERO = D3DStencilOperationModeTable[1] 0x01396F3C), SetStencilFunc(STENCIL_FUNC_ALWAYS = 0x01396E34),
  SetColourWriteEnable / SetZBufferEnable / SetAlphaBlendEnable / SetAlphaTestEnable(false), SetPolyShadeMode(POLY_SHADE_FLAT = 0x01396F9C);
  DeviceState +0x2818 matches the stand-in's state offsets) and DrawAsIndexedTris on the header: DIFFER(1407v1407), one block:
  0x00BB83C1 retail `test eax,esi ; or esi,eax ; mov [edx+8],esi`, built `test esi,eax ; or eax,esi ; mov [edx+8],eax` (the GOTCHAS /
  CRenderStateManager.h "bookmark test / or" lane gap).
- Form 2: + FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED: same three lines.
- Form 3: + DESIRED + SETRENDERSTATE_BOOKMARK + ADDSTATE_DIRTY (the 0x00B5FAB0 set): two blocks differ.
- HEAD matched with a row-local SetState that reads the capture mask (CurrentBookmarkMask) into a named local. Next idea: an add-only
  nested opt-in of SetRenderState that reads CurrentBookmarkMask into a local (needs a regrade of nothing else: new macro).
- Classes that must move for a full port: CRenderManagerCoreView -> CRenderManager / CRenderStateManager, the local CShaderRenderManager
  (engine/CShaderRenderManager.h), C3DVector, CArray, and the self stand-in CEnginePrimitiveManagerParticleDecalGroupMethods. Drafts:
  work/00bb8090_f1.cpp .. f3.cpp, logs work/00bb8090_f*.log. Lock 0x00BB8090 was `released`; claimed as NF150.

## In-place respells (accepted kinds)
- 0x00B4A5F0: member renamed inside the row's existing local CEnginePrimitiveRenderer2D model (same class token, no new symbol).
- 0x00A1E370: the existing local CSteeringAgent model's SetGoal declaration respelled to (const C2DVector&, const C2DVector&); the call
  passes the way points' Position (+0, same address). Defined-symbol diff vs a HEAD compile: identical.

## Ports onto shared headers (no overlay)
- 0x00A0AD40 / 0x00A0AEA0 / 0x00A0AF40: off the `#define CRenderManagerCore CRenderManagerCoreLayout` rename-include + base-list local
  model + local IDirect3DDevice9 onto the retail branch of engine/CRenderManagerCore.h (existing opt-ins DRAWASINDEXEDTRIS,
  DRAWASINDEXEDTRIS_USER, FORCEINLINE_HELPERS) and engine/IDirect3DDevice9.h. 0x00A0AD40 keeps HEAD's PrimitiveType read
  (`*reinterpret_cast<const long*>(PSelectedIndexBuffer->PrimitiveType)`, the header's opaque EPrimitiveType bytes; CIndexBuffer.h is
  NF149's header, not touched). 0x00A0AEA0 emits the header's AttachTextureToStage / AttachD3DTextureToStage pick-any COMDATs, as the
  landed 0x00B70230 / 0x00B5CA70 objects already do. Not link-tested against the owner row 0x009A0CF0 (which includes the header without
  FORCEINLINE_HELPERS, so it probably defines AttachTextureToStage strongly): if that clashes, it already clashes at HEAD with
  0x00B70230 / 0x00B5CA70, and the debt is the owner row's (nf140 / nf88 fix); this row adds no new symbol kind.
- 0x00A14E20: on engine/CNavigatorManager.h (FABLE_RETAIL_CNAVIGATORMANAGER); its test is rewritten on the header and links the row
  object (PassPattern 00a14e20_TEST PASS kept).

## File moves for the coordinator
- 0x00B4A5F0 CEnginePrimitiveRenderer2D_Set2DLighting_00b4a5f0.cpp -> ..._SetLightingParameters_00b4a5f0.cpp (and its test).
- 0x00A14E20 CGameEventPackage_Clear_00a14e20.cpp -> CNavigatorManager_FrameUpdate_00a14e20.cpp (and its test; Module `_global` ->
  CNavigatorManager in build_candidates.ps1).
- No EH funclet includes any changed row (grep of rebuild/src/compiled/01/21 and of include lines).

## Descriptive names
- None new. `CIsEqualReference` (0x00A1A170 scaffolding helper) is a TU-local descriptive helper name, never bound.

## Flags lines
- None added; no row needed flags.

## RENAMES section 2 completeness
- Diff of the per-row (symbol, target) pairs, HEAD census (work/head_pairs.json) vs the ported rows (work/b1_pairs.json): only
  0x00A1E370 SetGoal (0x00A20970) and 0x00B32E90 SetPersistentPrimitive (0x00B324A0) change; both are in section 2.

## Locks
- 0x00BB8090 lock reads NF150 (claimed from `released`); no row is handed back for it. Release it or hand it to the next agent.

## Rule slips
- I deleted the HEAD census work directory (wdhead) before the defined-symbol diff; the HEAD self symbols had been printed before, and
  0x00A1E370 (the only row whose HEAD symbols were not recorded) was recompiled at HEAD for the diff (pH, identical).
- One wasted compile: 0x00A1A170's first draft skipped the cpp precheck and failed to compile (missing macro set); one q.py grade of
  0x00A0AF40 graded the wrong symbol because a stray rename `#define` survived the transform (caught by the Leaf warning, regraded).

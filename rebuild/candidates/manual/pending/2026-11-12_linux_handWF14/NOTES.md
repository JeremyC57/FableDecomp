## 0x00959800

## 0x00959800 CAIStateGroup_HoverWanderAroundThing::IsRunnable -- RELOCATION_MATCH (6 grades)
- Made it match: port onto shared headers (sibling 0x00969E70 recipe: FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY makes both
  GetAllThingsInRoughArea instances visible noinline bodies -> edi caching / frame reuse), correct identities (GetWorld()&, GetWorldSeed,
  GetPos, vector<const CThingObject*> at 0x959380 / vector<const CThingDeadCreature*> at 0x9595C0, CIntelligentPointer<const CThingGameObject>),
  and the landed GFRandom 0x004CDAE0 body as a PLAIN `inline` in the TU: VC7.1 expands the first site and calls the out-of-line copy at
  the second, exactly like retail (no CAIStateGroupBase::Random exists in either PDB).
- Headers/flags: Flags none; overlay hdr/engine/CAIStateGroup_HoverWanderAroundThing.h opt-in FABLE_RETAIL_CAISTATEGROUP_HOVERWANDERAROUNDTHING
  (real polymorphic class on CAIStateGroup_HoverMovementBase, needs FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE); plus repo opt-ins
  GOTOBED_WORLD (GetWorld), C2DBOXI_CTORS+C2DBOXI, IS_THING_ALIVE, CINTELLIGENTPOINTER_COMPARE_NULL_INT and the 00969E70 set.
- Strict-headers regrade: RELOCATION_MATCH; vt.py PASS. Landed name note: 0x00959380 is not landed (draft name GetTrackNodesInRoughArea wrong;
  it is the GetAllThingsInRoughArea<const CThingObject,...,CThingFilter_None> instance per DOSSIER).
- Repair (verifier finding 1): local GFROR13 decl + inline GFRandom body replaced by `#include "engine/GFRandom.h"` (shared
  ?GFROR13@@YIJK@Z / plain-inline ?GFRandom@@YIKKPAK@Z); regrade RELOCATION_MATCH, strict-headers RELOCATION_MATCH, vt.py PASS.

## 0x004C9920


## 0x004C9920 CThing::IsAffectedByBulletTime() const -- RELOCATION_MATCH (grade 4 of 4, no flags)
- What made it match: porting onto the shared headers (CThing PeekTC under GETTC_EXPANDED for every lookup, HasTC forceinline,
  PeekPCurrentThingManager) with the real inline accessors FableWin.pdb places in Thing.cpp around 0x01CF6F80:
  CThingManager::PeekPlayerManager, CPlayer::GetPPlayerCharacter, CTCDParticleEmitter::IsAlwaysUpdating / PeekPThingAttachedTo
  (0x4b is TCI_PARTICLE_EMITTER, not a follower). const on the method, GetPCarriedByThing for 0x00705170, GetMainPlayer const&.
- Headers: overlay opt-ins FABLE_RETAIL_CTHINGMANAGER_PEEKPLAYERMANAGER (CThingManager.h) and
  FABLE_RETAIL_CTCDPARTICLEEMITTER_ATTACHED_INLINE (CTCDParticleEmitter.h); see hdr/README.md. CTCFadeOutAndIn is a minimal local
  model (no shared header exists; same model as landed 0x0049D310). Strict-headers regrade: RELOCATION_MATCH. Test: vt.py PASS.
- Name fix candidate: landed row 0x00705170 is spelled GetCarrier / GetCurrentSelectedEntryData; PDB (Ego_r + FableWin) is
  ?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ (per DOSSIER; also used by landed 0x004BD3C0).

## 0x00760E70


## 0x00760E70 NShape::CTrack::GetMinDistanceToTrack -- RELOCATION_MATCH (final.cpp; strict regrade RELOCATION_MATCH; test PASS)
- What made it match: (1) port onto shared headers with the PDB signature `bool GetMinDistanceToTrack(const C3DVector&, float&) const`,
  callee `C3DVector::AccurateNormalise` 0x00A14440 (not Normalise), strict `<` best update, `d1 < d2 ? d1 : d2`, Dot YXZ;
  (2) the segment box is a `C2DBoxF box(prev.X, prev.Y, prev.X, prev.Y)` declared BEFORE `C3DVector cur = *it` (fixes frame 0x44 + slot order);
  (3) the last x87 residue (`fld [esp+38h] ; fmul st(2)` + trailing `fstp st(1)`) was the perpendicular being built from a
  C2DVector view of the normalised direction: `C2DVector flatdir(dir); C3DVector perp(flatdir.Y, -flatdir.X, 0.0f);`.
  C2DVector is #pragma pack(1), so VC7.1 will not fold its member reads into x87 memory operands. ~70 grades of Dot-order /
  operand / paren / temp spellings never moved that hunk.
- Headers/flags: repo headers only (no overlay). Opt-ins: FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE, _DOT_ORDERED_YXZ,
  FABLE_RETAIL_C2DVECTOR + _FROM3D (??0C2DVector@@QAE@ABVC3DVector@@@Z, FableWin.pdb), FABLE_RETAIL_C2DBOXF. Flags: none.
- Local CTrack class (no engine/NShape_CTrack.h exists) derives from the shared NShape::CShape; method declared private: (PDB ABE = private const), repair 2026-11-12, still RELOCATION_MATCH (strict too). Local variable names are descriptive.
- No landed name proven wrong. The `mov [esp+3Ch],0` "hunk" shown by fg with identical bytes is a diff artefact.

## 0x006AC430


## 0x006AC430 CThingPlayerCreature::LearnExpression -- RELOCATION_MATCH (author, 2026-11-12, ~30 grades)
- What made it match: (1) port onto the shared headers (old draft's local models + FAKE GetDef body removed); (2) the REAL body of
  CDefinitionManager::GetDef<CExpressionDef>(long, CDefPointer<const CExpressionDef>*) const visible as a template via the header
  opt-in FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (template __declspec(noinline), out of line; 0x005F81BE is one folded copy
  shared by 13 instantiations) -> VC7.1 knows the def slot is not captured: def kept in ebx, no null test on either release,
  frame 0x10; (3) GetTC<T>(T**) form for the abilities lookup (`mov eax,[eax+4]; mov ecx,eax`); (4) CExpressionDef::DummyObject
  typed as its Ego_r.pdb type CDefIndex (inline operator long) -> DummyObject read before GetPos(), which fixed the whole
  item/map/inventory = ebp/edi/ebx rotation and the ebx/ebp shrink-wrap (the old "register tie").
- Headers (overlay $W/hdr, see hdr/README.md): CThingPlayerCreature.h _EXPRESSIONS (IsExpressionAvailable const / LearnExpression),
  CTCInventoryItem.h _TUTORIALCATEGORY, CTCInventory.h _NEXTFREESLOT, CTCInventoryAbilities.h _LEARNEDEXPRESSION,
  CExpressionDef.h _DEFINDEX. No pragmas, no flags.
- Name fix (dossier, Ego_r + FableWin): IsExpressionAvailable is const (QBE); the landed 0x006AC390 row spells it non-const.
  Callee 0x005F81BE: this row calls the template ??$GetDef@VCExpressionDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCExpressionDef@@@@@Z
  (FableWin _fablewin_api.tsv 017c036c / signatures 01dfc5a0; const, out param CDefPointer<const CExpressionDef>*). 0x005F81BE itself is a
  COMDAT-folded body labelled GetDef<CSpecialAbilitiesDrunkennessDef> (manifest / engine_api.tsv / bsim), landed as `GetDef_005f81be`.
  Correction (repair 2026-11-12): the earlier claim that its real spelling is the non-template QAE ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@VCExpressionDef@@@@@Z
  was wrong -- that symbol is in no extract under ghidra_out; the same wrong claim sits in the FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF
  comment of rebuild/include/engine/CDefinitionManager.h (that opt-in is no longer used by this row; the coordinator should fix the comment).
- Repair (2026-11-12): moved from the non-template GETDEF_EXPRESSIONDEF overload + in-TU body to the template; `def` is
  CDefPointer<const CExpressionDef>. Still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1); test PASS.
- Dead ends: CDefPointer release opt-ins (POINTEE / SELFDELETING / RESET) alone; adopting holder copy of the out slot (probe, 605);
  guard-return structure (616); declaration order of item/inventory; named GetPos reference (607); temp CCharString argument (599).

## 0x0076C9A0

## 0x0076C9A0 CTCEnemy::OnSerialise -- RELOCATION_MATCH (wf14, 15 grades)
- Made it match: port onto shared headers (same TU preamble as landed CTCEnemy::CTCEnemy 0x0076C8A0); visible GetSubDef<T>
  (engine/CDefClassBase_GetSubDef.h) + GetDef<T> bodies under auto_inline(off) (lets VC7.1 enregister the enemy-def handle in ebp
  and reuse its slot); CFaction::GetName() returning CCharString BY VALUE (FableWin.pdb ?GetName@CFaction@@QBE?AVCCharString@@XZ 0x02444310)
  as the operator!= argument and in the ternary `PFaction ? PFaction->GetName() : CCharString("")`; `bool differs = a && b && c`.
- Headers: overlay engine/CDefString.h opt-in FABLE_RETAIL_CDEFSTRING_NE_CHARSTRING (operator!=(const CCharString&) const, 0x0076D640).
  CFaction has no shared header: local model (private Name at +0, inline GetName). No flags.
- Old draft errors: (CPersistContext*,long) signature, GetDefinitions/GetWorld/FindFaction names, local models (see DOSSIER sec. 7).

## 0x00AF5430

## 0x00AF5430 NParticleEngine::CParticleSystem::ClearDecal -- RELOCATION_MATCH (source final.cpp, test test.cpp)
- What made it match: port onto shared headers + the FableWin.pdb CDecal inline accessors (GetParentHandle / GetPrimID /
  ClearParentHandle / ClearAll, overlay opt-in FABLE_RETAIL_CDECAL_CLEAR) with size-first bounds tests (`size() > group`, retail
  `cmp size,group ; jbe`); `std::__copy<CEnginePrimitiveHandle*,..,int>` declared as an explicit specialization (landed body
  0x00440260) so vector<CEnginePrimitiveHandle>::erase expands inline (with __copy's inline body it was called out of line: 536 bytes);
  then FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER fixed the last residue (group index kept in ebx across the loop head instead of
  its frame slot; ten spellings of loop/locals did nothing). ClearParentHandle needs FABLE_FORCEINLINE (natural inline refused: 440 bytes).
- Headers/flags: overlay engine/CDecal.h (FABLE_RETAIL_CDECAL_CLEAR) + engine/CParticleSystem.h (FABLE_RETAIL_CPARTICLESYSTEM_CLEARDECAL);
  no flags. Signature is non-const iterator& (PDB AAV); draft's const and its slot names (RemovePrimitive/OnLastHandleReleased/Destroy)
  were wrong -> ClearChildPrimitive / NotifyReleased / Delete.
- Repair (verifier findings): `// Retail:` line now lists all 5 call sites (xref: 0x00AF569A DeleteDecal, 0x00AF5797 DeleteAllDecals,
  0x00AF58E9 ClearAllDecals, 0x00B1DFA8 CPSCDecalRenderer::AddToBoundingSphere, 0x00B1FE56 CPSCDecalRenderer::PreparePrimitives);
  test's null-engine case now uses a non-empty decal and asserts it is emptied, its handle released, and no engine call made
  (unused `Engine none` removed). Still RELOCATION_MATCH (also strict headers); vt PASS.

## 0x00B68DA0


## 0x00B68DA0 CEngineLandscapeRenderer::RenderSortedPatches -- RELOCATION_MATCH (first author, ~30 grades)
- What made it match: port onto the shared headers (CEngineLandscapeRenderer / CLandscapeBackgroundPatch / CEngineLightingManager /
  CShaderRenderManager / CRenderStateCaptureBlock), a per-iteration `entries = SortedSubPatchList.begin()` pointer in the compaction
  loop, and -- the key -- ONE `++count` after the move/clear if-else (retail's two `inc ecx` are tail duplication; two increments in
  the source put count in eax and permute registers through the whole function). Faded-shadow step (repair): the PDB inline
  `GPLightingManager->SetEnableFadedShadowFactor(true)` with its `PeekGlobalShadowMode() != 1 -> enable = false` guard (out-of-line 0x00B66EB0)
  gives retail `mov al,1 ; je ; xor al,al ; cmp al,[ecx+0CEh]` directly -- no operand-order opt-in needed.
- Headers: overlay hdr/engine/CEngineLandscapeRenderer.h (FABLE_RETAIL_CENGINELANDSCAPERENDERER_RENDERSORTED: private decls of
  SetupCommonStates / PostRenderClearRenderStates / RenderSortedPatches) and hdr/engine/CEngineLightingManager.h
  (FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD: PDB-shaped PeekGlobalShadowMode + guarded SetEnableFadedShadowFactor; the
  ENABLEFIRST operand-order opt-in is dropped; header comment 'no out-of-line copy' corrected).
  Flags: none.
- Names: globals are NEngineInternal::GPShaderRenderManager (0x01436E14) and NEngineInternal::GPLightingManager (0x01436E9C) as in
  landed 0x00BBC460 / labels_*.tsv; the draft's g_pRenderStateManager is CRenderStateCaptureBlock::StateManager (0x013CB508). The
  caller Render 0x00B6B0B0 declares RenderSortedPatches public; PDB (Ego_r/FableWin) says private AAEXXZ.
- Repair (verifier findings, all applied): (1)+(2) above; (3) test rewritten: frames 2/3 put added entries at i == count so the
  in-place `AddedThisFrame = 0` branch is exercised (sentinel ZSorterHandles prove it does not rewrite the handle), and the
  shader-restore vs capture-restore order is observed through an event log from stub managers + an RAII capture block. Still
  RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1); vt.py PASS.

## 0x00941690

## 0x00941690 CAIStateGroup_KidPlayHopscotch::Update -- RELOCATION_MATCH (3 grades)
- Matched by porting onto the shared headers (CThing GETTC_EXPANDED + CVECTORMAP_INLINE_LOWERBOUND: the dead board lookup keeps its
  LowerBound loop and drops the select, exactly as retail) and storing each PerformActionAndWait result in the EAIActionReturn local
  before the temporary dies (a `!= 2` inside the full expression made a setne bool and cached 1 in ebx).
- Headers: overlay hdr/engine/CCreatureAction_PlayHopscotch.h opt-in FABLE_RETAIL_CCREATUREACTION_PLAYHOPSCOTCH (class derived from
  CCreatureActionBase, ctor 0x00941B20 + PerformAction/Clone declarations). No flags. Leaf is MAE (protected virtual), manifest label
  "CAIStateGroup_KidPlayHopscotch::CAIStateGroup_KidPlayHopscotch::Update" is a doubled-class typo.

## 0x009C4440


## 0x009C4440 CFileInstaller::IsFileCopyNecessary -- RELOCATION_MATCH (wf14 author, 2026-11-12)
- Source: $W/IsFileCopyNecessary.cpp, test $W/test_009c4440.cpp (vt PASS), strict-header regrade RELOCATION_MATCH, addmatch done.
- What made it match: (1) moved onto the shared headers: engine/CFileInstaller.h (CFileInfo SrcPath/DstPath, volatile ExitThread,
  IgnoreTimeStampWhenCopying, CompressedFileTypes map, static DirtyDisc), engine/CAFile.h, engine/CCriticalSectionLock.h; `info = file.operator->()`
  local fixes the source/dest spill order. (2) SetDirtyDiscError's real landed body (0x009C3530) in the TU as `inline` + `/d2inlT120`
  (>=110 expands it at the first site and leaves the cold GetFileSizeEx-failure site a call; default/<=100 expands neither, >=190 breaks).
  (3) FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY + MFIND_DECL: the tree's const find body visible, _M_find 0x009AB5D0 out of line
  (else find is an out-of-line hidden-result call); `found` as a separate const_iterator statement, compared `found == CompressedFileTypes.end()`
  (no map reference local: that hoists &map above ToCharString). (4) The last residue (ebx/edi swapped in the 3-second stamp test) was the
  size check: `!GetFileSizeEx(dst) || memcmp(...)` must be TWO ifs (each closing both handles, return true).
- Headers/flags: overlay hdr/fable_string.h FABLE_RETAIL_CCHARSTRING_TOLOWER (new opt-in: declares CCharString::ToLower() const, 0x0099ED10);
  opt-ins CCHARSTRING, CWIDESTRING, CCOMPRESSORZLIB, CACOMPRESSOR, DEF_CLASS_BASE, STLP_LIST, CAFILE_SPLITPATH_WIDE, CAFILE_MODIFIEDTIME_WACKY,
  CCRITICALSECTIONLOCK, STLP_TREE_MFIND_DECL, STLP_TREE_CONSTFIND_TEMPLATE_BODY. Flags: /d2inlT120.
- Names: no made-up names. Draft names proven wrong (all per DOSSIER sec. 3/4): free PathExists/SplitPath/GetFileModifiedTime ->
  CAFile statics (GetFileModifiedTimeAsSomeWackyWin32BigNumber), ConvertToCharString -> ToCharString, GetCodeSectionManager -> CTBaseSingleton<CFileInstaller>::Get,
  Cancelled -> IgnoreTimeStampWhenCopying, SizeCheckedExtensions -> CompressedFileTypes, CFileCopy -> CFileInfo. Global 0x013CA814 spelled
  with the header's `CFileInstaller::DirtyDisc` (landed 0x9C3530 / Pause rows call it DirtyDiscError / g_installerNoPauseWait: one address,
  several spellings -- coordinator should pick one; no PDB name found in this row's evidence).
- Note for the coordinator: the landed caller ProcessReads 0x009C53A0 carries a noinline stand-in of this function; it can now use this real body.
- Repair (2026-11-12): verifier finding applied. `info = file.operator->()` was NOT needed -- plain `file->SrcPath`/`file->DstPath` grades
  RELOCATION_MATCH (strict too), so the explicit operator call is gone (the earlier note claiming it fixed the 0x9C4454/0x9C4458 spill order was wrong).
  Probes confirm the other shape choices: a map-reference local hoists `mov edi,[esp+0x10]; add edi,0x94` from 0x9C455A to 0x9C4540 (DIFFER),
  and folding find into the `if` gives 615 v 609 bytes (DIFFER). Source Shape line now cites these retail bytes. vt PASS, addmatch rerun.

## 0x009F20B0 (not landed)


## 0x009F20B0 CMemoryAllocator::CMemoryAllocator(void*) -- author attempt 1 (wf14), DIFFER(596v598)
Best source: differ/009f20b0.cpp (= v50.cpp). Leaf `??0CMemoryAllocator`. Still a local model (CMemoryAllocator.h is declaration-only;
siblings model locally too) -- porting onto headers not done yet. `static __forceinline` IsValidSize changed to `static inline` (rule).
Fixed this attempt: vslot 3 is `unsigned long GetAllocationSize(void*)` called with 0 (PDB); loop 2 reads
`maxSize = MaxFixedAllocationSizes[type]` BEFORE the `AllocatorExists[type]` test (retail `mov esi,[edi-0x3c]` precedes the je;
35 -> 34 hunks). With that the register-masked code is identical; residue is pure register choice:
retail ebx = constant 0 through loop 1 (heap re-read from its arg slot, i and the fixed-size bool in an 8-byte frame), then ebx =
type/a in loop 2 (edi row ptr, edx size); ours heap -> ebx in loop 1 (frame 4, bool spilled into the heap arg slot), loop 2 row ptr
ebx / size edi / a edx / type ecx.
Ideas tried (all leave the identical residue unless noted):
- GetAllocationSize(void*) / (void*)0 args; IsAllocatorFixedSize(i) const member (PDB name); inline static
  Init/Enter/LeaveSynchronisation; bool via ==true / explicit expr; no bool local; statement orders in loop 1; no `type` local.
- fixed tested twice (v2: no jump-threading, 626 bytes); nested `if (MaxAllocSize != 0)`; explicit info pointer; allocator temp;
  loop 1 fully indexed (v24/v25 worse).
- locals at top in 3 orders; int counters; one counter shared by loop 1 + loop 2 outer, or loop 1 + inner a (VC splits webs: no effect).
- `long type = 0` declared at top / just before loop 1 (thinking the ebx zero web is loop 2's counter): 610-614 bytes, worse.
- InitializeMemorySystem real landed body in the TU (with and without auto_inline(off)); inline helper for the two `new`s.
- loop 2 shapes: found flag, indexed, nested if, int64 local, entry/table references, ulong/int a, size<max+2 (worse), maxSize+1 local.
- flags /G7 /G5 /Ox /EHsc; pragmas optimize g off / w / a. Zero-fill loops instead of memset (not rep stos: wrong).
- PROBES: heap forced to memory (volatile read) -> ebx becomes the zero in the prologue but i takes ebx in loop 1, zero immediates
  stay immediates; extra zero stores in loop 1 -> local `xor eax,eax`, no zero web. So VC7.1 never forms the cross-loop zero web here
  while heap or i is a candidate; retail treats neither heap nor i as worth ebx.
Best next idea: find the source fact that makes heap/i lose ebx in loop 1 -- e.g. check whether the original passes heap through an
inline member that takes it by reference (address-taken param) AND has more zero-register uses; or compare a sibling VC7.1 function
with the same `xor ebx,ebx` prologue + arg-slot reload pattern; or port onto the shared headers (CMemoryAllocatorBase.h with
FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE, real 15-slot vtable) in case class shape changes weights.

## 0x009F20B0 -- author attempt 2 (wf14), still DIFFER(596v598); best source unchanged (differ/009f20b0.cpp = a1.cpp)
Residue unchanged (register choice only: retail ebx = zero through loop 1, heap / i / fixed-bool all in memory; ours heap in ebx).
NOTE: the model declares the ctor public (`??0CMemoryAllocator@@QAE@PAX@Z`); the PDB symbol is protected (`IAE`), so the leaf in
the file does not match the object symbol (q.py warns and grades the largest function). Make the ctor protected when porting.
Findings (probes, do not repeat):
- PROBE heap AND i moved to globals (ebx completely unused in loop 1): constant 0 STILL stays immediate in the loop body
  (`push 0`, `test eax,eax`). So in our IR the zero constant is never a loop-1 body candidate; the zero web only continues
  into a loop region when it already sits in the same register at the top AND that register is free there.
- e1.cpp (allocator temp, `info.PAllocator = allocator;` after the if/else, then a SECOND `if (fixedSize)` around the vcall):
  prologue becomes retail's (`xor ebx,ebx`, ebx/bl stores, i=0 from ebx), heap and i go to memory (heap re-read from the arg
  slot like retail), the zero continues into the loop HEAD (`cmp [esi-0x14],ebx`, latch re-zeroes ebx) -- but the bool takes ebx
  in the body and the second test is not jump-threaded (retail has two stores and no second test). Closest regime so far.
- heap removed (global) => frame becomes `sub esp,8` with the bool at [esp+0x13] (retail layout) but i takes ebx (coalesced with
  the zero). So retail needs BOTH heap and i to lose to the zero.
- No effect: TU flags /GS /Gr /G6 /GB /Ob1 /Op; QFE-4035 compiler (FABLE_VC71=$S/wine/vc71); g_AllocatorInfo sized / defined
  static in the TU; `a`/`size` declared =0 at top; named `void* base = 0` (top / before loop / in loop; constant-propagated);
  loop 1 as do-while / goto; explicit placement-new null checks (also with heap global); ternary new; const bool; `continue` on
  !fixed; ulong bool; `&&` form; bool / allocator declared at function scope; extra `MaxAllocSize != 0` test; loop-1 body as an
  inline member helper (not inlined by VC: 419 bytes -- would need __forceinline, not allowed). /Os 558, /Oa 603.
- `long type = 0` at top + type used as the loop-1 zero: constant-propagated, no register.
Best next idea: find what makes VC7.1 thread the second `if (fixedSize)` of e1.cpp (retail's dead bool spill + duplicated
`mov [esi+4],eax` stores fit a threaded second test) -- e.g. the second test inside an inline accessor, or a bool assigned in
each arm; or raise the zero-use count per VC71_CODEGEN_RESEARCH 2.3 (zero uses written per arm before tail merging). Then port
onto the shared headers (CMemoryAllocator.h has no fields: needs an opt-in with the PDB layout, protected ctor).

## 0x009F20B0 -- author attempt 3 (wf14), still DIFFER(596v598); best source differ/009f20b0.cpp = t1.cpp (a1 + protected ctor, leaf IAE)
KEY FINDING (c2prio.py, tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py, run with FABLE_HDR_OVERLAY=$W/hdr):
- c2 colouring order for t1: ... heap #4 prio -18 (allowed only ebx after esi=info ptr, edi=type, ebp=this) -> ebx;
  i -31 -> split, memory; const 0 #1 prio -162 -> split, all pieces immediate except the prologue piece (esi).
- const 0 gains only +36 (block 1: the init-list/MemoryAllocated/i=0 stores, cost 4 x K 9); in-loop compares/pushes are
  cost 0. Its -162 comes from being live through loop 2's depth-2/3 blocks (refs: size=0, a=0, minSize=0) and to the tail
  `MemoryAuditBuffer = 0` (cost-1 ref in block 42).
- PROBE pc.cpp (loop 2 + tail removed): const 0 = -13 > heap -18 > i -35 -> const 0 takes ebx whole, heap and i
  "split, memory", and loop 1 is retail BYTE FOR BYTE (sub esp,8, xor ebx,ebx, bl store, cmp [esi-0x14],ebx, cmp eax,ebx,
  push ebx, heap re-read from the arg slot, bool at [esp+0x13]). So the whole residue = const 0's priority vs heap/i.
- Removing only the tail store (pa.cpp) or only the inner zeros (pf.cpp, probe) leaves const 0 at -165 / -217: both the
  tail ref and the inner refs keep it live through loop 2.
- c2 candidates are WEBS: sharing one symbol between loop 1's i and loop 2's a (s1.cpp) gives two candidates; no effect.
- heap address-taken (probe pd: Probe(&heap)) gives retail's prologue (xor ebx,ebx, bl) but i then takes ebx; i
  address-taken (pe) kills strength reduction (imul) -> not retail.
Also tried (no effect / worse): allocator=0 dead init (t2/t3); inline Ref() on heap/i/bool (folded, t4-t6); loop-1 body
wrapped in if(MaxAllocSize!=0) / !MaxAllocSize (x1-x3); variable arm first (y1); e1 merge with per-arm constant bool
(w1, VC does not thread), re-tested IsFixedSize (w2), ulong &2 bool (w3, 592); e1 + continue (z2); non-literal zero var
for loop-2 type init (z1/z2: const-propagated); loop 2 ulong a (614), int vars, ternary minSize (598, worse layout);
inner IsFixedSize spelled >>1&1 / ==1 / &2 (592).
Retail logic: since retail's loop 2 uses ebx for type/a, const 0 cannot be a whole-function ebx candidate there; so either
(a) heap and i fail their whole colouring (ebx taken somewhere in their ranges by a higher-priority web -- none visible), or
(b) const 0's live range in retail does not cover loop 2 (loop-2 zeros a different candidate?). Neither found yet.
Best next idea: use c2prio --trace on variants to find a source fact that lowers heap (-18) and i (-31) below const 0 or
removes them from the list (e.g. heap's web extended into loop 2, a 1-ref heap, K growth in loop-1 blocks), or that splits
const 0's range at loop 2 (e.g. loop 2 + tail in a separately optimised region: an inline member with its own pragma,
or the loop-2 zeros typed differently); verify each with c2prio before grading.
- Loop 2 moved into an inline member `bool BuildSizeTables()` (b1/b2, made-up name, probe only): VC7.1 does not inline it at
  default budget (371 bytes, loop 1 then matches retail); with /d2inlS1000 it inlines and the output is identical to t1,
  so inlining does NOT give loop 2 its own constant candidates. Hypothesis (b) via inlining is dead.


# Hand chunk 689 (ten never-attempted 621-937 byte rows, stand-in `_global::` labels): 8 of 10 byte-exact

## What matched (RELOCATION_MATCH, test PASS under g++ and VC7.1)
| address | identity | bytes | probes | recipe |
|---|---|---|---|---|
| 0x00C04850 | zlib 1.2.1 trees.c `send_all_trees` (Leaf `_send_all_trees`, `/GS`) | 621 | 1 | the whole landed trees.c TU (0x00C04260 recipe); catalog label was already right |
| 0x00CA02B0 | libvorbis res0.c `local_book_besterror` (Leaf `_local_book_besterror@8`) | 879 | 2 | landed res0.c recipe: `static __declspec(noinline) __stdcall`, VC7.1 gives the private stdcall convention |
| 0x00CB0370 | DirectShow BaseClasses wxutil.cpp `llMulDiv` (Leaf `?llMulDiv@@YG_J_J000@Z`) | 779 | 3 | SDK text verbatim, USE_LARGEINT undefined; `_allmul`/`_aulldiv`/`_aulldvrm` fall out of the 64-bit arithmetic |
| 0x00C67C00 | `CEaxSource::SetAllParameters(const EAXSOURCEPROPERTIES*)` | 854 | 1 | sibling of the landed CEaxContext::SetAllParameters 0x00C67AE0: 17 range checks (note: flExclusionLFRatio is NOT range-checked by retail, only passed to its setter), flag mask ~7, eighteen setters stopping at the first negative |
| 0x00CFA340 | `NScript::CQ_AmbushTradersScript::~CQ_AmbushTradersScript()` (implicit) | 792 | 2 | landed ~CQ_HangingTreeGoodScript recipe + STLport `_Rb_tree::clear()`/`~_Rb_tree_base` + `CTimer::~CTimer` from engine/CTimer.h |
| 0x00D3B010 | `NScript::CQ_FireHeartScript::~CQ_FireHeartScript()` (implicit) | 883 | 2 | same recipe; `vector<bool>` is the plain 12-byte vector in retail's STLport build |
| 0x00AC9D50 | `QuadricOptimiserInternals::COptimisedPrimitive::MeasureSegments()` | 874 | 9 | `__forceinline` per-vertex helper sharing one `C3DVector p` local (user copy ctor + operator=), same-TU `#pragma auto_inline(off)` body of the vector<C3DVector> fill ctor 0x00AD30C0 so VC hoists the box vectors' start pointers |
| 0x00606F80 | `CTCInventoryMap::~CTCInventoryMap()` | 937 | 3 | PDB layout shifted for the 0x130 retail NInventory::CTCInventoryBase, `engine/CBaseClass.h` + `engine/CRegion.h` + `engine/CCountedPointer.h`, local STLport vector/_Rb_tree/_List_base models, NHeroInformationScreens::CBase inline empty virtual dtor |

## Parked for Fable 5.1 (differ/)
- **0x00AB1990 `C3DAnimationBuilder::BuildBoneInfoTree(const C3DAnimationSequence*, unsigned long)`** -- DIFFER(812v812), 20 probes.
  Every byte matches except the three instructions of the inlined `__uninitialized_copy` loop inside the expanded `bones.push_back`
  overflow: mine addresses `[edx+eax]`, retail `[eax+edx]` (operand order only, same length). The single-site expansion of
  `_M_insert_overflow` (retail inlines it at the `bones` site, calls 0x00AB3300 at the `Children`/`roots` sites) is reproduced by
  the lane recipe (`__forceinline push_back_expanded` / `_M_insert_overflow_expanded` members). Levers without effect on the
  residue: increment order, while-form, advancing `__result` directly, explicit `this->`; open-coding the loop made it worse (822).
  Identity: Ego_r.pdb; the bone names come through a by-value `const CCharString GetBoneName() const` accessor on
  C3DAnimationSequenceData (retail destroys both ternary arms before `operator=`, which a plain ternary cannot give: VC elides it).
- **0x00A73E50 `CPhysicsMesh::GenerateLineListFromBaseline(float, CArray<C2DLineF>&, CArray<long>&) const`** -- best DIFFER(782v811)
  (probes 3/4/11), 12 probes. Semantics are settled (closest centre height to the baseline, then every triangle whose vertices
  straddle it is cut by the plane {0,0,1,height}; cut segments and triangle indices pushed; `lines.DeallocateUnusedMemory()`).
  Residues: (1) retail homes the second loop's `i` in the dead `baseline` argument slot (frame 0x48 vs mine 0x4c); (2) retail
  re-reads `TotalFaceIndices`/`TotalPoints` starts and the three vertex indices inside the cut block after the plane stores where
  VC CSEs mine; (3) retail places the `side0 == -1` test group after the cut body (deferred cold block) where every form I tried
  emits it before. Levers tried: continue/goto/flag/positive-OR/inverted tests/-1 group first/function-scope `i`/triangle
  re-read. Suspect the retail source passes the vertices through something that clobbers VC's alias facts (e.g. a CArray
  `operator[]` that is not an inline pointer index, or the sides computed by a helper taking `const C3DVector&`).

## Skipped
None.

## Label corrections
All ten catalog labels were stand-ins (`NAME_FIXES.tsv`); only 0x00C04850 `send_all_trees` was already right. The manifest's
module label for 0x00D34470 ("CQ_HangingTreeGoodScript") is wrong: it is the CQ_FireHeartScript ctor. The landed 0x00D3AFF0 file
is labelled a CLandscapeBackgroundPatch vector deleting destructor; it is `??_GCQ_FireHeartScript@NScript@@UAEPAXI@Z`.
The CTCInventoryMap ctor's second base is `NHeroInformationScreens::CBase` (vtable 0x0123A3E0), not a `CInventoryMapListener`.

## Header debt
- `engine/COptimisedPrimitive.h` flattens the STLport containers and `Parameters`; 0x00AC9D50 restates the class locally
  (Vertices +0x10, Faces +0x1c, SegmentCount +0x40, SegmentSizes +0x44, Parameters.Scaling +0x5c) with a `// Layout:` line.
- `engine/CTCInventoryMap.h` does not exist; 0x00606F80 carries the full +0x134..+0x254 layout (Ego_r - container deltas,
  CTCInventoryBase 0x148 -> 0x130) and `FABLE_STATIC_ASSERT(sizeof == 0x258)`.
- `engine/CLevelScriptBase.h` declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit, so both quest dtors use a
  local copy without that declaration (same debt as chunks 664/665).
- No `engine/CCharString.h`, `engine/CPlane.h`, `engine/C2DLineF.h`: local one-pointer / pack(1) models mirroring landed rows.
- 0x00AB1990 / 0x00A73E50 (parked) model C3DAnimationSequence(Data), CBoneInfo, CPhysicsMesh (+0x30 TotalFaceIndices, +0x3c
  TotalPoints) locally.

## Callee names (one address = one name; all checked against rebuild/oracles/reloc_bindings.tsv)
0x00A42470 `?GetIntersectionWithTriangle@CPlane@@QBE_NABVC3DVector@@00AAV2@1@Z`, 0x00A13490 `??$__uninitialized_copy@PAVC2DLineF@@PAV1@@std@@...`,
0x00440990 vector<long>::_M_insert_overflow (K size_type), 0x00441020 vector<float>::_M_fill_insert, 0x00AD30C0 vector<C3DVector> fill ctor,
0x00A74EA0 `?DeallocateUnusedMemory@?$CArray@VC2DLineF@@@@QAEXXZ`, 0x00CDC000 `??$__destroy_aux@PAUTeleportData@CQ_AmbushTradersScript@NScript@@@std@@...`,
0x007ACC50 / 0x00613AE0 / 0x006196D0 `_Rb_tree<...>::_M_erase`, 0x005C00A0 `??1CTCInventoryBase@NInventory@@UAE@XZ`, 0x00CBD510 `??1CScriptBase@@UAE@XZ`,
0x0099EBF0 `??0CCharString@@QAE@PBDJ@Z`, 0x0099EC30 `??0CCharString@@QAE@ABV0@@Z`, 0x0099EFB0 `??4CCharString@@QAEAAV0@ABV0@@Z`, 0x0099EAE0 `??1CCharString@@QAE@XZ`,
0x0099B510 `??1CWideString@@QAE@XZ`, 0x00BFE9BC `??3@YAXPAX@Z`, 0x00BFEA0E malloc, 0x00BFEA14 free, 0x00BFEAE6 memmove, 0x00BFEB50 `_allmul`, 0x00BFEE00 `_aulldiv`, 0x00F3FD50 `_aulldvrm`.

# agentFAM02: family STLport algorithm stand-ins (`_global::UninitCopy_* / UninitFillN_* / Fill_* / Copy_* / CopyBackward_* / DestroyRange_*`, 662 rows)

Result: **346 rows renamed** (`rows/<addr>.cpp`): 201 MATCH + 145 RELOCATION_MATCH, every one with its self symbol = the `// Leaf:` and
0 reloc-identity HARD findings (`work/chk.py` = q.py parity + `reloc_identity.check`). Tests are unchanged (every landed test is a
self-contained model that links nothing from the row): 346/346 VC7.1 /Od PASS with the catalog PassPattern; g++ -std=c++11 passes 82,
the other 264 are PREFAILs (the unchanged landed tests spell `__fastcall`, which x86-64 g++ rejects: "expected initializer before
'UC'/'F'/..."). At most 8 grades on a row this pass (STATUS.tsv notes). Nothing landed, no git, nothing written under the repo,
BUSY.txt untouched (no BUSY address is in this family), no other agent's directory touched.
**316 rows left** (LEFT.tsv, one reason per row), summary below.

## The settled spelling (one per algorithm; all from the shared model rebuild/include/stlport/fable_containers.h)
Every row is now `#include "stlport/fable_containers.h"` + the element type + ONE explicit instantiation, `// Flags: /Gr` (the
retail TUs are /Gr: first argument ecx, second edx). No local re-implementation of any algorithm.
| retail shape | STLport 4.6 function | leaf form |
|---|---|---|
| UninitCopy, `ret 8` | `__uninitialized_copy(I first, I last, O result, const __false_type&)` | `??$__uninitialized_copy@PAVT@@PAV1@@std@@YIPAVT@@PAV1@00ABU__false_type@0@@Z` (PB first from operator= / copy ctor) |
| UninitFillN, `ret 8` | `__uninitialized_fill_n(I first, N n, const T& x, const __false_type&)` | `??$__uninitialized_fill_n@PAVT@@IV1@@std@@YI...IABV1@ABU__false_type@0@@Z` |
| UninitFillN, `ret 4` | `uninitialized_fill_n(I first, N n, const T& x)` (helper expanded) | `??$uninitialized_fill_n@PAVT@@IV1@@std@@YIPAVT@@PAV1@IABV1@@Z` |
| Fill, `ret 4` | `fill(I first, I last, const T& x)` | `??$fill@PAVT@@V1@@std@@YIXPAVT@@0ABV1@@Z` |
| Copy, `ret 0Ch` | `__copy(I, I, O, const random_access_iterator_tag&, D*)`, D = int | `??$__copy@PBVT@@PAV1@H@std@@...PAH@Z` |
| CopyBackward, `ret 0Ch` | `__copy_backward(R, R, B, const random_access_iterator_tag&, D*)` | needs the existing opt-in `FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD` |
| DestroyRange, `ret 4` | `__destroy_aux(I first, I last, const __false_type&)` | `??$__destroy_aux@PAVT@@@std@@YIXPAVT@@0ABU__false_type@0@@Z` |
Per row the leaf is the landed STLport binder spelling (reloc_bindings) whenever one exists and names a symbol of the instantiation;
in 1 row (0x006FB1C0) the binder had a wrong class key and names no symbol, so the leaf is the instantiation's own symbol
(STATUS note; NAME_FIXES lists the binder spelling); the manual-spec rows also take the instantiation's own symbol.
/Os rows keep their `#pragma optimize("s", on)`; under /Os the public `uninitialized_fill_n` needs its `__false_type` helper
expanded: new overlay opt-in `FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE` (hdr/README.md, 11 rows).

## T, two sources per row (the `// Identity:` line of each row, RENAMES evidence column)
- 1st source: the landed caller's binder symbol (`??$<alg>@<T>...@std@@`) or, for the 10 manual rows, the landed caller's container
  type; 2nd source: the retail caller's catalog container type (`std::vector<T>::_M_insert_overflow` ...), the callee the body binds
  (T's copy ctor / operator= / dtor, catalog + bound name), or the caller's landed source naming T (`OK2b`).
- Const-ness: _M_insert_overflow / _M_fill_insert / reserve pass `T*`; operator= / the vector copy ctor pass `const T*` (STLport 4.6).
- Element types: shared engine header where one exists (with the guard defines its `#error` lines ask for, e.g.
  FABLE_RETAIL_DEF_CLASS_BASE, plus the class's own FABLE_RETAIL_<CLASS> opt-in when the plain header did not grade); CCharString /
  CWideString (no header) as one canonical declaration (Ego_r member `PStringData`, PDB copy ctor / dtor / operator= spellings);
  otherwise the landed stand-in's proven layout renamed to the real class (nested classes inside scope shells, namespaces as namespaces).
- 10 NOBINDER rows were specified by hand from caller + callee (work/manual.py); 6 renamed (STATUS "manual spec"): 0x00493959
  0x00493984 0x004AA060 0x005F7A1D 0x008E33D0 0x009E11A0; 0x009F6C00 graded but is withheld (invented SRecord12 in its model);
  0x00472E58 / 0x00593851 / 0x00592FD5 did not grade (header CFadeInFadeOutBase has no copy ctor; CDialogueLayerDef model).

## Retail folds identical COMDATs (contradicts GOTCHAS "no ICF in retail")
0x00B8EED0 is called from vector<pair<float,CSubtitledMessage::CCameraTag>>::_M_insert_overflow AND
vector<CEngineOcclusionManager::COccluderFlareEntry>::_M_fill_insert; 0x00513830 is called with both `CMapInfo*` and `const CMapInfo*`
iterators from one _M_range_insert. These 16 CONFLICT rows are left: one address, several real instantiations, the surviving name is
not provable. A renamed row whose callers all agree may still be a folded body of other (uncalled) instantiations.

## Left (316), LEFT.tsv
- 117 NOBINDER: no landed caller binds an STLport spelling (stand-in / invented-namespace binders like `?__uninitialized_copy@FableVector_00569450@@...`);
  many name T in the caller (pair<int,CWideString>, pair<int,CCharString>, CCategory, CInputEventPackage ...) but int-vs-long or the
  namespace is not settled from a second source. Next: one manual spec each (work/manual.py shows the form).
- 24 STANDIN_T: the only binder names a stand-in element (SRecord20, SValue8, CUnprovenPointee_*, CUnprovenEnum4_*).
- 16 CONFLICT: ICF-folded (above).
- 16 WITHHELD: graded MATCH/RELOCATION_MATCH but the element model still carries invented helper types (SCountBlock, CSecond_005bbdf1
  = a vector copy ctor stand-in, SKeyRecord, SRecord12, STree_*, CRetailVector ...); sources kept in old/excluded_invented/.
- 143 T proven but the rewrite does not grade: pairs whose second member is a nested class the stand-in modelled under another name
  (pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>, pair<long,C3DMeshVolume> ...: SRC_FAIL, the generator cannot map the
  stand-in member type), vector<...> / CArray<...> elements (retail calls the element's operator= at a shared ICF'd address such as
  0x00454886, the model inlines vector::operator=: DIFFER 74v66), engine headers that do not compile standalone (negative-subscript
  size asserts in NUISystem_CUIStateDef.h / CScriptGameResourceObjectScriptedThingBase.h, CIVCountedPointeeBase without AddRef), and
  def classes whose header declares an out-of-line virtual dtor (0x00450ACE CAbilityTrackDef: retail calls the dtor through the
  vtable, the header's user-declared dtor gives a direct call).

## Header debt
- Nested classes missing from their outer class's shared header, so the rows use a scope shell (`class CTCInventoryMap { public:
  class CUIMiniMapMarker {...}; };`; the verifier prints SHARED_HEADER_AVAILABLE for the shell): C3DAnimationSequenceData,
  CTCInventoryMap, CSubtitleRenderer, CThingMapManager, CRegionMinimap, CMeshDataBank, CFileInstaller, CEngineSubPrimitiveRenderer,
  CEngineLightingManager, CEngineLandscapeRenderer, CTCShop, CNavQuadTree, CMap, CHeroLogBook, CGPUScratchBufferTextureManager,
  CEngineInternalPrimitiveParticleGroup, CBankFile, CDefinitionManager (forward-declared nested only) and a few singles
  (`work/post.txt`). Adding the nested class to each header removes the shell.
- Namespaced element models (NInventory::CItem, NEnvironmentInternal::CBlendedEnvironmentTheme / CEnvironmentThemePriorityStackEntry)
  trip the verifier's name-only check against the unrelated global engine/CItem.h etc. (false positive).
- Member names inside nested-class models are the landed stand-in's (Ego_r layouts do not cover nested classes).

## Caller respellings (section 2): none applied
NAME_FIXES.tsv lists, per renamed address, the landed binders that still use another spelling (41 addresses; stand-in names,
non-template `?__uninitialized_copy@std@@...`, `_Destroy<__false_type>` for __destroy_aux). Renamed rows also bind element callees by
their PDB spellings (e.g. 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z, 0x0099B720 ??0CWideString@@QAE@ABV0@@Z where landed rows used
??0CSecond_0099ec30@@ / ??0CElement4_0099b720@@).

## Descriptive names
None introduced.

## Scripts
work/gen.py (row generator), work/run.py (variants + grading), work/chk.py (grade + reloc identity), work/post.py (rule scan),
work/report.py, work/manual.py; grade log work/grades*.tsv.

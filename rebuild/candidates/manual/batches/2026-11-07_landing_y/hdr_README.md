# hdr/ -- shared-header edits of chunk 847 (both opt-in, default path textually unchanged)

Both files were copied fresh from rebuild/include at edit time and re-diffed against it before hand-back (only the opt-in blocks differ).

## engine/CThingSearchTools.h -- `FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA` (opt-in, declaration only)
- Adds, inside the existing FABLE_RETAIL_CTHINGSEARCHTOOLS class body (before its closing `};`), the member template declaration
  `template<class T,class Filter,class Proc> bool ForEachInRoughArea(const C2DBoxI& area,const Filter& filter,Proc& proc) const;`
- Evidence: Ego_r.pdb `??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind2@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@VCThingFindNavigationLayerAt@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind2@...@@AAVCThingFindNavigationLayerAt@@@Z`
  (retail out-of-line instantiations 0x0063F4B0 Bind2 and 0x0063F7C0 Bind3, called by 0x0063E590); the row src/0063e590.cpp binds exactly those symbols (RELOCATION_MATCH).
- No layout change; nothing in the default path moved. Used by: src/0063e590.cpp.

## engine/CMap.h -- `FABLE_RETAIL_CMAP_NAVIGATIONLAYER` (opt-in, declaration only)
- Adds a forward declaration `class C3DVector;` (after the existing forward declarations) and, before the class-closing `};`,
  `long GetNavigationLayerAt(const C3DVector&)const;`.
- Evidence: Ego_r.pdb `?GetNavigationLayerAt@CMap@@QBEJABVC3DVector@@@Z`, retail 0x0081E920 (catalog CMap::GetNavigationLayerAt), called by 0x0063E590.
- No layout change. Used by: src/0063e590.cpp.

## Includers rechecked
Opt-in only, so existing includers keep the default path. As a sanity check four landed includers (1 of CMap.h, 3 of CThingSearchTools.h)
were regraded read-only with this overlay (`vland.py --recheck-landed`): see SIBLINGS.tsv.

## Header debt (see NOTES.md)
- No shared NShape headers (0x00760A70 uses a local CShape model); no shared CAudioSource / SPitchProperties header (0x00C71F80 local model);
  CSimBuildingDef retail offsets are Ego_r - 4 (Dinkumware vector in CParentDefClassBase) and are modelled locally in src/005029a0.cpp.

# Hand chunk 3019 (CTCWife): 1 of 1 byte-exact

All grades under VC7.1 RTM 13.10.3077 with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3019/hdr` (hdr/README.md: chunk 2020's overlay
plus one new opt-in in an overlay copy of stlport/fable_map.h). Grade script gr.sh, one line per full-row grade in grades.log.

## What matched (1)
- **CTCWife::CheckAppearanceChanges 0x006D1250** (RELOCATION_MATCH 1608v1608, `/GS`; test PASS g++ / VC7.1; vland RELOCATION_MATCH PASS;
  every call target checked with tools/rel.py). Starting point: chunk 2020's parked v6 (DIFFER 1629v1608, 410/516 exact). What closed it:
  1. Fatness tail: retail tests `change > 0` twice BEFORE the GetFrame call (fcom / fcomp into edi / esi, change kept on the x87 stack,
     abs value through the dead hero home `fst [esp+58h]`). Source: both categories in named locals ahead of the call
     (`long noticed = change > 0.0f ? 10 : 11; long cleared = change > 0.0f ? 11 : 10;`). FITTED (only spelling tried; matches).
  2. Tattoo site: `std::set<long>::iterator seen = ThingsSeenThePlayerWear.find(def_index); if (seen == ThingsSeenThePlayerWear.end())`
     (find before end(), 0x006D1734..0x006D173D). With the tail fixed this ALSO gives retail's this / hero allocation (chunk 2020 saw the
     opposite before the tail fix) and the whole frame: 1608v1608, 504/515 exact. PROVEN by the retail order of find and end().
  3. Last 11 instructions (loop-1 covered_areas load order 0x006D132E; tattoo key kept in edi across ~CCharString and sharing the
     tattoo CDefPointer frame word 0x006D170F..0x006D171E): the STLport bodies must be VISIBLE so VC7.1 knows the keys passed by
     reference are not captured -- the _tree.c insert chain (existing repo opt-ins FABLE_RETAIL_STLP_MAP +
     FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY) AND the tree's const find<long>. Diagnostics: find visible + noinline alone fixes loop 1
     (fm 511); + the insert chain = RELOCATION_MATCH. The plain inline find body is EXPANDED at the loop-2 and tattoo sites (1692 bytes).
     c2 inline trace (tools/inl.sh): find scores 88 / 68 / 60 at the three sites, the LowerBound sites retail expands 63 / 60 and the one
     it calls 100 -- no /d2inlT<N> separates them, and loop / guard spellings that lower the find frequencies also refuse the
     LowerBounds (while loop 1: LowerBound morph 78, tattoo find 71; continue forms, do-while, explicit size guards all graded worse;
     out-of-class definitions, an explicit instantiation and an earlier non-inlined user did not change the decision). So the find is
     visible but noinline: new overlay opt-in FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE (`__declspec(noinline)` under _MSC_VER;
     Inline-budget line in the row; EXTRA_RULES owner decision 2026-11-08 preference 3; same device as the existing
     FABLE_RETAIL_AUGMENTATION_PARTICLES find). The TU's out-of-line copies of find and insert_unique equal retail 0x00616580 and
     0x00512DA0 instruction for instruction (relative branch / call targets aside), which supports both header texts.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Names
- Row: ?CheckAppearanceChanges@CTCWife@@AAEXAAVCThingPlayerCreature@@@Z (Ego_r.pdb + FableWin.pdb name sets; chunk 2012 / 2020 evidence).
- Callees: all bound spellings unchanged from chunk 2020's source (rel.py: every target `same` except the three below).
  - 0x00616580: `??$find@J@?$_Rb_tree@JJU?$_Identity@J@std@@U?$less@J@2@V?$allocator@J@2@@std@@QBE?AU?$_Rb_tree_iterator@JU?$_Const_traits@J@std@@@1@ABJ@Z`
    (STLport member template, owner decision D1; the catalog / reloc_bindings name `?find@CAnimSet@@QAE?AUCAnimSetIterator@@ABJ@Z` from
    landed 0x0075D8B0 is a local stand-in -> NAME_FIXES.tsv). vland warns RELOC_CONFLICT because landed 0x00B2D9F0 binds the same symbol
    to 0x00B2EB20: retail holds two byte-identical copies of this instance (0x00616580 / 0x00B2EB20); expected, review at landing.
  - 0x0071DE20 CTCHeroMorph::GetTattooThings: UNBOUND in reloc_bindings (spelling from chunk 2012, both PDB name sets).
  - 0x0122DEDC: the float zero constant (`__real@00000000`), many descriptive bindings exist.
- Descriptive (no original name): the locals (`covered_areas`, `hair_visible`, `def_index`, `seen`, `noticed`, `cleared`, ...);
  EWifeAppearanceCategories values are written as casts (no enumerator names in any name set).

## Header debt
- engine/CTCWife.h has no EWifeAppearanceCategories enumerators (forward enum only).
- The noinline find opt-in lives in an overlay copy of stlport/fable_map.h (hdr/README.md).

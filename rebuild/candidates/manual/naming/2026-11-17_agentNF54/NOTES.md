# NF54 notes: game-side tail, first half (36 entries, 73 worklist rows)

Per-row detail is in STATUS.tsv; per-entry detail is in APPLIED.tsv. RENAMES.tsv section 1 lists self rows; section 2 lists every
changed binding, taken from a before/after compile of each row. Callers outside the worklist that still bind an old spelling, and the
crude self rows I did not re-author, are in NAME_FIXES.tsv. Header opt-ins are described in hdr/README.md.

## Policy applied uniformly
- **Stand-in class carrying the binding:** a made-up local class, struct or free function that carries the wrong binding
  (CULongVector, CWorldClock, CIntelligentPointerOut, CMember_005bc0d3, GetInventoryTabState, ...) is deleted. The row then uses the
  shared header's class (adding an overlay opt-in where a member is missing). Other, unrelated local models in the row are left as
  they were.
- **Wrong member inside a local model of a shared-header class:** the member spelling is not changed in place, because that is a
  renamed stand-in (NF44 round-1 rule: "respell the local declaration only for classes that have no shared header"). Either the
  row moves onto the header within budget, or it is `blocked/port` with the class named. This blocked 0x004CA010 (local
  CThingManager), the seven CloseInventory rows (local CTCInventoryBase family) and 0x00722270 / 0x007222B0 (local
  CTCAppearanceMorphBase::CMeshMorphSet).
- **Self rows of my targets:** I re-authored them on the shared headers where the body is a small library or accessor shape:
  0x005178A0, 0x0051E580, 0x005B33F4, 0x005BC0D3, 0x005ED300, 0x004FB470 (e207, found in the final reconcile), plus the earlier
  0x00458F89 and 0x0049B6B0. The crude early stand-in
  self rows 0x005D96F0 and 0x005D99D0 (invented globals, no callees modelled) are authoring tasks; they are in NAME_FIXES and were
  not attempted, nor was the e71 self row 0x004091C0: it is still `LoadFileList`, on a dozen local models, so it needs a
  whole-row port (NAME_FIXES).
- **Pass criterion for every port:** RELOCATION_MATCH or MATCH, an unchanged relocation count, and only the intended bindings
  changed (diffrel.sh against a grade of the landed source).

## Entries of this session part (after the e206 / nf44:4 work recorded earlier)
- **e26:** the queued `CArray<unsigned long>::push_back` is rejected (REJECTED.tsv). 0x005178A0 grows through
  vector<unsigned long>::_M_insert_overflow 0x00464ACE, while vector<long>::push_back is a separate COMDAT (0x00485FF3 ->
  0x00440990). Two callers push through std::vector<unsigned long> references (MsgGetThingsKilled's PDB parameter, and
  CComponent::ShapeChildren), so the entry was applied as `vector<unsigned long>::push_back`.
- **e182:** the two binders moved onto engine/CFileFormatBrush.h (MAPHEADER), boost::scoped_ptr<CFileFormatMapHeader> (FableWin.pdb)
  and the CWorldMap.h LEVELHEADER opt-in. Their own leaves became the PDB const / protected spellings, and caller 0x00506E30 is in
  NAME_FIXES.
- **e54:** the claim is proven and the self row 0x0051E580 was re-authored. The only binder, 0x004CA010, is blocked (local
  CThingManager and CThing models; the callee 0x00A37300 is the stand-in CClassFactory_Retail::ReregisterAllocatedClass, which has no
  PDB name).
- **e105 / e39 / e38:** these three entries share one binder, 0x00957F20, which moved onto CGameTimeManager.h and
  CIntelligentPointer.h (CTOR_OUTOFLINE + ASSIGN_OUTOFLINE). The self row 0x005ED300 (a non-template stand-in operator=) was
  re-authored. e38's operator= appears only in FableWin.pdb; the second source is that every call site pairs it with the
  RTTI-proven 0x0056C860 constructor.
- **nf44:52:** the inner value stand-ins became std::pair<CWideString,long>. The outer key / list stand-ins stay descriptive, because
  their owner is unproven.
- **e50:** 0x005B33F4 is the CAnimInit argument constructor (Frame is zeroed by its member constructor, then Init 0x0070C050). The
  old note read the order as "not a constructor". Both binders moved onto CAnimInit.h.
- **nf44:5:** 0x005BC0D3 = ~vector<CEngineTextureMorph> (0x14-byte elements, Ego_r.pdb TextureMorphs members). Six binders moved;
  0x00722270 / 0x007222B0 are blocked.
- **nf44:54 / nf44:7:** all seven rows (two carry both entries) are blocked/port: local CTCInventoryBase-family models, a dozen
  classes each. The rows also bind 0x005EE01A as SetAbilityLevel, whose PDB name is
  `?SetAbilityLevelTo@CTCInventoryAbilities@@QAEXW4EHeroAbility@@J@Z`; this is for the port list.
- **e196 / e193 / e114:** every binder moved onto engine/CTCInventoryItem.h (CATEGORY / ISTROPHY), fable_string.h and the definition
  manager headers. The local empty ETCInterfaceType enums were replaced by engine/ETCInterfaceType.h.

## Descriptive names I kept or used (no original found)
- SPairLongCSecond_005b12f4, CRetailList_node0x10 and the other address-suffixed outer stand-ins were kept as they were. Their class
  is unproven; only the member types inside them moved.
- vector<unsigned long>::push_back, ~vector<CEngineTextureMorph>, __destroy_aux<CEngineTextureMorph*> and the
  pair<CWideString,long> copy constructor are STLport instances. Except where the PDB lists the Dinkumware twin (FableWin.pdb
  0x01CA02D0 and 0x01E0CC40), they are descriptive-by-library: the body shape and the callers prove them.

## Tests
- I wrote new behaviour tests for the renamed self rows whose old test modelled the stand-in:
  - rows/0051e580_test.cpp (the old test used inline assembly; PassPattern OK_0x0051e580 kept);
  - rows/005b33f4_test.cpp (PassPattern 005b33f4_TEST PASS kept);
  - rows/0065574c_test.cpp (earlier).
- The other tests are standalone and still pass, as the vland runs show.

## Final reconcile
- Every RELOC_NAME warning in the first final recheck whose symbol is a RENAMES section-2 new_symbol was checked against RENAMES
  section 1 and NAME_FIXES. Three targets carry the right leaf but a stale catalog label (0x004AA190, 0x004AB640, 0x004AC940);
  the coordinator refreshes those labels from the leaf.
- Two target self rows had the wrong name:
  - 0x004FB470 (DrawGetNoMaps): re-authored as GetRegionCount, MATCH. Its caller 0x00602E30 still binds GetNoMaps (NAME_FIXES).
  - 0x004091C0 (LoadFileList): the e71 self row, left to NAME_FIXES.
- The RELOC_CONFLICT warnings on 0x005D97B0 ("0x005D99D0 in landed 005c44fa") come from the landed 0x005C44FA. That row bound
  0x005D99D0 under the GetInventoryCategory spelling; my e114 port of 0x005C44FA removes the conflict.
- NF55 holds the other binders of e105 (seven rows) and of nf44:1 (two rows), so both entries are `done (this half)`.

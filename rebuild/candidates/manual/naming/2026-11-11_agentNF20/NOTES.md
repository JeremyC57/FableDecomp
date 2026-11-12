# NF20 notes (2026-11-11)

Totals: 52 queue entries: 10 done, 42 partial, 0 rejected. 65 binders were respelled across 68 changed rows, all RELOCATION_MATCH (67) or MATCH (1), and every test PASSes the catalog PassPattern.
Each row used 2 or 3 grade compiles. That includes one final old-vs-new verification compile per row (tools/pairs.py), so all rows stay within 10.

- Entries 1-31 are NF19 partials whose reasons I re-checked and left as they were: ICF/COMDAT-folded callers that need re-authoring, stand-in classes with no shared header, and the unsettled 0x00644627. APPLIED.tsv carries NF19's reason.
- Entry 32 (0x00AF7D80) is now done: 0x00AF8170 respells all 10 NParticleEngine::CPSC*::Alloc calls SAPAV -> SIPAV. Ego_r.pdb and FableWin.pdb agree on SI for all 10.
- Self-row catalog fixes found while working:
  - 0x004DBF56 is CTCTargeted::~CTCTargeted (it was labelled C3DAnimationBlendState::~).
  - 0x004DDF6F is CTCUserActionPointBase::~ (it was labelled CPatchTesselationEdgeStrip::~).
  - 0x009F4360 is CSurface::Unswizzle (it was labelled Swizzle).
- 0x00593965 / 0x00593E3E are now std::__push_heap / std::__adjust_heap (STLport 4.6), replacing the namespace stand-ins N00593965 / FableHeap_00593e3e.
- 0x005F9A7D is now std::__destroy_aux<std::pair<long,CDefPointer<const CHeroAbilityDef> >*>. This replaces a descriptive Destroy_range.
- Secondary respellings in the same rows (RENAMES section 2) bind further callees now that the rows sit on shared headers: 0x009E3820, 0x009F2F60, 0x004AB490, 0x004DF88A, 0x005928A8, 0x00592DFB, 0x0059308A, 0x005B26E9, 0x005BCACE, 0x005F9BFE, 0x0071F430, and 0x00AF7D10..0x00AF8100.
- New tests for 005f9a7d, 004de6ed and 004e0ae8: the old tests linked stand-in symbols, so they gave LINK_FAIL against the renamed objects. 00b44f20_test.cpp follows the const CShaderLight& signature.
- reloc_bindings.tsv is stale for 0x00686830: 0x004DCBCE and 0x005DAE40 already bind ??1CTCBase@@UAE@XZ in the repo (wave 31, family tcd).
- Open header jobs:
  - (a) CLipSyncDataBank.h as CBankFileAsync : CBankFile, which the self row 0x00A42980 needs. The callee spellings are proven: GetAsyncEntry 0x9D58D0, GetEntryDataSize 0x9CC240, BeginReadDataAutoAlloc 0x9D6100, CCountedPointer<CBankFileAsyncData>::operator= 0x9A4440, CResourceBank::AddResource 0x9FC3D0, CResource::CResource 0x9FBEC0, and the nested CLipSyncDataBankEntry::CData : CResource (size 0x34).
  - (b) Inline-ctor opt-ins for CTCInventoryItem / CTCInterestingToVillagers / CTCHeroStats / CTCActionUseBase, so their own ??_G (0x004DC5E4, 0x004DC8FA, 0x005814C9, 0x004DBF76) can emit.
  - (c) An implicit-dtor opt-in for CTCCreatureGroup.h / CTCPhysicsBase.h (0x004E5C71, 0x006B14F0).
- The made-up names in the changed rows are only local helpers (MakeOne / DestroyOne emission helpers), not retail functions. No placeholder names were added.

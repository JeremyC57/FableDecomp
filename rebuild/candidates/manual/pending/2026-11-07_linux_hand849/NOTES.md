# Chunk 849 notes

All six rows were held Codex RELOCATION_MATCH sources written on another header model. Each one is rebased onto rebuild/include with
additive `FABLE_RETAIL_*` opt-ins; hdr/README.md lists every header change.
Result: 6 of 6 RELOCATION_MATCH (none is a plain MATCH; all are relocation-masked). None parked, so differ/ is empty.
Final read-only vland run with the final overlay: 6 WINS, RELOCATION_MATCH PASS (logs/vland.log).
bcheck shows 0 reloc-masked byte differences for every row. relpair pairs every call target and literal with retail.

## 0x00AE2E20 CParticleProgram::OptimiseSpringOrder(CArray<CParticleDistanceConstraint>&) const (`/Gr`)
- Shape: the Codex body, with one change: `set<unsigned long>::iterator` replaces its const_iterator spelling. Breadth-first spring ordering:
  a vertex -> set of neighbours (CArray<set<unsigned long> >), a distance array, then a swap of particle_list with next_particle_list each round,
  then a sort of (distance, index) pairs and a rebuild of the springs.
- Levers:
  - FABLE_RETAIL_CARRAY_PUSHBACK_FASTPATH. Retail calls `_M_insert_overflow` 0x00464ACE directly from the seed push_back.
  - A row-local `template<> __declspec(noinline) CArray<std::set<unsigned long> >::CArray(unsigned int)` body. Without it there is a
    26-line register residue.
  - Also VECTOR_SWAP (retail expands `swap` in place at 0x00AE30C7..0x00AE30DC) and SORT2 (`sort` 0x00AE7DC0 called out of line).
- Names:
  - OptimiseSpringOrder: proven (Ego_r and FableWin PDB). The catalog label "LTextBinTree_BuildAndSort" is a placeholder (LABELS.tsv).
  - DELTA_NOTIONAL_ZERO 0x0129BA3C: the landed majority spelling; 0x00838EC0 spells it g_NormaliseEpsilon (NAME_FIXES).
- One symbol, one address CONFLICT: `??$__destroy_aux@PAV?$set@K...@std@@YIXPAV...ABU__false_type@0@@Z` is called here at 0x00AE76E0, but landed
  0x0051C470 already binds it at 0x0051AC70. The two bodies are byte-identical (two COMDAT copies of one instantiation, no ICF). The row
  matches either way, and vland reports RELOC_CONFLICT. The coordinator decides; it is noted in NAME_FIXES.tsv. I did not invent a second name.
- The callee 0x00AE7880 is landed as `std::vector<std::set<long> >::vector(unsigned)`. This row calls it as
  `CArray<std::set<unsigned long> >::CArray(unsigned int)`:
  - FableWin.pdb has that exact symbol, and the Codex TPI local is CArray<set<unsigned long> >. The element type `K` is proven.
  - Whether the function is CArray(n) or vector(n) cannot be told from the bytes, because CArray(n) inlines vector(n).
  - Recorded in NAME_FIXES and LABELS.

## 0x0058E99F CTCInventory::OpenInventoryShortcut(long) (`/O1 /Oy-`)
- Shape: the Codex body, with `ParentThing->HasTC` (our retail CTCBase holds ParentThing as a pointer). It tests the bitset bit, deletes the
  menu components, then a switch (jump table 0x0058EF31) opens the requested menu and selects the entry.
- New FABLE_RETAIL_CTCINVENTORY branch:
  - Bases: CTCInventoryBase and NHeroInformationScreens::CBase (RTTI).
  - Members from +0x134; sizeof 0x228.
  - The six PDB methods.
- Label correction: 0x0058A763 is CTCInventory::OpenPCStatusMenu, not OpenPCSkillsMenu (0x00587BFF is OpenPCSkillsMenu). The PDB has both names.
  The jump table sends cases 3/6/7 to 0x0058A763 and cases 5/8 to 0x00587BFF.

## 0x0041D21B NUISystem::CManager::CreateComponent(long) (`/Os /Oy-`)
- Shape: the Codex body. It switches on the UI definition's component type, then does a sized operator new and a ctor call, 44 of them.
- Levers:
  - The 8 new `*_MEMBERS` opt-ins and the 5 new NUISystem headers give retail's allocation sizes.
  - A same-TU noinline `CDefinitionManager::GetDef<NUISystem::CUIDef>` body under `#pragma auto_inline(off)`, as Codex had it. It stops
    VC7.1 reloading and null-testing the CDefPointer at exit.
- Names:
  - Ctor spellings come from the PDB name sets.
  - New headers CDraggableInto, CNavButton, COk, CControllerDisconnect, CSpellContainerList: RTTI base plus ctor address (hdr/README.md).
- NAME_FIXES: 0x0052CC50 is the CChangingStateComponent ctor (one landed spelling uses EditBoxBaseNative). 0x009ACCC0 is
  CDefinitionManager::GetDefNameFromGlobalIndex. 0x0044C6B0 is CGameDefinitionManager::Get.

## 0x009CF3A0 CBankFile::WriteSymbolHeaderFile() (no flags)
- Shape: the Codex body, unchanged. It builds a map<CCharString,CArray<unsigned long> > of entries by symbol, writes the public and the
  commented private enumerations to a CMemoryFile, and dumps that to the dev and retail header paths.
- Model: engine/CBankFile.h is HOLD, so retail CBankFile stays row-local, as in landed 0x00AB8E10. Its parts:
  - CBaseClassNonCopyable base
  - 20 virtual slots
  - typed members at the header offsets
- New opt-ins: FABLE_RETAIL_CMEMORYFILE_DUMPTOFILE (0x009E9330) and FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS (0x009A7E20 / 0x009A7ED0 plus
  GBankFileManager 0x013CA79C).
- New headers: CPackedUIntArray.h (operator[] const 0x00A629F0) and CBankFileEntryUpdateData.h.
- Names:
  - The catalog label "CBankFile_WriteSymbolFile" is a placeholder; the PDB has WriteSymbolHeaderFile.
  - GBankFileManager comes from Ego_r's dynamic initializer and atexit names. Four landed spellings differ (NAME_FIXES).
  - 0x009D3E80 is the STLport `_M_erase` of the map tree (catalog: CRetailTree_erase009d3e80::Erase).
- Header debt: engine/CBankFile.h needs a FABLE_RETAIL_CBANKFILE polymorphic branch, taken from the row-local model in src/009cf3a0.cpp
  (CBaseClassNonCopyable base, vptr, the 20 slots, members as typed there). The header is under HOLD, so I left it alone.

## 0x0057E2F1 CTCHeroStats::OnSerialise(CPersistContext&, bool) (`/O1 /Oy- /Oi`, protected virtual, MAEX)
- Shape: the Codex body. It transfers the stats with the CPersistContext Transfer / TransferVector templates. On save, the crime map is mirrored
  through `map::operator[]` and the Marriages list is copied into a vector (std::copy plus back_inserter); on load the reverse.
- Levers:
  - New FABLE_RETAIL_CTCHEROSTATS_SERIALISE branch: typed Ego_r members at the default offsets; sizeof 0x1DC asserted.
  - New CHeroStatsDef.h and EHeroTrainingStatus.h.
  - CPersistContext ISSAVING_OUTOFLINE: retail calls 0x004AEF50.
  - STLP_BACK_INSERTER and STLP_LIST_CLEAR_OUTOFLINE.
  - STLP_LIST_SIZE_DISTANCE. e1 was 8 lines off with the for-loop size(): head and walk registers swapped. STLport's
    `distance(begin(),end())` form matched.
- Names:
  - std::copy 0x00581376 (list -> vector) and 0x005813A0 (vector -> list) come from the STLport 4.6 instantiations and the caller types. The
    catalog labels are Construct_CRangeString_00581376 and CTCCameraPointScripted::GetLookDirection, and the landed bindings are
    RangeInitialize@CRangeString_00581376 and GetLookDir_helper. All of these are wrong (LABELS and NAME_FIXES).
  - 0x0057F58E is `_List_base<CHeroMarriageInfo>::clear` (landed: ?Reset@CMarriageList).
  - 0x00569FE0 is TransferVector<float>, as landed 0x00560720 already binds it (catalog: Transfer<CQuestScoreRecord>).
- Descriptive or single-source names:
  - CGameDefinitionManager::PHeroStatsDef (+0xDC) is descriptive. It replaces the Codex reinterpret_cast and is read as
    `GetDefinitionManager().PHeroStatsDef->MaximumStamina`.
  - EHeroTrainingStatus enumerator spellings come from Codex's PDB read only.
  - CHeroStatsDef members are Ego_r, at +0x3C after the packed 0x39-byte base; checked against 0x0057E448 and landed 0x0057DE14.

## 0x00501450 CWorldMap::GenerateRegionConnectivityGraph() (no flags)
- Shape: the Codex pass 13 body. For each region:
  - It loads the region (the hero is put in limbo).
  - It collects every CTCDRegionExit thing (GetAllThings with CThingFilter_HasTC) and every CTCActionUseScriptedHook thing
    (CThingFilter_HasTCOfLeafType).
  - It inserts "region;target" strings into a set<CCharString>.
  - It writes the set to a file through CDiskFileWin32.
- Levers (11 attempts; the full history is in STATUS.tsv):
  - CTHINGFILTER_HASTC_OUTOFLINE. Retail calls operator() 0x006A80A0. With the inline body visible, VC7.1 hoists the filter temporary's store
    above the region loop: 235 lines down to 92.
  - FABLE_RETAIL_STLP_MAP (the visible `_tree.c` insert_unique body) shares the insert pair temporary's slot: 92 lines down to 8.
  - The main player as a named local, `CPlayer& player = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer();`, then
    `player.GetPControlledCreature()`: 8 lines down to a match. Without it, phero and current_region swap spill slots.
  - Codex's PDB locals list has no `player` local, so this is a shaping lever with no PDB support. Declaration order and const on either
    local did not move the slots (f8 to f10).
  - CVECTORMAP_INLINE_LOWERBOUND: retail expands the non-const LowerBound at 0x00501645..0x00501687 and calls the const one, 0x004365B0.
- New headers CTCDRegionExit.h, CTCActionUseScriptedHook.h and CThingFilter_HasTCOfLeafType.h. Their bases, TCI values, members and
  GetPEntranceConnectedTo come from the Codex TPI read (single source besides Ego_r member names; marked descriptive where only Codex has it).
- Twin: GetNameOfRegionConnectingTo is called at 0x007792D0. The catalog gives the same name to the byte-identical 0x007791F0. Neither address
  is bound yet; only 0x007792D0 is proven here.
- NAME_FIXES:
  - LoadRegion 0x00500540: `W4ELoadType@CWorldMap@@` nested enum, per Ego_r.
  - GetAllThings 0x0048D400: the template with CThingFilter_HasTC; the catalog label is garbled.
  - GetMainPlayer 0x00449970: returns a reference, QAEAAV.
  - GetPControlledCreature 0x00487DC0.
  - CCharString::SaveToFile 0x0099E5F0: QBEXAAVCAFile.
  - The CCharString set insert_unique / _M_erase pair 0x004AC380 / 0x004ABA70.

## Labels
LABELS.tsv holds only wrong or placeholder labels:
- the two placeholder row labels (0x00AE2E20, 0x009CF3A0)
- 0x0058A763 OpenPCStatusMenu
- 0x00569FE0, 0x00581376, 0x005813A0, 0x0057F58E, 0x0051AC70, 0x009D3E80, 0x0048D400, 0x00AE7880

## Process notes
- f6 and f7 of 0x00501450 were first graded at the same time on the same address by mistake. Both were regraded one at a time, with the same
  results.
- The first 0x0057E2F1 compile hung because ferr.py ran without FABLE_HDR_OVERLAY, so the new CHeroStatsDef.h was missing. The python timeout
  stopped it, and no cl.exe was left running.

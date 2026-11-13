# DOSSIER 0x0060EEB0 -- CTCInventoryMap::ConstructPCMapList (identity scout, bwf1)

Prior scouts already wrote a very full outline. Read these first: `prior/scout/outline.md` (frame map, block-by-block pseudo-C++,
call table, inline splits) and `prior/scout/headers.md` (names, slots, overlay). Below are the facts, re-checked against the current repo state.

## 1. Retail shape (dis_all.txt)
- `sub esp,2B8h; push ebx; push ebp; push esi; push edi`. No frame pointer, no `fs:0` SEH (built without /EHsc), no /GS cookie, no x87
  control-word / SSE. `ret 8` at 0x006116BB (__thiscall, 2 stack args). This is a speed TU: loop heads are aligned with `lea esp,[esp]` /
  `lea ebx,[ebx]` pads. **No `// Flags:` line and no `optimize` pragma**, which matches every same-TU sibling.
- There are two epilogues: return 0 at 0x0060F5DB (shared by the `check_only` and `num_regions == 0` exits) and `mov eax,[esp+40h]`
  (list) at 0x006116AD.
- **Draft frame is 2B4h, retail is 2B8h.** Retail has one more 4-byte stack slot. Candidates (outline sec. 2): S16C is `this` of
  list::_M_insert_dispatch and is never written; S20 is reused three times (uninitialised quest_card, then the recall `const CRegion*`,
  then misc_def). Check the slot map against ours with the frame-masked diff before you change any statements.

## 2. Name / signature (two sources)
- `?ConstructPCMapList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@_NPA_N@Z`. Sources: ghidra_out/egor_pdb_names.tsv:11688
  (Ego_r 0x006621A0) and ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv:209391 (0x020A1D80, 13165 b / 3028 instr).
  It is public and non-virtual: `NUISystem::CComponent* ConstructPCMapList(bool, bool*)`.
- **The manifest label is right. The reloc binding `?BuildMapMenu@CTCInventoryMap@@...JJ@Z` is wrong**
  (prior/scout/NAME_FIXES_candidates.tsv). The declaration is in the repo header `rebuild/include/engine/CTCInventoryMap.h:33` under
  FABLE_RETAIL_CTCINVENTORYMAP_PCMAPLIST as `(bool a, bool* b)`.
- No PDB has the parameter names. The draft's `check_only` / `has_regions` are descriptive, so mark them so in NOTES.
- Callers (`w23/xref.py`): 0x00589B1C in CTCInventory::OpenPCMapMenu (landed 00/58/CTCInventory_OpenPCMapMenu_00589a30.cpp) and
  0x00611817 in CTCInventoryMap::OpenInventory (landed 00/61/..._006116e0.cpp, same TU). Both pass (false, 0).

## 3. Callees: landed file / landed name / PDB name (flag = landed spelling disagrees with the PDB)
| retail | landed file (rebuild/src/compiled/) | PDB spelling | flag |
|---|---|---|---|
| 0x007021F0 | 00/70/CTCQuestCard_GetMarkerRegionName_007021f0.cpp | ?GetTeleporterRegionName@CTCQuestCard@@QBE?AVCCharString@@XZ (Ego_r 0x00723D30 + FableWin) | **landed name wrong**. Draft spells it right (overlay CTCQUESTCARD_TELEPORTERREGIONNAME) |
| 0x004FB470 | 00/4f/CWorldMap_DrawGetNoMaps_004fb470.cpp | ?GetRegionCount@CWorldMap@@QBEJXZ (Ego_r 0x004CEBA0 + FableWin) | **landed name wrong**. Draft spells it right (overlay CWORLDMAP_REGIONCOUNT) |
| 0x0051D200 | 00/51/global_PushBackCRegion0051d200_0051d200.cpp | vector<CRegion>::push_back(const CRegion&) | descriptive file name. Out of line at ONE site only (0x0060F168) |
| 0x004A9CB0 | 00/4a/unattributed_FreeTreeCTreeOwner004a9cb0_004a9cb0.cpp | STLport set<long> _Rb_tree::_M_erase (no PDB source: Dinkumware PDBs) | descriptive |
| 0x00619D50 | 00/61/unattributed_InsertUniqueCRetailTree00619d50_00619d50.cpp | map<long,CRegion> _Rb_tree::insert_unique(const value_type&) | descriptive |
| 0x005BEB57 | 00/5b/CTCInventoryBase_GetMiniMapGraphic_005beb57.cpp, Leaf `QAEJPBVCRegion@@@Z` | ?GetMiniMapGraphic@CTCInventoryBase@NInventory@@QBEKAAVCRegion@@@Z (Ego_r 0x00620EC7) | **landed Leaf wrong**. Header NInventory_CTCInventoryBase.h:174 already has the PDB form (`unsigned long GetMiniMapGraphic(CRegion&) const`) |
| 0x005FA990 | 00/5f/CTCInventoryMap_GetGuildSealRecallRegion_005fa990.cpp | ?GetGuildSealRecallRegion@CTCInventoryMap@@ABEJXZ | ok. Called at 0x0060F458, EXPANDED twice (0x0060FDCB.., 0x00610048..) |
| 0x005FD410 | 00/5f/CTCInventoryMap_GetRegionsToDisplay_005fd410.cpp | ?GetRegionsToDisplay@CTCInventoryMap@@QAEXAAV?$set@J...@Z | ok (local model) |
| 0x005FBEA0 | 00/5f/CTCInventoryMap_GetRegionTeleporter_005fbea0.cpp | ?GetRegionTeleporter@CTCInventoryMap@@ABEPAVCThing@@ABVCRegion@@@Z | ok |
| 0x00411570 | 00/41/CBasicStringchar_Compare_00411570.cpp | ?Compare@?$CBasicString@D@@KIJPBD0@Z | ok |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,...>::LowerBound | ok (called, key 0x6C) |
| 0x006196D0 / 0x00618100 / 0x0051CF10 / 0x00512DA0 / 0x0042D2C6 | STLport rows, all landed with Leafs | | ok |
| 0x0042B312 / 0x0052E5D0 | 00/42/CDefinitionManager_GetDef0042b312, 00/52/CUIState_CUIState_0052e5d0 | GetDef<CUIMiscThingsDef> / ??0CUIState@NUISystem@@QAE@XZ | ok |
| 0x00603000 / 0x005FF190 / 0x00605570 / 0x00428494 | UNLANDED | CreateMiniMapMarkers / CreateMapMarkers / ConstructRegionDescription / CManager::CreateMenuEntryComponent | declared in headers / overlay |

The rest (CCharString / CWideString / CWorldMap / CTCBase::PeekWorld 0x00686A80, PeekWorldMap 0x00686D20, GetWorldMap 0x00686D30 /
CManager / text bank) is in the outline sec. 4 table. All of it is landed and the draft spells it in the PDB form. Virtual slots
(CComponent 3/59/65/66/68/82/124, CList 144/145/152/156/158, CZoomingComponent 143/144/145, CViewport 142, CSprite 145, CText 144,
CBase 3 IsInventoryScreen, CWorld 13 PeekWorldMap, CWorldMap 16 GetMapNumberAt) are in headers.md sec. 3, all E+F names.

## 4. Members
- this: +0x130 is the NHeroInformationScreens::CBase subobject (vcall slot 3 IsInventoryScreen). The CTCInventoryMap members are
  pmap_real +0x188 (CZoomingComponent*), MapList +0x214 (CList*), MapRegions +0x218 (std::map<long,CRegion>) and PanMode +0x248 (bool).
- struct_layouts_egor.tsv has the same members typed (pmap_real, MapList, MapRegions, PanMode). Its offsets are Ego_r's
  (440/620/624/676 dec), so do not use them as retail offsets.
- **In the repo header these are `unsigned char pmap_real[4]`, `MapList[4]`, `MapRegions[12]`** (CTCInventoryMap.h:58,67).
  The typed members exist only in the overlay `hdr/engine/CTCInventoryMap.h` under FABLE_RETAIL_CTCINVENTORYMAP_MAPMEMBERS.
- Other members: CTCBase +4 ParentThing; CThing +0x8E CurrentMapNumber; CRegion +0x18 Name, +0x20 RegionDefName, +0x54
  AppearOnWorldMap (engine/CRegion.h); CUIMiscThingsDef +0xE8 PCRingCenter, +0x1F8 HeroDollFrameTL_PC, +0x200
  HeroDollFrameEmulateListOffset; CWorld +0x154 GuildSealRecallPos (overlay CWORLD_GUILDSEALRECALLPOS); CQuestManager +0x5C
  ActiveQuestCards; CUIState fields (StateChangeFlag=0x67, StateChangeType=3, UpdateTime=0.5f).
- 22 of the draft's opt-in macros are NOT in the current repo and exist only in $W/hdr. They must ship with the hand-back:
  C2DVECTOR_NAMED_RESULT_SUB, CACTION_INLINE_CTOR, CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS, CREGION_ASSIGN,
  CRGBCOLOUR_CTOR4_INITIALISE, CTCINVENTORYMAP_{GUILDSEALRECALLREGION,MAPMEMBERS,QUESTMARKERS,REGIONDESCRIPTION_ALL},
  CTCQUESTCARD_{ACCESSORS,STATGETINTERFACETYPE,TELEPORTERREGIONNAME}, CTHING_PEEKTC_ORIGINAL_CHAIN,
  CUISCREENUNITCONVERTER_VECTOR_INLINE, CUISTATEDEF_IMPLICIT_DTOR, CUISTATE_IMPLICIT_COPY, CWORLDMAP_REGIONCOUNT,
  CWORLD_GUILDSEALRECALLPOS, CZOOMINGCOMPONENT_ZOOM, STLP_ALLOC_PROXY_REBIND, STLP_TREE_INSERT_UNIQUE_HINT_BODY.
- **`FABLE_RETAIL_CSPRITE` is defined nowhere** (repo or overlay), so that define is dead.
- STALE overlay headers (HINTS) must be re-based by hand: fable_containers.h, CTCInventoryQuests.h, CQuestCardDef.h,
  NUISystem_CZoomingComponent.h, C2DVector.h, NUISystem_CUIStateDef.h.
- The draft models no class locally. Everything goes through shared headers (good).

## 5. Callee-body visibility / inline split (what the author must reproduce)
- **vector<CRegion>::push_back** is EXPANDED at 0x0060EFF7, 0x0060F3E8, 0x0060F4DF and 0x0060FCD9, and CALLED (0x0051D200) only at
  0x0060F168, inside the GetRegionCount loop with its 3-operand `&&`.
- **GetGuildSealRecallRegion** is CALLED at 0x0060F458 and EXPANDED in both loops. Its body has to be visible: the draft defines it
  inline from the landed 0x005FA990 text, which is correct.
- **CBasicString<char>::Compare** is CALLED in the two RegionDefName de-duplication loops (0x0060F39E, 0x0060F4CA) and EXPANDED in
  the later Name compares. The draft has the same-TU template body (from 4007).
- CDefinitionManager::GetDef is CALLED (0x0060F62B). The draft has a same-TU body under `auto_inline(off)`, as landed 0x006001E0.
- `CCharString::operator!=(const CCharString&)` is out of line (0x0099E900). `operator!=(const char*)` is always out of line.
  `operator==` is always inline: pointer-equal shortcut, then a length compare, then Compare.
- PeekTC<const CTCQuestCard>: the bit +0x2C>>12 is MATERIALISED to [esp+13h], LowerBound is CALLED, and quest_card is left
  uninitialised (reloaded at 0x0060F229 on a miss). The draft uses `const CTCQuestCard* quest_card; (*card)->PeekTC(&quest_card);`
  with FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN. The same-TU siblings use CTHING_GETTC_EXPANDED, but that form gives a non-materialised bit.
- Inline STLport bodies (map clear, set ctor/dtor, list copy/dtor, vector size `imul 2E8BA2E9h`): see outline sec. 5. These require
  STLP_MAP / STLP_SET / STLP_LIST(_CONSTRUCT_HELPER).
- CCountedPointer<CComponent>(T*) is expanded at all 16 sites (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA). CAction ctor is inline
  (CACTION_INLINE_CTOR). CDefPointer release uses CDEFPOINTER_RELEASE_SELFDELETING_POINTEE + CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES.

## 6. Siblings (same TU 0x005FA9xx..0x00619F50, all with NO `// Flags:` line)
- 00/60/CTCInventoryMap_ConstructRegionDescriptionShops_006001e0.cpp: closest shared-header UI builder (CVECTORMAP_INLINE_LOWERBOUND,
  CWORLDMAP, CWORLD, CREGION, CTHING_GETTC_EXPANDED, STLP_LIST, CTEXT, CDEFINITIONMANAGER, CTCINVENTORYMAP_REGIONDESCRIPTION), with
  GetDef in the same TU under auto_inline(off).
- 00/60/CTCInventoryMap_FrameUpdate_006061b0.cpp (CQUESTMANAGER, CTCQUESTCARD_*, CTCINVENTORYMAP_FRAMEUPDATE).
- 00/61/CTCInventoryMap_OpenInventory_006116e0.cpp: the caller, defined right after this row, local-model `V(n)` vtables,
  `auto_inline(off/on)`.
- 00/5f/CTCInventoryMap_GetRegionsToDisplay_005fd410.cpp: statement text for the list copy and the quest-card loop.
- Family match: /opt/fable_lane/w22/agent4007 (CTCInventoryQuests::ConstructQuestList 0x0061B610). Use its tools/cd.sh (direct-call
  comparison) and tools/bt.sh (branch-target check), and LESSONS.md (prior/earlier_4007/LESSONS.md).

## 7. Likely model errors in the draft (DIFFER 10242 v 10254, 427 hunks, 2778/2900 frame-masked equal)
1. **FITTED folding guard `if (regions.empty()) { ... }`** around the whole gather phase. It was added only to halve the inline
   frequency, and its natural form is unknown. Two leads, both from 4007 LESSONS: (a) a real branch level in front is what makes
   callees "called instead of expanded" (lesson c2058); (b) `if (n != 0)` in front of a `for` halves the loop body's frequency with
   no code change. Look for a genuine condition in retail's own control flow, or an `||` reorder per lesson f4007, rather than a dead
   guard. The guard is not ship-quality.
2. **Frame 4 bytes short** (2B4h v 2B8h). Find the missing named local (see sec. 1) before you chase registers. Retail also caches
   regions._M_start/_M_finish in ebx/edi at the join 0x0060F54A..0x0060F552 (`mov ebp,[S18]` = this reload; `mov edi,[S28]`;
   `mov ebx,[S24]`). The draft reads _M_start there instead.
3. **Second FITTED spelling**: `state.Position.X = origin.X + state.Position.X` (operand order chosen to match fld order). Expect to
   revisit it. Fewer remaining residues: `this` loaded before the last push (0x0060EFBE / 0x0060F001 / 0x0060F01C), fld/fmul order
   0x0060F752, fxch/fadd 0x0061136A, SIB index order 0x0060FE86 / 0x00610510 / 0x006105A6 (lesson 4007: an IL-amount effect; look
   for the missing inline layers that FableWin proves, e.g. one-argument GetTC/PeekTC templates or accessor layers).
4. De-duplication loops: retail jumps from a match straight to the iteration end (0x0060F384 / 0x0060F3A9 -> 0x0060F41B; 0x0060F4B4
   / 0x0060F4D5 -> 0x0060F51A). The draft's `bool already_listed; break; if (!already_listed)` is acceptable only if VC folds it. A
   `goto next` / early-`continue` form is the alternative to try if the hunks are there.
5. The draft's `regions.size()` loops in the de-duplication: retail computes size once (esi) and uses an index in ebp and a pointer
   in ebx. That is consistent with `for (j = 0; j < regions.size(); ++j) regions[j]...` (loop-invariant size).
6. Hand-back debt: re-base the 6 STALE overlay headers, remove the dead FABLE_RETAIL_CSPRITE define, and add NAME_FIXES for
   0x007021F0, 0x004FB470 and the 0x005BEB57 Leaf.

## 8. Re-verification and new facts (second identity pass, 2026-11-12 late)
Re-checked against the current repo: sec. 2 name (egor_pdb_names.tsv:11688 Ego_r 0x006621A0; FableWin.exe.signatures.tsv:209391
0x020A1D80 13165 b / 3028 i; egor_signatures.tsv:12356), the callers (xref: 0x00589B1C, 0x00611817 only), the three wrong landed
callee spellings (files still `CTCQuestCard_GetMarkerRegionName_007021f0.cpp`, `CWorldMap_DrawGetNoMaps_004fb470.cpp`, 0x005BEB57
Leaf still `QAEJPBVCRegion@@@Z`), and the repo CTCInventoryMap.h members (still `unsigned char pmap_real[4]`, `MapList[4]`,
`MapRegions[12]`; the header offers FRAMEUPDATE / HISCREEN / PASTQUESTS / PCMAPLIST / QUESTREVIEW / REGIONDESCRIPTION /
REGIONINFO_CTOR / TELEPORT / VSLOTS; MAPMEMBERS, QUESTMARKERS, GUILDSEALRECALLREGION, REGIONDESCRIPTION_ALL are overlay-only). All hold.

New:
- **Console twin = ConstructMapList 0x0060C900** (`?ConstructMapList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@_NPA_N@Z`,
  egor_pdb_names.tsv:11687 Ego_r 0x0065F950; `ret 0Ch`, frame 298h). Disassembly saved as $W/twin_dis.txt. Its gather phase is the
  SAME source with the SAME call/expand split: GetRegionCount called 0x0060CB25/0x0060CBBD, push_back called once 0x0060CBA7,
  Compare called 0x0060CDCE/0x0060CEFA, GetGuildSealRecallRegion called 0x0060CE88, GetDef called 0x0060D069 (ours:
  0x0060F0E6/0x0060F17E, 0x0060F168, 0x0060F39E/0x0060F4CA, 0x0060F458, 0x0060F62B). So whatever produces the split (the FITTED
  `if (regions.empty())` today) is shared text of both functions, i.e. a habit of the original author, not something specific to the
  PC arm. Its exit is `num_regions = size(); if (!num_regions)` computed with `imul 2E8BA2E9h` then `jne` on the add flags
  (0x0060CFC7..0x0060CFE4): supports the draft's `unsigned long num_regions = regions.size(); if (num_regions == 0) return 0;`.
- **Our build has the TWIN's register allocation at the head**: built x2 head = `xor ebx,ebx; mov [ebp+248h],bl; cmp eax,ebx` exactly
  as 0x0060C908..0x0060C91F. Retail PC instead stores `mov byte ptr [ebp+248h],0` (immediate: no zero register yet), zeroes edi first
  and ebx later, and both ebx/edi end up caching regions._M_start/_M_finish. That is an allocation residue driven by later code (the
  PC function's extra zoom arm / frame), not by the head statements. The head order `PanMode=false; vector regions; MapRegions.clear();
  CCharString marker_name("")` is the same in both twins (draft is right; h1/h2 reorders did not help: 427/424 hunks, same size).
- **SCC is a conditional-destruction flag word zeroed at function entry** (0x0060EED6 `mov [esp+0CCh],edi`; set `or ebx,1` 0x006104CC,
  tested/cleared 0x00610544..0x0061055F). It belongs to the `CCharString("HeroGuildComplexInside")` temp inside an `||` / `?:` in the
  main loop (0x006104C3: `recall == -1 && region.Name == CCharString("HeroGuildComplexInside")`-shaped). In our x2 build the same
  flag lands at [esp+0ECh]: our frame layout differs by 0x20 below it -- use this flag slot as a ruler for the 4-byte frame gap.
- Attempts already graded this session (ATTEMPTS.log; do not repeat): guard removed (x1, y1-y3: 10500 B, 566-568 hunks -- worse, the
  split really needs one more branch level); guard on the B arm only (x2: same as draft, 10242/427); `begin()!=end()` guard (j1: same);
  **`if (regions.empty()) return 0;` before `num_regions = size()` (j2: 10238, 423 hunks, best hunk count but 4 B further off)**;
  head reorder h2 (424 hunks); dead-local rulers r1/r4/r12 (no effect), parameter-initialised dead locals r2/r10/r34 (10236, 423-425:
  the frame does react to extra live locals); t1 `regions.clear()` after decl (10322, worse); t2 `empty()?false:true`, t3 nested ifs
  at the join (no change).
- Twin note for the frame: the PC function's extra 4-byte slot is NOT in the twin's head (twin frame 298h has its own extra param);
  compare twin vs PC slot maps of the gather phase (twin S78 marker_name is at [esp+60h] after the push = S5C) to see which slot the PC
  function adds.

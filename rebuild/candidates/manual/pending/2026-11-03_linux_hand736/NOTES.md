# Chunk 736 -- CMap / CNavQuadTree loaders and four mislabelled small rows

Result: 4 matched (payload.json; vland on the full header mirror, every one PASS), 1 parked with its best source in differ/,
2 not attempted (the two 2.5 KB LoadFromFile bodies; read and mapped below, no budget left after the nav TU work).

| Row | Function (proven name) | Catalog label | Result | Attempts | Lever / residue |
|---|---|---|---|---|---|
| 0x00C1EE30 | CEventTriggerRef::GetDriverCount | CMap::GetWorldMapNumber | MATCH | 2 | `m_pTrigger->GetDrivers()->GetCount()` |
| 0x00C5A1F0 | CSoftwareVoice::Stop | CMap::Update | RELOCATION_MATCH | 2 | qualified base Stop as the tail |
| 0x00C5A2A0 | CSoftwareVoice::Reset | CMap::Update | RELOCATION_MATCH | 2 | base Reset, three zero stores, two ring Flushes |
| 0x00A78E20 | CNavQuadTree::IsAreaClear | CNavQuadTree_InitializeLeafNode | RELOCATION_MATCH (retail_asm) | 8 | same-TU growth specialisations + same-TU IsAreaBlockedByLines |
| 0x00A7A0A0 | CNavQuadTree::SetUpRegions | CNavQuadTree_InitializeWithEvents | DIFFER(1270v1222) | 12 | copy-ctor per-site inline split |
| 0x00A7AF60 | CNavQuadTree::LoadFromFile | (same) | not attempted | 0 | -- |
| 0x00821A80 | CMap::LoadFromFile | (same) | not attempted | 0 | -- |

The brief asked for >= 10 attempts per row before parking: 0x00A7A0A0 got 12; the two LoadFromFile rows got none (budget), so
they are reported NOT_ATTEMPTED, not parked.

## Names (two sources each)
- The two "CMap::Update" rows and "CMap::GetWorldMapNumber" are not CMap at all (CMap::Update is Ego_r `MAEXXZ`, a virtual of a
  0x1D40-byte game class; these sit in GameCODA / libpk). 0x00C5A1F0 is slot 2 and 0x00C5A2A0 slot 17 of the CSoftwareVoice
  vtable 0x012B5C60 (+ 0x012B5CD0, 0x012B5D70 / 0x012B8AE0 for slot 2): data refs of the bodies, the class every other slot binds
  (ctor 0x00C5B1A0, ?SetParameters@CSoftwareVoice), the destructor 0x00C3F490 calling slot 2 while not stopped, the stream voice
  0x00C5C290 calling 0x00C5A2A0 as its base slot 17, CCodaVoice::Play 0x00C59720 naming slot 17 Reset. Class / method names are
  descriptive (no PDB / RTTI for GameCODA) but follow the landed CCodaVoice / CSoftwareVoice rows; NAME_FIXES respells the
  `CVtable_012b5c60` / `CAudioDevice_00c5c290::Prepare` bindings and the wrongly landed base slots 0x00C59710 / 0x00C59860.
- 0x00C1EE30: the bound ?GetDriverCount@CEventTriggerRef@@QAEKXZ (caller 0x00C2E4D0 passes the result as InsertDriver's
  position). The same 8-byte object is `CItemAction` in the 0x00C1EE00..0x00C1F4E0 landed rows; the class choice is open in
  NAME_FIXES_QUEUE row 590 ("pick ONE name at landing"): this row uses the spelling bound to its own address.
- 0x00A78E20 = Ego_r ?IsAreaClear@CNavQuadTree@@QBE_N... (the bool& cleared first, u64 ids pushed, bool result tested by the
  caller CNavQuadTreeNode::Initialise 0x00A79B30; donor TU position). 0x00A7A0A0 = Ego_r ?SetUpRegions@CNavQuadTree@@AAEXJ@Z
  (one long, CNavRegion appends, sole caller the end of CNavQuadTree::Initialise 0x00A7A8D0; donor position between
  InitialiseLines and ClearNodes).
- Descriptive: CAudioRingBuffer::Flush 0x00C5CEA0 (no PDB), CCodaVoice slots 3..6/8/9/13 (no name evidence; v05-style names
  only where nothing exists), g_QuadTreeLevelSizes 0x0129DCAC (no data symbol), DELTA_NOTIONAL_ZERO as landed (naming wave 18).

## Matched
- 0x00A78E20 (src/00a78e20.cpp): local vectors of `const list<C2DLineF>*`, the topology scan through __ftol2, the four cell
  bounds through GFFloatToLongFloor (the shared asm helper; ebp frame), unsigned cell counters (2^32 fix-ups). Two levers:
  (1) the header's `_M_insert_overflow` template bodies are expanded at both push_backs (1238v1042): same-TU explicit
  specialisations with the header's __true_type body under `auto_inline(off)` keep them out of line (0x00A81590 / 0x00617370);
  (2) with IsAreaBlockedByLines declared only, the vector escapes and VC7.1 reloads `_M_finish` after every element store
  (1043v1042, node in esi instead of ebx): its same-TU body (the landed 0x00A76F30 logic) fixes both. Grade retail_asm: the row
  has no asm, but vland's purity gate needs its own `// Retail-asm:` line for the helper (GOTCHAS.txt).
- The three small rows: local models in the style of their landed GameCODA / libpk siblings (no engine header exists for
  CCodaVoice / CSoftwareVoice / CAudioRingBuffer / CEventTriggerRef).

## Parked: 0x00A7A0A0 SetUpRegions (differ/00a7a0a0.cpp, best s7 on the headers, 128 masked lines)
- Body: region 1, then per region seed GetNodeAt -> MakeRegion flood over a growing list -> next region; per layer / node
  AddNavigableNodeNeighbouringSwitchableNodeToRegion loop (flood, next region, nodes.clear()); last, RemoveRegionZero nodes
  deleted and replaced by CNavQuadTree::BlockedNode. Structure, frame, allocation order, list destructors and the part-2
  expansion all match.
- Residue 1 (the size): retail calls the implicit CNavRegion copy ctor 0x00A83110 at the first two Regions.push_back sites and
  expands it only at the third (part 2, triple loop); VC7.1 expands sites 2 and 3 for every spelling tried (continue form,
  ++ in the argument, explicit in-class / out-of-class copy ctor, condition spellings). The 2026-11-03 coordinator note says
  such splits follow c2's block frequency; the statement shape that lowers site 2's frequency is not found.
- Residue 2: retail ORs the packed region id in both expansions while the out-of-line ctor 0x00A78680 XORs (and LoadFromFile's
  inline copy at 0x00A7B079 XORs too); the parked source uses `|` in its same-TU ctor body to show the rest of the diff. The
  real source of the `or` is open (maybe a different helper in SetUpRegions); do not land a `|` ctor.
- Residue 3: counter slot / register rotation in parts 2 and 3 (edi/ebx swapped, part-3 layer counter in the dead argument
  slot [esp+40h] in retail); likely follows residue 1.

## Not attempted: groundwork
- 0x00A7AF60 CNavQuadTree::LoadFromFile(CAFile&, long mapIndex, const C2DBoxI&, CASuspendableProcess&), 2503 B, `and esp,-8`
  frame, no EH frame (TU without /EHsc). MapIndex = mapIndex; MapPos = ((float)box.X, (float)box.Y) (fild, stored at +7/+0xB);
  CCachedFile(0x4B000) 0x00A5B590 at esp+0x74, OpenFromFile 0x00A5B210, CAFile::ReadVar<int> 0x00516180, CCachedFile::Read
  (ptr, n, 0) 0x00A5B370 x17; version < 5 -> ~CCachedFile 0x00826380, return false; MapWidth / MapHeight / region count, then
  Regions.push_back(CNavRegion(MapIndex, i + 1)) (inline ctor with xor, copy 0x00A83110, clear 0x00A80A20); action points:
  reserve 0x00A80980, per point the throttled `if (GFGetTime() - last >= [0x0129DD30]) process.Update()` (vslot 4), read 12
  bytes, Position += MapPos, push_back (overflow 0x00517BA0); a local map<long, CANavQuadTreeNode*> (0x18-byte header) seeded
  with insert_unique(pair(0, &BlockedNode)) 0x00A81A70; NavigationLayers.resize(n, CNavigationLayer()) (__copy 0x00A85870,
  destroy 0x00A85930, _M_fill_insert 0x00A85CC0, temp layer destroyed via 0x00A83420 / 0x00A827D0); per layer Nodes.resize(
  (long)(MapHeight*k * MapWidth*k)) (k = [0x0129DD98]; memmove erase / _M_fill_insert 0x00A81890); node records (version >= 6
  adds a byte): blocked -> BlockedNode (NavLayer byte 0x013D2D39), CNavQuadTreeNode 0x00A76580, CNavNavigableLeafNode
  0x00A772D0, CNavSwitchableLeafNode 0x00A79A30 + SetAsBlocked 0x00A78800; SwitchableNodes push (0x00A82480) + sort 0x00A841F0;
  tail _Rb_tree::_M_erase 0x00A7E340, ~CWideString, CCachedFile::Close 0x00A5AFA0. Needs the CCachedFile, map and node-ctor
  models first; same TU as IsAreaClear / SetUpRegions (reuse the overlay).
- 0x00821A80 CMap::LoadFromFile(CAFile&, CASuspendableProcess*), 2648 B, /GS cookie (no EH frame). Entry: 0x0044C6B0, `new
  (0x10822)` header with two CFileLargeThemePalette 0x0050EE40 (+0x16, +0x841E), CCachedFile(0x4B000) + OpenFromFile,
  CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ReadFromFile 0x0051A240, HeightMap->Clear 0x008F6050,
  NoChangedHeightAreas / NoChangedGameAreas / SeaLevel = 0, SetPosition 0x00A5B020, GFGetTime, CFileFormatMapHeader::
  ReadFromFile 0x0051CBB0, ThingUIDCount from the header (+5/+9), InitialisePassabilityFromThemes = (header +0x8416 < 1),
  flag 0x013B8636 -> ThingUIDCount = 1; `_stricmp(palette name, 0x012442B4)`: either a 256-entry ThemePaletteEntryList from
  the CEngineThemeDef defs (GetNumberOfDefsOfType 0x009AD3B0) or per palette entry GetDefGlobalIndexFromName 0x009AD410 /
  GetDef 0x0042B144 / GetDef<CEngineThemeDef> 0x0063F3A0 with CDefPointer releases; then the theme index vector<long>
  (overflow 0x00440990, GetPDefFromInstantiationName 0x009ADA40, GetDefClassIndexFromGlobalIndex 0x009ACCA0) and the cell loop
  over HeightMapSize (+1 each way when header byte +0x15): CFileFormatHeightMapCell reader 0x008275B0, height *2048 into the
  12-byte height cells (bitfields at +9 merged with `shl ; xor ; and ; xor`), the 7-byte game-map cells at quarter resolution.
  Ego_r donor 0x0067CFA0 (3712 B, Dinkumware).

## Header overlay (hdr/; hdrcheck.txt: default=same for every changed file)
- engine/CANavQuadTreeNode.h FABLE_RETAIL_CANAVQUADTREENODE: built on the pending chunk 721 round 4 branch (merge with it; same
  macro), plus slots 9..15 (IsSwitchable, IsSwitchableOn, protected GetLeafNodeAt, MakeRegion, AddNavigableNodeNeighbouring
  SwitchableNodeToRegion, RemoveRegionZero, GetMemoryUsage) and the inline GetSize / GetAreaCovering; the branch now defines
  FABLE_RETAIL_C2DBOXF and includes fable_containers.h.
- engine/CNavBlockedQuadTreeNode.h: chunk 721 round 4's branch, copied unchanged (the parked SetUpRegions needs the type).
- engine/CNavQuadTree.h FABLE_RETAIL_CNAVQUADTREE (new branch): retail class at STLport sizes (0x88), nested CNavigationLayer
  (0x28), CNavigationTransitionPoint, static BlockedNode, the Ego_r methods used here.
- New: engine/C2DLineF.h, engine/CNavRegion.h, engine/CNavigationPosition.h.
- stlport/fable_containers.h FABLE_RETAIL_STLP_LIST: `_List_iterator_base::_M_decr`, `_List_iterator::operator--`,
  list::back() (STLport 4.6 _list.h).
- Header debt: the landed CNavQuadTree-TU rows (ctor 0x00A7A760 with MapPos at +8, IsAreaBlockedByLines on a
  `CNavQuadTree_Methods` wrapper, CNavRegion ctor) still use local models; they can move onto the new branch.

## Siblings (SIBLINGS.tsv)
37 landed rows rechecked with `vland --recheck-landed` on the final full mirror: the 4 FABLE_RETAIL_STLP_LIST users, every
includer of CNavQuadTree.h / CANavQuadTreeNode.h / CNavBlockedQuadTreeNode.h (default path) and a sample of fable_containers.h
users: 9 MATCH + 28 RELOCATION_MATCH, all PASS. The later include-order fix only adds a guarded #define inside the
FABLE_RETAIL_CNAVQUADTREE branch, which no landed row defines.

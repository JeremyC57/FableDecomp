# 0x00509A20 CWorldMap::GetMapNavigationAreaUpdateInfo -- B24S1 (2026-11-24)
## State
- differ/ draft (Codex pass 11, 3188v3120) does NOT compile on today's tree: engine/CTCGraphicAppearance.h (now
  NGraphicAppearance_CTCGraphicAppearance.h), engine/CThingFilter_OverlappingArea.h (no repo header; hand855 overlay model for row
  0x00642AA0 + local models in landed 0x006437D0 / 0x00511710 / 0x00511A10) and stlport/navigation_algorithms.h are missing; its defines
  FABLE_RETAIL_NAVIGATION_UPDATE / C2DVECTOR_UI / FABLE_STLPORT_HTTP / _CONSTRUCTORS / _UI_ACTIONS / QUEST_COMPLETION exist nowhere.
  fg.py on it = TIMEOUT (cl hangs on the missing include). Needs a full port.
## FableWin (/Od, 0x01C8F390) facts
- UID: `GFGetMapThingUIDFromThingUID(p_thing->GetUniqueID())` (FableWin 0x01C903B0, ?GFGetMapThingUIDFromThingUID@@YI_K_K@Z, __fastcall,
  returns uid with the high dword & 0xFF), right after the function in FableWin = same source file; retail inlines it (and eax,0FFh at 0x00509B90).
- `GetPhysicsTC()->HasPhysicsMeshObject()` (not GetPhysicsMeshInfo), `area.GetWidth()` / `GetHeight()` for the TopologyWeights resize,
  `C2DVector(const C3DVector&)` at the action-point push, `name == ACTION_POINT` evaluated before C3DVector / CRightHandedSet ctors.
- prior: hand719 round4 (3220v3120): retail CALLS the CSwitchableBlockage copy ctor 0x005199E0 in the push_back growth path; frame 0x128 v 0x12C.
## B24S1 port log (grades b1..b12, all /Gr)
- b1..b7 SRC_FAIL while porting (each fixed by a repo opt-in or an hdr/ overlay, see hdr/README.md); b8 first compile 3240v3120 h145.
- Repo opt-ins used: FABLE_RETAIL_CCHARSTRING(+_SUBSTRING), C2DVECTOR(+FROM3D, TOC2DCOORDI), CSTRINGPARSER, C2DBOXI_GETWIDTH/GETHEIGHT,
  STLP_BACK_INSERTER, STLP_ORIGINAL_COPY_DISPATCH_PASS22, STLP_COPY_NOINLINE, STLP_COPY_GENERIC_INLINE, CTCGRAPHICAPPEARANCE_DUMMYOBJECT(NAMES),
  CWORLDMAP(+_HEIGHTAREA), FILTER_BIND3_FUNCTION, CTHINGSEARCHTOOLS, MAP_CACHED_VOLUMES_PASS26, CNAVQUADTREE, CNAVIGATIONPOSITION_CTORS.
  World is CWorld& in CWorldMap.h (World.PeekThingSearchTools()); GetAllThings / ClipAndConvert: header declarations (retail calls both).
- b9 (+inline quest-filter ctor, out-of-line blockage copy) 3080 h181; b10 (+FableWin spellings: GFGetMapThingUIDFromThingUID inline
  __fastcall helper, HasPhysicsMeshObject, GetWidth/GetHeight) = b9 bytes; b11 (implicit blockage copy, inline std::copy) 3272 h152;
  b12 (draft's local __copy specialisation dropped; header noinline __copy) = b11 -> differ/.
- calldiff.sh (row dir): call sequence of the last obj v retail. b12: identical except the push_back overflow path (retail: __uninitialized_copy
  0x0051A810 then CALL copy ctor 0x005199E0; ours expands the copy = malloc + _M_insert_dispatch).
- Next: (1) the overflow-path copy ctor split (inline budget inside the expanded _M_insert_overflow; try /d2inlT diagnostics, then the
  header form of _M_insert_overflow / _Construct); (2) frame +20 and the prologue's extra zeroed dword (ours zeroes [esp+24h] and [esp+28h],
  retail only the list node slot): compare the locals (Ego_d/FableWin.pdb local records) with the draft's; (3) the draft still defines
  local explicit specialisations of __uninitialized_copy x2 and list::_M_insert_dispatch under auto_inline(off): replace with header
  opt-ins (pick-any check needed either way).
- Prologue reading (b11): ours zeroes [esp+24h] AND [esp+28h] then stores the list sentinel node to [esp+28h]; retail zeroes [esp+24h] and
  stores the node to the SAME slot => our std::list<const CThingPhysical*> local is 8 bytes (an extra word before _M_node), retail's 4.
  First thing to check next: sizeof(std::list<const CThingPhysical*>) under this TU's STLP opt-ins (an un-EBO'd empty allocator base in
  _List_base / _STLP_alloc_proxy would do it) -- likely part of the +20 frame.
- Size probe (probe_sizeof.cpp via vt.py, this TU's opt-ins): list<const CThingPhysical*> = 4, CFilter_Bind3<...> = 12, OverlappingArea 4,
  HasNavigationInformation 1, IsInQuestOrNullQuest 4 => the list is NOT the extra word. Corrected reading: ours zeroes the list slot AND a
  second word ([esp+28h] before the push) that retail never initialises there -- a zero-initialised local hoisted to the prologue
  (candidates: action_point_index / index_into_trans_points / a loop counter); find it in b11's listing (last_objd.txt) next.

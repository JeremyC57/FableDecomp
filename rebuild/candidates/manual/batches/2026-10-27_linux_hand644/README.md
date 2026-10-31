# Chunk 644 -- 1 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 644` (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0, no new
bindings): CBusyResourceManager::OnPostSwapScreens 0x00A618D0 (catalog "Deque_Process_And_Rebalance" is wrong; Ego_r name set, and the
PDB's function order matches retail: ctor 0x00A61880, this row, ClearAllQueries 0x00A61A80, dtor 0x00A61B10) -- STLport 4.6 `list::size()`
written as `distance(begin(), end())` with the input-iterator `__distance` that copies the first iterator cleared the edi/ebx swap in the
move block on the first grade; the placeholder device slots were replaced by the real d3d9.h method names (CreateQuery is slot 118). The
D3D interfaces are modelled locally because rebuild/include has no shared D3D header (header debt). Nine rows stay DIFFER in
`pending/2026-10-27_linux_hand644/differ/` (007751a0 CTCShop::SetShopKeeper ParentThing/bits register swap -- the bitset reference
operator[] gives retail's registers but leaves a dead `add eax,34h` --, 0057aeb4 CTCHeroStats::GetFavouriteWeapon 421v427 def kept in edi,
006e94d0 CCombatManager::IsCreatureInAttackRingSector frame 0x1c v 0x24 second find key slot, 009a3460 ReadTextFileIntoArray resize temp
colouring, 006aeea0 IsCreatureFollowingHeroAcrossRegions scratch rotation around the GetDefName hidden pointer, 00b4b8b0
CalcLocalWorldToBaseShadowSpace and 00b9d730 IsInsideFustrum x87 order, 00b81820 CEngineSimulationManager::FrameStart one store among
the pushes, 00a10560 WordWrap_FindNextLineA `cmp eax,esi` zero register). Lessons: retry any STLport list edi/ebx tie with the distance()
form of size() first; reading a bitset through its reference operator[] decides the reloaded pointer's register, a bool HasTC written with
`/ %` costs 17 bytes; same-file stand-in GetDef/GetSubDef bodies remove the def-slot escape and fix the frame; an explicit
`CCharString("X", -1)` temporary makes the object chain load first. 7 NAME_FIXES + 5 review rows queued (prefix c644):
IsCreatureValidFollower const/const CThingAICreature&, SetStockState(ESTOCK_STATE), IsPermanentlyStocked const, IsChild (landed
`@Hero_CheckChild@4`), CThing::GetDefName (three class variants wrong), NGameText::CDataBank::GetTextBySymbol const returning const
CWideString, GDefStringTable 0x013CA828 (four descriptive spellings bound). Labels: ghidra_out/labels_chunk644_2026-10-27.tsv.

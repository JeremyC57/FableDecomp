# Chunk 711 (NA054) -- 10 of 10 byte-exact (RELOCATION_MATCH), no header changes (2026-11-02)

Rows (`verify.log`: WINS 10, all PASS; two STLport rows under `/Gr` like the rest of their TU): ~CTCFamilyMember 0x00778D20 (implicit; ??_G in
slot 0 of vtable 0x0126AADC; the label said CTCInventoryItem::OnDie), ~CTCFlashOnHit 0x007E3420, the STLport byte twins
`__uninitialized_fill_n<CTextureMorphMap*,...>(..., const __false_type&)` 0x0086D090 and the three-argument `uninitialized_fill_n` front end
0x0086D570 (each named from its own caller), the private CEngineLandscapeRenderer::SetupBackgroundStates 0x00B671A0 (label "UpdateLighting"),
NScript::CV_FisticuffsClubScript::SetCompletedThisClub 0x00E827A0 (switch; table checked), the `_beginthread` start routine
CFileInstaller::BeginDecompressionThread 0x009C4420, the `dynamic atexit destructor for 'desc'` thunk `_$E2` 0x0122A940 of GetSectionDesc,
LHNetworkLib::LSocketServer::CloseSocket 0x0090AA10 and CCharString::Insert 0x0099F3D0 (block-scope buffer: 44v42 at function scope).

Second spellings at landing: the landed CEngineLandscapeRenderer::Render 0x00B6B0B0 called 0x00B671A0 by the stand-in `UpdateLighting`
(public); respelled in this commit to the private SetupBackgroundStates (Ego_r), binding updated, recheck RELOCATION_MATCH + PASS
(respell_recheck.log). Queued PRIORITY (NAME_FIXES_QUEUE.tsv, c711): the LHNetworkLib namespace for the landed LSocket rows (0x0090B8E0,
0x008F8C70), CCharString::MakeOwnCopyOfDataOrEmptyString 0x0099EB40 (two stand-ins), the GetSectionDesc static `desc` 0x013CAA54 and the
landed GetSectionDesc's access / return type, the ??_G stand-ins of 0x007E3420 and 0x0086D570, and the global 0x01436E9C (eight bound
spellings, none from a PDB; Ego_r's CEngine::AddEngineComponent<CEngineLightingManager>(..., CEngineLightingManager*&) suggests a CEngine
member slot -- left for the naming pass). The agent's 20 NAME_FIXES rows are queued as c711; labels in ghidra_out/labels_chunk711_2026-10-27.tsv.
Uncatalogued functions found: 0x00B66EB0 CEngineLightingManager::SetEnableFadedShadowFactor (out of line, no callers) and 0x009C5870
CFileInstaller::BeginThread.

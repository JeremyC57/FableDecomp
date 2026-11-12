# chunkQ25 notes

Matched (RELOCATION_MATCH, VC7.1 + g++ tests PASS, strict-header regrade clean):
00489d40 CPlayer::CreateCharacter, 00847520 CCreatureAction_PerformExpressionExtended::FinishAction, 004b5ee0
CQuestManager::SaveGameState, 007fc210 CTCKraken::CreateTentacles, 009658b0 CAIStateGroup_RecoverStealableItem::Update.

DIFFER: 00bf0b20 CEnginePrimitiveRenderUtil::SetupOutlineStencilStates -- DIFFER(1290v1290). The only residue is 2 instructions of
prologue scheduling: retail stores the first colour byte G (`mov [esp+1Dh],al`) and loads the outline-glow pointer before
`push esi`, then loads A; ours loads A first. The slot and the remaining bytes match. Best source (ported onto the shared
CRenderStateManager / CShaderRenderManager / CEngineMeshEffect / CEngineScreenEffectOutlineGlow headers, no local models, no bare
`__forceinline`) is in differ/00bf0b20.cpp. 15 grades; the residue did not change for any spelling tried.

Landing requirements:
- Merge the hdr/ overlays first (hdr/README.md lists each opt-in). 00bf0b20 also needs CEnginePrimitiveRenderUtil.h's
  _OUTLINESTENCILSTATES opt-in if it is landed later.
- 00489d40 defines CPlayer::GetPControlledCreature (0x00487DC0, already landed) out of line in its TU, with its real body
  (`return ControlledCreature.Get();`). That is a callee's real body, not a stand-in, and the match depends on it.
- Local models with no shared engine header (only _quarantine/ or none): CPlayerDef and CThingRespawnInfo (00489d40),
  CTCKrakenTentacle and CCarrySlotDef (007fc210), CTCStealableItemLocation (009658b0). Each carries its Ego_r.pdb members.
  None matches the strict-header regex, because each is either a derived class (`: public`) or has no engine header.
- Rows 00847520..00bf0b20 were graded with an overlay dir lacking fable_string.h (see hdr/README.md); 00489d40 with the full hdr/.

Names:
- 0x01436E3C is NEngineInternal::GPScreenEffectOutlineGlow (Anniversary PDB public symbol + the CEngine::Initialise store of a
  CEngineScreenEffectOutlineGlow). Two landed rows spell it g_pWaterRenderer / g_pEngineScreenEffectOutlineGlow (NAME_FIXES.tsv).
- 0x0066AD90 is CTCSoundPlayer::ReleaseAttachedLoopingSound; two landed rows spell it StopSample_0066ad90 / StopSound (NAME_FIXES.tsv).
- 0x007120D0 (landed as DestroyMembers / `std::_Dest_val<...>`) is an ICF-folded derived-action destructor (destroys a
  CIntelligentPointer at +0xa8, then ~CCreatureActionBase). 0x009658B0 calls it as ~CCreatureAction_PickUpGenericBox (implicit),
  but other actions with the same shape may fold onto it, so no NAME_FIXES row: the coordinator should pick the canonical name.
- Corrections to the drafts (not landed spellings): 0x006925C0 GetFramesRunning() (the draft had GetEnvironmentFrames); 0x007FC210
  calls IsEditorActive (the draft had IsLoading); 0x009658B0 builds PickUpGenericBox 0x00843700 / DropGenericBox 0x00842150.

Helper scripts: g.sh now sends sbs.py's stderr to /dev/null (a BrokenPipe traceback appeared once under `| head`); the coordinator
already patched sbs.py for MATCH output.

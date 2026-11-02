# Chunk 705 (NA048 + four repairs) -- 10 of 10 new rows byte-exact, all four landed DIFFER / TCC_FAIL rows repaired (2026-11-02)

New rows (`verify.log`: WINS 10, MATCH / RELOCATION_MATCH + PASS; bindings multi-check 0; no asm, volatile, flags beyond optimize("s")):
NCameraModes::CScriptedControl::Update 0x0087BBC0 (MATCH; slot 1), the byte twins CCreatureAction_LeapLong::FinishAction 0x008B74A0 /
CCreatureAction_WhisperSommersaultForward::FinishAction 0x008C0B10 (each from its own vtable; `if (PCreature->HasTC(77)) RemoveTC(77)`, 77 =
CTCSolidToThing), NCreatureMode::CModeUnderground::OnDeactivate 0x008D6710, NCreatureMode::CModeDragonFireBeam::OnDeactivate 0x008DBB00, the
CAIStateGroup twins CAIStateGroup_MinionSitOnSeatBase::Update 0x0090F740 (protected) / CAIStateGroup_BirdFlyToFood::Update 0x00913F00, CSoundManagerLH::
StopAllSounds 0x00A3EBC0 (MATCH; descriptive -- slot 16 of the CSoundManagerLH vtable, the sound module is in neither PDB; the "CNavigatorManager" label
was wrong), CNavQuadTreeNode::RemoveNeighbours 0x00A75E00 (MATCH; protected virtual slot 19) and CEngine::GetEditorAnimation 0x00B23CB0.

Repairs (`repair_land.log`: WINS 4; the old sources and tests removed, bindings of the four addresses dropped, the new sources landed): 0x00692230 /
0x006922C0 (the NCreatureAction::CActionEventExplosion / CActionEventRemoveObject ctors; the landed sources had no `// Leaf:` line, and since 84edb11e4
engine/CBaseClass.h's virtual dtor makes the TU also emit a 40-byte ??_G, which a leaf-less payload grades instead of the 30-byte ctor (lane_tools/recheck.py's
leaf fallback grades them clean) -- re-landed with Leaf lines on the real Ego_r CActionEventBase members), 0x004D9861 (??1CFireheartMinigameDef@@UAE@XZ, the implicit def dtor; the only caller is ??_G
0x004D9845, slot 0 of vtable 0x0123E584, RTTI CFireheartMinigameDef : CSubDefClassBase; shared def headers and optimize("s"), was an invented
CDefPointeeBase_Derived class) and 0x009E1440 (STLport std::vector<CTVertexRHWColSpecTex1>::erase(T*, T*), MATCH; the July-era test included the
source by path and never compiled; "Render2DDrawList::CopyBlock" was a descriptive label). The old 0x009E1440 source is a bootstrap fixture
(rebuild/build_bootstrap.ps1): fixmove moved it to rebuild/integration/bootstrap_asm/src/.

17 labels applied (`ghidra_out/labels_chunk705_2026-10-27.tsv`); NAME_FIXES.tsv 18 rows queued c705 for the naming pass after matching: CThing::RemoveTC
0x004C9840 (12+ invented spellings, incl. CBaseObject::OnPropertyChanged in 0x008C0B40), the CAIStateGroup members AIAction_RandomFlyToPos /
GetRandomFlightPos / MoveToSeatAndSitDown / AIAction_DoNothing / Terminate / PeekWorld, CModeStandBase::OnDeactivate, CEngineResourceManager::
SetupEditorAnimation, CMeshDataBank::PeekAnimation, KillBeamEffect, CSoundEmitterLH::Shutdown virtual (UAE), and 0x004D9861's leftover caller binding
?CFireheartMinigameDef_Destruct@@YIXPAX@Z. Decisions (coordinator): the CSoundManagerLH +0x4C list modelled as list<CSoundEmitterLH*> in this row only
(the landed ~_List_base 0x00A3FF60 keeps its spelling until the naming pass); 0x01436E98 = g_pResourceManager (the queued established form); the
CSoundManagerLH sibling slots go to the naming pass as one family. Header debt (NOTES.md): CClass.h, CNavQuadTreeNode.h, CANavQuadTreeNode.h,
CEngine.h, CEngineResourceManager.h, CMeshDataBank.h, CWorld.h, CAIStateGroupBase.h, CModeStandBase.h, CCreatureAction_LeapLong.h flatten the vptr.

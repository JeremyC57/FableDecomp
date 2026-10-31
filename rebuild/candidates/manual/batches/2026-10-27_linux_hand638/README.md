# Chunk 638 -- 2 of 10 landed (2026-10-31), plus a four-sibling respell pass

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 638` (vland --land 2/2 RELOCATION_MATCH + PASS), then a full
recheck of the two matches and the four respelled siblings: 6/6 RELOCATION_MATCH, multi-check 0.
- CGameScriptInterface::GetAllThingsWithDefName 0x008A8890 -- the landed sibling 0x008A9C40's search stand-in takes the filter's
  address, so the filter is stored before `&found` is formed as in retail; then respelled to the PDB at no byte change (real
  std::vector<CScriptThing> / std::vector<CThing*> specialisations, const template GetAllThings<CThing, vector<CThing*>,
  CThingFilter_HasDefGlobalIndex>, CThing::GetScriptThing() const, CDefinitionManager::GetDefGlobalIndexFromName const, engine/CWorld.h,
  CGameScriptInterface.h, CCountedPointer.h).
- CWorldMap::Update 0x004FF920 (virtual; the old leaf matched no symbol, so the old grade was of the wrong function) -- the callback
  temporary `&CThingPhysical_SetAsMoving` passed per call and a noinline same-TU body for the ForEachInRoughArea template let VC7.1
  hoist the callback store into the loop preheader. Local CWorldMap model: engine/CWorldMap.h flattens the vtable.
Respell pass (one address, one name): the landed siblings 0x008A8570, 0x008A86C0, 0x008A89D0, 0x008A9C40 carried CScriptThingVector /
CThingVector / CThing_Retail / CWorld_Retail / CThingSearchTools stand-ins for 0x008ADBA0, 0x004C7E50, 0x008ACD30, 0x008AD5F0,
0x009AD410, 0x0044C6B0; all four were rewritten to the PDB names (STLport vectors, const template search members with explicit
`template <>` stand-in bodies, shared headers) and stayed byte-exact; 7 binding rows replaced (`respell/bindings_new.tsv`), new rows
recorded for reserve, the GetAllThings / GetAllThingsInRoughArea instances, the CThing sort and 0x0052B270. vland's remaining
RELOC_NAME warnings are label-DB debt (0x008ADBA0 and 0x008AD950 carry propagated wrong labels). 0x008A86C0 keeps a pre-existing
`#pragma pack(push,1)` (purity debt, queued). Eight rows stay DIFFER in `pending/2026-10-27_linux_hand638/differ/` (006c06d0
GetRentMoneyPos x87 operand order, 00a112f0 CMatrix3x4::TransformPlane frame 0x24 vs 0x1c slot sharing, 007175d0
CActionSummonerLightningAttackTarget::Update ebx/ebp tie, 007729a0 CalcIsStallLike + 006061b0 CTCInventoryMap::FrameUpdate edi/esi
tie, 006d8830 CTCHeroExperience::AddSuccessfulMultistrikeHit fst/push order, 00a1abf0 IsNewDestinationGoingToCauseRestart compare vs
fadd order, 00773950 IsOpeningHours key lea eax/ecx tie). Lessons: same-TU template body hoists a preheader store; a search stand-in
must take the filter's address; `WARN leaf matches no symbol` = wrong virtual/const spelling; explicit std::vector<T, std::allocator<T> >
specialisations give STLport names at no cost; shared headers clear vland warnings at no cost. 17 NAME_FIXES + 7 review rows (c638).
Labels: ghidra_out/labels_chunk638_2026-10-27.tsv.

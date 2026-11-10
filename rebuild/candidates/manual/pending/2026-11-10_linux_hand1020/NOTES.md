# chunk 1020 (agent1020) notes

**Overlay:** every grade (ATTEMPTS.log, gall.sh) ran with `FABLE_HDR_OVERLAY=$A/hdr` exported; see hdr/README.md for each opt-in.
Default paths of all overlay headers are unchanged. **Probes:** `work/p.sh` compile-only probes (our disassembly only, no retail
comparison, logged in PROBES.log) were used between grades and are not counted as grades; c2prio runs likewise.
**gall.sh note:** 006dc6a0 is a split row (447 bytes, SPLITS.tsv). gall.sh / q.py grade it against the uncut 709-byte catalog row and print
DIFFER(447v709); against the 447-byte cut (`work/gr.py --cut 447`, and finish.py's oracle) it is RELOCATION_MATCH.

## Matched (src/ + test/)
- 0x009E81E0 `CPersistTraits<C2DVector>::TransferIn` -- Ego_r name (catalog CBrainBehaviour wrong). Callees: CStringParser
  PeekNextItemType / ReadAsSymbol / ReadAsString, CPersistTraits<float>::TransferIn (body visible, noinline), CSymbolMap::FindSymbol
  0x009E5170, CPersistContext::PersistError, CCharString concatenation.
- 0x006DC6A0 `CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn` (split 447) -- Ego_r + FableWin; only caller 0x006DD580.
- 0x005D8978 `CPersistContext::TransferMap<long,unsigned long>` -- size TU, map<unsigned long,long>.

## Parked (differ/), with what each needs next
- 0x0047042A CSoundMap::TransferBinaryIn 716v716: one SIB order (retail `[eax+ecx+4]`). Needs a spelling where the scaled index is the
  first operand at the CDefString element address; seven spellings inert. Callees: resize 0x004706F6 (STLport), Sort 0x00470CD8 /
  0x0047119A, CDefString::TransferBinaryIn 0x0044FC00, ReadULONG / ReadSLONG.
- 0x005C0A98 CPersistTraits<NInventory::CItem>::TransferBinaryOut 368v369 (split 369): retail pushes the conversion temporary's address
  before evaluating CGameDefinitionManager::Get() (0x0044C6B0) / GetDefNameFromGlobalIndex; the class specialisation is row-local
  (header debt: a shared CPersistTraits<NInventory::CItem> needs a home -- NInventory_CTCInventoryBase.h).
- 0x0067E290 CDegradableDef::Transfer 273v273 (split 273): expression-temporary eax/ecx/edx rotation in the GraphicType save arm
  (c2prio --rotation); bool traits as the PDB class specialisation (BOOL_CLASS) kept.
- 0x006E94D0 CCombatManager::IsCreatureInAttackRingSector 423v426: frame-slot share of the two GetUniqueID key temporaries (retail one
  slot) and the unfolded `add eax,18h ; lea ebx,[eax+40h]` for the attacked info's CombatWheel. The local-model draft got both with a
  node-returning find whose result is `->value` (+0x18); the shared map's const_iterator `->second` folds. Next: try a node-pointer
  spelling inside the GetCombatantInfo opt-in, or c2prio on the key temporaries. Same-TU helper bodies (GetRing, GetRingRadius,
  GetAttackerProngVector, GetProngSeparationHalfAngle) are in the row, as in the landed neighbours.
- 0x00BCF4F0 CEngineInternalPrimitive2DAnimatedMesh::Initialise 374v374: x87 reassociation in the Scale block only. Header debt:
  retail base size 0x128 v Ego_r 0x12C (which member is shorter is unproven; SIZE128 opt-in puts it in the opaque RenderInfo tail).
- 0x00A229E0 CRightHandedSet::CRightHandedSet(const C3DAngles&) 691v689: frame + x87 order of the axis-angle block; overload settled as
  ??0CRightHandedSet@@QAE@ABVC3DAngles@@@Z (Ego_r donor order, retail ctor callers pass a C3DAngles).
- 0x006668A0, 0x00681E60: NOT PORTED this chunk (no grades used); differ/ holds the prior drafts with their prior grade lines. They still
  need the shared-header port (00681E60: COpinionPersonalityDef / COpinionOfHeroDef headers; 006668A0: CThingCreatureBase combat-anim
  members).

## Names
NAME_FIXES.tsv lists landed spellings proven wrong (CSymbolMap::Lookup -> FindSymbol, GetAttackerProngPosition ->
GetAttackerProngVector, CProng::GetRotatedPosition -> GetProngVector, the CVectorMap Sort labels, invented resize / find stand-ins).
No made-up names in src/ or differ/ (the ported rows).

# Chunk 467 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A word-by-word copy straight from the argument pointer (no `lea` destination pointer, no esi temporary) needs a memberwise user copy ctor on the small value class (`C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}`, likewise C2DCoordI). The implicit bitwise copy forms a lea pointer, or sinks the last word's store below a later argument push (0x0076DE20, 0x00BEB490).
- An inline intelligent-pointer member built from a raw pointer is `template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { CIntelligentPointer(T* p) { SetPItem(p); } };`. This gives the out-of-line base ctor 0xA01B10, the derived vtable store, then SetPItem 0xA01B90. Without /EHsc there is no EH frame (0x006C5410).
- Retail CWorld runs 0xC to 0x10 bytes below Ego_r.pdb past +0xAC (12-byte STLport vectors and CArrays): CreatureGenerationDisabledGroups is +0xD0 (header +0xDC), SaveGameMarkerPos +0x141 and SaveGameMarkerAngleXY +0x150 (header +0x151 / +0x160). Offsets up to Paused +0xA4 agree (0x0049D930, 0x0049E590).
- In the packed CWorld, SaveGameMarkerPos is a 15-byte slot: a 1-aligned C3DVector at +0x141, 3 pad bytes, then the float at +0x150. Without the pad the fstp lands at +0x14D (0x0049E590).
- CWorld::GetCurrentRenderCamera(const CInterpolationInfo&) takes `Paused ? info.PausedCurrent.WFInterpolate : info.Current.WFInterpolate`. CInterpolationInfo is six 8-byte CInterpolationInfoSet {GTPredictedTimeSinceLastRenderFrame, WFInterpolate} plus a long; +0x04 is Current and +0x14 is PausedCurrent (0x0049DA00).
- Two CWorld methods dispatch through the CGameEventDispatch table 0x013B9288: ProcessEvent calls ActionFunction (+0x00) and ReceiveLocalEvent calls LocalActionFunction (+0x10). The landed form (a forward-declared CWorld_Methods pointer to member) matches both (0x0049D980).
- A leaf that names a `struct` parameter while the PDB uses `class` falls back to grading the largest function. One more case this chunk: `ABVC3DVector@@` in SetSaveGameMarkerPos needs `class C3DVector`. Check with objdump every time (0x0049E590).
- GameCalls slot 379 (+0x5EC) is PauseAllNonScriptedEntities(bool), already named in the landed CDF_ShineLight::Main. A VPAD256..VPAD1 macro ladder (256+64+32+16+8+2+1) declares the 379 slots ahead of it in a few lines (0x00CBE09A).

Review at landing:
- Local models next to existing engine headers. Each header models the vptr as a data member, so the class's virtuals cannot be declared on it:
  - CCarryableDef (0x004DD924)
  - CWADFile (0x00A39D20, 0x00A39D80)
  - CWaterPatchMesh (0x00BEAB20, 0x00BEB490)
  - CWorld (0x0049DA00, 0x0049DA60)
- Local models where the CWorld.h offsets disagree with retail: 0x0049D930 and 0x0049E590. The header is generated from Ego_r.pdb and needs a retail reconciliation past +0xAC.
- An already-landed source is misnamed: `rebuild/src/compiled/00/49/CWorld_ProcessEvent_0049e1d0.cpp` is CWorld::ReceiveLocalEvent, not ProcessEvent. The PDB order puts it between PeekThingManager and GetChecksum, and it calls LocalActionFunction. 0x0049D980 here is the real ProcessEvent, and both sources define `CWorld_Methods::ProcessEvent`. Rename the landed one before both sit in the tree.
- Catalog rows labelled with the wrong class (see NOTES.md Label corrections): five "CWaterPatchMesh" ??_G rows, two "CComponent::SetParentColour", "CLandscapeBackgroundSubNodeSplitBranch" ??_G, "CWStringParser::Init", the second "ReadAsInteger" (really ReadAsFloat), "CWeaponDrawState::CalcBlockDamageReduction", and three "CWeaponTrailEntry".
- Two int3-after-noreturn rows (0x00A0D0E0, 0x00A0D140) sit in differ/ with passing tests. Land them against the N+1 span.
- No row needed more than 3 grades (0x0049E590, 0x00BEB490 took 3).

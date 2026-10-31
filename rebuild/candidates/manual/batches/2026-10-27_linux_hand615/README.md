# Chunk 615 (twelfth parked-row chunk from the sweep ranking, 10 same-length rows 414-481 bytes): 4 of 10 landed

Landed (vland --land 4/4 RELOCATION_MATCH + PASS, multi-check 0): 0x00849BE0 CCreatureAction_NavToPos::FrameUpdate (full PDB
respell -- GetTC<T>, GetPos, CPlayer& GetPlayer(), protected virtual base FrameUpdate -- fixed the ebp/ebx choice), 0x0088AE40
GFSerialisePointer (PeekWorld / PeekDefinitionManager accessors, Transfer<unsigned __int64>, const RemapUIDForSerialisation),
0x008CAD50 CCreatureAction_SummonerLightningOrbInto::FrameUpdate (dot-product term order fixes the cross product's operand order;
GetPhysicsTC accessor) and 0x00867010 CCombatAction_GenericResponseToDecapitation::Initialise (the parked "SetSpin(m_spin)" was
ModifyHealth(-GetMaxHealth(), true); NCreatureMode::CModeKnockedDown(CThing&, const CCharString&) over
CTCCreatureModeManager::CModeBase(CThing&); copying NextFilter out of a block takes the frame from 0xc to 8). Parked 6
(pending/.../differ/): 00a24d70 BuildSharedAnimation 39 -> 7 lines (Bank loaded before Index ahead of GetEntryDataType), 00a11f20
434v428 with the proven callee (inline matrix-inverse x87 schedule), 008cdd40 frame 0xd0 v 0xd4, 00680050 one swapped pair,
006a9690 lea before the angle reload, 00b17410 descriptor dtor placement. Lesson: PDB respell comes first -- it won 4 of 4; wrong
stand-in call shapes change the compiled code, not just the symbol. 31 NAME_FIXES + 5 review rows queued (prefix c615: landed
0x00740820 / 0x0073EAC0 mode ctors, 0x007239C0 SetRHSet, 0x00A229E0 const C3DAngles&, GetPItem CBaseObject*,
CCreatureActionBase::GetWorld 0x00692940, CTCHaste::UpdateActionSpeedMultiplier 0x00800020, CResourceList::ShrinkResources).

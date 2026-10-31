# Chunk 612 (tenth parked-row chunk from the sweep ranking, 10 same-length rows 303-337 bytes): 3 of 10 landed

Landed (vland --land 3/3 RELOCATION_MATCH + PASS, multi-check 0): 0x0093C2F0 CAIStateGroup_StealthSeekEnemy::IsRunnable (PDB
symbols; PeekTC as the HasTC early-return form; a same-file stand-in body for the const LowerBound cuts the frame to 8; the
GetCreature() accessor on the AreThingsEnemies call only shifts the eax/ecx/edx rotation into place), 0x009852C0
CAIStateGroup_OpinionReactionRoot::FollowUpdate (a named `EMovementType movement = run ? 5 : 3;` local declared before `reached`)
and 0x007D9D70 std::vector<KnickKnack>::_M_fill_insert (same-file noinline STLport bodies for __uninitialized_copy, __copy_backward
and fill). Parked 7 (pending/.../differ/): 00970df0 frame 0x58 v 0x50 (retail reuses a vector slot for the CFilter_Bind2 temporary;
a user ctor on the empty CThingFilter_CanBeUsedAsCover removed a stray zero store), 00c71f80 Pitch fstp between the reloads,
0058301a one store a call early, 0085c7d0 vptr/argument registers swapped on the slot-70 call, 00760a70 pop esi one x87 instruction
off, 00aa39a0 row-3 integer store before the fstp pair, 00c3db80 swapped vtable-load registers. 20 NAME_FIXES + 5 review rows queued
(prefix c612: 0x007F6170 is CTCTargetingAI::IsThingValidTarget (landed IsThingAttackable), GetPos, C2DBoxF::ToC2DBoxI,
NCameraModes::CBase::AddSubComponent, CTCPerceiveThing::GetPerceivedThing, CTCEnemy::AreThingsEnemies, CIntelligentPointer<CThing>
ctor / operator=).

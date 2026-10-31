# Chunk 606 (third parked-row chunk from the sweep ranking, 10 same-length rows 181-197 bytes): 1 of 10 landed

Landed (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0): 0x009713E0 CAIStateGroup_CreatureUseCoverBase::DuckBehindCover
(protected, takes bool: the PDB inline accessor chain GetWorld().GetCombatManager().GetCombatDialogueManager().GetPDialogueDef()->Hiding
on the shared engine/CDefPointer.h replaces the invented stand-ins, 18 -> 0 diff lines; a named EAIActionReturn local compared after
the temporary dies; `== 2 ? true : false` for the bare sete). Parked 9 (pending/.../differ/), four closer: 00c1c330 20 -> 2 lines
(named float += locals; result.y store after the last fstp), 00c40590 12 -> 8 (operand order of two front.X products), 0052eaf0
4 lines with every callee respelled, 008098b0 / 007fa160 one fstp placed after the next push (LTCG scheduling gap: the 4035 QFE
compiler differs the same way), 0084cd10 forward-Z copy early, 00732d40 env pointer register + one fxch, 00bca6b0 push esi
placement, 00774c60 register colouring (now on a real STLport vector). 13 NAME_FIXES + 5 review rows queued (prefix c606: four
landed bodies misspell their own symbol -- GetUIScale const, CAIStateGroupBase::GetWorld (46 wrong bindings), GetThingCompareMultiplier
private const, CEngineSubPrimitiveBase::operator new -- and nine caller-binding corrections). Catalog label fixes: 0x00732D40
CTCWeatherVane::FrameUpdate, 0x007FA160 std::__push_heap, 0x0052EAF0, 0x00C1C330 disproved.

# Chunk 618 (fifteenth parked-row chunk from the sweep ranking, 10 same-length rows 622-719 bytes): 2 of 10 landed

Landed (vland --land 2/2 RELOCATION_MATCH + PASS, multi-check 0): 0x0071BA60 CTCHeroMorph::GetMorphStrength (case 9 written as
`float difference; return GFLimitReturn(difference = GetMorphStrength((EHeroMorphType)8) - Fatness, 0.0f, 1.0f);` places the
temporaries and the shared switch slot; vland confirmed the jump table) and 0x00849700
CCreatureAction_StealThingFromShop::PerformAction (small steps on the parked source: the landed sibling 0x00849380's CreateThingAt
shape, the CThingObject::Create PDB spelling, named component/manager locals, ternary GetInterface on the non-const path and if-form
on the const path; full rewrites onto the sibling's model grew the body to 711-725). Parked 8 (pending/.../differ/), three closer:
006668a0 GetPossibleCombatAnimations 81 -> 4 lines (same-TU noinline GetSubDef<CAppearanceDef> body; DEFAULT fallback pointer in the
name slot), 009644f0 76 -> 56 (PCurrentAction on engine/CCountedPointer.h; this/&PStealableItem ebx/ebp), 005c71aa 12 -> 10 (same-TU
noinline vector::resize makes the early return skip the vector free as retail does); 007993f0 hidden-return CDefPointer slot,
00542780 FrameWidth/FrameHeight load order, 0047f30f / 00576c4f / 00576ed1 the shared `push ecx` / `fsubp` ordering gap (not caused
by optimize("s")). 14 NAME_FIXES + 5 review rows queued (prefix c618: IsSpaceInInventoryForItem 0x005BF2ED and GetMaxNumberOfItems
0x005D9050 proven from PDB order + spacing; sibling 0x00849380 misspells CThingObject::Create and GetMaxNumberOfItems; PeekComponent
0x004C7A70 const reference form; WitnessedCrime takes const COpinionDeed&; four catalog labels wrong).

# Parked-row retry sweep, wave 2 section 3 (2026-10-30): 32 of 353 parked rows landed, 99 improved

Section 3 = retail 0x007820E0-0x0088B0C0 (engine range). The first agent was killed by the 20:51 container restart with its files
intact; a resumed agent attempted every remaining row (all 353 WORKLIST rows have a STATUS line, no early stop). Landed 32, all
PASS (2 MATCH + 30 RELOCATION_MATCH per vland; the agent's own count was 5 MATCH + 27 RELOCATION_MATCH), all plain C++, no
retail_asm row. Three more byte-matching bodies (0x007D97E0, 0x007D9A00, 0x008068B0: Dinkumware iter_swap / helper
instantiations over pair<C3DVector,long>) are held in `../../pending/2026-10-30_parked_retry2_sec3/name_review/` because their
names are not proven — queued, not landed. Two PENDING_HEADER rows (0x007CF7E0, 0x00802D10) match only with a `CDefPointer(ENullDef)`
ctor that engine/CDefPointer.h lacks (migration wave 10). 99 improved sources are in `../../pending/.../differ/` (three are
retail_asm-classified helpers kept as improvements), including first full drafts of the five skeleton parks 0x0079F970,
0x00848020, 0x0087CFE0, 0x0082AFA0, 0x0087D590 (1.0-1.5 KB each, 10-370 differing lines left); 217 held with residues in STATUS.tsv.
Levers (wins): true-extent grading per BOUNDS_FIX.tsv 7 (0x0083B3E0 240, 0x00806220 81, 0x0085AE20 59, 0x007A8BE0 576 ending at its
jump table, 0x0085ED90 36; 0x00871A80 547 improved only); as-is regrade of never-graded pass-8 parks 5; out-parameter
`bool GetInterface(T**, long)` instead of HasTC+GetTC 4; PDB inline accessors (GetParentThing, GetTCGraphicAppearance, GetPhysicsTC) 4;
shared engine/CDefPointer.h / CCountedPointer.h 6; landed CrossProduct `(float)(double)` shape 3; Dinkumware iter_swap over
pair<C3DVector,long> 4; TU-sibling STLport list/map model 3; inline `operator CCharString()` 2; ternary GetInterface store /
same-TU LowerBound / HasTC bool 3; FastInvSqrt + `operator*=` Normalise 1; vector ctor operators + inline reciprocal 1; goto shared
tail 1. See NOTES.md for the per-row table and LESSONS.md for the reusable shapes.
NAME_FIXES.tsv: 18 rows (protected/private access letters on GetCamTools and PlayAnimation, CAnimInit::Init, CTCShapeManager::GetBestTrack,
CTCSolidToThing::RemoveThingNotSolidTo, CWideString::Sprintf, CTCEnemy::AreThingsEnemies static __fastcall, CCRC::Calc,
CSpecialEffectsDef::GetEffectFromString, CDefClassBase::GetSubDef<CSpecialEffectsDef>, CTCHeroOpinionDeedLog::PeekPDeedEffectsDef, the
three slot-34 DrawGetThingDisplayInformation bodies, CInitBaseClass::IsValid, CThingPhysical::GetHealthPercentage, the const
CPlayerManager::GetPlayer, CThing::PeekPlayerManager) + 6 coordinator review rows in the queue (name_review trio; 0x004365B0 vs
0x0040F020 LowerBound duplicate copies; 0x004C7A70 GetDisplayEngine vs PeekComponent; g_helperFocal 0x013BAC48 three spellings; respell
the landed slot-34 twins 0x0082D1D0 / 0x006A9D10; the CDefPointer(ENullDef) header gap).
Review notes: 0x0083ECF0 / 0x0083F420 landed with same-TU stand-in bodies for callees (only the leaf is graded); respell the landed
twins 0x0082D1D0 / 0x006A9D10 → DrawGetThingDisplayInformation; callers of 0x0099A330 / 0x006A4E40 / 0x00449910 / 0x004C79F0 carry the
wrong spellings listed in NAME_FIXES.tsv.
Coordinator: vland --land 32/32 (one RELOC_CONFLICT warning on 0x007B2D50: LowerBound 0x004365B0 vs 0x0040F020, byte-identical
bodies, queued), bindings_multi_check 0, no RELOC_IDENTITY.

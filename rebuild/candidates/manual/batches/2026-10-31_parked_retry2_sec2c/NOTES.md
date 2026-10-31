# Section 2c (wave 2 parked-row retry) -- notes

Scope: the 85 held section-2 rows of WORKLIST.tsv (retail 0x00647575-0x007805D0) that section 2b never reached.
Every row has a STATUS.tsv line (address, status, grades tried, lever, residue). Grader: VC7.1 RTM 3077 through q3.py.

## Per-lever wins (match or residue reduction)
| lever | matched | improved | rows |
|---|---|---|---|
| inline const accessor for a member used as a call argument (`GetParentThing()` instead of `ParentThing`): moves the argument load after the component load | 1 | 1 | 006cf3b0 (RELOCATION_MATCH), 006d00a0 |
| inline-asm FPU ceiling helper (retail_asm grade, landed 0x0079C5F0 / 0x004FA9A0 precedent) + PDB member layout | 1 (retail_asm) | 1 | 00747da0 (RELOCATION_MATCH, retail_asm/), 0077a430 |
| shared def model (`FABLE_RETAIL_DEF_CLASS_BASE` + engine/CDefClassBase.h + proposed `CDefPointer(ENullDef)` ctor) with same-TU GetSubDef<T> real bodies | 2 pending header | 4 | 00659c7e, 0071d3b0 (PENDING_HEADER: RELOCATION_MATCH with the ctor), 006668a0, 006c6990, 0074c130, 0074b2b0 |
| same-TU noinline real callee body / mod-ref stand-in (parked "LTCG" labels retested) | - | 3 | 006fda20, 0077dc40, 007070d0 (held) |
| named locals / named copies / named const& (operand order, register choice) | - | 8 | 006a64d0, 006bfcd0, 006afe60, 0066c7b0, 0075e270, 0075aaf0, 00726980, 0072f750 |
| inline bool getter (bool argument pushed through a frame byte) | - | 1 | 0069e2e0 (length now 237v237) |
| STLport in-place iterator / templated GetTC<T> | - | 2 | 006c6e80, 00688360 |
| PDB signature / in-place matrix temporaries / structural fill-in of parked drafts | - | 5 | 007055d0, 00759f80, 0064d404, 00727d00, 006e0230 |
| post-increment inside the name expression | - | 1 | 00761c00 |
| head draft from the retail decode (skeleton / triage-only rows: first blocks written from the disassembly, branch targets aligned) | - | 13 | 0064a9c8, 00694890, 006a40d0, 00649952, 00647575, 0064ccb3, 00647ab4, 0064822a, 0064b8b3, 0064e2cc, 00700ae0, 00701010, 0077d5b0 |

Totals (85 rows, all attempted): 1 MATCHED (src/006cf3b0.cpp, payload.json), 1 MATCHED_RETAIL_ASM (retail_asm/00747da0.cpp,
retail_asm_payload.json), 2 PENDING_HEADER (hdr/), 39 IMPROVED (differ/; 13 of them head drafts of skeleton rows), 42 HELD
with the levers tried named in STATUS.tsv.

## Skeleton rows (parked as empty bodies)
The 13 skeleton/triage rows (1.1-1.9 KB HUD draws, opinion maths, hero-centre update) got head drafts only: the first one or
two blocks written from the retail decode, so later passes start from correct class offsets, callee names and branch
structure. 006e5a00 (CScriptConversation::FrameUpdate) is the best next target: its parked "lane gap" is the same
CeilToLong FPU block (0x006E5E43, 0x006E5F4B) that made 00747da0 a retail_asm match.

## Descriptive names used (not in the PDB; for review)
- CRegionDef +0x9c..+0x9e flags, CEnemyDef FactionIndex (+0x28), CDrawQuestInfo fields, CPlayerGuiDef +0x30c/+0x578/+0x580
  members (earlier rows of this section).
- CTCCameraPoint_3F (TC 0x3F) Param40/44/48/50, Flag4F (0069e2e0), CTCPhysicsBase SetPhysicsMesh/SetInteractionFlags slots
  (006b05a0), CCameraModeBase::AllowsLookAround slot 4 (00697cc0) -- all from the parked drafts, unchanged.
- Inline parent accessor `CTCBase::GetParentThing() const` (006cf3b0/006d00a0): inline, no symbol; the spelling follows the
  majority of landed rows (18 GetParentThing v 6 GetThing); no PDB evidence for either.

## Name evidence gathered
- 0x00487DC0 = `CPlayer::GetPControlledCreature() const` (Ego_r ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ,
  65 reloc bindings); the catalog label `CPlayer::GetHeroCreature` and the parked `CPlayer::GetHeroThing` are not PDB spellings.
- 0x006AEBA0 = `CTCRegionFollower::RemoveFollower(CThing&)` (Ego_r ?RemoveFollower@CTCRegionFollower@@QAEXAAVCThing@@@Z; catalog
  CTCRegionFollower::RemoveFollower); the parked `RemoveRegionFollower` is not a PDB name.
- 0x006D5180 = `CTCFollowed::RemoveFollower(CThing&)` (Ego_r takes a reference; parked used a pointer).
- 0x0049D870 = `CWorld::GetFrame` (Ego_r ?GetFrame@CWorld@@SIJXZ); parked spellings GetWorldFrame / GetCurrentFrame are wrong
  (still present in the parked drafts of 006c6e80, 00683770, 006d00a0 -- respell at landing).
- 0x007055D0 signature per Ego_r ?GetCarriedPositionInfo@CTCCarryable@@QBE_NAAVC3DVector@@AAVCRightHandedSet@@_NM@Z.

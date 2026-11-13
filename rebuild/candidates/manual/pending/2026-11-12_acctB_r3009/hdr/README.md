# Chunk 2027 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2027/hdr`)

Every change to an existing header is an additive opt-in: with none of the new macros defined each copied file preprocesses to its
source text (CWillButtonHelperTimer.h is the pending hand-back 2026-11-11_linux_handH40j's version, whose STATE_INLINE block is
itself an opt-in). Two headers are new. Only engine/CQ_OrchardFarmRaidScript.h is needed by the matched row 0x00DCD350; the rest
serve the parked row 0x007C6E50. Landing note: re-copy HEAD first and re-apply the blocks.

## New headers
| header | content | evidence |
|---|---|---|
| engine/CQ_OrchardFarmRaidScript.h (row 0x00DCD350) | `NScript::CQ_OrchardFarmRaidScript : CLevelScriptBase<CQ_SunnyvaleMasterData>` (needs FABLE_RETAIL_NSCRIPT): the Ego_r.pdb members at retail offsets (PDB - 0x14, - 0x18 past the 12-byte CrateList), sizeof 0x10c; nested `ERaidComment` (enumerators unproven), `CCrateTeamManager` (Ego_r 64 bytes: MemberCount +0x18, TeamReinforcementsTimer +0x1c ...), forward declarations of the seven entity classes; opt-in FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN: `CTeamSpawn : CEntityScriptBase` (ParentClass pointer +0x14 .. BanditsLeftID +0x3c, sizeof 0x40) with the Ego_r / FableWin method set | struct_layouts_egor.tsv (CQ_OrchardFarmRaidScript 292, CCrateTeamManager 64, CTeamSpawn 64); Main 0x00DCD350 reads +0x50 / +0x52 / +0x78 / +0x7c / Teams +0x8c (shl 6, +0x18 / +0x1c), stores +0x64; the landed MakeTeamMemberComment 0x00DCDA80 (+0x4c / +0x84), Main 0x00DCC770 (+0x78 / +0x8c), CTeamSpawn::Init 0x00DCD1D0 / ctor 0x00DCD160 agree; vtable 0x012DB2D4 slots 1 Main / 2 Init / 3 GetParentScript. MissionFailed's PDB type is `<unnamed-type-MissionFailed>` (4 bytes, no enumerator names): modelled as `long` (header debt) |
| engine/CSpecialAbilitiesTurncoatSpellDef.h (row 0x007C6E50) | `CSpecialAbilitiesTurncoatSpellDef : CParentDefClassBase` with the five Ego_r vector<float> members at retail offsets StaminaCostPerSec +0x3c .. TurncoatRange +0x60 .. TurncoatXPUpEveryNSecs +0x6c (12-byte STLport vectors, sibling CSpecialAbilities*Def layout) | struct_layouts_egor.tsv (0x90, 16-byte vectors after a 0x40 base); FrameUpdate reads TurncoatRange at +0x60 (0x007C6F7E); landed SetPTarget 0x007C6940 reads TurncoatTime at +0x54 |

## Opt-ins added to copies of repo headers
| header | opt-in | change | evidence |
|---|---|---|---|
| engine/CTCTurncoatSpell.h | FABLE_RETAIL_CTCTURNCOATSPELL | the polymorphic retail class on CTCBase (vtable 0x0126E5C4 overrides, Ego_r / FableWin method set incl. private Start/StopTurncoatingEffects / FinalCleanup), members at Ego_r order with the 0x34 retail timer: PDef +0x40 .. XPFrameCounter +0x94, sizeof 0x98; the generated flat layout stays the default | rtti-vtables.tsv; both PDB name sets; FrameUpdate reads every member offset; landed Start/StopTurncoatingEffects, SetPTarget, CancelMove agree |
| engine/CWillButtonHelperTimer.h (H40j copy) | FABLE_RETAIL_CWILLBUTTONHELPERTIMER_SETSTATE_INLINE | inline `SetCurrentButtonHeldState(EButtonHeldState)` storing CurrentState | FableWin.pdb 0x0271B8C0 (COMDAT right after the spell's FrameUpdate), no Ego_r / Fable.exe copy; retail stores `mov [esi+20h],1` in place (0x007C6E91 / 0x007C7528) |
| engine/CTCSpecialAbilities.h | FABLE_RETAIL_CTCSPECIALABILITIES_TURNCOAT (inside the FULL branch) | StatGetInterfaceType + inline UnLockAbilitiyMutex (unless CANCELFORCUTSCENE already declares them) and the static __fastcall `FadeOrKillEffect(CThing*)` | FableWin.pdb ?UnLockAbilitiyMutex@CTCSpecialAbilities@@QAEXXZ (retail stores +0x0c / +0x10 in place, 0x007C6EF4); Ego_r / FableWin ?FadeOrKillEffect@CTCSpecialAbilities@@SIXPAVCThing@@@Z = the bound spelling of 0x0072AC40 (body: null / dead test, emitter fade) |
| engine/CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_MODIFYSTAMINA (needs _STAMINA) | `ModifyStaminaBy(long)` declaration + FableWin inline `GetStamina() const` | Ego_r / FableWin ?ModifyStaminaBy@CTCHeroStats@@QAEXJ@Z (0x0057B1DB, bound); FableWin ?GetStamina@CTCHeroStats@@QBEJXZ, retail reads +0x58 in place (0x007C73A0) |
| engine/CTCTurncoat.h | FABLE_RETAIL_CTCTURNCOAT | selects the existing complete retail class branch (until now only under FABLE_RETAIL_CTCTARGETINGAI_ATTACK) without the targeting-AI pointer variants | Ego_r / FableWin records; IsTurncoated 0x007C7890, ModifyTurncoatedness 0x007C7A00, SetTurncoatOriginator 0x007C76D0 |

## Chunk 3009
No header changes: hdr/ is chunk 2027's overlay copied unchanged (chunk 3013 made no change to the headers this row uses). No rows regraded for headers.

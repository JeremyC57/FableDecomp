# Chunk Q2 notes (agentQ2, 2026-11-11)

All grades were taken with `FABLE_HDR_OVERLAY=$A/hdr` exported (tools/gg.sh, gall.sh); hdr/README.md lists every overlay change
(each behind a new opt-in, default paths untouched; one NEW class header).

## Matched (src/ + test/)
| address | identity | grades | lever |
|---|---|---|---|
| 0x0052F1B0 | `?IsZoomIndependent@CComponent@NUISystem@@UBE_NXZ` (8 bytes; the catalog row swallowed 21 more virtuals, SPLITS.tsv) | 1 | tail call of slot 101 |
| 0x007E6750 | `?FrameUpdate@CTCOracleMinigame@@UAEXXZ` (832 bytes with its jump table, SPLITS.tsv) | 4 | string literals passed straight to `const CCharString&` (implicit temporaries keep their own frame words) |
| 0x008E0A10 | `?FrameUpdate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXXZ` | 1 | shared headers only (new class header + 4 opt-ins) |
| 0x004C84B0 | `?SaveBinary@CThing@@QAEXAAVCDataOutputStream@@@Z` (catalog "CThing::Serialise" was a propagated label) | 8 | traits readers visible in the TU, a named `uid` local, one flag local transferred twice |
| 0x004C9120 | `?Save@CThing@@UAEXAAVCCharString@@@Z` | 2 | the landed SerialiseThingInfo body repeated inline, a named local for IsLevelPersistent() |

## Identities proven on the way (LABELS.tsv / NAME_FIXES.tsv)
- 0x00647308 is `?OracleSymbolSize@CDrawQuestInfo@NPlayerGui@@QAEJXZ` (landed as GetElementCount), 0x006463F0 `?OracleSymbolPopFront@..` (landed as
  CRenderTarget::DetachTarget), 0x006463E1 `?OracleSymbolPopBack@..`.
- 0x00995EB0 is `?Write@CDataOutputStream@@QAEXABVCMemoryDataOutputStream@@@Z` (landed as Write(const CByteBlock&)).
- 0x004CBD50 is `CPersistTraits<signed char>::TransferBinaryIn` (landed as <unsigned char>).
- 0x0051E2E0 is `?IsSerialisingGameState@CThingManager@@QBE_NXZ` (landed file / Leaf still say CTCInventoryBase::GetCanItemBeQuickAssigned).
- 0x004C08C0 `NGraphicAppearance::CTCGraphicAppearance::GetDummyObject`, 0x006E0880 `CTCDParticleEmitter::Create(long, const C3DVector&, bool)`,
  0x007E6450 `CTCOracleMinigame::UpdateTints` (unlanded callees, PDB spellings in LABELS.tsv).
- In retail CThing::SaveBinary writes the game-state flag twice (the level flag slot gets IsGameStatePersisted too; IsLevelPersistent is called and
  its result dropped): a retail behaviour, reproduced.

## Extents (SPLITS.tsv)
- 0x0052F1B0: 8 bytes; 0x0052F1C0..0x0052F3C0 are 21 further CSprite / CComponent virtuals (rtti-vtables.tsv has their slots; not authored: not in ROWS).
- 0x007E6750: 832 bytes; 0x007E6A90 (`mov eax,ecx ; ret`), 0x007E6AB0, 0x007E6AC0 follow.
- 0x00A53C50: 1055 bytes (to the `ret 10h` at 0x00A5406C); eight small functions follow.
- 0x004199C0: NOT a function start (interior of 0x00419880..0x00419C9A).

## Not matched / not reached
See STATUS.tsv (one line per row) and differ/ (a two-line note per row read). No row was parked after grading: every graded row matched.
Rows read but not authored, with what each needs: 0x008F61A0 (new CHeightMap.h), 0x004199C0 (wrong start; open ladder idiom), 0x00C604E0
(constructor store order: needs the nested settings structs), 0x00446EF0 (input-action class family), 0x008E20F0 / 0x008E24F0 (lightning-record
loop; the ScorpionKing rows are the template), 0x007B5E50 (size-TU, ebp frame), 0x00A53C50 / 0x00A4D330 / 0x00A4CEC0 (table-driven slerp
with fistp helpers).

## Header debt
- engine/CTCOracleMinigame.h and engine/CCreatureAction_ScorpionSummon.h keep their generated flattened default; the opt-in class
  replaces it per TU. The five landed CTCOracleMinigame rows and the landed ScorpionKing rows still use local models and could migrate.
- CThing::IsLevelLess / IsGameStatePersisted / SerialiseThingInfo(bool, ..) are repeated as `inline` definitions in the two CThing rows
  (the retail TU expands them): a shared `FABLE_RETAIL_CTHING_TU_INLINES` opt-in in engine/CThing.h would hold them once.

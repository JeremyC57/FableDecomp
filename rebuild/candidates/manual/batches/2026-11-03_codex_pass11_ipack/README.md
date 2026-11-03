# Codex pass 11: 0x005EB9E0 CTCTavernGame::FrameUpdate, using the packed intelligent-pointer branch (2026-11-03)

`verify.log`: RELOCATION_MATCH 1472/1472, test PASS. All 58 call targets and all 16 data relocations pair one-to-one with retail. The working attempts are in `probe/005eb9e0.log` (13 attempts).

## Retail packing

Retail packs CBaseIntelligentPointer / CIntelligentPointer<T> to alignment 1, but not the classes that contain them:
- CTCOwnedEntity::POwner is read at +0x0D (`lea edi,[eax+0Dh]` at 0x005EBACD).
- OwnerUID sits at +0x18, so the owner class itself is not packed.
- Ego_r has 60 of its 648 CIntelligentPointer members at offsets that are not multiples of 4.

The new opt-in branch FABLE_RETAIL_CINTELLIGENTPOINTER in engine/CIntelligentPointer.h declares both classes under `#pragma pack(push, 1)`. Per the owner's rule, engine classes retail packed take a pragma in their own header.

## Headers

- New: CTCOwnedEntity.h, CTCVillageMember.h, CTavernGameDef.h.
- Retail branches extended: CTCTavernGame, CThingAICreature, CPlayer, CPlayerGui, CSubtitleRenderer, CTCBase, CThing, CThingBaseDef, fable_game.h.
- Every changed header reads default=same.

## Includer rechecks

- 633/633 on the overlay (agent, `includer_recheck_grades.tsv`).
- 40/40 sampled on the landed headers (coordinator).

## Follow-ups

- **SetState 0x005E9B20 is not landed yet.** Its body is defined in this TU and grades RELOCATION_MATCH alone.
- **Name fixes:** `NAME_FIXES.tsv` / `SECOND_SPELLINGS.tsv` are queued PRIORITY (cx11p).
  - The correct PDB spellings now bound at 0x005E9B20 / 0x005EB080 / 0x005EA950 sit beside older wrong spellings.
  - 0x013BAE1C has three descriptive spellings.
- **Inconsistent landed model:** 0x0077F5C0 (CTCHeroCentreDoorMarker) packs the owner class in a local model. That contradicts the OwnerUID +0x18 evidence; review it.
- **Stale retail-parity records:** 0x00449990 / 0x004496B0 / 0x004499E0 are recorded MATCH in retail-parity.tsv but grade RELOCATION_MATCH on the committed headers.

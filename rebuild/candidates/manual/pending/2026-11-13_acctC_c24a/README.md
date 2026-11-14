# acctC hand-back c24a (agentC24, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x0058F9BC | CTCInventory::OpenPCPhotojournal | 2,012 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
Flags `/GS` and `#pragma optimize("s", on)`, as the landed siblings OpenPCMapMenu / OpenPCOptionsMenu / OpenPCInventory.
Needs the additive `FABLE_RETAIL_CDATEANDTIME_LESS` opt-in (`hdrdiff/`). Retail calls GetFileModifiedTime a second time and
drops the result (0x0058FB8F..A9); the row keeps that call.

Review items: landed CTCInventory::HasScreenshots (0x00581B33) calls 0x0041A600 as `GetSaveDir`; 0x0041A600 is
`NResourceDirectoryNames::GetPhotojournalDir` (GetSaveDir is 0x0041A540). Label fixes in LABELS.tsv (0x005940D5, 0x00999850).

`open/`: drafts of 0x007BFE30 UpdateCollision DIFFER(1628v1648) and 0x006823D0 FrameUpdate DIFFER(960v962) (successor agentC25).

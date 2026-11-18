# 0x00A3A410 naming evidence

The selected RTM object emits `?AddFile@CWADBank@@QAEXABVCWideString@@@Z`. Ego_r donor 0x00B93DE0 and FableWin donor 0x031E0FE0 carries this exact decorated spelling.
The Ego_d module record identifies `CWADBank::AddFile` in `lib_wad_bank.obj`. CWADBank::AddFile (debug length 1230).
Retail directly calls this member at 0x0050226E in landed 0x00502170 CWorldMap::CreateWorldLevelWad.
Retail directly calls this member at 0x00502287 in landed 0x00502170 CWorldMap::CreateWorldLevelWad.

## Shared headers

The selected source uses row opt-ins in `CBankFile.h`, `CDateAndTime.h`, `CFileInformation.h`, `CMemoryDataInputStream.h`, `CWADBank.h`, `CWADBankStateBlock.h`. See the history receipt for the exact leaf and the relocation report for operand evidence.

## Behavioral fixture

The selected body checks entry creation, changed size, changed timestamps with different bytes, and the skip when identical timestamps accompany differing bytes. Its files and streams are in-memory fixtures.

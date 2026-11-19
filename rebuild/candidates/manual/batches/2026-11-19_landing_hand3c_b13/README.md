# Landing hand3c b13 (2026-11-19): SetDirtyDiscError unification + GetMarkerFileName (0x009C33A0)

**One text everywhere.** All copies of SetDirtyDiscError now use FableWin 0x0304C9B0's text:
`CTBaseSingleton<CFileInstaller>::Get().ExitThread = CTBaseSingleton<CFileInstaller>::Get().ExitThread || on;`
with the Ego_d names `on` and `cs`.

**Respelled landed rows** (diffs in `landed_rows.diff`):
- 0x009C3530: moved off its local class onto engine/CFileInstaller.h + CCriticalSectionLock.h.
  - Its static now has the PDB name CFileInstaller::DirtyDisc.
  - It matches only with the shared header's volatile ExitThread; the source carries a Retail-volatile line for 0x009C3555.
- 0x009C37A0, 0x009C3820, 0x009C4440: the inlined `if` text replaced with FableWin's. 0x009C4440 keeps /d2inlT120.
- 0x009C38C0, 0x009C3590, 0x009C3660: parameter and local names only.
- 0x009C31E0 Pause: its local model's descriptive `DirtyDiscError` static respelled to DirtyDisc (NAME_FIXES).
  - The binding `?g_installerNoPauseWait@@3_NA` was replaced by the symbol this row emits.
  - The stale `?DirtyDiscError@CFileInstaller@@2_NA` binding was dropped.

**New row:** 0x009C33A0 GetMarkerFileName.
- Extent: 315 bytes, int3-bounded.
- Its only caller is OnWriteFinished 0x009C39C2. It is a file static, so it lands in that TU.

**Recheck:** all 9 rows RELOCATION_MATCH + PASS (`logs/recheck_good.txt`).

**Known pre-existing issue:** 0x009C3590 / 0x009C3660 fail strict HEADER_POLICY. Their landed local CAFile / CTBaseSingleton models predate this batch. hand3c has queued their shared-header ports.

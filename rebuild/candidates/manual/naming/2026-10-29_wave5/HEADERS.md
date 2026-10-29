# w5 header change: rebuild/include/fable_string.h (APPLIED in this wave's overlay, every includer rechecked)

The new header is `naming/w5/fable_string.h.w5` (diff: `fable_string.h.diff`); copy it over
`rebuild/include/fable_string.h`. CWideString gains four declarations and AllocStringData becomes protected:

| member | retail | PDB (Ego_r + FableWin) | emitted (G form, no /Zc:wchar_t) |
|---|---|---|---|
| `const wchar_t* GetAsCharArray() const;` | 0x0099AF10 | `?GetAsCharArray@CWideString@@QBEPB_WXZ` | `?GetAsCharArray@CWideString@@QBEPBGXZ` |
| `long FindCharPos(wchar_t c) const;` | 0x0099B070 | `?FindCharPos@CWideString@@QBEJ_W@Z` | `?FindCharPos@CWideString@@QBEJG@Z` |
| `void __cdecl Sprintf(const wchar_t* format, ...);` | 0x0099BA70 | `?Sprintf@CWideString@@QAAXPB_WZZ` | `?Sprintf@CWideString@@QAAXPBGZZ` |
| `void SetLength(long length);` | 0x0099BC50 | `?SetLength@CWideString@@QAEXJ@Z` | same |
| `AllocStringData` private -> protected | 0x0099B3C0 | `?AllocStringData@CWideString@@IAEPAVCWideStringData@@PB_WJ@Z` | `...@@IAEPAVCWideStringData@@PBGJ@Z` |

(SetLength is beyond the w4a HEADERS.md list: 0x0055E6B0 needs it to drop its CWideStringOperations adapter; both
PDBs list it and 0x0099BC50 is already bound to `?SetLength@CWideString@@QAEXJ@Z` by a landed row.)

## Includer recheck
`tools/includers.py fable_string.h` -> 19 headers reach fable_string.h, 1,132 catalog rows include it (source or test,
directly or through those headers; `fs_inc.rows.txt`, files `fs_inc.files.txt`; 009cdf50 is not reachable by the dry
recheck: no catalog bytes). The other grep hits under rebuild/src/compiled + rebuild/tests (~110) only mention the
header in comments.
- `runs/fs_new.*`: all 1,131 reachable rows with the new header (`OPEQ_INCL=naming/w5/inc_fs`) and repo sources.
- `runs/fs_base.*`: the same rows with the repo header (pair baseline).
- `runs/fs_head.*`: the first rows of fs_new again (SetLength was added to inc_fs while fs_new was running).
- `runs/final_new.*`: the 408 edited rows (64 of them includers) with the new header and the composed overlay.
Result (`python3 tools/cmp.py fs_base fs_new`, `fs_new fs_head`):
- 1,127 includer rows MATCH / RELOCATION_MATCH + PASS in both the baseline and the new-header run.
- 4 rows fail identically at baseline with the repo header (pre-existing, not caused by the header):
  0040d2a0 RELOCATION_MATCH FAIL, 0040d400 RELOCATION_MATCH FAIL, 004d9861 DIFFER(36v33) PASS, 009e1440 MATCH TCC_FAIL.
- fs_head (rows 1-260 with the final header, SetLength included) = fs_new verdicts and pairs exactly.
- Pair deltas: only `?AllocStringData@CWideString@@AAE...` -> `...IAE...` (0x0099B3C0) in 0099b6b0 and 0099b800.
  Two more deltas are the anonymous-namespace hash (`?A0x...`) changing between compiles (00870fc0
  GetDataFromEventString, 00925920 KrakenRoarLoop vtable); GOTCHAS says to normalise those; both rows RELOCATION_MATCH PASS.

Rows that use the new members (in this wave): 0041a430, 0055cfb0 (GetAsCharArray), 0055e6b0 (Sprintf, FindCharPos,
SetLength), 00993f00 (GetAsCharArray via fable_string.h). They fail to compile without the header change, so the header
and those four rows land together.

## Not done (needs a generated-header change)
- `engine/CDataOutputStream.h` is generated data-only (vptr + NewLine_PStringData); 0x00993F00 models CDataOutputStream
  locally to define `WriteNullTerminatedWString(const CWideString&)`. A member-function section in gen_class_headers.py
  output would let it include the shared header.

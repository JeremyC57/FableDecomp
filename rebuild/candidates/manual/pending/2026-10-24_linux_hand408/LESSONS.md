# Chunk 408 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `g.sh <addr>` does not read `// Leaf:` / `// Flags:` from the source. A /Gr or /GS row graded through it shows a false
  DIFFER, as the sort family 0x00992750..0x00993420 did. Grade with `gall.sh`, or pass the leaf and `--flags`.
- STLport map walks (0x0098E390..E510, 0x0098F2A0): retail uses the STATIC `_Rb_global<bool>::_M_increment(node)` form
  (node copied to a register, then the result stored back). The member `++it` form is 4 bytes short on ThreadMain
  (172 vs 176).
- When an STLport iterator is passed by value (0x009913D0 map::operator[] into insert_unique), it needs a user copy
  ctor. Without one, the copy is a plain dword push and the body length differs.
- 0x009914E0 CCRC::CalcFromPathname: `return CalcFromString(file.ToLower())` through an inline CalcFromString reuses
  the temporary's slot. A named `CCharString lower = file.ToLower();` gets its own slot (182 vs 186 bytes).
- Sort family (pair<long,CSoundBankRecord>): the implicit record copy ctor is inlined when copying from `*iter` /
  `T(*i)`, but an out-of-line copy (0x009922F0) is emitted when a by-value parameter is passed on. Choose the source
  form per call site to match.
- 0x009975E0 CAFile::IsForbiddenSymbolInComponent: retail is `bool forbidden = false; for (...) if (hit) { forbidden =
  true; break; } return forbidden;`, which gives the `xor bl,bl` flag register plus a tail-duplicated `mov al,1` exit.
  The early-`return true` form drops ebx (43 vs 55 bytes).
- 0x00997930 CAFile::GetFilenameFromPathname: retail inlines the wide CAFile::SplitPath body (null drive/dir branches
  folded away) even though SplitPath also exists out of line (0x00997620). In the source, define SplitPath in the class
  body and call it with `(path, 0, 0, &fname, &ext)`.
- /GS CAFile path rows (0x009974A0, 0x00997510, 0x00997620, 0x009976D0, 0x00997780, 0x00997930): plain local arrays
  (`wchar_t buf[260]`, `_MAX_DRIVE/_MAX_DIR/_MAX_FNAME/_MAX_EXT`) in declaration order reproduce the retail frame
  layout exactly. The /GS param-caching lane gap did not show up for these fastcall static helpers.
- VC7.1 lacks `std::wcsrchr` in `<cwchar>` (TCC_FAIL in a test). Use a local helper, or the global `wcsrchr`.
- VC7.1 tests: aggregate init in a for-init statement is a compile error (TCC_FAIL). Declare the iterator first.
- VC7.1 without `/Zc:wchar_t`: a source that names `wchar_t` with no CRT header fails (C2143). Include `<stddef.h>`.
- Heredoc warning for authors: an unquoted heredoc runs the backticks in comments (`ret 0xc` disappeared from a
  comment line). Quote the heredoc delimiter.

Review at landing:
- 0x009974F0 CAFile::SetCurrentPath and 0x00997510 CAFile::GetProjectPath are ALREADY LANDED (RELOCATION_MATCH) as
  rebuild/src/compiled/00/99/CAFile_SetCurrentPath_009974f0.cpp and CAFile_GetProjectPath_00997510.cpp (shared
  `fable_filesystem.h` form). The chunk sources are self-contained duplicates; keep the landed ones.
- The landed 0x00996BC0 source names the CMemoryDataOutputStream reserve forwarder (`add ecx,8 ; jmp 0x00997330`) a
  `_Cons_val` forwarder. It is `vector<char>::reserve` via Data at +8.
- The manifest has "ParserIsSeparatorNext" on both 0x00997DA0 (correct) and 0x00997E90 (actually
  ParserReadPastSeparator).
- Local models sit next to existing headers: CWideString / CCharString (rebuild/include/fable_string.h) and CAFile
  (rebuild/include/fable_filesystem.h). The chunk sources use local class models because those headers declare
  different member spellings (storage_ vs PStringData) and lack Find/SplitPath/MakePath. Consider extending the
  shared headers.
- Sound-module names (CASoundManager members, CSoundBankRecord, CSoundManagerInit) are descriptive: there is no PDB
  coverage.
- 0x009913D0 (parked) is a scheduling-only diff. Retry it on the QFE-4035 toolset.

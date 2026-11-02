# Chunk 717 header hand-back

## fable_string.h (non-engine header, `hdr/fable_string.h`)
- Change: an opt-in branch `#ifdef FABLE_RETAIL_CCHARSTRING` declares `CCharString(const char* text, long length = -1);`
  (0x0099EBF0). The `#else` branch is the existing text, unchanged, so no existing includer can change.
- Why (two sources): Ego_r.pdb and FableWin.pdb hold only one const char* constructor, `??0CCharString@@QAE@PBDJ@Z`
  (the others are the default, copy and `(CAFile&, long)` constructors), and Fable.exe has no one-argument overload.
  Retail nonetheless converts bare literals implicitly: braced CCharString arrays (0x0121E260, 0x0121E3A0), the
  `vector<CCharString>(18, "")` initializer 0x0121A460 that matches only through the implicit conversion (chunk 673
  GOTCHAS), and a `const CCharString&` bound to a literal (0x0121EF70). So the retail declaration carries the default.
- Users: chunk 717 rows 0x0121E3A0 and 0x0121EF70 (`#define FABLE_RETAIL_CCHARSTRING` before the include). With the
  stock header both are SRC_FAIL. With the overlay both are RELOCATION_MATCH. The same sources without
  FABLE_HDR_OVERLAY are SRC_FAIL, which proves the overlay copy is the one used (no `#error` needed).
- Path: opt-in only, so no includer recheck is needed. The default branch was re-diffed against the live repo copy
  at hand-back: the only difference is the opt-in block.
- At landing: copy `hdr/fable_string.h` over `rebuild/include/fable_string.h`. The landed rows that define their own
  local `CCharString(const char*, long = -1)` model (e.g. 0x0121E260, 0x0121A460) could later switch to this branch.

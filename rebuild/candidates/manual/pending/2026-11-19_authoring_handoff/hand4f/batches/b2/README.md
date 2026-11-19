# hand4f batch 2 (2026-11-19): 0x005DC340 CAnimationSet::GetAnim -- RELOCATION_MATCH + test PASS in the strict read-only vland
- FableWin /Od loop text (Ego_d names start_it / it / entry_num) on the shared headers (existing opt-ins only; hdr/ empty; no flags).
- NEEDS A DECISION: the function ends in `#ifdef _MSC_VER __assume(0); #endif` after the loop (an unreachable marker, same idiom as the
  landed engine/CPersistContext.h switch defaults). The brief's device list says "no __assume" (the acctC ban measured __assume as an
  ASSERT form). Evidence and the 15 failed alternatives are in the source header and NOTES.md.

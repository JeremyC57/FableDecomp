# NF177 lessons

- **Check a virtual claim against the vtable slot before trusting a "called directly" note.**
  - In q11918, slot 11 held a 5-byte `jmp` to the claimed address. So the claimed address was the private helper, not the override.
  - PDB pairs with identical Ego_r bodies (both 76 B, same hash) are a sign of an inlined wrapper. FableWin's debug sizes then tell the
    wrapper from the body.
- **A one-row PRIORITY entry can be a family.**
  - q12088 (one const find) pulled in a caller whose own leaf was wrong (QAE vs PDB QBE), and three more callers of that caller.
  - Probe every referencer of every address whose spelling you change, not only the entry address.
- **Pointer-to-reference respells (PBV to ABV) of a callee are byte-neutral.** The same address is pushed. Change the declaration and
  dereference at the call (`*thing`).
- **A row test that stubs a callee must follow the callee's constness.**
  - The 0x00BDC180 row passed its grade but its test failed with LINK_FAIL, because the stub kept the non-const signature.
  - Ship rows/<addr>_test.cpp with the matching macro.
- **Catalog-only entries whose binders already bind are done-already**, unless the parked UNMATCHED row carries a wrong module or name
  (q12096): then the fix goes in NAME_FIXES.tsv.
- **Inlined static helpers** (Enter/LeaveSynchronisation in q12001): spell the expanded body in the same terms the landed out-of-line rows use
  (the same global symbol). That way the change does not add a new spelling at the data address.
- **Never put `git` in a shell one-liner**, not even a read-only `status` inside a compound check. Use `cmp` or `ls` on files instead.
- **NF172 drafts in naming/<agent>/work/ are a good start.** The 0x00BE0930 draft graded MATCH unchanged; graded and verified first.
- **`ls rebuild/include/engine | grep` misses headers under engine/_quarantine/, but selfcheck sees them.** Run `find rebuild/include -name
  <Class>.h` before deciding a class has no shared header. 0x00BDBFF0 first shipped a NEWLOCAL this way.

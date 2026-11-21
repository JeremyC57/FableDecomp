# NF212 lessons (one line each)
- vland takes compiler flags only from the payload's `flags` field (mkpay copies the catalog CompilerFlags), never from a row's `// Flags:` line: patch the payload for a new per-row flag, or vland reports the no-flag DIFFER.
- A /Os frame-pointer TU on the shared STLport vector: sweep /d2inlS first (0x004333B3 matched at S34 with no forceinline macro); then drop each forceinline opt-in once and regrade, the flag alone may be enough.
- A visible noinline helper body emits a pick-any copy compiled under the row's own pragmas: under `optimize("y", off)` it differed from the owner (41 v 38). A declaration-only opt-in restores HEAD's opaque call and binds the owner's template spelling.
- `FABLE_RETAIL_STLP_FALSE_TAG_POD` is the switch for a tag temporary retail zero-fills (`xor eax,eax ; stosb`); the default tag has a user constructor and is never stored.
- Rows that modelled the vector as a header-less stand-in moved in one form each: the stand-in member becomes std::vector<CCharString> (or the stand-in token sits on it when the self leaf names it), the overflow stays declaration-only if HEAD's was, else OUTOFLINE + the owner's macro.
- Test helpers for protected STLport words: a derived struct with static accessors that form `&Derived::_M_start` (type `T* Base::*`) works on any vector object, including a member of a local class.

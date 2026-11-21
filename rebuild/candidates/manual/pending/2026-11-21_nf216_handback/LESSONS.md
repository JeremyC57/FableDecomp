# NF216 lessons (one line each)
- VC7.1 takes `#pragma optimize` for a template member's COMDAT from the END of the TU: moving a row-head optimize("s") after an include, or wrapping an explicit instantiation in optimize("t"), changes nothing for it (only non-template inline copies move).
- Prove a "same-TU copy equals its owner" route with cmpcopy on each row, not only the leaf grade: under a size-mode TU the leaf still matches while the copy stays 19 bytes.
- A visible, called CBasicString<char>::operator== body (NOINLINE_COMPARE_ZERO + COMPARE_BODY) is what lets VC7.1 keep the string data in a register across `name == "A" || name == ""`; it closed NF62's 0x004FE630 residue.
- For `set.find(x) != set.end()`, VC7.1 evaluates end() first; a named iterator for the find result reads end() after the call, as retail does.
- A landed test can be a verbatim snapshot of the row source with local models; it keeps passing after the row moves onto headers, but its snapshot marker then names the old source.
- comdiff (every function COMDAT body, relocations zeroed, HEAD vs form) is the check for TU-wide devices; symdiff only compares names.
- A non-template inline explicit specialization takes the optimize mode at its definition: between optimize("t") and ("s") it gives a size-mode TU the owner's /O2 pick-any copy (0x005A4CDD draft).

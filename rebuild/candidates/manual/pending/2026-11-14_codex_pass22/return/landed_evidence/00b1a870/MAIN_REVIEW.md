# Independent main review

1310 target bytes and14 selected called helper bodies exact;180 code/data/vtable relocations checked. Fresh VC71 /Od /W3 /GR- and GCC64 C++03 complete26880-case behavior suite passed. Main independently compiled same-header size/frame-pointer TU and performed both ordinary COMDAT link orders: size-first preserves all15 selected bodies including153-byte vectorTransfer and34-byte input; speed-first retains rejected248-byte copy. Integration REQUIRES size TU first, not only the target object. No aliases or suppressed speed copy.

Combined59-row/179-header regression preserves all emitted C++ code and relocation names, including explicitly unselected copies. Helpers do not add assigned match progress.

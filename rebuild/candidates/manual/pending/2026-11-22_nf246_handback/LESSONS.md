# NF246 lessons (one line each)
- Census the DEFs as well as the relocations: a self row that strongly defines a 62-byte `~list` clashes with every pick-any 5-byte `~list` copy in the family's shared-header rows.
- A family row with a local vector / tree model is not automatically blocked: check the shared header for the exact member (fable_map.h has erase(iterator)) and try the in-place respell (move the declared destructor from list to the local _List_base) before calling it blocked.
- A row whose leaf is only a prefix makes xref.py print junk targets: rerun it with the full mangled leaf from objdump.
- Removing an opt-in can drop it from a header's FABLE_STLP_NOINLINE trigger list: measure the other noinline members after a swap.
- A HEAD link test that supplies a callee into the row object breaks when the callee's spelling moves: rewrite it on the shared headers.

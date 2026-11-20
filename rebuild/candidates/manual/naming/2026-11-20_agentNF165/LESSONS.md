# NF165 lessons (one line each)
- An overlay copy of a header that uses directory-relative includes ("EAXNativeTypes.h") does not preprocess from hdr/ until the
  unchanged dependency headers sit beside it. Precheck with cpp and STOP on any error: cl spins under Wine on a missing include.
- With the GameCODA SDK legal, the fastest identity check for a CODA row is `llvm-nm CODAApi.lib` for the class, then compare the
  release member's length with the retail row (EAX4FXSlot 1552 = 1552, CalcParams 858 = 858) and its relocation order.
- VC7.1 /GS still emits the cookie for 16-byte-aligned `short[128]` locals, so a byte-array stand-in can become the SDK's short buffer
  without a cast.
- A bool getter `return (flags >> n) & 1;` compiles to retail's `xor eax,eax ; mov al ; shr ; and eax,1` with no normalisation, so the
  propagated `long`-returning tiny rows re-author to the PDB-style `bool ... const` leaf in one grade.
- A test that derives from a shared-header class with declared-only virtuals must define those base virtuals, or the base vtable does
  not link (LINK_FAIL with no log line).
- reloc_bindings.tsv's one-witness-per-symbol rule still gives the complete SPELLING set per address. Combine it with a source grep
  of the REFS rows for the wrong token to find every binder without compiling 50+ rows.

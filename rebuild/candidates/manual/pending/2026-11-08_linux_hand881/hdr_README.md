# hdr/ overlay (chunk 881)
No header changes. Both rows model the Intel dvec.h classes locally (M128 / I32vec4 / Is32vec4 / I16vec8 / Is16vec8), exactly as
the landed mono twins 0x00C7BAF0 / 0x00C7E420 and the landed out-of-line dvec rows 0x00C7B7D0..0x00C7B970 do (RTM VC7.1 ships no
dvec.h). A shared `dvec.h`-style header for the libpk SSE2 TU would be a landing decision; the model must keep plain-inline
members (no __forceinline): the rows' bytes depend on VC7.1 running out of inline budget on them.

# Chunk 617 -- lessons

1. **Check the catalog identity of "parked" template rows against the landed siblings first.** Two rows parked as
   GetPBestThingInRoughArea were really GetAllThingsInRoughArea instantiations. Copying the landed 0x008E3EB0 body and changing
   only the template arguments (the filter class from its operator() binding, the type mask from the cmp) matched both on the
   first grade.
2. **Explicit guard + do-while** (`long i = 0; if (i < count) { const T* p = data; do { ...; ++i; p += stride; } while (i < count); }`)
   sinks the source-pointer load below the guard like retail (0x00F3ABF0 value loop).
3. **Named float compound assignment sets the dot-product term order** (`float a = y*y'; a += z*z'; a += x*x';`, 0x006AD700).
   It does not fix the operand form of a later `v * a` product.
4. Get/Peek twin accessors: take the order from the Ego_r donor addresses (Get before Peek or the reverse) and match it to the
   retail address order. Catalog labels for these are often swapped.
5. Inert levers, don't repeat them: named pointer locals and self aliases (ebx/ebp swaps); named filter locals (0x0063E6F0); Normalise
   rewritten as `r * X` or through a component pointer (0x006A4FB0); `*this = *this * r` (+24 bytes).

## Review at landing
- 0x0096BCB0 / 0x00919780: the catalog names (GetPBestThingInRoughArea<...>) must be replaced by the GetAllThingsInRoughArea leaves
  (NAME_FIXES.tsv). The vland RELOC_NAME warnings for 0x0049C6F0 / 0x0049C700 / 0x00788FFC are catalog-label mismatches, and the
  bindings already carry the right spellings.
- Both src rows keep the landed sibling's inline class model (SHARED_HEADER_AVAILABLE CBaseClass warning, the same as the landed
  file).
- NAME_FIXES respells callers of 0x004C7AA0 (PeekWorld -> protected GetWorld IAE), 0x005BC4C4, 0x007FEF40 and the CTCCarrying
  reference forms. These touch many landed files.
- 0x0063F360: choose between ~list and ~_List_base.
- 0x00A442E0 and 0x006B13B0 names are probable, not proven (NOTES.md).

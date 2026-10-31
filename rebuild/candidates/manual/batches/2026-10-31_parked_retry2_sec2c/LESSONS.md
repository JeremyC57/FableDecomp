# Section 2c lessons (new rules)

1. A member read used as a call argument loads too early (before the TC component load) when written as the bare member;
   reading it through an inline const accessor (`GetParentThing()`) defers the load to the push. 006cf3b0 matched on this
   alone after 25 flag combinations and two parked passes; 006d00a0 lost 3 of 7 diff lines.
2. When a `bool` argument is pushed via `mov [esp+x],dl ; mov edx,[esp+x] ; push edx`, the value came out of an inline
   getter returning bool (0069e2e0: length 231->237 = retail).
3. The parked "fistp lane gap", "LTCG" and "x87 term order lane gap" labels were wrong more often than right: 0064d404 has
   no fistp at all, 006fda20/0077dc40 move with same-TU bodies, 0075e270's X-term order came from a const& position local.
4. Retail-asm FPU ceiling helper (fld/fadd 0.5/fistp/fild/fsub 1/compare) is a documented retail_asm block; reuse the
   landed 0x0079C5F0 helper text with a `// Retail-asm:` evidence line rather than chasing it in C++.
5. `#define FABLE_DEFPTR_INLINE inline` (not __forceinline) in the shared def model reproduces retail's per-site
   inline/out-of-line split of the CDefPointer members (0074b2b0).
6. `__forceinline` on a find loop is needed under the /Os-shaped TUs; a re-read of the list header in the loop condition
   matches retail's reload.
7. Declaration order alone never moved a frame slot in this section (0070e8b0, 00714370, 006a03a0, 00752f30: every
   remaining 4-byte frame difference is the allocator packing a local into a dead argument or temporary slot).
8. VC7.1 canonicalises operand order of commutative x87 multiplies written in source (006afe60: `v.Z*n.Z` and `n.Z*v.Z`
   compile identically); the operand order follows which pointer was loaded last, so change the load order instead.
9. A compiled-out type check (`xor al,al ; jne` after homing a pointer into a dead argument slot) is not reproduced by any
   inline shape tried (struct-by-value, address-taken reference, loop guard): 006af2d0, as sec2b's 006af3f0/006af4e0.
10. Same-length rows with a pure register rotation inflate ndiff counts (00713d50: 104 lines for a 2-register swap);
    read the side-by-side before judging distance.

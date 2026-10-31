# Retry wave 2 sec2b -- lessons (new rules)

1. A float difference returned directly folds to `fsubp`; `float result = a - b; return result;` keeps the second operand
   live and gives retail's `fsubr` join (006aff40, 006b0020). Try it on any one-line float-return residue.
2. Stepwise sums (`r = a; r += b; r += c;`) pin the x87 evaluation order; permute the term order to match retail's loads
   (X,Z,Y in 0075fe30). Combined with named const references for subtraction operands this matched a whole row.
3. `*it++` on a reverse_iterator needs the post-increment that returns a copy; the pre-increment form moves the load.
4. A catalog row that runs past its jump table into the next function is an over-span: grade over the true extent, record
   BOUNDS_FIX.tsv + oracle_extra.tsv, and land it (0077fde0 matched at once; its pending test already existed).
5. When an out-parameter iterator is copied into a second iterator, put the first in its own `{ }` block: VC7.1 frees its
   frame word and the frame matches (00733cf0, 30->15).
6. A small inline the lane emits out of line (counted-release dtor, HasTC) wants `__forceinline`; retail inlined it, and
   early `return 0`s on the null path then let VC7.1 drop the null test from the release (0065a41c, 53->39).
7. An unnamed tag temporary (`f(.., tag_type())`) and a named tag local land in different frame slots and change which
   argument is loaded first; try both (006e3580, 15->9).
8. A virtual that "fills a CCharString*" in the parked model but whose retail call is followed by `mov eax,[eax]` with no
   destructor is a by-value return of a 4-byte non-POD (CDefString with a user copy ctor): model it that way (006ea0c0).
9. `T* p = 0; if (cond) p = ...;` and `cond ? ... : 0` allocate differently: retail's `xor ebx,ebx` + test is the if form.
10. Check STATUS.tsv, not the head of rem.tsv, before picking the next row (three rows were re-worked by mistake; the earlier
    differ for 0074a4e0 was restored from var/0074a4e0_f.cpp).
11. Families that did not move with any lever this run: register tie-breaks (ebx/ebp, esi/edi swaps), frame-slot packing into
    dead argument slots (00699e60, 006eb700, 006b75a0), shrink-wrapped callee-saved pushes (00683b00), a member cached across
    an out-of-line call in retail but reloaded in ours (0070d780, 006c0b20), and constants held in a byte register (00761980).

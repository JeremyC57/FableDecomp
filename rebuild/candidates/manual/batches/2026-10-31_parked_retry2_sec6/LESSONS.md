# Section 6 lessons (new rules)

1. **A nested `if` drops the /EHsc frame; an early `return` keeps it.** A guard object (PKCProtectionInstance) with no
   throwing call in scope: VC7.1 removes the fs:[0] frame for `{ Guard g; if (a != b) { ... } }` but keeps it (one
   state -1 store hoisted above the branch, one dtor call) for `{ Guard g; if (a == b) return; ... }` (00c14b50/00c14bc0).
2. **Check the TU's flags against landed same-TU rows before blaming the EH frame.** A parked `// Flags: /EHsc` on a
   script TU whose landed rows carry none made the implicit dtor EH-framed and out of line (00dc3ea0/00dcc000).
3. **A catalog span that ends in a separate body is an over-span** (`ret; int3; ret`, or `ret; int3 pad; <unlisted body>`):
   fix it in BOUNDS_FIX.tsv + oracle_extra.tsv before reading the residue (00b48be0/cb0/d80, 00be37e0).
4. **Read the PDB member type before modelling a container.** `list<long>` vs `list<unsigned long>`: binding an unsigned
   argument to `const long&` makes a converted temporary, which is exactly retail's "homed in the dead arg slot, value
   kept in a register" shape (00bbce10). Invented helpers that reproduce it (a `long` parameter) are a symptom.
5. **Shared CCountedPointer.h fixes (T*) ctor tails:** its `if (data) Info = new CCPPointerInfo(...)` keeps the failed-new and
   null-data `Info = 0` tails separate (00e64d10) and removed a whole register/frame residue in 00b2c4e0. Run the header
   move on every row with a local model (tool: ccpmig.py) -- it is the cheapest win.
6. **for(;;){ if (c) break; ... }** moves the tail-merge survivor compared with `while (!c)` (00f07b80).
7. **A `const T&` view keeps self-copies and re-reads after stores** (the "volatile candidate" signature without
   volatile): read through the view, write through the pointer (00c34630; CDefPointer dtor 0122ae7e/01228b70). It is
   inert when no store intervenes (00c35dd0, 00c35290).
8. **Statement order folds a member RMW:** `total += n; m_pos += n;` gives `add [mem],reg` (00c62790).
9. **retail_asm FtoL replaces volatile FtoL stand-ins** (`fld f; fistp r` / `fld f; mov edx,out; fistp dword ptr [edx]`).
10. **Dead frame-temporary zero stores** (retail zero-fills a frame temporary after the copy that already used the
    constant) do not reproduce from memset / ctor temporaries / member-wise locals: 00be24d0, 00be2540, 00eb3f40 family.
11. **No dtor after a fill temporary means a trivially destructible element** -- use it as identity evidence (00f3fb60).
12. **VC7.1 folds an inline-ctor static initialiser into .data** when every store is constant: a retail `_$E` dynamic
    initializer of constant stores (01223f00/01223f60) needs some other source shape (unsolved).

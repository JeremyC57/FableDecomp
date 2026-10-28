# Chunk 532 lessons (GOTCHAS one-liners)

- A scoped named temp (`{ CCharString n("X"); found = f(n, ...); }`) puts the CCharString dtor before the `found` test; this is the form that matched (0x008334F0).
- A retry grid guarded by `if (!found)` around the loops, with `!found && x <= 5.0f` in each loop condition, reproduces retail's `test cl,cl ; jne` exits from the inner and outer loop (0x008334F0).
- `pos = building->GetPos()` through the C3DVector copy constructor emits fld/fstp triples; a POD struct copy emits `mov` dwords (0x008334F0).
- STLport `_Rb_tree::_M_insert` is private: spell it `AAE`, not `QAE`; the hinted `insert_unique` in the lane needs the real `_Rb_tree_iterator` `operator++` / `operator--` out of line (0x0052A2D0, 0x009EF4D0).
- An inline CAnimInit 7-argument constructor that calls `Init` 0x0070C050 on a temporary matches retail's temporary-ctor form (0x00753630).
- `extern const float EPSILON` stays unfolded (`fld [EPSILON] ; fmul [EPSILON]`); a literal float product folds to one constant (0x00753630).
- `length > EPSILON * EPSILON` compiles to `test ah,5 ; jp`; `EPSILON * EPSILON < length` gives `test ah,41h ; jne` (0x00753630).
- To keep a by-value small `pair` return on the hidden pointer (as retail does) give the pair user constructors; a POD pair of two words comes back in registers (0x00834530).
- An empty filter class with no user constructor is zero-initialised as a temporary (an extra store retail lacks); a user ctor removes it (0x00834530).
- An anonymous-namespace function's line 1 starts with the bare name, and its Leaf is the prefix `?Name@?A0x` (0x00834530, 0x0055E2C0).
- Signs of LTCG in retail that one TU cannot reach: a register held live across an out-of-line call that receives its address (0x0077DC40), and a temporary sharing a parameter's home slot (0x00834530). Park them as `lane gap: LTCG`.
- A `T& r = this->member;` reference local does not change the register allocation; VC folds it away (0x007634E0).

## Review at landing
- The crypto names in the landed `global_StdDecryptString_0055f090.cpp` / `global_StdEncryptString_0055f140.cpp` are swapped (StdEncrypt / StdDecrypt and EncryptBuffer / DecryptBuffer); see NAME_FIXES.tsv.
- `rebuild/include/engine/CThingAICreature.h`, `CTCHeroCentre.h` and `CTCLook.h` need real members so these rows can drop their local models.

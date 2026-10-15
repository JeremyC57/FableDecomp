# Lessons from hand chunk 132

1. A memberwise copy of a 44-byte POD-ish record (CCreateDeviceParameters) written as an implicit copy compiles to `rep movsd`;
   retail's field-by-field copy (bool copied as a byte, dead fields dropped) needs an inline user memberwise copy constructor.
2. A table walk `for (long i = 0; table[i].Field != -1; ++i)` with `const T& rec = table[i]` reproduces retail's cursor (VC
   strength-reduces to the record or to &Field); the pointer-cursor `for (p = table; p->Field != -1; ++p)` does not.
3. `(fov * 0.5f) * (double)TWO_PI` keeps the two multiplies (`fmul dword` + `fmul qword` of (double)float-constant); an all-float
   `fov * 0.5f * TWO_PI` folds the constants into one.
4. In size TUs, `__forceinline` is needed for: CEnginePrimitiveHandle's constructor, its Clear(), counted-pointer release/assign
   helpers, memberwise copy constructors of small vectors/boxes, and accessors used as `this` of a two-call chain.
5. CEnginePrimitiveHandle members in a constructor: the counted Data pointer must be a member object with its own inline
   constructor, else the shared zero register (`xor ebx,ebx`) is hoisted above the handle's vtable store.
6. A stat-bar alpha computed `(x)(Alpha * 255.0f)` must be an `unsigned char` parameter: with `long` the `mov ecx,<obj>` for the
   virtual call drifts to the last push.
7. A converted reference argument (`IS_THING_ALIVE<CThing*>(derivedPtr)`) keeps the local in a register and homes a temporary in
   the frame; binding the reference to the local itself forces it to memory and re-reads it.
8. Interface lookup with an OUT-OF-LINE bitset test followed by the component call: an inline `HasInterface` + a `__forceinline`
   lookup returning the component puts it straight in ecx (the out-pointer GetInterface goes through eax).
9. `if (x > 0)` on an unsigned member gives `jbe` (retail), `!= 0` gives `je`.
10. Chained status calls returning on the first negative result: nested `if (result >= 0)` with one final return matched; early
    `return result` after each call swapped the `this`/index registers.
11. A by-value CRGBColour built in the argument area (`push ecx ; mov eax,esp ; mov [eax+n],bl`) needs a user copy constructor;
    without one VC builds a frame temporary (and a frame pointer) and pushes it as a dword.
12. Two float scale fetches feeding later divisions: write both as named float locals before the divisions (dividing right after
    each fetch moves the first division between the calls).
13. Lane gaps seen here (parked): separate frame slots for two by-value call temporaries (retail) vs one shared slot (lane);
    a dead LARGE_INTEGER high-half store retail keeps; `push imm`/`lea ecx` scheduling inside runs of zero stores (24i family).

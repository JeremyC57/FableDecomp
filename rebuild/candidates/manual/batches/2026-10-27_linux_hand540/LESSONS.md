# Lessons, chunk 540 retry

1. STLport `operator[]` written as `*(begin() + n)` is not the same as `_M_start[n]`. The begin() form gives retail's
   byte-offset CSE (`shl ecx,2 ; cmp [ecx+eax],0 ; ... push [ecx+eax]`). `_M_start[n]` CSEs the whole element address
   and costs 2 bytes. This closed 005c0e81 and also fixed 005c71aa's `cmp [eax+esi*4]`.
2. Renaming a parked source to its PDB spellings did not change codegen on 6 rows (0059fc17, 005c71aa, 005cdd86,
   005d8978, 005fb140, 005c0e81). Do the naming pass first, because it costs nothing. Check that the leaf's own mangled
   name equals the PDB name: 0059fc17 is private (`AAE`), and `struct C2DVector` mangles `U` where the PDB says `V`.
3. Same-TU noinline bodies of the real callees are load-bearing. 0059fc17 jumped from NDIFF 13 to 85 when the
   GetDef/GetSubDef bodies were dropped. The parked file had invented `FindDef_a/b` bodies; the landed
   `GetDef<T>` / `GetSubDef<T>` models (real literals and manager calls) give the same codegen, so use those.
4. The size of a hidden-return temporary moves frame slots. In 005d8978, modelling `map::insert` with STLport's
   `pair<iterator,bool>` grew the frame by 4. The 4-byte iterator return keeps retail's frame size.
5. When the size matches but the bytes do not, look for a pure slot permutation. In 005cdd86, naming the first value
   and dropping the precomputed `spacing + 0x19` local brought 860v864 to 864v864. The rest is a frame-word
   permutation: declaration order, a shared variable and top-of-function declarations were all inert for it.
6. Retail can home two different traits objects in one dead argument slot (005d8978: both
   `CPersistTraits<unsigned long>` and `<long>` use `lea ecx,[ebp+0Ch]`). Separate, block-scoped or reordered locals did
   not reproduce this under RTM.
7. Harness note: difflib's `autojunk` heuristics distort the alignment of functions over about 200 lines (exp NDIFF 244
   against qd 80 on 005cdd86). Pass `autojunk=False`. Also add `ONLY=` / `SHOW=` filters, so one variant can be
   inspected without editing the variants file.
8. CDefPointer release shapes need a device. By-value results release in two steps and null the pointer
   (`add eax,4 ; dec [eax] ; cmp [eax],0 ; ... mov [x],0`); out-parameter locals release in one step
   (`dec [ecx+4] ; jne`). Explicit specializations `CDefPointer<const T>` for the by-value types reproduce both. The
   device carried over unchanged from 005c71aa to 0059fc17.

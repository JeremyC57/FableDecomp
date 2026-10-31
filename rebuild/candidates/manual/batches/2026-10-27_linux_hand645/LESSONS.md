# Chunk 645 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A named key local plus `a[k] = a[k] + 1` fixes a radix histogram's register rotation** (0x00B847D0). `++counts[(key >> s) & 0x3ff]`
  (and ++/+=/post-inc, index types, while loops) gave the index in eax reusing the memset zero, key in edx and `lea ecx ; mov edx,[ecx]`.
  `unsigned long radix = (key >> s) & 0x3ff; counts[radix] = counts[radix] + 1;` gives retail's own `xor edx,edx` index, key in eax
  and `mov ecx,[esp+eax*4+..] ; lea eax,[..] ; inc ecx`. In the scatter loop the named radix alone (`index = offsets[radix]++`)
  fixed the `lea ecx ; mov eax,[ecx] ; lea edi,[eax+1]` shape. One grade took the row from a whole-loop rotation to one xor position.
- **A pointer named inside the arm moves a split copy into the arm** (0x00870E00). With `CCharStringData* p = s.PStringData;
  if (!p) {strcmp arms} else { p->Data == "A" ...; p->Data == "B" ... }` VC loads ecx, then `mov esi,ecx` before the `jne`.
  `CBasicString<char>* str = &p->Data;` declared in the else arm puts the copy in the arm (`mov esi,ecx` after the jne), leaving only
  the load register (retail esi, copy to ecx per call). Worth trying on the twin 0x00871290.
- **A named local for a member read after a call reuses the dead `this` register** (0x004888C0): `SetupInitialPlayerMode();
  long number = Number; Character()->SetPlayerNumber(number);` loads `mov ebx,[ebx+28h] ; push ebx` like retail where the
  inline `SetPlayerNumber(Number)` used edx/eax.
- **Frame-byte packing of two bool locals follows definition vs the first load of the dead argument** (0x00BA50A0): a local defined
  after the argument's first load is eligible for that argument's slot, and the address-taken `failed` wins it over the plain `flag`
  whatever the declaration order. Retail gives the slot to `flag` with `failed`'s store late; no source order found reproduces both.
- **The x87 operand order of a dot product is canonical** (0x00872110): swapping DotProduct's arguments, reordering its terms, or
  spelling the sum out produce identical code; only materialising a vector (naming it before another local) changes it, and that
  costs frame space. Park such one-pair residues on sight.
- Confirmed ties (two or more new levers each, no movement): callee-saved swap between `this` and a multi-role web (0x004888C0,
  0x004FEEC0 even with `*(begin()+n)` and a named base pointer), scalar temp packing in a vector expression (0x0087BD70: splitting
  the sum into statements only makes the frame smaller), store/reload scheduling (0x00853670, 0x0095B820).

Review at landing:
- No row matched. Grades this wave (one lever each, parallel probes counted): 00870e00 8, 00ba50a0 6, 00b847d0 8, 004888c0 5,
  00872110 5, 0087bd70 5, 004feec0 2, 00853670 2, 0095b820 1, 00838ec0 1 (plus one reproduction grade per row). 00b847d0 and
  00870e00 kept moving and are the best candidates for another session.
- Local models next to engine headers (inherited): 00838ec0 (CThingGameObject.h flattens the vptr; CHitParameters local with
  `#pragma pack(push, 1)` -- must go before landing), 004feec0 (CWorldMap.h has 16-byte STL containers), 0095b820, 00ba50a0.
- Inherited same-TU stand-ins with bodies: 0x004FEEC0 defines a noinline `Find` (the landed __find 0x0050E540) and 0x0095B820
  defines a noinline `CThingSearchTools::GetAllThingsInRoughArea` body (0x008E3EB0) to stop argument escape -- review before landing.
- NAME_FIXES.tsv: the CHitParameters setters 0x00868590 / 0x00868CE0 / 0x008685F0 / 0x00868CA0 are bound with pointer or
  non-PDB spellings in reloc_bindings.tsv; the PDB takes references and an EDamageAttribute.

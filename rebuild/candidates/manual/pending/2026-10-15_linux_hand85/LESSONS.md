# Chunk 85 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `T* p = 0; if (bit) p = driver;` gives retail's `xor eax,eax ; test ch,2 ; je ; mov eax,[edi+5Ch]`; the ternary
  `bit ? driver : 0` gives `je ; mov ; jmp ; xor` (0x0077A130, GetTCDriver probe on CThing UsingComponents word 3 bit 9).
- A thing def at CThing +0x70 used in place (`ParentThing->PDef->GetSubDef(&PX)`) has no add-ref; a COPY into a local
  (`CDefPointer<CThingDef> d(ParentThing->PDef)`) gives `test ; inc [esi+4]` + release + tail `jmp [edx+4]`
  (0x0080C3C0 vs 0x008100A0, same as landed CTCAISummon 0x007FDE70).
- A `__fastcall` free callee with two member references and a stack argument: VC fetches the stack argument first
  (right to left), then `lea edx` / `lea ecx`; a float result divided by a member is `fdiv [esi+off]` (0x0080C210).
- Iterator loop over a 12-byte-element STLport vector with an IS_THING_ALIVE body: plain `for (it = begin(); it != end(); ++it)`
  re-reads end every trip and aligns the loop head with `lea esp,[esp]` (0x0080C410).
- `(long)(def->secs * g_fps)` with an extern long = `fild [g] ; fmul [def+off] ; call __ftol2` (0x0080C3C0, 0x007CEDE0).
- Empty-body destructors of TCs: members destroyed in reverse order at RETAIL offsets (4-byte STLport list, 12-byte vector),
  a `__forceinline` CDefPointer release (with `Object = 0`) last; `#pragma optimize("s")` for the Kraken/Explosion TUs
  (0x004DCD2B, 0x004E1437, 0x004E3EE1), default for 0x006C1BE0.
- A named local for the dividend (`unsigned long f = A(); return f / B();`) keeps the retail call order; the bare `A() / B()`
  calls the divisor first (0x00C1D850).
- Inline `HasTimedOut()` that reads the member into a local before calling GetWorldFrame reproduces `mov edi,[esi+14h] ; call ;
  add edi,1Eh` (0x007F8540).

Review at landing:
- 006817f0 needed more than 4 grades (sub vs add switch normalisation, lane gap); 0077a130, 00681790, 006820e0, 007714b0 used 4.
- Local CTCCreatureOpinionOfHero model next to the existing engine/CTCCreatureOpinionOfHero.h (PDB layout, 12-byte lists) --
  the retail 4-byte STLport lists move every member past +0x54; the header does not fit the bytes.
- Local CTCBase / CTCBase_Calls models next to engine/CTCBase.h in the destructor and ctor rows (the header holds the layout only,
  no ctor/dtor declarations).
- 00681790 and 006817c0 are byte-identical bodies both catalogued as TransformForcedOpinion (two overloads or a COMDAT twin);
  land them from one source.
- 0x004C73D0 is labelled CVertexBufferWin32::DoSizeof in the manifest but is CThing::GetPosition (several rows call it).

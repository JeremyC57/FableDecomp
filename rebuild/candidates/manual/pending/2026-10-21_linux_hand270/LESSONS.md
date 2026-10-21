# Chunk 270 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- HasTC must be a `__forceinline unsigned long` that returns the bit mask (`bits[t>>5] & (1UL << (t&31))`) to get
  `test byte/dword [x],imm` or `test ah,imm`. A `bool ... != 0` version gives `shr ; test cl,1`, and a non-forced
  version becomes a call.
- The advisor's helper for the CTCHeroExperience stat fractions (006d68d0/006d6cb0): a named `long level`, a named clamped
  `long maxLevel`, then `float f = (float)level; return f / maxLevel;`. It fixed everything except one fmul/store swap.
- An uninitialised absent arm that reads the lookup-key slot (`mov eax,[esp+10h]`) comes from either the out-param
  `T* p; GetInterface(type, &p)` idiom in size TUs, or a value-returning inline `T* r; if (bits) {...; r = ...;} return r;`.
- Two identical calls in an if/else (e.g. `x = a.GetPItem()` / `x = b.GetPItem()`) tail-merge into `lea ecx,[..]` arms
  plus one shared call. A ternary or a pointer variable gives `add eax ; mov ecx,eax` instead (007800b0).
- With a call in a later argument (`SetText(GetGameSubsystem()->Font, text, ...)`), a named `CGameSubsystem* s = ...`
  before the call puts the field load in edx after the constant pushes (retail form).
- The IS_THING_ALIVE frame temp copy (`mov [ebp-8],ebx ; lea ecx,[ebp-8]`) needs a pointer conversion: the factory returns
  `CThingObject*` and IS_THING_ALIVE takes `CThing* const&`. The same type binds the variable itself and spills it.
- A CDefPointer released through a pointee member `void Release() { if (--m_refCount == 0) Destroy(); }` gives
  `dec [ecx+4] ; jne` even when the pointer is address-taken. An inline `--Object->m_refCount` in the dtor reloads the
  pointer between `dec` and `cmp`.
- Pointer-member bitfields (`unsigned char Dead : 1; Persistent : 1;`) give `lea ecx,[esi+91h] ; mov al,[ecx] ;
  test al,1 ; ... or al,2 ; mov [ecx],al`.
- `x += y` on a member reached through a getter call gives `add eax,13Ch ; add [eax],edi`. An inline helper
  `AddExperienceSpent(long)` gives retail's `add [eax+13Ch],edi`.
- A STLport map iterator whose `operator++` calls the cdecl `_Rb_tree_increment` 0x004292C0 gives the
  `push esi ; call ; mov esi,eax ; ... pop ecx` loop.
- `delete Components[i]` on a polymorphic pointer emits the null test plus `push 1 ; call [eax]` by itself.
- Count the frame from retail when sizing a by-value helper object: CSoundInit is 0x20 bytes in retail (Flags +0x14),
  not 0x34. The wrong size made frame 0x50 v 0x3c.
- Audio-library constructors at 0xC1xxxx are speed TUs with EH: `// Flags: /EHsc`. Identity blocks are member
  sub-objects with inline ctors, stored before the first out-of-line member ctor.
- x87 C3DVector maths (007e8a40) reproduces nearly verbatim from `Cross` / `Dot` / `operator*` / `operator+` inline
  members with an up vector `C3DVector(0,0,1)`. VC folds the `*1` terms and factors `dx*0 + dy*0` into `(dy+dx)*0`.
  Getter-call order needs named `CThing* parent = ParentThing; CThing* target = ...` locals.

Review at landing:
- 006d68d0 / 006d6cb0 are parked at the advisor's v16/t16. The labels are wrong (Handedness / IsToAutoBlock).
- 0057770a / 005778ca (Experience vfunc 89/90) differ only by one esi/edi swap in the ability loop. They are good
  advisor candidates, and so is the CTCDoor Open/Close single register pick (00730c00 / 00730dd0).
- 007800b0 matches in shape. Only the frame slot packing (0x40 v 0x3c) is off.
- 00c11d00 is an unnamed audio-library constructor (not CTCHero::Initialize). It differs only in scheduling.

# Chunk 281 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Counted-pointer copies and aliasing (0x00AA79C0):
  - an inline copy ctor followed by `++Info->RefCount` makes VC7.1 re-load the copied Data after the `inc` (the local's address is taken through `this`);
  - retail's no-reload shape is an implicit (memberwise) copy `CRootPointer root = chunk.RootChunk;` plus `T* p = root.Data; if (root.Info) ++root.Info->RefCount; p->Call(...)`;
  - the same holds for the push_back copy-construct (0x00AA8830): a non-ctor `Construct()` with the Info pointer read into a local first.
- Throwing an empty exception class (0x00AA79C0): `throw E();` with a user-declared empty ctor `E() {}` copies an uninitialised byte from the dead argument slot (`mov cl,[esp+30h] ; ... mov [esp+38h],cl`); without the ctor VC zero-fills it (`xor al,al`). 4-byte id compares are intrinsic `memcmp(a, b, 4)` (`lea edx ; mov eax,[edx] ; mov ecx,OFFSET ; cmp eax,[ecx]`).
- Reading into the dead argument slot (0x00AA79C0): a 4-byte stream read through an inline `unsigned long ReadULong() { unsigned long v; Read(&v, 4); return v; }` homes the value in the dead stream-argument slot; a named local read via `Read(&local, 4)` takes a frame word.
- Inline STLport list insert at end (0x00555EB0): name the old tail first -- `prev = head->_M_prev; node->_M_prev = prev; node->_M_next = head; prev->_M_next = node; head->_M_prev = node;`. The textbook `head->_M_prev->_M_next = node` re-reads `head->_M_prev` after the node stores.
- One counted-pointer type, two dtor shapes (0x00555EB0): CCountedPointer<CAction>'s inline dtor clears both words after the release, CCountedPointer<CComponent>'s does not. Model them as separate types when one function shows both.
- Parent-model reuse: 0x00555EB0 matched in three grades from the landed CRedefinerList::Initialise 0x005566A0 (its derived class). Before modelling a catalogue row, grep `rebuild/src` for landed callers of its address.
- Filter locals store in reverse declaration order (0x0084E590, AdvB3): retail storing +0x10 before +0xc = declare the second filter first.
- Frame packing (0x0084E590): VC7.1 shares frame words between dead filter objects and later vectors/floats only when they sit in a `{}` block. Wrapping filters + rect + GetThingsInRect in a block took the frame from 0x94 to retail's 0x7c. A C3DVector copy (`C3DVector p = thing->GetPosition()`) instead of a reference gives retail's "X on the x87 stack, Y spilled to a frame word" around two `_ftol` calls.
- Call order inside an operator+ (0x0084E590): `a() + b()` evaluates b() first. Retail's GetCentre-then-GetPosition order is `GetPosition() + GetCentre()`. A by-value return used straight in the expression keeps the returned pointer in a callee-saved register.
- A dot product over a temporary difference (0x0084E590): write `(Max - Min).Dot(dir)` with an inline `Dot` returning `Z*o.Z + Y*o.Y + X*o.X`. This consumes the three differences off the x87 stack (`fmul [dir.Z] ; fxch ; fmul [dir.Y] ; faddp ...`); a named `size` vector leaves three `fstp st(0)`.
- `!(x > y)` vs `x <= y` (0x0084E590): retail's `test ah,41h ; je skip` after `fcomp [y]` is `if (!(x > y))`; `x <= y` gives `jp`.
- A same-TU stand-in can make it worse (0x0084E590): giving GetThingsInRect a same-TU noinline body let VC see its effects and shrank the function (510/573 v 593). Keep a callee extern when it is out of the TU in retail and the caller's own code already matches its shape.

Review at landing:
- These rows needed more than 4 grades:
  - 00aa79c0: 9;
  - 0084e590 (parked): 15;
  - 0084dc10 (parked): 5;
  - 00aa8830 (parked): 5;
  - 009d1320: matched by AdvB3, after my 5+ grades.
- 00555eb0 restates the landed CRedefinerList model locally (no engine header). Its CRedefinerListDef stands in for the CScrollableList def (2-packed, +0x142/+0x146 descriptive).
- 00aa79c0 / 00aa8830: rebuild/include/engine/CChunkedFileSuperChunk.h / CChunkedFileChunk.h exist, but local models were used. The functions need the inline Read, the virtual slots (ReadFromFile slot 7, Clone slot 9) and the CBaseClassNonCopyable base, which the generated headers flatten into `__vftable`.
- 009d1320's test (advisor-written) needed two g++ fixes: `#define __cdecl`, and `memset` from `sizeof(void*)` plus `#define long int` for the 64-bit host.
- Landed 0x00E2A900 declares RunCutsceneMacro 0x00CBFB7D as RunScriptedCutscene with a wrong 4th-parameter type.

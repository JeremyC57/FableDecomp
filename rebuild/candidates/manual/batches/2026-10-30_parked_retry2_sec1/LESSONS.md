# Wave 2 section 1: lessons (new rules, one line each)

- Replace a parked local CCountedPointer with the shared engine/CCountedPointer.h before anything else.
  - It can change codegen, not just names: 6 rows matched and 3 shrank.
  - The real inline bodies in a `/Os` TU still call 0x00429C15 / 0x00428928 / 0x004291DE out of line, and the component
    pointer then lands straight in esi.
  - Do it mechanically: strip the template and CCPPointerInfo, include the header, respell m_pObject → Data,
    GetPtr → Get, Clear → ReleaseData.
- A CCharString length accessor written with two returns (`if (!p) return 0; return p->Length;`) and an `unsigned long`
  result keeps retail's null diamond. A ternary, or a `long` result, lets VC7.1 thread the null path (005db5f0).
- For a vector size guard, `unsigned size() > 0` keeps the `sar`. `size()`, `!= 0` and `!empty()` fold to a test mask
  (005361d3).
- A loop that loads Last before First (`mov eax,[+44h] ; mov ebx,[+40h] ; sub eax,ebx`) with First held in a register is
  a guarded do-while with the element pointer declared inside the guard (004afc60). A for-loop with the pointer declared
  first loads First first and copies it.
- A colour taken apart through `high = colour >> 8` and stored with ebx live is the four-argument CRGBColour(r, g, b, a)
  ctor called with (high >> 8, high, colour, alpha) and assigned as one dword (00404d20). Member-by-member stores reuse eax.
- An empty allocator tag overlaid on a LIVE argument slot (`lea eax,[ebp+0Bh]` on the `__x` slot, no push-ecx frame word)
  is not a lane gap. Give the tag-taking `_List_base(const allocator&)` a same-TU noinline body that never reads the tag
  (0053cb20).
- Zero stores to pointer members that retail schedules around the next call's setup (vptr, D0, D4, D8, push, DC) come
  from member objects with an inline null ctor (CDefPointer handles), not from `void*` members in the init list (0044c6c2).
  - Check the class destructor: CDefPointer::Release on each member confirms the type.
- A same-TU stand-in body for an STLport insert_unique that leaves its arguments untouched lets VC7.1 prove the pair
  temporary unchanged, and the temporary's release disappears (00554030 probe).
  - So the escaped-local counted-pointer release (`dec [eax] ; cmp [esi],0`) needs a faithful callee body, not a stub.
- Copying a C3DVector through a user copy ctor that assigns Z, Y, X in its body gives retail's Z, Y, X x87 loads, but the
  squares then start from Y (005fa990): a half-fix, recorded here for the next try.
- `/G5`, `/G6`, `/G7`, and `/O1 /Oy-` in place of the optimize pragmas, do not move any scheduling-only residue tried
  here (0045cfad, 0047d296, 0047dd9d).
- Tooling: check that every copied grading script exports its own QTAG. gall.sh did not, so it graded in the shared
  `$S/wine/q/<addr>` work dirs.
- Rewriting every inline ternary accessor as two returns made no difference on 31 rows (batch b4/). The two-return lever matters only where the accessor feeds a null diamond retail keeps (005db5f0).

# Chunk 228 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A HasTC result bound to `const unsigned char&` inside an unchecked GetInterface template reproduces retail's frame-byte spill, and the miss arm reads the uninitialised local from that slot (00903e20, also 00903f30).
- Action ctors used inside `new` expressions (`AddAction(new CActionWait(f))`, `CCountedPointer<CAction>(new CAction)`) need `__forceinline`, or VC calls them out of line (00903ac0, 00904430, 00544c10).
- A same-TU stand-in body for STLport `__find` under `#pragma auto_inline(off)` moves the iterator-category tag into the dead argument slot and fixes the frame (004b01b0, 007621c0).
- x87 sums and products follow statement order: write `f = t*t; f *= t` for a cube (006ffb10) and `s = Z*Z; s += X*X; s += Y*Y` for a squared length (0087ba90). A three-term dot `x + y + z` loads the right inner operand first (00989540).
- `operator/(float)` implemented as a multiply by the reciprocal is CSE'd across two uses: one `fdivr 1.0` feeds both increments (008525f0).
- With implicit `operator=` and a user copy ctor, C3DVector assignments become `lea` pointer copies; value operators go through frame temporaries (0087bbf0, 008525f0).
- Reusing ONE named C3DVector local for successive temporaries packs the frame to retail's size (0087bf40: 0x18 -> retail 0x14).
- A nested success path with a single `return -1` at the end gives retail's shared failure tail (00aac1d0).
- A same-TU implicit copy ctor is inlined at its first use and emitted out of line at its second (006e61a0).
- An inline BeginsWith of the form `return p == 0 ? false : true` gives the single null test (004380a0).
- Counted-pointer release: inline the release over the block {refs, __fastcall destroy, object} and re-read the block from its frame word (0054a5d0). CDefPointer release: a null test, then the def's ReleaseTemporary on the ref base at +4 (005566a0).
- The min/max scan in CRangeCompressor::TestRangeBias (00f3a5d0) is decided by declaration order: declare the signed min first and copy the other three from it (`long minS = in[0]; long maxS = minS; unsigned long maxU = minS, minU = minS;`). With `if (min > v)` orientation this went 322v318 -> 318v318; the four if/ternary spellings compile identically.
- An equality test written on an unnamed expression, `if ((E - S) / O == -1.0f)`, keeps the quotient on the x87 stack (`fld c ; fld st(1) ; fucompp`) for the later `fabs((E - S) / O)`. A named local spills it before the test, and VC then rematerialises the fabs at its use (0056eb48).
- Branch layout follows the source: retail's `jne PLAY` with the not-playing block as fallthrough comes from `if (!Play) a = Finished ? 1.0f : 0.0f; else {...}` (0056eb48).
- A by-value CCharString argument is built in place in its slot (`push ecx ; mov ecx,esp ; push -1 ; push str ; call ctor`) only when CCharString declares a copy ctor (0x0099EC30); without one VC builds a temporary and copies it (0056eb48).
- A loop bound written `part != 3` (not `< 3`) gives `jne` (0086be40).

Review at landing:
- 0x0056E545 is landed as `CRecord_0056e545::CRecord_0056e545` (manifest "CDateAndTime") but is the CSliderAnimator ctor: same TU as ExtractParenthesis 0x56EAD6 and Update 0x56EB48, and the layout matches the PDB CSliderAnimator exactly. Rename it when landing (rebuild/src/compiled/00/56/CRecord0056e545_CRecord0056e545_0056e545.cpp).
- Catalog rows with wrong names that this chunk grades under the correct leaf: 00c55b70 (CAudioContext::Reset), 004b4e30 (SetQuestScriptPersistScript), 00c0dab0 (CString copy ctor, Leaf `??0CString@@QAE@ABV0@@Z`), 004b01b0 (CQuestManager::IsRegionValidInRegionLockingMode; parked).
- 00ca53f0 holds two functions (a 13-byte thunk plus a body at 0x00CA5400): split the catalog row.
- Local models used where engine headers exist: CScriptGameResourceObjectScriptedThing in 00903ac0 and 00903e20 (00903e20 also has a local CScriptThing; both keep the header's offsets but need retail's polymorphic shape), CScriptConversation in 006e61a0/006e5690, and CScrollingComponent in parked 00547050. 00904430, 006ff810, 006ffb10 and parked 009fc250/009049f0/00904bd0 include the engine headers. Check the local models against rebuild/include/engine/.
- 0x009DB700 CRenderManager2D::AddText: its landed caller declares the font as `CFontBank*`, but the PDB says `CFontBank&`. Same codegen, but the signature should be reconciled.
- NShape::CShape::UpdateSelectedPoint (007621c0) takes `const C3DVector&` per the PDB, not by value.
- The manifest's 0x00405F00 "CGuiControlNumber::SetSLongValue" is IntToWideString(long) (__fastcall, hidden result in ecx), as several landed sources already declare it.
- No row needed more than 4 grades. 0086be40, 00f3a5d0 and 0056eb48 each used their full 4.

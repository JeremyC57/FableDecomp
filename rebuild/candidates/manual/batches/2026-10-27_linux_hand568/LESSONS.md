# Chunk 568 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0093b1d0) A by-value-passed CCountedPointer<T> local needs same-TU noinline bodies for BOTH its T* ctor and its dtor (I-3). With either one declared-only, the local keeps a private slot for its whole lexical scope (frame +8 here). With both visible, it shares the low 12-byte slot with the TC keys, C2DVector temps and filter temporary, as in retail. Putting the local in a `{}` block instead moves it into the wrong sharing group.
- (0093b1d0) A copy-ctor diff where VC stores Info and then reloads it before the `if (Info)` test can come from a wrong frame layout. Here it vanished once the slots matched, so fix the frame before rewriting the landed `: Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }`.
- (0093b1d0) Two C2DVector/C3DVector lessons:
  - `C2DVector(a - b).Normalise()` with a PDB `const C3DVector operator-(const C3DVector&) const` gives retail's `fsub [eax+4]` / `fsub [eax]` on the GetPhysPos result.
  - `const C2DVector operator*(float) const` fixes the fxch/fmul order of `dir * GFFloatRandom(...)`.

Review at landing:
- 0093b1d0 took many research grades (well over 4), mostly frame-layout probes, before the I-3 stand-ins closed it.
- The stand-in bodies (0x008AB380 / 0x008AB3D0) call operator new/delete and reference `CCountedPointer<CModeSitBase>::DeleteData` (0x008AA810, declared only). Check that the lander's relocation identity accepts them.
- Landed 0090f830 names vtables 0x0127261C / 0x01267BBC "CAnimFilterSit" / "CModeSitListen"; RTTI says CAnimFilterAnyGroup / NCreatureMode::CModeSitFloor.
- NAME_FIXES.tsv lists wrong bound spellings for 0x004C7950, 0x00662EF0, 0x008AB9D0, 0x008ABAA0 and 0x008AA810.

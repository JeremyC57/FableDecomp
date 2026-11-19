# Landing hand3c b22 (2026-11-19): NParticleEngine::CPSCDecalRenderer::AddToBoundingSphere (0x00B1DC70)

**Result:** strict vland --land, RELOCATION_MATCH + `00b1dc70_TEST PASS` on the first grade; no flags.

**Source:** FableWin 0x02F55EB0 statement for statement, with Ego_d locals, in the idioms of the landed PreparePrimitives 0x00B1F200.
- The same-TU ScaleParticleDimensions body is visible and noinline; it MATCHes at its own address 0x00AF8F00.
- `// Retail-asm:` covers three GFUnpackFloat calls and one GFFloatToLongFloor.
- `// Inferred:` FableWin's radius mask `(Flags & 0x40) && (Flags & 0)` folds to 0.5f.

**Header:** new opt-in CPSCDecalRenderer.h ADDTOBOUNDINGSPHERE (the declaration). The header-user recheck is 7 / 7 good.

**vland warnings:** RELOC_NAME on 0x0129BA3C is DELTA_NOTIONAL_ZERO, already in b15's NAME_FIXES.

With this batch the decal family (b20-b22) is complete.

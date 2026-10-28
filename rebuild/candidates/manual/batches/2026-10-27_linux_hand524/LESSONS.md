# Chunk 524 (re-queue) -- lessons

## GOTCHAS one-liners
- STLport std::string LOCAL (0x0083EBD0 toLowerCase): with only declarations of `reserve` and `_M_throw_length_error`
  the local's address escapes and VC7.1 keeps it in memory (dead zero stores, 313v281); same-TU out-of-line bodies
  under `#pragma auto_inline(off)` (reserve = the landed 0x00777DB0 body, _M_throw_length_error = puts/abort) give
  RELOCATION_MATCH with start/finish in edi/esi.
- STLport 4.6 `basic_string()` allocates 8 in the constructor BODY (_string.c), not through `_String_base(a, 8)`.
- PDB mangling of the STLport string type: `basic_string@DU?$char_traits@D@std@@...` -- char_traits is a struct (U).
- The LHNetworkLib TU is /Gr: free functions are __fastcall without a keyword (YI in the PDB); pass `--flags /Gr`.
- Two adjacent zero bytes stored as `mov word ptr [x+0Bh],0` after a byte store = `memset(&a[1], 0, 2)` (0x00825E00).
- `a = b = 0;` stores b first (0x00826C90: MinimapZone before SoundLayer).
- 3-byte / 2-byte member array copies (`mov bp,[edx]; mov dl,[edx+2]`) = memcpy intrinsic of the array (0x00826F10).
- `CRightHandedSet(C3DVector(0,0,1), forward)` folds Up into immediate stores only when C3DVector has a USER copy
  constructor (the landed DropWeapon model); without it one temp word survives (0x008100D0: 300v280 -> 280v280).
- 0x005296B0 is `??0CThingPhysical@@QAE@K@Z`; CThingGameObject's constructor is inline (vtable 0x01244F7C after it).
- CTCBase-derived vtables: slot 6 = OnInitialActivate -- a quick identity check for "vfunc" rows (0x00816AC0).
- Tests: `unsigned long` / `long` are 8 bytes in 64-bit g++; packed-struct size checks in tests must use `int`.
- gall.sh passes everything after `// Flags:` as flags: keep that line to the flags alone.

## Review at landing
- vland: no RELOC_CONFLICT / RELOC_DUPLICATE. RELOC_NAME warnings are label corrections only (0x00692910,
  0x00661E70, 0x005DEC70, 0x00822BF0 / 0x00822CA0 / 0x00822D90, the CAFile_* labels of 0x00825E00 / 0x00826C90 /
  0x00826F10); see NOTES "Label corrections".
- 0x008265C0 and 0x00826F10 call the parked 0x00825E00 / 0x00826C90 by their full PDB spellings; 0x008271E0 calls
  0x00826F10 (landing in this batch).
- NAME_FIXES.tsv (44 rows): the important ones touch many landed callers -- 0x0049D870 CWorld::GetFrame (~50
  spellings), 0x006644F0 SetCurrentAction, 0x00693EF0 ~CCreatureActionBase, 0x006924B0 GetRandomAnimFromName,
  0x00693B30 CCreatureActionBase ctor (AAV), 0x005296B0 CThingPhysical(unsigned long), 0x008388D0
  CThingGameObject::ConstructFromParams (IAE, reference), 0x006869D0 CTCBase::GetDefinitionManager, the landed
  placeholder 0x00810970 (AddThingToIgnore), 0x00777DB0 reserve / 0x0083EBD0 toLowerCase (template string spelling).
- Follow-up row: 0x0083ECF0 LHNetworkLib::toUpperCase is the byte twin of 0x0083EBD0 (toupper 0x00BFED16); the same
  source with toupper should land.
- 0x008082C0's symbol carries a TU-specific anonymous-namespace hash: bind it from the built object, not by hand.

## Why the earlier attempts stalled (likely; inferred from what this run needed)
- The catalog labels pointed at the wrong classes (CAFile_*, CParticleMorphs_*, CString_ToLower, GiveItemToThing,
  bare "Create"), so the identities had to come from RTTI / PDB donor order first.
- The landed bindings of the shared callees use wrong spellings (PAV owners, global GetAnimation*/CreateShot/DropWeapon,
  non-template basic_string); copying them would break the one-address-one-name rule, so each callee had to be
  re-derived from the PDBs.
- Rows needed non-obvious shapes: the CReplaceSerialise template chain, STLport same-TU bodies plus /Gr for
  toLowerCase, the C3DVector user copy constructor, memset/chained-assignment stores in the cell conversions.
- The seven parked rows are register-allocation / scheduling residues (or, for 0x00816AC0, an unknown def-pointer
  shape) that the 4-grade budget did not close.

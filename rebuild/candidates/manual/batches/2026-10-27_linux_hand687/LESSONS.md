# Chunk 687 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Catalog extents can split one function into several rows (0x00A0F4B0 + 0x00A0F6EA + 0x00A0F98B are one
  1462-byte STLport `_M_insert_aux`). When the retail bytes at the row end have no `ret`, check the next rows
  before authoring; grade against a custom oracle (`bin/qo.py`, QORACLE) and flag the extent fix.
- VC7.1 escape analysis: a local `std::vector` whose `this` is passed only to same-TU
  `__declspec(noinline)` bodies that do not leak it keeps its data pointer in a register across calls
  (no reload of `_M_finish`). Real STLport member calls that store `this` break the match; write the
  stand-ins without a leaking store (0x00ACA5F0).
- PDB `/Gr` (fastcall) members: `// Flags: /Gr` plus explicit `__thiscall` is not needed; the member
  keeps thiscall and only free/static callees change (0x00ACA5F0 CompareWeight is `__fastcall`).
- `if (!(x > 0.0f)) break` emits `test ah,41h; jne`; `x <= 0.0f` emits `jnp` -- choose by the retail opcode.
- Float->DWORD bit copies (`*(long*)&f`) give `fstp [local]; mov eax,[local]; mov [dst],eax`
  (0x00C61D10 owner word = float bits of height).
- An inline GetProcAddress-style accessor is inlined only inside a loop body (hot) and called out of line
  elsewhere in the same function (0x00C733D0).
- Vendor rows (zlib 1.2.1, libvorbis 1.0.1, strmbase) match from the exact vendor text; do not "clean"
  them. libvorbis statics compile under the VC7.1 private convention (args in registers), so keep `static`.
- EAX 4.0 setters: the SDK struct field order drives the call order; the integer setters are the same
  body shape as the landed CEaxContext ones (two of them were landed as CLight methods -- fixed in NAME_FIXES).

Review at landing:
- 0x00A0F4B0 extent merge (three rows -> one 1462-byte row) before vland accepts the source.
- NAME_FIXES.tsv: 36 rows, several respell landed callers (CLight::SetIntegerProperty2/3,
  CWorldManager::InitializeSteeringAfterSave, CBrainOutput::SetAsPumpCombatActions, DrawGetEnvironment,
  CInputEvent::GetKey, CMouseCursor::TurnToPointer, CTCHeroExperience_00c13140, ClassifyShaderExtension).
- 0x00C73AF0 name parameter type conflicts with the landed caller 0x00C62920 (`const char*` vs
  `const unsigned short*`); the WideToMultiByte call proves wide.
- CSoundSystem vs CAudioOwner for vtable 0x012B1E24: pick one.
- The three parked rows are at the "scheduling/register/layout residue" stage; worth a second pair of eyes
  with the differ/ files rather than more probes from me.

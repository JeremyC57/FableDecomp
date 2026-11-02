# Chunk 711 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- When a caller inlines a PDB inline setter and no caller-side form matches, byte-scan .text for the member's disp32 to find
  an out-of-line copy of the setter (0 callers is normal) and match the setter alone first: 0x00B671A0 needed 28 forms until
  0x00B66EB0 fixed the setter text (`if (mode != 1) enable = false; if (enable != member) { member = enable; flags |= 1; }`).
- A two-byte local string buffer homed in a dead `char` argument slot comes from a block scope around the buffer and its one
  use (0x0099F3D0: function scope gives 44v42).
- `if (p && !p->IsDead())` loads and tests a member before the dtor's vtable store; the by-reference template
  IS_THING_ALIVE(p) keeps the store first (0x007E3420).
- A function-local static's atexit thunk is authored by compiling the owning function with the static and grading leaf
  `_$E2` (0x0122A940; `_$E1` would be another static of the same TU).
- Byte near-twins ret 8 vs ret 4 over the same element: ret 8 = four-argument `__uninitialized_fill_n(.., __false_type)`,
  ret 4 = the three-argument public `uninitialized_fill_n` front end (0x0086D090 / 0x0086D570).
- Opaque members of a script class: `unsigned long X_[n]`, not `unsigned char X_[4n]`, or later bool/long members shift
  (0x00E827A0 sizeof assert).

Review at landing:
- 0x00B671A0 took 30 probes plus one setter probe (x29); the global 0x01436E9C spelling is a coordinator decision.
- Local base models next to existing headers: CCharString (fable_string.h lacks PDB names), CFileInstaller
  (fable_file_installer.h is opaque and vptr-less), flattened CEngineLightingManager / CEngineLandscapeRenderer used via
  layout-rename.
- Errors in already-landed sources: 0x00E82650 calls `InitialiseVariablesCheck`; 0x009F0240 uses `s_empty`, `Handle`, public
  const QAEABU; 0x008F8C70 / 0x0090B8E0 model LSocket in the global namespace; 0x0099EB40 is declared public; 0x009A06B0 takes
  `const char*`; 0x0086E4B0 binds 0x0086D570 as ?UninitFill@@...; 0x004BFAE0 bound without NGraphicAppearance.

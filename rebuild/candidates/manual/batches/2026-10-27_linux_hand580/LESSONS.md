# Chunk 580 -- lessons (GOTCHAS one-liners) and review items

- A local vector built by the out-of-line STLport fill ctor `vector(n, const T&, const allocator&)` homes the value and the empty allocator temporaries in ONE dead argument slot only when a same-TU noinline body of that ctor is visible (real STLport 4.6 body from the landed 0x009D2A40); declared-only, VC7.1 gives the allocator its own slot (0x00F3AE50, 353v353 reloc-mismatch on one `lea`).
- `if (weight < 0) weight = 0; else if (weight > 1) weight = 1;` after `weight = x` reloads weight from memory for the second test; retail compares the computed expression still in st(0) -- write the tests on the expression (`if (signal * 2.0 < 0.0) ... else if (signal * 2.0 > 1.0)`) (0x00F36710, 329v327).
- A member pointer used for two calls (`Noise->PerlinNoise(..)` twice) is re-read after the first call; retail keeping it in ebx means the source read it into a local first (0x00F36710).
- A `double` result clamped with early `return 0.0;` / `return 1.0;` reproduces retail's two separate epilogues with `fstp st(0) ; fld const` (0x00F36710).
- `result *= 1.0f / 3.4f` gives a qword pool constant holding the float-rounded value (0.29411763880904596): a qword constant that is exactly a float means a float literal in a double expression.
- Functions whose vtable is discarded (non-COMDAT TU, class never constructed) have no data reference at all; name them by PDB donor order against neighbours (0x00F36710 between PerlinNoise group and the Hybrid/Hetero fractals).
- STLport inline `resize` -> `erase` on a POD vector folds the `memmove` away (the copy count is end - end = 0) only through the `__n ? memmove : result` form of __copy_trivial; write erase with that guard.
- In a behaviour test, `strstr(log, "Completed")` matched "WatchForCompletedQuests": delimit logged tokens (";Completed;") in tests.
- Never `#include` a file outside the repo include path in a cx/q.py compile: cl.exe under Wine hangs until the timeout instead of failing.
- Quote every heredoc (`<<'EOF'`) when writing sources: backticks in header comments otherwise run as shell commands.

Review at landing:
- 0x00F36710: local NFractal::CFractal / CRidgedMultifractal with a virtual GetFractalHeight (retail vtable exists in the PDB
  layout but was discarded from Fable.exe); the object emits `??_7CRidgedMultifractal@NFractal@@6B@` which has no retail
  address. Local CNoise declares only PerlinNoise (0x00F35C60, manifest name; not yet in reloc_bindings).
- 0x00F34160: local CFireHeartPatternDef over the shared CDefClassBase (retail vtable 0x012CCC50; NAME_FIXES respells the
  landed `??_7CDefWithByteVector@@6B@`). The n == 1 copy is written as placement new (retail inlines that one copy while the
  loops call _Construct 0x00D34EC0 out of line), as the landed sibling 0x00F34FB0 does.
- 0x00F37F60 vs 0x00F381F0: the CResourceEntry copy ctor is inline (memberwise) in 0x00F37F60 but declared-only in
  0x00F381F0 (retail calls 0x00F37970 there); the landed 0x00F37970 defines it out of line. Same-TU noinline bodies emit
  COMDATs for 0x00F37CC0 / 0x00F37AD0 / 0x00F37D30 / 0x009D2A40 (helper bodies of real STLport code; the Leaf pins the row).
- Script rows 0x00E389E0 / 0x00EF7600 / 0x00ED7830 / 0x00D27050 model CScriptThing, CScriptBase, CEntityScriptBase,
  CGameScriptInterfaceBase, the predicates, CEntityScriptBinding and the CScriptGameResourceObject* classes locally (all
  have retail vtables) pending the migration agent's headers; 0x00D27050 now includes engine/CBaseClass.h.
- `pGlobalScriptDef` (0x00E389E0) is modelled as `extern CScriptDef*` (the landed CCrate1::Main model); the PDB shows a
  dynamic initializer for it, so check the declared type when the header lands.
- 0x00D27050 master-data type CLevelScriptBase<CQ_SunnyvaleMasterData> is the Ego_r.pdb choice (probable; no retail
  evidence beyond layout).
- 0x00F3AE50 globals 0x0143E980 / 0x0143E97C are descriptive names.
- 0x00F3D200 parked: name CheckForReshadedVerts rests on PDB name set + donor order/size + signature (probable).

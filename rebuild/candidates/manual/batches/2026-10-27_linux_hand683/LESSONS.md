# Lessons (chunk 683)

GOTCHAS one-liners:
- A float array mirrored from a member array (`float gain[6]` for `m_gains[6]`) explains a 0x20 frame whose branches store only
  3 values: VC7.1 keeps every array element's home; named float locals (even declared up front) coalesce into dead parameter slots.
- `sin(x * K)` / `cos(x * K)` with the product inline keeps the angle on the x87 stack (`fld st0 ; fsin ; fxch ; fcos`); a `float angle`
  local is rounded through memory (`fstp/fld`) and makes VC hoist the common `fsin` above the channel branch.
- `(flags & 2) ? 0x08000000 : 0` becomes `and/shl 1Ah`; retail's `xor edi,edi ; test ; je ; mov edi,X` is `attributes = 0; if (flags & 2) attributes = X;`
  and the `= 0` must precede the earlier member assignment that shares its zero register.
- A `throw CBBBFileException(message, type)` whose message temporaries are destroyed BEFORE the throw while two strings are not: the
  two are named locals (`error = GFGetLastWindowsError(); message = ... + error;`) whose destructors are unreachable after the throw.
- STLport-style vector member (start/finish/end_of_storage, `size() = finish - start`, `operator[] = *(start+n)`) makes VC7.1
  recompute `(end-begin)/sizeof` for a second `index < size ? &v[index] : 0`; a begin/end pointer-pair model CSEs it (the
  PreloadReflectionTexture 0x00BEBCA0 residue).
- The `GetFrame` body of CGraphicInfoFrame (landed 0x0041C260) reused as a texture-returning `__forceinline` member reproduces
  GetReflectionTexture byte-for-byte except the push placement.
- libpk delay-effect slot 4 (Update) uses `LookupDirectionFilterPair(4, x, y, &first, &second)`: a literal 4 where the siblings pass
  g_pAudioMaster->TapTable.
- Script `while (Game->IsQuestActive(..) != false)` -> `test al,al ; setne`; the plain form gives `mov bl,al`.

Lane gaps seen:
- Callee-saved `push ebx` sunk past a lookup diamond (two guards + join) into the block that first defines ebx: the lane always
  groups it with the pushes after the preceding early return (25 forms; 0x00BA9830). Known gap (wave 25 "never sunk inside a row guard").
- The 5-instruction pointer swap `eax=[a]; ecx=[b]; [b]=eax; mov eax,ecx; [a]=ecx` with the forwarded value used by the next test
  (0x00C6EEF0): no C++ swap form produced it (24 forms).

Review at landing:
- differ/00c6eef0.cpp holds `__asm` (the landed retail_asm kernel bodies as same-TU stand-ins); it is a parked differ, never src/.
- `_$E2` RELOC_CONFLICT warnings on the two dynamic initializers are inherent (one `$E2` symbol per TU).
- Descriptive names only for the libpk rows (0x00C6D150 CVtable_012bed7c::Update, 0x00C6EEF0 CVtable_012b8a8c::Process); the
  class names follow the landed ctor/SetVolume/Update siblings.

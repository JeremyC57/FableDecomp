# Chunk 586 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A library static with a private register convention keeps its retail C name only inside `extern "C" { static
  __declspec(noinline) ... }`: plain `static` in a .cpp mangles `?bitreverse@@YAII@Z`; the extern "C" block gives
  `_bitreverse` / `_read_buf` with the same bytes (0x00C96320, 0x00C01090). Both rows matched first try with the callee's
  verbatim body above the caller at plain /O2 (the chunk 585 rule holds for zlib and libvorbis callees too).
- Lock-scoped bodies with an early exit: `if (!Playing()) { ...; return; }` duplicates the destructor epilogue up front
  (286v257); `if (Playing()) { ... return-inside ... } else { ... }` gives retail's single exit (0x00C27890).
- Two separate `if (x) return -1;` tests keep retail's shared -1 block on the second test's fall-through and its own
  epilogue; `if (a || b) return -1;` merges it at the end (0x00C62790, as the EXTRA_RULES return-block note).
- Out-of-line `break` arms are laid out in source order: `if (loops) { seek ... } else { done = true; break; }` puts the
  flag block last as retail does; `if (!loops) { done = true; break; }` first puts it before the seek-error block (0x00C62790).
- `Game->Call(CCharString("..."), 0)` pushes the 0 before building the temporary; retail built the string first, i.e. a
  named `CCharString name("..."); Game->Call(name, 0);` (0x00CDF800).
- Arithmetic that feeds a later call: retail's `add ebx,eax ; sub edi,eax` before `call Advance` means `total += n;
  frames -= n;` precede `cursor.Advance(n)` in the source (else n is kept in a callee-saved register and total spills).
- An 8-byte local class with only 32-bit members still got retail's `and esp,-8` EH frame for free in 0x00C62790 (VC7.1
  aligns the frame by itself there); no `__declspec(align)` needed.
- Module-local zero GUIDs: GameCODA has its own all-zero GUID at 0x012B8FF4 next to uuid.lib's GUID_NULL 0x012C0694; give
  it its own name or vland flags RELOC_CONFLICT against the landed `_GUID_NULL` (0x00C5DB40).
- CScriptDef offsets: Ego_r is 0xBC high around retail +0xF00..+0xFB4 (the Dinkumware 0x10 vectors before it); TeleporterActivationDist
  is retail +0xFB4.

Review at landing:
- 0x00C27890 introduces `_CODA_IsSourcePlaying@4` for 0x00C35CA0 and `?g_soundLock@@3VPKCCriticalSection@@A` for
  0x0143BAFC (ten wrong spellings bound); apply NAME_FIXES.tsv rows 0x00C35CA0 / 0x0143BAFC / 0x00C0FFC0 / 0x00C0FFE0 to the
  landed CCodaPlayback rows (Update 0x00C276B0, SetVolume 0x00C27620, LogStatus 0x00C27760 use CScopedLock /
  CProfileLock_00c27760 / g_soundLock[] / IsPlaying_00c27760) in the same landing, or vland will see several names.
- 0x00C72930 binds `?Initialise@CVtable_012B5C10@@UAEJPAUCAudioSound@@@Z` to 0x00C596E0, whose landed row is
  `CEnvironmentSpellEffectInterface::ClearScreenFilter` (respell that row; NAME_FIXES.tsv). The base class keeps the
  landed placeholder name CVtable_012B5C10: a naming-pass decision for the libpk voice base.
- 0x00C01090 / 0x00C96320 each carry a same-TU static body (read_buf, bitreverse): make sure the lander keeps only the
  graded leaf and maps the static callees by address (0x00C00D50, 0x00C962B0), as for 0x00C9C1E0 in chunk 585.
- 0x00C01090's own symbol is extern `_fill_window`; retail's is a deflate.c static. The bytes are the same; a static leaf
  would need a same-TU caller (deflate_stored) and is not worth it.
- 0x00C6B9D0 (parked) contains the retail `__asm` helper (retail_asm grade, Retail-asm line present): it sits in differ/
  because it does not match yet; when it does, it belongs in retail_asm/.
- Landed-row errors seen: 0x00C73770 `SetDefaultString(CDefString*)` is a thiscall constructor of the platform-info
  object (not a CDefString call), 0x00C35CA0 is not CEngine code, 0x00C5DB30 is the CDSoundAccelerationQuery constructor,
  0x00C62A20 is ~C16BitSamplePtr; 0x0139C9F0 is bound twice (g_mixBlockLength, g_blockFrames) -- 0x00C6B9D0 uses
  g_mixBlockLength like its CStateVariableFilter siblings.
- No row needed more than 4 grades (three parked at 4; the extra compiles on matched rows were a C-linkage, a GUID
  rename and a shared-header switch, all bytes-neutral).

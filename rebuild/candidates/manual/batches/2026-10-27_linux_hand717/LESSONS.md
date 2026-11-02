# Chunk 717 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Naming a late-.text gap-scan row.** First find its `.CRT$XCU` slot (`xref.py` gives a `.data 0x01374xxx` hit). Then
  align retail's XCU run (literals, objects, `fps * k` shapes) with the TU-contiguous
  `dynamic initializer for 'X'` runs of Ego_r.pdb / FableWin.pdb. The Ego_r body sizes in
  `ghidra_out/naming_stragglers/ego_r.exe.signatures.tsv` are a second check: most equal retail.
  This named 9 of 9 rows here (0x0121F220 .. 0x01221B60).
- **A namespace-scope `const T& X = "lit";`** (a reference bound to a temporary) compiles to retail's odd initializer:
  1. `push <thunk> ; call _atexit` comes FIRST;
  2. then the temporary's constructor runs into the TU-local `_$S2`;
  3. then `mov [ref],offset temp`.
  An atexit before the construction, plus a pointer store of the object's own address, marks this shape
  (0x0121EF70, NFire::OPINION_SOURCE_DEF_NAME).
- **VC7.1 numbers a TU's dynamic initializers `_$E1`, `_$E3`, `_$E5`** (the even numbers go to atexit thunks).
  - Example: a row whose initializer reads an anonymous-namespace object initialized earlier in the same TU
    (0x0121F7A0 reads JACK_TOTAL_DURATION). Define both in retail order; the row's Leaf is then `_$E3`. A `_$E2`
    Leaf names no symbol and silently grades the largest function.
- **A const anonymous-namespace object becomes an internal plain-C symbol** (`_MIN_TIME_TO_SIT`, storage class
  static). A non-const one becomes `?X@?A0x<hash>@@3JA`.
  - The `?A0x` hash depends on the source file path, so two rows that touch one anonymous object bind it under two
    hashes.
  - An unreferenced const anonymous object keeps its dynamic initializer: no dummy reader is needed.
- **An unaligned `_$E1` row is a size-optimised TU.** `#pragma optimize("s", on)` gives retail's reuse of the malloc
  argument slot (`mov [esp],thunk ; call _atexit ; pop ecx`). /O2 gives `push thunk ; ... ; add esp,8` (0x0121A6AC vs
  the /O2 twin 0x0121A6D0).
- **Grading the row's own TU at its atexit thunk's address** (`_$E2` / `_$E3`) proves the thunk's identity at no
  extra cost. That is how 0x01228E44, 0x012294D0 and 0x01229B40 were shown to be atexit thunks. Their landed
  sources are invented stand-ins.
- **`fable_string.h` has no implicit `const char*` -> CCharString conversion** (no default length). Retail has one,
  because its only constructor is `(const char*, long = -1)`. Use the new opt-in `FABLE_RETAIL_CCHARSTRING`
  (hdr/fable_string.h) for braced literal arrays and literal-bound references. The explicit
  `CCharString("..", -1)` form gives the same bytes there.
- **`int static * float constant` in an initializer** comes out as `fild [g] ; fmul [k]`, and `k / g` as
  `fild [g] ; fdivr [k]`. Both match through the established inline `CMainGameComponent::GetConstantFPS()` (no
  by-value temporary in a dynamic initializer).

Review at landing:
- hdr/fable_string.h: new opt-in branch, default path untouched (no includer recheck needed). Copy it before landing
  0x0121E3A0 / 0x0121EF70; without it both are SRC_FAIL.
- NAME_FIXES.tsv: 15 rows. These landed readers carry stand-in spellings of this chunk's globals and must be respelled
  in the same landing: 0x007B7A70, 0x0093A8C0, 0x0073A6F0, 0x0073A760, 0x0073A7D0, 0x006E2940, plus the 13 landed
  rows bound to 0x013B8A98.
- VALUE_DISPLAY_DURATION / MIN_TIME_TO_SIT / JACK_* / MIN_SPEECH_FRAMES are anonymous-namespace objects. A respelled
  reader in another source file can only declare them under its own `?A0x` hash: one retail object, several lane
  symbols. This needs a coordinator decision (keep the readers' extern stand-ins, or accept per-file hashes).
- Landed fakes to re-author from `thunks/` (each graded RELOCATION_MATCH from this chunk's row TU):
  - 0x01228E44 and 0x01229B40: `AutoTinyThunkTarget` stand-ins; their `// Leaf: CEntry>_const&>` matches no symbol.
  - 0x012294D0: labelled `CFontBankStateBlock::CreateCRC`.
- Landed own rows with descriptive names, to respell to the proven names (NAME_FIXES):
  - 0x0121F7C0, 0x0121F7D0, 0x0121F800 (JACK_FADE_DOWN_TIME, the COMB_IDLE / WAVE CModeBase statics)
  - 0x0121E3D0 (MIN_SPEECH_FRAMES)
  - 0x01221B80 (RANGE_TIME_TO_SIT)
- Landed test 0x0121E260 (CTCWife::WifeAppearanceDialogueGroupStemNames): its literals ("..._SLIMY_" ..) are not
  retail's. Retail and the landed source have "..._GENERAL_" .. "..._BARETORSO_" (0x0125F608 ..); the test still
  passes because it checks only its own copy.
- engine/CList.h is NUISystem::CList flattened into the global namespace. vland's SHARED_HEADER_AVAILABLE on
  0x0121A6AC is a false positive.

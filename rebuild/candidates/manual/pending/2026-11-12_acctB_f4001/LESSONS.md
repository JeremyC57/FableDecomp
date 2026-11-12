# Chunk 4001 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00CE7670) A 32 KB generated script function is best rebuilt from a machine-made draft: decode the retail call sites into
  pseudo-statements (callee from the vtable slot, strings from the CCharString ctor slots), generate C++ from them, freeze the
  draft and fix it with a patch file keyed by draft line. A whole-function compile takes 1 s, so grade the whole function every
  time; partial sources are in another optimizer regime (zero constant not kept in ebx, bools in memory) and tell nothing.
- (0x00CE7670) In a script Main, `TRetVoid _Fail_Val; ... if (IsActiveThreadTerminating()) return _Fail_Val.Return();` is not
  cosmetic: with plain `return;` the shared exit that destroys a local (`~seh_hero ; ret`, 0x00CEF03C) is laid out inline
  instead of at the function end, and the 129 one-instruction expansions change the caller size c2 uses for inline scores.
- (0x00CE7670) A switch over a sparse enum: when retail's jump table sends a value to a neighbour's label, the source has that
  enumerator as an extra `case` on the same label (`case EGP_WHITE_BALVERINE_KHG: case EGP_WHITE_BALVERINE_WITCHWOOD:`); without
  it the table partition and every compare of the dispatch tree differ.
- (0x00CEB1C4) `if (flag) { f(175); } else { f(170); }` gives `mov ecx,0AFh ; jne ; mov ecx,0AAh ; call`; the ternary
  `f(flag ? 175 : 170)` gives neg/sbb/and. (0x00CED4F0) `if (x == 1) .. else if (x == 2) ..` gives `cmp eax,1 .. cmp eax,2`;
  `switch (x)` gives dec/je.
- (0x00CE91C6) A `while (true)` wait with two exits: the terminate check at the loop top and none after the bottom
  NewScriptFrame gives retail's rotated loop (check duplicated at the bottom); a check after the bottom frame adds a block.
- (0x00CEA983, 0x00CEC431) Message loops with a latch: `bool GaveMessage` declared in a block around the loop, the frame first
  in the body, then `if (IsRegionLoaded) { check; if (!GaveMessage) { check; ..; GaveMessage = true; } } else { check;
  GaveMessage = false; }`; c2 merges the two arms' first check into one call.
- (0x00CED227, 0x00CEE687) `while (Game->IsQuestActive("..") != false)` gives `setne bl`; `while (Game->IsQuestActive(".."))`
  gives the neg/sbb/inc form of the `!` loops.
- (0x00CEE906) Inline split in a cold block: read the c2 trace before anything else. Here four callees are expanded at scores
  63439..89689 and the next called one scores 164066, so one `/d2inlT89690` reproduces the split exactly once the two members
  retail calls although they score LOWER are declared out of line through header opt-ins (0x007E72A0 at 18067, 0x006E7A60 at
  21). Forced header opt-ins cannot expand an implicit destructor (a user-declared one adds a vtable store).
- (0x00CEE967) An implicit base ctor nested in an expanded ctor is evaluated at frequency 1 and is always expanded; when retail
  calls the base ctor's COMDAT there, only an out-of-line declaration reproduces it.
- pd.py-style relocation-masked diffs hide a wrong call target: after the instruction diff is empty, pair every relocation
  symbol with the retail target (tools/rel.py, or vland) -- that is how the expanded base ctor (0x0099A380 for 0x006E7A60)
  was found while the masked diff already looked close.
- A behaviour test for a long script can be checked against retail, not against the source: generate the expected call order
  from the retail disassembly in address order and run the source so that every wait loop takes exactly one frame; execution
  order then equals address order, with the few conditional regions lettered by address (tools/mktest.py).
- Lane: grading this row without FABLE_HDR_OVERLAY left cl.exe spinning under Wine instead of failing fast; `timeout` on q.py
  kills the Python parent but not the cl.exe child, which must be stopped by its PID.

Review at landing:
- The row used 14 full-function grades (g01..g14); the owner lifted the cap for this row.
- `// Flags: /d2inlT89690` must reach the catalog CompilerFlags; the row also needs the two overlay headers (hdr/README.md).
- New opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR in engine/CScriptGameResourceObjectBase.h; the two
  other callers of 0x006E7A60 (0x00D315EC, 0x00D3173E) can use it when their rows are written.
- New shared header engine/CGameflowScript.h; the landed siblings 0x00CEF3B0 (CoreQuestReminder) and 0x00CEF8E0 (OnPersist)
  still have local class models and can be retyped (retype_landed.py).
- The row declares `enum EScriptAIPriority`, `enum ETutorialCategory` (27 / 29 only) and the three AddLogbook* functions
  locally, as the landed script Mains do; a shared header for them would remove the copies.
- First binding of 0x006E7A60 in reloc_bindings.tsv comes from this row (`??0CScriptGameResourceObjectBase@@QAE@XZ`).
- The manifest row 0x00CEF550 has module `CGameflowScript` without `NScript::`.

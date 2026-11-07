# Chunk 820 notes (six parked-row retries)

Result: 3 RELOCATION_MATCH (0x00D7B510, 0x00DA91A0, 0x00E64210) and 3 parked (0x00DD5A60 21 lines, 0x00E64FB0 16, 0x005E78F0 4).
All six names were already proven (catalog labels right; LABELS.tsv and NAME_FIXES.tsv are header only). Relocations of the three
matched rows were checked with bin/relpair.py, which pairs every reloc with the retail dword. Every call target equals its retail
address. Every symbol is `bound-same` in rebuild/oracles/reloc_bindings.tsv, except one that is not bound yet:
?GetDistanceBetweenThingAndPosition@@YIMABVCScriptThing@@0@Z = 0x00CBE551 (landed rebuild/src/compiled/00/cb/global_GetDistanceBetweenThingAndPosition_00cbe551.cpp,
not bound anywhere else). The only BOUND-DIFF entries are compiler `__real@` float constants. 0 string mismatches.
Leaf symbols were confirmed by grading against the `// Leaf:` symbol (q.py extracts it from the object). Tests: g++ PASS and VC7.1
PASS (tall.py) for src/; g++ PASS for differ/*.test.cpp. Every attempt is in ATTEMPTS.log and STATUS.tsv.

## 0x00D7B510 CQ_HeroSoulsThunderScript::CHeroThunder::Main -- RELOCATION_MATCH (attempt a1)
- Shape: the closing idle loop is chunk 817's else-wait `if (IsActiveThreadTerminating()) return; for (;;) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`.
  VC7.1 tail-merges the pre-loop test into the loop test, so the health-loop exit jumps to retail's single bottom test
  (0x00D7C2A5 jmp 0x00D7C2AF). Chunk 816's bottom `NSF; if (!T) continue; return;` (a2) also matches. a1 is kept because it
  is the simpler spelling. `for(;;){T; NSF; ...}` (a3) keeps the chunk 798 residue.
- The test adds an IdleLoop check.
- **0x00D82640 CBriarRose (asked by the brief, not authored):** the lever carries. In probe/00d82640_x1 the 817 idle loop takes
  the parked chunk 789 source from 38 to 32 masked lines (3276v3260), and the idle-loop head/bottom residue is gone. What is left
  is the prologue CIsThingAlive copy (+16 bytes) and the alignment pads it shifts. That remainder is a separate problem.

## 0x00DA91A0 CQ_MinionClifftopChaseScript::CMapKeeper::Main -- RELOCATION_MATCH (attempt d4)
- Shape: `CScriptThing escort = Game->GetNearestWithScriptName(Game->GetHero(), "MinionEscort");` is a value local. Both tests are
  `if (!reinterpret_cast<const CScriptThing&>(escort).IsAlive())`. Diagnostic d2 (the banned landed CScriptThingView device, not a
  source) matched as well. That proves retail has a value local with a virtual IsAlive and `this` computed first (the lea hoisted
  above the loop-head jne 0x00DA9771).
- What fails, and how:
  - plain value, static_cast, C-style cast, `(&e)->` and `(*&e)`: the call is devirtualised, 34 lines;
  - a reference local, an alias reference or an inline helper: the call stays virtual but there is no hoist, 2 lines.
- **Review flag:** a same-type reinterpret_cast is a compiler lever (the cast-only form of GOTCHAS I-44), probably not Lionhead's
  spelling. It is plain C++ with no view struct. The coordinator should decide whether it is acceptable. If it is not, the
  fallback is the chunk 797 park (2 lines).

## 0x00E64210 CV_ChickenKickingScript::CKickedChicken::Main -- RELOCATION_MATCH (attempt t1)
- Shape:
  ```
  float dy = strikePos.Y - arenaPos.Y;
  float dx = strikePos.X - arenaPos.X;
  C3DVector direction;
  direction.X = dx; direction.Z = 0.0f; direction.Y = dy;
  ```
  The final test is `direction.Dot(a) > cos(...)`.
- Finding (a18-a20, mXZY/mYXZ/mYZX/mZXY): when the vector is built by the ctor, the inlined Dot is direction-first. When it is
  built member-wise, the Dot is `a`-first in every product, and the term order is the reverse of the store order. Stores X, Z, Y
  give retail's Y, Z, X (0x00E64B4B). Assigning the differences directly computes X before Y. The named dy/dx floats restore
  retail's Y-then-X evaluation. t2 (only dy named) also matches; t1 is kept because it is symmetric.

## 0x00DD5A60 CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main -- parked, DIFFER(2246v2250) 21 lines (a0 = chunk 786 source)
- Root cause (new; a3 disassembly):
  - Retail: the GUARDS hit-arm `if (T) return;` exit CALLS ~CTimer out of line (0x00DD6293 jne 0x00DD5B0B). It shares the
    [call 0x00CD4470; epilogue] tail with the first cell-door exit.
  - Ours: VC7.1 expands ~CTimer at that exit and cross-jumps it into the function-final block. That block then schedules
    `push ebp` first and absorbs the cell-door exit 0x00DD5DDA.
  - This is the per-call-site inline-budget/frequency blocker (documented family).
- Diagnostics only (`/d2inlT<n>`, never a source):
  - at thresholds 68-73 the hit arm goes out of line as in retail, but so does the comment arm (2 lines);
  - so retail's comment-arm exit is slightly hotter than its hit-arm exit, while ours has them nearly equal;
  - a8 (an extra plain pre-loop T) reproduces the hit-arm call without the flag, but also moves the comment arm out of line.
- 21 source attempts in total: loop shapes 816/817, do-while(0) on every T, a named bool, De Morgan, nested ifs, `== true`, scopes.

## 0x00E64FB0 CV_ChickenKickingScript::CChickenMaster::Main -- parked, DIFFER(15210v15210) 16 lines (a0 = chunk 818 source)
- Register colouring only:
  - at FadeScreenOut 0x00E6716A, eax/edx are swapped;
  - at the first spectator GetThingWithScriptName 0x00E671E0, they are swapped too.
- Sites 1 and 3 have identical surrounding code but are coloured differently, so the cause is global.
- 10 attempts:
  - do-while(0) Ts, whole-branch do-while(0), a named bool, scopes, a hoisted CRGBColour and swapped branches: all identical;
  - no inline ~CScriptThing (a5) and a plain inline GFIntToWideString (a8): both worse.
- Needs hdr/ (chunk 818 opt-ins, carried unchanged).

## 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate -- parked, DIFFER(2340v2340) 4 lines (a0 = chunk 795 source)
- x87 scheduling in the digit-count block: around the fmul/fdivp, retail spills TargetExperience and loads Gold in a different
  order.
- 10 attempts:
  - locals, {} blocks, casts and declaration placement: all identical;
  - a GetConstantFPS local (a5) and a ternary clamp (a9): both worse.

## Header debt
None. hdr/ carries chunk 818's two opt-ins unchanged (hdr/README.md).

## vland warnings (read-only run: 3 RELOCATION_MATCH PASS, no RELOC_IDENTITY)
- RELOC_NAME 0x00CDBFB0: the catalog label is the placeholder `CRetailTree_erase00cd344a::~CRetailTree_erase00cd344a`. It is the
  std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> dtor (LABELS.tsv). Note that reloc_bindings.tsv also binds
  ?Destroy@CCutsceneCastMap / ??1CCutsceneCastMap / ?AutoTinyFirstMemberDestructor_00cdbfb0 to this address (one address, several
  names). This is older debt, not touched here.
- RELOC_NAME 0x0129BA3C: the catalog says `EPSILON` (naming wave 13). reloc_bindings.tsv and naming wave 18 say `?DELTA_NOTIONAL_ZERO@@3MB`,
  which is what 0x00E64210 binds (bound-same). Both labels are in ghidra_out/labels_*.tsv. No PDB is on this lane to settle it,
  so it is not listed in LABELS.tsv; the coordinator should resolve which wave wins.
- RELOC_NAME 0x0099E4C0 `CCharString::operator_char_const*` (the catalog spelling of ??BCCharString@@QBEPBDXZ) and RELOC_ALIAS for the
  rand / _CIacos import thunks are expected.

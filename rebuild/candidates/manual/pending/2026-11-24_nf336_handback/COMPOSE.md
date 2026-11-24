# NF336 one-TU compose probe (evidence only, not a deliverable)

Date 2026-11-24. VC7.1 RTM `cl /nologo /Zs /O2 /Oy /W3` (BASE_FLAGS) under Wine, header overlay = this hand-back's `hdr/` on the repo tree.
The TU is the row sources concatenated in address order, which is what `#include`-ing them gives (`nf.py zs` copies the file into
its own work dir, so relative `#include "r_<addr>.cpp"` lines would not resolve). Each part is precheck-clean (gcc cpp + clang) on its
own; the composed TUs were prechecked in an earlier call than the /Zs. Error lines are mapped back to their part (tools/zsall336.py; a
line inside a header is reported as `header:<file>`).

| TU | parts | result |
|---|---|---|
| A: as landed after this hand-back (`evidence/compose_all25.cpp`) | the 23 moved rows + 0x00E95380 + HEAD 0x00E96C30 | **fails**: 100 errors (`evidence/compose_zs_head96c30.txt`) |
| B: with 0x00E96C30's shared-header attempt (`evidence/compose_all25n.cpp`) | the 23 moved rows + 0x00E95380 + `evidence/attempt_00e96c30.cpp` | **fails**: 16 errors, no warnings (`evidence/compose_zs_attempt96c30.txt`) |

## What is gone
No C2011 / C2118 / C2953 type redefinitions remain among the 24 moved rows: the local class models (CV_HauntedHouseScript,
CScriptDataBase, CCharString, CScriptThing, CSpawnedFuncBase, CScriptProcessBase, the four nested entity classes) are all replaced
by the shared headers.

## What is left
1. **HEAD 0x00E96C30 (TU A only).** Its local STLport `vector<CScriptThing>` / `__type_traits` / `allocator` / `CCharString` /
   `CV_HauntedHouseScript` models collide with the shared headers (4 C2011, 5 C2953, 2 C2995), and 46 follow-on errors land in
   `CGameScriptInterface.h` (C2955 / C3203 / C2555). This row's port is skipped (STATUS: it changes the COMDAT set). Until it is
   decided, the Gate 0 blocker class (type redefinition) is still present in this object. This is decision Q1.
2. **Duplicate function bodies (TU A and B): 16 lines in TU B (11 C2084 + 5 C2264), the same 7 duplicates the HEAD link already shows
   (`evidence/link_final25.txt`: 5 symbols, HEAD set = new set):**
   - `NScript::CV_HauntedHouseScript::CV_HauntedHouseScript(CScriptDataBase*, CGameScriptInterfaceBase&)` is defined in 0x00E93D50
     (its own row), 0x00E93E60, 0x00E96DD0 (`inline`) and 0x00E96E10, each to emit or expand it. The four texts are identical.
   - each nested class's 3-argument constructor and its `Alloc` are defined in both its Alloc row and its `??_G` row
     (0x00E955B0 / 0x00E95670 CM_QuestControl, 0x00E95690 / 0x00E95750 CM_UndeadHere, 0x00E95BF0 / 0x00E95CB0 CM_HeroInsideHouse,
     0x00E95CD0 / 0x00E95E60 CHHUndead). The texts are identical.
   A composer that keeps the first definition of each would clear these. They are not class-model conflicts.

## What /Zs cannot see (silent in one TU)
Include guards make the first part's opt-ins win; later parts' `#define`s for an already-included header are inert, so the
composed TU compiles but would emit different code for these rows:
- **`~CLevelScriptBase<T>`.** 0x00E93D50 / 0x00E93E60 / 0x00E96DD0 leave `FABLE_RETAIL_CLEVELSCRIPTBASE` undefined (declared dtor:
  the HEAD form, COMDAT set = HEAD). 0x00E96E10 defines it (implicit dtor: HEAD 0x00E96E10 already emitted the implicit thunk). One
  TU can hold only one of the two. Retail's `??_GCV_HauntedHouseScript` 0x00E96E10 calls ~CScriptBase 0x00CBD510 directly, which is
  the implicit form; the implicit form also emits the unreferenced `??1?$CLevelScriptBase` thunk in each of the other three rows
  (NF320 Q2), see decision Q2.
- **`~CSpawnedFuncBase`.** 0x00E95F10 defines `FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY` (decl-only, so its `??_GCSpawnedFuncBase`
  copy calls 0x00CDD4C0 out of line = retail 0x00CDD4A0). 0x00E95F80 needs the implicit dtor: it emits the `??1CSpawnedFuncBase`
  copy, now EQ retail 0x00CDD4C0. A family-wide decl-only would drop that copy from 0x00E95F80. The two rows want opposite models.
- **CCountedPointer.** 0x00E95F80 defines `FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR`, `_VISIBLE_CALLED_DTOR`, `_SHAREDATA_DECL_ONLY`
  before the preamble. In the composed TU `engine/CCountedPointer.h` is already included by the first part, so they do nothing.
  B's own first-pass defines (CSCRIPTTHING_ORIGINAL_ACCESS, BASE_OBJECT, CCOUNTEDPOINTER_RELEASEDATA) are another set of the same kind.
- **0x00E95380's helper.** Its anonymous-namespace `ReferenceReturn()` scaffolding is harmless in one TU, but it is not retail.

## Reading
The 24 moved rows no longer conflict as class models. One TU of this object still needs: (a) 0x00E96C30 decided (Q1); (b) duplicate
definitions removed, keeping one each (mechanical); (c) one family-wide choice for each per-TU opt-in above (`CLevelScriptBase` dtor,
`CSpawnedFuncBase` dtor, CCountedPointer forms). Retail was one TU, so a single choice exists. Proving which choice keeps every row
byte-equal is a composed-object build (B's Gate 0), not something /Zs can show.

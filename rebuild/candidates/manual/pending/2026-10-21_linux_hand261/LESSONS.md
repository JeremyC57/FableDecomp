# Chunk 261 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU stand-ins.** Give each out-of-line callee a noinline body under `#pragma auto_inline(off)` that only writes the out slot or returns the lower_bound result. This lets VC7.1 share the key slot with the out pointer and keep defs in registers with no release null test. Covers:
  - LowerBound, both the const 0x4365B0 and the non-const 0x40F020;
  - GetSubDef 0x669580, 0x8C5A50;
  - GetEffectFromString 0x669D30.
  
  Matched 0x8C44B0, 0x8C1C30, 0x78B410.
- **Argument evaluation order with inline accessors.** An inline accessor's GetPItem is hoisted before the argument pushes; a direct `PCreature.GetPItem()` call runs in right-to-left order. Pick per argument (0x8F47E0, 0x8BAC40, 0x8C1C30). In a two-argument call `f(A->GetPosition(), B->GetPosition())`, the pointer of the argument that uses the accessor is fetched first (0x8C55B0).
- **Compiled-out debug helpers.** Calls that appear with an unused result (two GetPosition calls, nothing consumed) come from an empty inline debug helper whose arguments are still evaluated (0x8C55B0).
- **Named local for a computed argument.** `long loops = N - 1;` declared before a constructor call moves the dec in front of both GetPItem calls (0x843C10). The first temporary's implicit dtor is called out of line and later ones are inlined; do not force it.
- **Vector operators.** The ctor forms `C3DVector(s*X, s*Y, s*Z)` and `C3DVector(X+o.X, ...)` give "store each product directly" and "Z, Y, X computed then stored" (0x8C44B0, 0x8F5060).
- **Float member copies.** `Time = expr; PrevTime = Time; VillageTime = Time;` gives `fst Time; mov edx,[Time]; fstp PrevTime; mov VillageTime,edx`. The chained `Time = PrevTime = expr` gives three float stores instead (0x6BBC30).
- **Conditional temporaries in new-expressions.** `X->AddInput(new T(CCharString-literal, ...))` builds the name only when allocation succeeds. That produces flag bits in one frame word; the first cleanup clears its bit, the last does not. An inline lazily-created singleton getter runs after the new-expression (0x6BBC30).
- **Dead parameter slots.** VC7.1 reuses dead parameter slots for later locals (the printed value and the message in 0x9EC430; mid and the tag temporaries in 0x52B6A0). A by-reference out-item struct holding a CCharString is constructed first in the frame (0x9EC430).
- **Hidden-pointer returns.** A small class needs a user copy ctor to return through a hidden pointer. The vslot `CDefString`-like result in 0x8B0B60 is consumed by dereferencing eax.
- **Counted-pointer release shapes** (0x6946C0, 0x8B0B60 parked):
  - `dec [reg]; jne` comes from a cached count pointer in a register;
  - the reload form `dec [eax]; mov eax,[slot]; cmp [eax],0` comes from the two-statement dtor on an address-taken temporary;
  - once VC hoists 0 into ebx for the zeroing stores, `--x == 0` turns into `dec; cmp [reg],ebx`.
- **Test hygiene.** A g++-passing test can fail under VC7.1 through a static-array overflow (0x8C0FB0, now fixed). VC7.1's std::string has no `clear()`.
- **FableWin.pdb export names** (`_fablewin_api.tsv`) identify mislabelled constructors from their console-callback strings (0x6BBC30 = CGameTimeManager ctor).

Review at landing:
- 0x6BBC30: the catalog says "CCombatManager Init" but it is the CGameTimeManager constructor. Land it under the right class and name; the PDB layout is in struct_layouts_egor.tsv (retail set is 12 bytes, PDB 16).
- 0x52B6A0: the catalog says "CCombatWheel::Insert" but it is STLport vector<CCharString>::_M_range_insert.
- Local base models were used where engine headers exist: CCreatureAction_ScorpionCharge, CCombatSequenceGeneric, CCreatureActionBase, CCombatAction_*. The PDB CCombatActionBase is 0x18 larger than retail's 0x114, so each source carries `// Layout:` evidence. Retype via the `_Retail` header pattern if the strict verifier insists.
- 0x8C55B0 models PTarget and the CThing dead flag locally. The landed 0x8C5490 uses quarantine headers for the same class pair.
- Coverage names 0x8C4F50 "CCreatureAction_BriarRoseShotFarInto"; the landed file names it the ScorpionChargeStrike ctor. Reconcile the manifest.
- The landed 0x00528040 distance is declared with 2 arguments. Retail callers push 3 (the iterator-category tag reference), which is harmless for cdecl but worth noting.
- Parked twins 0x867B40 / 0x867D20 and 0x6946C0 / 0x8B0B60 share one fix each. Retry them together.

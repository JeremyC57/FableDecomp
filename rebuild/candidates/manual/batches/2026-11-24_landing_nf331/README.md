# Landing nf331 (Account A, 2026-11-24): the CScriptThing::operator= pick-any unit, 1 row

Hand-back: `pending/2026-11-24_nf331_handback/` (UNIT.md first); tools: `naming_batch/agent_tools/nf331/`. This is the third CScriptThing
member unit of the NF326 / NF328 shape.

## What it does
- **The change, at 0x008AB980:** `??4CScriptThing@@QAEAAV0@ABV0@@Z` becomes `inline`, plus one `?A0x` helper,
  `ReferenceScriptThingAssignment`, that emits it out of line as a pick-any COMDAT. The `// Leaf:` line becomes the full symbol (NF299 Q2).
- **The helper** uses NF328's ruled device: `#pragma inline_depth(0)` over that one function, restored right after it, below the row's
  code, with an `// Inline-budget: scaffolding, not retail` line. It is scaffolding debt, like NF326 / NF328's helpers.
- **Census:** 497 objects compiled at HEAD, from a text gate of 458 rows plus closure greps and a 1-in-10 re-scan control. 91 objects
  define the member:
  - 1 strong, the self;
  - 70 pick-any copies equal to retail;
  - 20 pick-any copies that differ.

  After the change: 0 strong, 71 equal, the same 20 differing.

## Proof
- **Agent:**
  - pair grade HEAD / new: RELOCATION_MATCH both;
  - uproof, on the pair-grade objects and again on vland's: `??4` goes from selection 1 to 2, with bytes and relocations equal to HEAD
    and retail; the only new DEF is the helper; UND set, `.drectve` and every other section equal; no `??0` / `??1` / `??_7` / `??_G`;
  - /FORCE link of the 497 objects: duplicate lines 108 -> 18; exactly 90 `??4CScriptThing` lines removed, none added; unresolved sets
    identical (1,761).
- **Coordinator:**
  - `orig/` equals the repo;
  - recheck on the landed tree: RELOCATION_MATCH PASS (`logs/recheck.log`);
  - on that object, `??4CScriptThing` is COMDAT selection 2, 74 bytes, and pickany_vs_retail gives EQ for both inner CCountedPointer
    COMDATs.

## Rulings (coordinator)
- **D1 (accepted):** 0x00E97070 is claimed by B, so it was not compiled. Its text defines the member out of class as `FABLE_FORCEINLINE`,
  which can only yield a pick-any copy, never a strong one. The census keeps this one text-only hole.
- **D2:** the 20 differing copies are queued as T3 `nf331` debt, one line per copy, with their shape:
  - 17 leave the inner `CCountedPointer<CScriptThing>::operator=` (0x00704580) as a call;
  - 0x00CFD6C0 calls a stand-in;
  - 0x00D6D670 and 0x00E12580 expand to ShareData.

  Pick-any is not yet sound for this member while they differ. The `nf328 0x008AB980 unit` line is closed (`T3_CLOSED.tsv`).

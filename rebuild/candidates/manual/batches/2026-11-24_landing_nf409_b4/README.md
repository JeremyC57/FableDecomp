# Landing nf409 b4: 0x00656EAD is `??_GCQuestInfoElementCounterList@CDrawQuestInfo@NPlayerGui@@QAEPAXI@Z`

The exact spelling comes from FableWin. Retail confirms it: `new 0x28`, then the ctor 0x00656C0A, then vftable 0x0125A504's RTTI.

Ruling R-NF409-2 (advisor decision, owner delegated; EXTRA_RULES 2026-11-25): a row-end `#pragma optimize("", on)` is scaffolding that changes only the end-of-TU `vector<CEnginePrimitiveHandle>` chain.
- With it, `~vector` is EQ at 0x00440C40 and every chain COMDAT is tier 1. Without it, `~vector` is a reached NEQ.
- comdat_cmp with and without the pragma shows that only the 9 chain COMDATs change.
- No CompilerFlags apply to the row or to the tier-1 definers.
- `Sub_destroy` has no other user, so its table line is deleted.

Coordinator: 1/1 landed, RELOCATION_MATCH PASS, 0 LITERAL_DIFF, self applied. With this landing the made-up `?vector_deleting_destructor@@...CLandscapeBackgroundPatch` has no definer left.

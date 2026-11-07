# Landing 2026-11-08h: chunk 859 (Fable retry) round-1 matches

Three of the twenty parked near misses closed; all RELOCATION_MATCH + PASS (read-only vland grade, then `--land`; no headers changed).

| Row | Grade | Note |
|---|---|---|
| 0x00A852B0 CVectorMap<unsigned __int64, std::list<C2DLineF>, CKeyPairCompareLess<...> >::Insert | **RELOCATION_MATCH + PASS, landed** | same-TU noinline bodies of list::_M_insert_dispatch 0x005152C0, the pair copy ctor 0x00A825D0, vector::_M_insert_overflow 0x00A84C20, ~list 0x005127A0 |
| 0x0058301A CTCInventory::CreateEditBoxActions | **RELOCATION_MATCH + PASS, landed** | same-TU noinline list insert / push_back bodies (0x005939EE / 0x0042B646) |
| 0x006A9690 CThingPlayerCreature::ResolveRelativeMovementAcceleration | **RELOCATION_MATCH + PASS, landed** | `float& turn` for push-then-lea order; binds `CThing::PeekWorld` 0x004C7AB0 (GetWorld is 0x004C7AA0) |

- **Names:** `ghidra_out/labels_landing_2026-11-08h.tsv` (the three rows, NFractal::CHeteroTerrainFractal::GetFractalHeight 0x00F36A30,
  DELTA_NOTIONAL_ZERO 0x0129BA3C). Queue `land0811h`: 12 landed placeholder / wrong spellings (PeekWorld 0x004C7AB0, list push_backs 0x005939EE /
  0x0042B646, RotateXY 0x00A14040, AccurateNormalise 0x00A14440, PeekMyPlayer 0x0049D9D0, CCamera::GetAngleXY 0x00A0C380, ...); the catalog keeps
  the landed spellings of 0x004C7AB0 / 0x005939EE / 0x00A229E0 until those respells land.
- Catalog sync restricted to this landing's addresses. `bindings_multi_check.py` prints 1, the pre-existing NullCoord.

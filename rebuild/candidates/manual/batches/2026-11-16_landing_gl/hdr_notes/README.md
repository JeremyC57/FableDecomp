# landing gl header changes

| header | opt-in | what | row |
|---|---|---|---|
| engine/CTCPhysicsBase.h | FABLE_RETAIL_CTCPHYSICSBASE_GETTHINGSEARCHTOOLS (new) | protected `const CThingSearchTools& GetThingSearchTools() const;` (0x006AFD40, Ego_r 0x006EC500 / FableWin 0x0246E010 `?GetThingSearchTools@CTCPhysicsBase@@IBEABVCThingSearchTools@@XZ`) | 0x006AFD40 |

The block sits right after the class head, so the 17 later FABLE_STATIC_ASSERT lines are pinned as `FABLE_STATIC_ASSERT_AT(<old line>, ...)`
(pincheck: 0 mismatches). Default preprocessed text unchanged; whole-tree equivalence in `../logs/equivalence_changed_input_rows.tsv`.

# acctC hand-back c28a (agentC28, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x0061E6D0 | CTCInventoryQuests::ConstructQuestListPC | 12,356 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
Plain C++: no compiler flag, no `#pragma inline_depth`, no assert device. Statement shapes follow the landed twin 0x0061B610
ConstructQuestList; each non-obvious spelling cites its retail address in the source header comment.

Review items for the owner / coordinator:
- New additive opt-in `FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE_CONVERTXY` (inline ConvertVector written through the
  PDB-named ConvertX / ConvertY). Evidence: both names in Ego_r and FableWin; out-of-line 0x0052E580 still matches from the new
  text; FableWin ConvertVector 28i/81b v this text 27i/77b (old text 29i/95b). The ConvertX / ConvertY bodies are INFERRED
  (FableWin 15i/45b each, ours 13i/42b): the row needs the opt-in only for a c2 record-count threshold (two SIB operand orders).
- `hdrdiff/CCountedPointer.h.diff` carries the overlay's unused fitted `RESET_ELSE_*` opt-ins from the map-row work: the row
  does not define them; do not apply them at landing.
- The behaviour test is fact-based (six runs, five failing mutants) on the twin's mock engine; it has no retail call-order table.
- Catalog label fixes seen as RELOC_NAME warnings (e.g. 0x0099EE20 labelled CCharString::ToUpper) are in coordverify.log.

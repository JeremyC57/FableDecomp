# chunk 826 lessons (one line each)

- Check a parked source's local layouts against sizeof padding: a `_padNN[0x100 - 0x92]` after a CThing whose last member is at +0x91 starts at 0x94, putting the next member at +0x104 (0x00960120 read PCurrentAction 4 bytes off for 10 attempts).
- Count calls retail has vs ours (agent826/bin/calls.py): an inline helper called out of line (CDefPointer release, CCharString::IsEmpty) is a budget/forced-inline fix worth 100+ diff lines, not a register residue.
- Standalone bit tests: only a non-bool mask or the if-return form (`if (word & mask) return true; return false;`) gives retail's `test ch,ch ; jns`; every `!= 0` / shift / cast bool spelling gives `shr ; test cl,1`, and the if-return form inside PeekTC/GetTC loses the stored presence bool.
- `AddSpawnedFunction(new X(...), "")`: the implicit "" temp is built before the new-expression; `CCharString("")` explicit builds it after and destroys it first (retail).
- A `rand()` argument next to an implicit CCharString temporary: retail calls rand first, then builds the name, then pushes; write the count as its own statement (0x00ED5F50 md 381 -> 22; chunk 812's angle lesson generalises).
- x87 partial-dead-code sinking below a loop's break test happens when the loop holds a destructible temporary; a named bool for the break condition keeps the products above the calls (mini-proved; 0x00951FB0).
- A const-reference bound to a bool temporary (`const bool& present = HasTC(t)`) keeps a dead spill of the presence bit but narrows a byte-boundary bit to a byte load; a plain bool folds into `test byte`.

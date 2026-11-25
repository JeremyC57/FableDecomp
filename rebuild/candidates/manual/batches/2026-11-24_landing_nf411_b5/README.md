# Landing nf411 b5: 50 StatGetClassTypeName rows spell retail's class-name literal

Each row went from `CCharString((const char*)0x0123...., -1)`, grade `baked_string`, to `CCharString("<Class>", -1)`.

NF411 checked every target against Fable.exe:
- the address is in .rdata, at the start of a NUL-terminated narrow ASCII string;
- the text equals the class name exactly;
- the address is the one retail's own function pushes.

Each test now asserts the string with `strcmp`. LITERALS.tsv in pending/2026-11-25_nf411_handback lists the exact spellings.

Gates (NF411):
- vland: RELOCATION_MATCH PASS on both sides, with 0 LITERAL_DIFF and 0 LITERAL_UNCHECKED.
- objsec: the leaf differs only in the 4-byte field, which is now a DIR32 relocation to the `??_C@` literal. The only added section is the literal itself.
- `baked_string_addresses` returns [].
- r10cmp: every copy is now byte- and reloc-identical to CTCBase::RegisterTCs' copy (R1 tier 1), which closes the R10 debt.
- /FORCE link of binder + rows: 0 -> 0.
- A one-character case mutant is caught by vland, by the linked test, and by r10cmp.

Coordinator:
- 50/50 landed, 0 LITERAL_DIFF.
- reserved_check (default refs, incl. FullDecomp-B): 0 reserved.
- Baselines OK.
- h3 (0x004D4A86) and h4 (0x004D5853) were held in the first run by their T3 lines and landed alone with those lines set aside and restored. T3 nf407 0x004D4A86 `literal` is closed.

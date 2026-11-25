# Baked string addresses (NF388 q10)

569 landed rows cast a retail address to a string pointer (for example `(const char*)0x0129E564`) instead of spelling the literal. They grade MATCH by construction: the address is a plain immediate with no relocation, so neither the relocation-identity check nor the new LITERAL_DIFF check can see them, and the code would not survive relinking at another base. Fixing a row needs no ruling: spell the literal (same bytes, a relocation present, no LITERAL_DIFF). Some targets may be data symbols rather than literals, so decide per row. Whether these rows count as coverage until fixed is the owner's grade question (raised 2026-11-25).

## Owner decision (2026-11-25): separate grade until fixed
These rows are counted under their own grade, `baked_string`. They are excluded from every genuine coverage figure and listed on their own dashboard line. A row returns to `genuine` once its literal is spelled.

Detection is `tools/decomp_pipeline/crawl/purity.py` `baked_string_addresses()`. It finds a cast of a retail address (0x00400000-0x02000000) to a char / wchar_t / unsigned short pointer, an LPCSTR-family type, or a `reinterpret_cast` to one of those, in code only (comments and string literals are ignored). `tools/landed_metrics.py` uses it. At the decision it found 571 rows: 568 of this audit's 569, plus 3 spellings the audit missed (0x007A6DB0 `const char* const`, and 0x007BAD60 / 0x008B4CA0 `reinterpret_cast`). The audit row it no longer finds, 0x004D31C0, was fixed by nf406_b2.

The owner's instruction for the fix: do it correctly, because some of these strings name retail game files (paths, archive / def / script names) that a future recompilation must still load. So every fix spells the string exactly as retail's bytes hold it:
- the same characters, case, path separators and escape sequences;
- narrow or wide as retail stores it;
- an address that is not a NUL-terminated literal (a data object, a table, a string inside a larger buffer) is not turned into a literal. It is modelled as the data symbol it is, or held.

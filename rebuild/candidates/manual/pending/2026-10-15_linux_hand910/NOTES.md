# Chunk 910 (skipped-row ledger: the two functions of catalogue row 009b98b0)

- 009b98b0 CParsedItem::ConvertToString() const: the chunk 151 agent's body (a dense switch over Type 1..4 returning by hidden pointer)
  was already byte-identical; the row only needed splitting so the 128 bytes end at the jump table. Callees GFIntToCharString
  @ 0x0099F830, CCharString::FormatDouble @ 0x009A08B0, the CCharString copy and (const char*, long) constructors.
- 009b9930: a 3-byte `ret 4` right after the jump table with no padding and no references (xref 0). Named NoOp_009b9930 (descriptive).
- The chunk 151 oracle still carried the 144-byte multi-row for 009b98b0 and was removed before grading.

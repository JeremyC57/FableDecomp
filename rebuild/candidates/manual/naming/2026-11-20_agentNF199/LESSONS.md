# NF199 lessons (one line each)
- q4389: for the u64 combatant map, MFIND_DECL (declaration only) matched 7 of 7 rows that called find without a same-TU body; the one
  row whose HEAD needed a same-TU noinline body (0x006EA770) differs under every fable_map.h form (this kept in ebp, node+0x18 not
  materialised): the iterator wrapper changes c2's numbering where the bare node-returning stand-in did not.
- A local stand-in value type with a real header (CCombatantInfo_Retail) can be dropped for engine/CCombatantInfo.h when the header is
  retail-sized (static assert 0x70): 0x006ED9E0 matched first time.
- Tests that declare the row's member and define the stand-in callee link the row object: after the port they need the real
  container (NF195 0x006E9270 test shape); standalone host-model tests need nothing.
- q4451 is not one shape: of 82 rows a strict transformer took 2 (the forced free operator== block); the rest define Compare bodies,
  CBasicStringChar wrappers or member operators, so census the exact local block before writing a transformer.
- q4456's CCharStringMember rows are one shape (14 rows, one payload, all RELOCATION_MATCH): a stand-in member type swapped for the
  header class is the cheapest port in the queue.
- Old tools/ copies carry other agents' paths (agentNF126 / NF130): grep them for agentNF before use.

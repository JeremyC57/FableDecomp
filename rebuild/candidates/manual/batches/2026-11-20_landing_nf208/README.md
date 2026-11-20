# Landing nf208 (Account A, 2026-11-20): family pass FP-VECTORINSERT, vector<CCharString> overflow binders (8 rows)

Agent NF208 ran the first family pass (unit = row; advisor decision, owner delegated, at nf204). It handed back at a row boundary
(context) with 8 rows ported, 2 not-a-binder, and 12 blocked or not reached. Its files are in `naming/2026-11-20_agentNF208/`. The four
entries stay blocked under the family pass and are unclaimed for the next single-agent slice: q6332 / q6341 (4 binders left: 0x004333B3,
0x004B2FE0, 0x004B4A10, 0x00A2ED10) and q6333 / q6342 (0x00485DBC, not started).

- 8 rows land, all RELOCATION_MATCH, every test PASS.
  - The five same-TU-body QAE rows move onto the shared headers and now bind the protected IAE `_M_insert_overflow` at 0x00433530:
    0x008AE5E0 / 0x008AE810 / 0x008AEA40 (CTCAIScratchpad::AddGossip*), 0x006FC8E0 (for_each / CFillEventNames), 0x00CB5C90
    (CScriptInfoManager::AddScript; its stand-in CVectorMap::Add is the Ego_r Insert(key, value) via the existing opt-ins).
  - 0x00A72EE0 / 0x00A72F40: the CCharStringVector stand-ins move onto fable_string.h and the STLport vector (overflow stays
    declaration-only, as at HEAD).
  - 0x0044BFF0: a self rename from a free `__fastcall` overlay stand-in to `vector<CCharString>::push_back`. All 23 landed referencing rows
    bind the new spelling and none binds the old leaf (`work/probe_44bff0.tsv`); the address goes from 2 spellings to 1.
- Coordinator call (the nf184 scaffolding precedent): the brief's "declaration-only first" plan measured DIFFER 92 v 115 on 0x008AE5E0 (the
  `__false_type` tag gets its own push). What matches is the existing FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE plus the owner's own
  FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE. Without the second macro the rows still grade RELOCATION_MATCH, but their overflow copy is
  280 bytes against the owner's 316. With it the copy equals the owner (bytes and relocations), the same reason the nf184 size-pragma
  blocks were accepted. No new FORCEINLINE is added.
- Headers: add-only, declaration-only opt-ins; `orig` equals HEAD; pinned 0; none of the 47 / 9 landed includers defines them:
  engine/CTCAIScratchpad.h ADDGOSSIP / ADDGOSSIPVILLAGES / ADDGOSSIPFACTIONS (FableWin `SIX` spellings); engine/CSymbolMap.h
  FINDFIRSTSYMBOLWITHVAL_VALUE (the one-argument overload 0x009E5090, both PDBs).
- Pick-any (`logs/pickany_check.txt`): the IAE overflow copy in all five QAE rows equals 0x00433530 (316 = 316). The push_back copies
  in 0x00A72EE0 / 0x00A72F40 equal the new 0x0044BFF0 (59 = 59). Placement new is unchanged from HEAD.
- Link: 0 LNK2005, as at HEAD. Recheck: split gate 0 created; 50 good, 0 bad. tablecheck: 0x009E5090 3 -> 2, 0x00433530 5 -> 4.

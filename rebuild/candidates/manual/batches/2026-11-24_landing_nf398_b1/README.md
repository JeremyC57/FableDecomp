# Landing nf398 b1: the map<unsigned __int64, CGroupMemberData> tree (CTCCreatureGroup::Members), 4 rows as one unit

Rows (the unit is 0x004DFB8F; all RELOCATION_MATCH PASS):
- 0x004DFB8F `_Rb_tree::_M_erase` (self respelt);
- 0x004E1BD2 `clear` (NF396's held b3 text);
- 0x00813460 `erase(first, last)`, moved onto stlport/fable_map.h;
- 0x004E350F `~_Rb_tree`.

Table changes (apply_table: 0 misses; multi-target count stays at 1):
- the 4-argument `_M_erase` line is replaced by the STLport spelling (one address, one name; NF396 Q1);
- `?Destroy_004dbe39@CInner` is replaced by `??_G?$pair@$$CB_KVCGroupMemberData@@@std@@QAEPAXI@Z` at 0x004DBE39;
- a `clear -> 004e1bd2` line is added.

Header opt-ins (add-only, off by default):
- stlport/fable_containers.h `FABLE_RETAIL_STLP_DESTROY_FASTCALL`;
- engine/CGroupMemberData.h `FABLE_RETAIL_STLP_GROUP_MEMBER_TREE_CLEAR_SIZE_OWNER`, plus the nested `_DESTROY_SIZE_OWNER`.

Rulings (advisor decision, owner delegated, 2026-11-25):
- Q1, accepted: a narrow extension of the 2026-11-20 size-pragma rule, written up in EXTRA_RULES (2026-11-25). Checked by the coordinator before landing:
  - the owner rows 0x004DBE39, 0x004DFB8F and 0x004E1BD2 each set `#pragma optimize("s",on)` in source;
  - the block ends with `optimize("", on)`, and 0x00813460 has no row-head pragma;
  - CLEAR_SIZE_OWNER on its own changes only `clear` (51 -> 41 bytes), with the leaf RELOCATION_MATCH either way (pending/2026-11-25_nf398_handback/probe_00813460_noclear.cpp);
  - no other users of the removed spellings exist in rebuild/src, rebuild/oracles or rebuild/include.
- Q2: the second `deallocate` body is debt (T3 nf398).
- Q3: 0x004DD6F3 is accepted as the unique tier-2 home, and a raw E8/E9 scan of retail shows nothing calls it.
- 0x004E350F lands with the unit:
  - retail references it only from the call at 0x004E5C77, which follows `lea ecx,[esi+18h]` (Ego_r Members +24 = 0x18), and the `jmp` at 0x004E409B;
  - no other claim, T3 line or lock was open on it.

Gates (NF398): vland HEAD and new; pk_all and pickany_vs_retail on both tables (0 NEQ, 0 reach flips); /FORCE link (duplicates 0 -> 0, unresolved 41 -> 36); census by header reach; equiv_check over 57,158 files.

Coordinator checks after landing:
- 4 of 4 landed;
- each self is defined in its own object (three members have a unit different from their address);
- self_symbols column 4 is aligned to each row's Leaf line;
- 0 LITERAL_DIFF;
- baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- T3 nf396 0x004DFB8F is closed (T3_CLOSED_BY_HAND.tsv), and nf391 0x006CE220 is narrowed.

Follow-ups are T3 nf398 lines:
- port ~CTCCreatureGroup 0x004E5C71;
- re-row 0x004DBE39 so its self is the `??_G` pair (its self and table spellings split until then);
- catalog adds for 0x004DD6F3 and 0x004E409B;
- the deallocate debt;
- relabels and test respells;
- try the 0x0057FEFB lever.

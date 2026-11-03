# 00cdc070 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 2. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00cdc070/NAMING_EVIDENCE.md

# Identity and native evidence

Exact public virtual leaf `?OnPersist@CQ_SunnyvaleMasterData@NScript@@UAEXAAVCPersistContext@@@Z` is present in both original PDB sets. Original class receipts evidence/00cdc070_types.json contain Ego_r11e3a and FableWin11ff4. Native constructor00cdb9e0 installs012c2748; slot1 at012c274c is00cdc070. Constructor uses TeddySolution+54 and BoastRegions+134; native allocator00cdbd20 size144 corroborates the complete reconciled root layout. Full original owner fields are retained. Extent2503 ends RET4 at00cdca34, followed by CC.

Original locals00cdc070_locals.json identify context, NPCAttitudesTransferVector (vector<long>), BooksPreviouslyOpened_Vector (vector<bool>), and long i. Both original PDB globals identify NPCAttitudes as long[3]; native loop walks0143e928 through0143e934. The second absolute relocation carries end-pointer addend12 and is separately audited. Native copies both arrays into temporary vectors, transfers them, and frees them; it does not copy loaded values back. This behavior is intentionally retained.

All81 Transfer/TransferVector operations preserve native field, key, type, order and explicit/default-value selection. Original CPersistContext GetDefaultVal symbols use protected static fastcall KI. Native helpers00cdca40/50/60 and0040e160/240/250/260 return typed zero; CCharString default helper0040e1e0 returns the empty string. The new private CPersistContext opt-in declares the original protected template, without changing default behavior or layout. The ordinary two-argument Transfer wrapper delegates to the original three-argument member and typed GetDefaultVal.

One donor ABI divergence is explicit: both original PDBs name vector<bool>::push_back(bool), with packed20/28-byte bool containers. Retail scripts use the generic12-byte byte vector (GOTCHAS chunk673). Native0053cb4f loads the address parameter, copies the byte through it, and increments finish by1, proving const bool&; therefore its genuine retail generic-template symbol is `?push_back@?$vector@_NV?$allocator@_N@std@@@std@@QAEXAB_N@Z`. The by-value donor symbol is wrong for this body. Original generic vector<long> const-reference method, full bool/long TPI and native helper are preserved in00cdc070_vector_types.json and00cdc070_bool_push_back.disasm. No claim is made that this retail-specific instantiated string occurs verbatim in the newer PDB.

Selected attempt01 RELOCATION_MATCH2503. All212 leaf relocation operands, including exact literal bytes and global end-pointer addend, are checked; applying those actual operands makes all2503 object bytes equal retail. Actual objdump is history/00cdc070/attempt_01/actual_object.disasm. Exact selected-body fixture passes GNU64 C++03 and native VC7.1 /GR-:192 save/load/clear scenarios check all81 operations, defaults, every retained/unpersisted field, signed32 edge values, all8 book masks, vector no-copyback behavior and string ownership. Root migration and independent review remain with main.

Unique-target audit: evidence/00cdc070_UNIQUE_CALLEES.json records all23 unique callee/global identities, existing settled/prior bindings, original typed field and literal-key call-site windows, duplicate-zero disambiguation, native helper bodies, and explicit bool-vector donor ABI divergence.79 scalar transfers plus2 vector transfers are recorded. No source/header/test/grade snapshot changed.

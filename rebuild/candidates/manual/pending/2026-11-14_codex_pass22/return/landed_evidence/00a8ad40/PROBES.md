# 00A8AD40 probes

Read complete best, all3priors, every handback and all6neighbours. Earlier dead ends: ushort count locals, primitive-size-before-main-size, top declarations, term/store order and helper changes. Old sources used fake stream/bank pointer signatures; Q27 rebase was not buildable with supplied headers because its LOADBINARY profiles were absent.

Original both-PDB identities and actual module locals prove private InitialisePointers(unsigned long,unsigned long), not neighbour's guessed public bool overload. Actual locals are is/texture_bank/discard_primitives, static_block_count/anim_block_count, buffer_size/prim_buffer_size/bone_name_buffer_size, three64bit overflow checks, current_bone_name, loop counters, prim_buffer/prim_dest and a CScopedLockFlagBool lock. Baseline family1 restores existing shared base profile and exact missing declarations/access, retaining Q27 control flow for comparison. Source uses portable signed64 spelling under compiler guards; no new inline-control device.

Family1: complete shared profile compiles1621/1621,26strict hunks (two missing declaration/access compile corrections not extra families). Family2: restore original PDB local CScopedLockFlagBool lock with real shared ctor/dtor, replacing manual true/false stores. Native exits confirm scope releases on every return; no inline-control change.

Family3, concrete original-source evidence: donor0336639E..0336671C computes expressions directly into overflow_check_1/2/3. Only after all checks,033668CA..03366930 recomputes all three unsigned sizes. Q27 wrongly named the unsigned size first and assigned it into the check. Restore both genuine evaluations; not an arbitrary extra use. This should change count/size lifetime priority at first differing native ReadUWORD result.

Family3 result1603/1621,5loose hunks: real count scheduling restored but local frame disappeared. Family4 restores donor03366BB3 early discard-success return and puts primitive destination in its original function scope. This tests the concrete lifetime difference of the address-taken destination, not artificial storage.

Family4 MASKED_MATCH:1621/1621,545/545instructions,zero differences. Early discard return extends prim_dest scope to original function scope and restores the exact frame, spills and alignment. No compiler flags or inline-control additions. Full relocation and behavioral closure follows.

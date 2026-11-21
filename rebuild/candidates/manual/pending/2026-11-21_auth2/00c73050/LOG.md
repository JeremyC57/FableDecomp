# 0x00C73050 -- AUTH2 log (continues S/w22/auth1/00c73050/LOG.md; base b0.cpp = AUTH1's, /d2inlT150)
1. c2 hook (advisor step 1; S/w22/auth2/c2x/c2prio_x.py = AUTH1's + C2X_FORCE=a:b, a diagnostic relink of the sorted list at SORTED):
   - f1 C2X_FORCE=32:9 (item #32 just before queue #9, i.e. also before #22/#31): #32 ecx, #31 edx -> DIFFER(670v672) (ecx/edx swapped).
   - f2 C2X_FORCE=22:9,31:9,32:9 (order ... 21, 22, 31, 32, 9, 19 ...): #22 eax, #31 ecx, #32 edx, #9 ebx -> object = RELOCATION_MATCH
     (S/w22/auth2/objcmp.py on the kept object). The causal chain is CONFIRMED: retail colours queue after #22/#31/#32.
   Needed: queue's priority (23) <= 20 (its key 11 loses every tie with 22/31/32/19), with 22/31/32 unchanged.
2. Per-block priority shares (blk.txt, --blocks): queue +20 in block 1 (entry, K 10), +28 in the loop body (block 26, w2 K7), +16 block 30,
   +12 block 35 (QueueNextBuffer), +6/+4 blocks 28/29, +4 block 13; live-through penalties -1..-8 over blocks 2-11, 14-25, 31-33.
   Retail's `lea ebx,[ebp+54h]` sits in block 1 (0x00C73076, between the type cmp and je), so the definition must stay in block 1.
3. Ego_d / Ego_r: no CAudioSource / UpdateStream / Audio_PlaySourceAndProcessQueue anywhere (Ego_d.pdb strings and types.txt: 0 hits;
   egor_pdb_names.tsv: 0). No line table: the text is unconstrained by Ego_d, and the name has no PDB second source (TLC-only OpenAL code).
4. r1 = b0 with `CircularQueue& queue = sound->m_queue;` (reference local, `.` access): DIFFER(672v672) residue 3 (no move; same candidate).

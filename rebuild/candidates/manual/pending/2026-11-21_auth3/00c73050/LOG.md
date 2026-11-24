# 0x00C73050 -- AUTH3 log (continues S/w22/auth2/00c73050/LOG.md; base b0.cpp = AUTH2's, /d2inlT150)
1. Forced runs (advisor step 1): C2X_FORCE=22:9,31:9,32:9,19:9 / +25:9 / +3:9,14:9 (queue after #19, #25, #3, #14): all RELOCATION_MATCH.
   The target is a THRESHOLD: queue anywhere after #32 works (down to below prio 11 at least). Need queue prio <= 20 (key 11 loses ties).
2. Forced fs (C2X_FORCE=...,9:5: sound coloured AFTER queue): DIFFER(673v672), ebx/ebp swap. So a second constraint: sound (prio 28, key 16)
   must stay >= queue. Block-1 K drops hit sound 3x (cost 6 v 2); loop-block (26) K drops hit both by 4.
3. Readouts (no grade unless queue <= 20): v1 switch(type) queue 21 sound 22 (block 1 K 10->9: const 1 left block 1) -- graded once to
   see the code: DIFFER(670v672) (dec/sub instead of cmp 1 / cmp 0x10): dead. v2 queue before ApplySound: 23. v3 SVoiceRequests bitfield
   m_bit7: 24. v4 `now >= m_positionB`: 23. v5 named `bool looping` for QueueNextBuffer: 23.
   Block-1 refs (full readout): import GetProcAddress, &g_AudioContext, const 0, const 1 (cmp type,1), type, queue, sound, now x2, this.
   Consts used both inside the loop and before it appear "referenced" in block 1; loop-only const (alSourceUnqueueBuffers) in preheader 21.
4. v6 (predict: two SourcePlay expansions add candidates in live-through blocks -> queue and sound drop equally; bytes only if c2 cross-jumps)
   v6 readout: queue 12 sound 17, item edx (lever 3 works); graded DIFFER(719v672): only a partial tail merge of the two SourcePlay copies. Dead.
5. v7/v8/v9: resolver bodies in the landed 0x00C575F0 if/else form (all / SourcePlay / Unqueue). Predict: two stores to pfn -> extra refs/candidates in resolver blocks (live-through for queue+sound) -> both drop
6. v10/v11: resolver with a local fn pointer (all / Unqueue only). Predict: local fn + global store split into two candidates in resolver blocks -> queue/sound drop equally
   v10 readout: queue 19, sound 24, tail ecx, item edx, queue ebx, sound ebp (gate passed). v11 (Unqueue only): queue 21, sound 26, item edi.
   GRADE v10 (/d2inlT150): RELOCATION_MATCH. Grades used 3.
   v10 inline ablation (q.py): no flag / T100 525, T130-T144 600, T145..T250 RELOCATION_MATCH, T280/T300 720 -> smallest /d2inlT145.
   CONSISTENCY CHECK (advisor): the same local-fn body out of line: 0x00C575A0 GetSourcei DIFFER(69v70), 0x00C575F0 SourcePlay DIFFER(74v75);
   retail's out-of-line copies (0x00C575A0 / 0x00C576E0 / 0x00C575F0) call `call [esi+68h]` (member reload) = the member-call body.
   => v10's resolver text is CONTRADICTED by retail's own out-of-line copies of the same inline functions: NOT a landable match (evidence only).
   Mechanism learned: extra candidates in the resolver blocks (live-through for queue and sound) lower both equally.
7. v12: resolver with a named `void* proc` for the resolved address, member call kept (out-of-line compatible?). Predict: proc adds a candidate/join in resolver blocks -> both drop
   v12 readout: queue 15, sound 20 (key 16 > 11), tail ecx, item edx, queue ebx, sound ebp. GRADE v12 /d2inlT150: RELOCATION_MATCH (grades 4).
   Consistency (proc form, member call kept): out of line 0x00C575A0 GetSourcei, 0x00C576E0 SourceUnqueueBuffers, 0x00C575F0 SourcePlay:
   all RELOCATION_MATCH; landed inline users rewritten to the proc form (every resolver in the file): 0x00C57C90, 0x00C57D80, 0x00C57AF0,
   0x00C72620, 0x00C72800, 0x00C72A40: all RELOCATION_MATCH. So the proc body is consistent with every copy checked (v10's was not).
   ablation readouts: proc in SourcePlay only 21 / GetSourcei only 21 (item edi) / SourceUnqueueBuffers only 19 (item edx); all three 15.
8. Final 00c73050.cpp = v12 minus the unused `stopped:` label and SVoiceRequests struct, new header: RELOCATION_MATCH at /d2inlT145
   (smallest; T144 600v672 = IsPlaying out of line; T270 720v672). f2 (cast dropped) 669v672 -> cast kept. Test vt.py: PASS.
   MATCHED. NOTES.md, MATCHES.tsv, STATUS.tsv written.
9. Advisor review: consistency sweep widened (0x00C57460 added; every ternary resolver in each file converted, cons/toproc2.py): all 10
   RELOCATION_MATCH. Smallest spelling (proc only in SourceUnqueueBuffers) graded: RELOCATION_MATCH (00c73050_smallest.cpp). Header moved
   to Leaf/Flags/Inline-budget on lines 2-4 with an `// Inferred: named resolved address --` label; test m_current now mirrors the row
   (PASS). State: matched, ruling requested (all three v smallest).

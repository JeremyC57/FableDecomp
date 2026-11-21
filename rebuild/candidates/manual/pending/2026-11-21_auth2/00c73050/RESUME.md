# RESUME NOTE (not a stall: 1 grade of 25 used; returned on context budget) 0x00C73050 Audio_PlaySourceAndProcessQueue / CAudioSource::UpdateStream (672 B) -- AUTH2, 2026-11-21
S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad. Log: S/w22/auth2/00c73050/LOG.md.

## Best source
S/w22/auth2/00c73050/b0.cpp (= AUTH1's b0 = p25N c7_best), /d2inlT150 (`// Inline-budget:` base kept): DIFFER(672v672), 3 lines
(item #32 edi v retail edx at 0xC731D2/D5/F4).

## Advisor step 1 done: the causal chain is CONFIRMED by the c2 hook
- Hook: S/w22/auth2/c2x/c2prio_x.py (AUTH1's kit + C2X_FORCE=a:b, relinks the sorted candidate list at SORTED 0x10783B6B; diagnostic).
- Colouring #32 before #9 AND before #22/#31 (C2X_FORCE=32:9): #32 ecx, #31 edx -> DIFFER(670v672) (wrong).
- Colouring queue #9 after #22, #31, #32 (C2X_FORCE=22:9,31:9,32:9): #22 eax, #31 ecx, #32 edx, #9 ebx -> the object is
  RELOCATION_MATCH against retail (S/w22/auth2/objcmp.py). So the only difference between ours and retail is that queue (prio 23)
  is coloured before the three prio-20 candidates; retail colours it after them.
- Requirement for a natural text: queue's priority <= 20 (its key 11 loses every tie), #22/#31/#32 unchanged at 20 (raising #32 alone
  puts it before #31 and gives the wrong ecx/edx pair, as f1 shows).

## Where queue's 23 comes from (S/w22/auth2/00c73050/blk.txt)
+20 block 1 (entry, K 10: the definition; retail's `lea ebx,[ebp+54h]` 0x00C73076 is in block 1, so it must stay there), +28 the
processed loop body (w 2, K 7: DequeueNext), +16 / +6 / +4 the line-125 condition blocks, +12 QueueNextBuffer, +4 the capacity test;
live-through penalties -1..-8 over blocks 2-11 (the request / 64-bit time check), 14-25 (GetSourcei, loop head) and 31-33.
A drop of 3 needs, e.g., one fewer candidate referenced in block 1 (K 10 -> 8), or queue live through ~3 more K-weighted blocks, or the
loop body referencing a different (forwarded) temp for the queue.

## Ego_d check (advisor step 3)
No CAudioSource / UpdateStream / Audio_PlaySourceAndProcessQueue in Ego_d (strings, types) or Ego_r (names): no line table, so the text
is unconstrained by Ego_d and the name has no PDB second source. Keep the catalogue name unless p25N's has a second source (NAME_FIXES
not written: no two sources).

## Tried (AUTH2)
r1 reference local (`CircularQueue& queue`): 672 residue 3. Earlier negatives: AUTH1 p2/p3/p4 (sound->m_queue spellings CSE back to one
temp prio 23), bf1..bf3; B's 71 condition spellings + 13 loop forms (untargeted: they did not read queue's priority).

## Next round (answered by the brief / rulings; no question pending)
- (a) plain respellings are free (no Ego_d table) but must stay targeted: read queue's priority with the hook (--blocks queue) for each.
- (b) an extra read of `queue` only on the 0x00E9E420 redundant-read terms. (c) an invented loop helper: dropped.
- FIRST: one more forced run with #9 moved further down (after #19, #25, ... e.g. C2X_FORCE=22:9,31:9,32:9,19:9 and lower), since a
  natural prio <= 20 with key 11 puts queue after #19 (key 38) too: learn whether the target is a window or a threshold.
- BEFORE grading (a): read the p25N / p25R tables in rebuild/candidates/manual/pending/2026-11-20_p25_open/00c73050/ so no variant
  repeats a recorded negative.

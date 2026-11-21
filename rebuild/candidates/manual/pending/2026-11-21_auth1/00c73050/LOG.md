# 0x00C73050 Audio_PlaySourceAndProcessQueue (CAudioSource slot 9) -- AUTH1 log
1. b0.cpp = p25N c7_best.cpp (+ /d2inlT150): DIFFER(672v672), residue 3 lines (item temp edi v retail edx at 0xC731D2/D5/F4).
   c2prio_x XCMP dump (x0.json): threshold total_m1 = 3 (g7b dwords [1,1,1,1]); at the three IsLooping tuples (tline 22) the counter is
   #1 this 1, #29 1, #5 sound 2, #9 queue 2, #32 item 3 -> #32 masked to {ebp,esi,edi}. Counter grows with colouring order
   (#1 0th, #29 4th, #5 7th, #9 9th, #32 12th).
2-4. bf1..bf3: CAudioObject flags as bitfields (type:7, bit7:1, looping:1, rest:23); IsLooping returns the bitfield (plain, != 0, == 1);
   type read as the bitfield. Hypothesis: a bitfield extract gives other IL tuples at the IsLooping test (fewer counted), keeping shr/test.
   -> bf1 672v672 residue 3 (no move); bf2 672v672 residue 3; bf3 674v672 residue 20 (worse).
Advisor re-read of x0.json: the counter counts previously coloured candidates in BYTE registers (eax #29, ebx #9) plus itself; esi/ebp
do not count. allowed ids: 1 eax 2 ecx 3 edx 4 ebx 6 ebp 8 edi. #9 queue's allowed set is {ebx} alone (live across calls), so if #32 were
coloured BEFORE #9 its counter would be 2 (no mask -> edx) and #9's mask would be empty (the builder skips an empty intersection, call
0x1077B3C9 + je). Lever: #32 prio 20 v #9 prio 23 -> lower queue's priority (fewer `queue` references, CSE'd back to ebx).
5-7. p2: `sound->m_queue.DequeueNext()` in the loop; p3: `QueueNextBuffer(&sound->m_queue, ...)`; p4: both.
   -> p2/p3/p4 672v672 residue 3. Probe on p4 (x4.txt): the front end CSEs `&sound->m_queue` back into one temp (#26, prio 23, refs 8,
   ebx): queue's priority does not move with its spelling.

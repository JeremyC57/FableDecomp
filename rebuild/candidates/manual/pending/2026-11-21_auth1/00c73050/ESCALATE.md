# ESCALATE 0x00C73050 Audio_PlaySourceAndProcessQueue / CAudioSource::UpdateStream (672 B) -- AUTH1, 2026-11-21

## Best source
`$S/w22/auth1/00c73050/b0.cpp` (= pending/2026-11-20_p25_open/00c73050/p25N/c7_best.cpp), flags /d2inlT150 (or the same bytes with
FABLE_FORCEINLINE GetSourcei / IsPlaying and no flag, p25R src/c7s3/fi.cpp). DIFFER(672v672), 3 differing lines. libpk has no shared
header for CAudioSource / CAudioObject / CircularQueue (local models, as every landed CAudioSource row).

## Remaining diff
```
 mov ecx, dword ptr [ebx + 0x10]        ; queue->m_tail          (#31, ecx both)
 mov edx, dword ptr [ebx]
-mov edx, dword ptr [edx + ecx*4]       ; queue->m_items[tail]   (#32: retail edx, ours edi)
-test edx, edx
+mov edi, dword ptr [edx + ecx*4]
+test edi, edi
 ... test ecx,ecx ; jne ; mov ecx,[ebp+10h] ; shr ecx,8 ; test cl,1 ; je   (IsLooping, tail == 0 path)
 neg eax ; sbb eax, eax
-and eax, edx
+and eax, edi                           ; m_current = capacity ? item : 0
```

## New measurement (c2prio_x.py, C2X_XCMP=1, x0.json / x4.txt in this folder)
- The mask that removes edx from #32 is the 0x107A00CF compare `ctr == total-1`, total = sum of dwords 0x108A7B1C..28 = [1,1,1,1] = 4
  (one per BYTE register eax/ecx/edx/ebx); it is applied at byte tuples (the three IsLooping shr / and tuples) and skipped when the
  intersection with {ebp,esi,edi} would be empty (call 0x1077B3C9 ; je) [INFERRED from reading those instructions, not measured].
- Counter at those tuples, colouring order: #1 this (esi) 1, #29 capacity (eax) 1, #5 sound (ebp) 2, #9 queue (ebx) 2, #32 item 3.
  Reading (fits every value): ctr = 1 + previously coloured candidates live there in BYTE registers (#29 eax, #9 ebx); esi / ebp do not
  count. #31 (tail) is never met at those tuples (no xcmp event) -- it is NOT the interferer (corrects my first reading).
- Register ids in the dump: 1 eax 2 ecx 3 edx 4 ebx 6 ebp 8 edi. #32 choose allowed {edx, edi}, at choice {edi}; #9 allowed {ebx} only.
- Consequence [INFERRED, not measured]: retail's edx needs #32 coloured before #9 (prio 20 v 23: ctr would be 2, and #9's mask would be skipped as empty) or
  #29 not live at the IsLooping tuples (retail keeps capacity in eax across them, so the IL liveness looks the same).

## Tried this pass
p2/p3/p4 (`sound->m_queue` at DequeueNext / QueueNextBuffer / both, to lower queue's priority): 672 residue 3;
the front end CSEs it back into one temp, prio 23 (x4.txt). bf1..bf3: m_flags as bitfields (type:7 / bit7:1 / looping:1 / rest:23), IsLooping = the bitfield (plain / != 0 / == 1), type = the
bitfield: 672 residue 3, 672 residue 3, 674 residue 20. (B's tables in NOTES_p25R.md: 71 condition spellings + 13 loop forms, all edi.)

## Question for the advisor
The bytes agree everywhere but #32's register, and the cause is a pressure counter, not the IL shape. Should I (1) keep hunting a natural text that
raises #32's priority above queue's 23 (or lowers queue's below 20) -- every queue spelling CSEs to the same temp, so it would have to
be an extra no-code reference of the item (a redundant read, the 0x00E9E420 Inferred precedent), or (2) treat this as a c2 numbering tie (like 0x005BFD5E, "parked for good") and leave it
escalated? Also: does the /d2inlT150 base need a TU-flag proof before any match here counts, or is the FORCEINLINE pair acceptable as
the Inline-budget form?

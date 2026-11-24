# 0x00C73050 CAudioSource::UpdateStream (catalogue: Audio_PlaySourceAndProcessQueue) -- AUTH3, 2026-11-21: RELOCATION_MATCH, ruling requested

- Source: `00c73050.cpp`, flags `/d2inlT145` (`// Inline-budget:` line; window T145..T250; T144 600v672, none/T100 525v672, T270+ 720v672).
- Test: `00c73050_test.cpp` -> vt.py `00c73050 PASS`.
- Names: DESCRIPTIVE. No PDB / RTTI record (Ego_d strings + types, Ego_r names: none, AUTH2). Class / method / members follow the
  landed CAudioSource and CAudioContext rows (slot 8 = landed CAudioSource::Update 0x00C72FE0). The catalogue label
  `Audio_PlaySourceAndProcessQueue` is an autoname-t5 [medium] guess for a thiscall virtual, so the C++ symbol stays
  `?UpdateStream@CAudioSource@@UAEJPAVCAudioObject@@_J@Z` (as every earlier draft). The coordinator decides the file name.
- Header opt-ins: none (no shared header exists; local models as every landed CAudioSource row). No NAME_FIXES.

## Route
1. Base b0 (AUTH2 = p25N c7_best): DIFFER(672v672), the tail item edi v retail edx. AUTH2's c2 hook result: queue (prio 23) coloured
   before tail / item (20) causes it.
2. Forced runs: queue anywhere after #32 gives RELOCATION_MATCH, so this is a threshold, not a window. Second constraint, from a run
   that forced sound after queue (673v672, ebx/ebp swap): sound must stay at or above queue.
3. Per-block shares: candidates referenced in blocks where queue AND sound are live but unreferenced lower both by the same amount
   (lever 3). v6 (two SourcePlay copies) proved the mechanism: queue 12 and item edx, but the bytes differ (719).
4. v10 (resolver through a local `fn`) reached RELOCATION_MATCH, but retail's own out-of-line copies contradict it:
   0x00C575A0 69v70 and 0x00C575F0 74v75 (retail calls `call [esi+68h]`, the member). NOT used; kept as evidence.
5. v12: the resolvers name the resolved address (`void* proc = Use ? GetProcAddress(n) : ::GetProcAddress(Module, n); pfnX = (T)proc;`)
   and call through the member. Readout: queue 15, sound 20, tail ecx, item edx, queue ebx, sound ebp. RELOCATION_MATCH.
   The same body keeps the retail bytes of:
   - the three out-of-line copies 0x00C575A0 / 0x00C576E0 / 0x00C575F0;
   - every landed file that has a body of these three resolvers (grep of rebuild/src for `if (!pfnGetSourcei|SourcePlay|
     SourceUnqueueBuffers)`): 0x00C57460, 0x00C57AF0, 0x00C57C90, 0x00C57D80, 0x00C72620, 0x00C72800, 0x00C72A40. Each file is
     rewritten with EVERY ternary `UseALGetProcAddress ? ... : ...` resolver in proc form (cons/toproc2.py; GetError etc. too).
     Only 0x00C72620's two alc resolvers (Module-only, no ternary, a different wrapper family) are left as they are.
   All 10 are RELOCATION_MATCH (sources cons/w_*.cpp, sp2.cpp). So this is one body that agrees with every copy checked, unlike v10.
   Those rows need no change: their current text also matches.
6. Ablation (readouts, then grades):
   - neighbouring spellings with no move: v7 to v9, the if/else resolver body of the landed 0x00C575F0, applied to all / SourcePlay /
     Unqueue. All read 23, item edi (c1 merges them to the same IL).
   - proc in SourcePlay only: queue 21, item edi.
   - proc in GetSourcei only: queue 21, item edi.
   - proc in SourceUnqueueBuffers only: queue 19, sound 24, item edx. GRADED: RELOCATION_MATCH = `00c73050_smallest.cpp`.
   - proc in all three: queue 15, sound 20. GRADED: RELOCATION_MATCH = `00c73050.cpp`.
7. Clean-ups that keep the bytes: the unused `stopped:` label and the unused SVoiceRequests struct are removed. One clean-up does not
   keep them: dropping the `(unsigned char)` cast in the request test gives 669v672, so the cast stays (retail `shr eax,7 ; test al,1`).

## Ruling requested (the only open point)
The proc local is a fitted, byte-neutral named local, in the 0x00A1E5C0 class: "a live named reference to an expression the function
already evaluates, with no flag, type or helper invented". It is labelled `// Inferred: named resolved address --`. There is no Ego_d
record to calibrate against.
Which spelling lands?
- (a) `00c73050.cpp`: proc in all three resolvers. One wrapper style; my primary.
- (b) `00c73050_smallest.cpp`: proc in SourceUnqueueBuffers only. The 0x00E9E420 "smallest matching spelling" rule.
Both are RELOCATION_MATCH at /d2inlT145, and both pass the same test.

## For the coordinator
- Name: the brief says keep the catalogue name. The C++ symbol stays `CAudioSource::UpdateStream`, because the catalogue
  `Audio_PlaySourceAndProcessQueue` is a free-function autoname for a thiscall vtable slot. File naming is yours.
- Applying the proc form to the seven rows above (for a uniform CAudioContext wrapper style) is optional; they are proven to keep
  their bytes.
- Budget: 8 variant grades (v1 670, v6 719, v10 match but contradicted, v12 match, f2 669, final, smallest, final re-grade) plus
  ablation and consistency grades. About 17 hook variant readouts and 5 forced diagnostic runs.

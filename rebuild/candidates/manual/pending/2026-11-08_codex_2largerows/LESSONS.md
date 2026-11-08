# New lessons from this continuation

1. Above VC7.1's large-function threshold, hiding the real counted-pointer constructor body behind a declaration can prevent an eight-byte local sharing its native frame slot. Exposing the existing faithful body while preserving out-of-line calls fixed Action's frame and one constant-register sequence. An invented body is not an acceptable substitute.
2. A four-byte frame excess can be boolean padding rather than an extra scalar. Map the generated local slots back to native uses. In SetFlag, assigning the flag in each real branch increased its live-reference rank, removed padding and restored the native store timing after IsFalse.
3. A named string can correctly make the output larger: PHOTO_CAPTION needed a missing lea, not a shorter direct push of the return pointer.
4. Receiver caching must follow native reload timing. Caching PInventoryTrade before FindChild shortened code but was wrong; caches inside successful branches match the reloads.
5. Check source shape before selecting the numerically closest size. Several variants got shorter by losing native cleanup machinery or spilling different float components.
6. A diagnostic compiler trace is evidence only after its selected section matches an independent ordinary compiler build. Never use instrumentation output as the grade.
7. Preserve provenance when a newer handoff arrives. The 13 named-bool changes already in incoming v23 must not be counted a second time.
8. Final selected sources remain parked. Exact frames and high instruction similarity do not imply byte identity, correct relocation binding, or passing behavior tests.

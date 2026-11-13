## 0x004093E0

- (wf21, 0x004093E0) A small constant-trip `for (i = 1; i < 4; ++i)` loop that retail keeps rolled is a `do { } while (++i < 4)`: VC7.1 fully unrolls the for form, and the unrolled copy reshuffles the whole function's register allocation.
- (wf21, 0x004093E0) A vector walk whose head is `xor edx,edx ; mov [res],edx ; mov eax,[begin] ; cmp eax,[end]` reads the vector through a local const reference from the class's same-TU accessor (`const std::vector<T>& modes = GetDisplayModes();`), not the member directly.
- (wf21, 0x004093E0) VC7.1 aligns a loop head only when the pad is at most 9 bytes (retail: 6-byte pad kept, 10/12-byte pads skipped); a pad-only residue moves with the block layout.
- (wf21 #3, 2026-11-13) A def arm laid out after a later loop's back-jmp (null arm falling into the join) = one assignment moved out of the null arm into a guarded statement after the if-else (`if (def.IsNull()) x = 0;`), threaded by c2 (0x004093E0).


# Sprite viewport float materialization

These two partial builds reproduce the 4,600-byte retail extent with different instruction schedules. The selected row has 1,190 of 1,198 aligned instructions with 61 positional masked byte differences. The two controls preserve the exact 112-byte transition helper, the three mapped inline copies, all 155 linked operands, all 70 local control transfers and all four Direct3D virtual slots. Their complete native and portable fixtures pass 6,144 render cases and six primitive conversions.

| Control | Aligned instructions | Positional masked differences |
| --- | ---: | ---: |
| `symmetric` materializes both extent differences | 1,184 | 42 |
| `asymmetric` materializes only width | 1,185 | 36 |

The raw FableWin getters at `0x01A66530` and `0x01A66560` return through unnamed float scratch slots. Their `lib_box_f.hpp` line records are 86 and 87, each with only `this` in the local records. Optimized Ego_r stores the height subtraction `BRY - TLY` at `0x00A6256B` and the width subtraction `BRX - TLX` at `0x00A6258D` through the same float scratch. Both feed the viewport upload at `0x00A625A4`. The height reload and width memory multiply are different machine schedules for the same precision boundary. Retail VC7.1 keeps both subtractions through the scales on x87 until the two argument stores at `0x00BAB367` and `0x00BAB37C`.

The symmetric control spells that donor boundary using one unnamed C++03 float temporary in each getter. The asymmetric control isolates the scheduler effect. Switching its active axis from width to height produced identical native leaf bytes. The donor does not establish an asymmetric source expression.

## Replay

Start each control from an independent copy of the original Pass 26 handoff `include/` tree. Apply the checkpoint's top-level `hdr/PATCHES.json` in order, then apply the corresponding `alternate/<control>/PATCHES.json`. Each control adds one guarded replacement in `engine/C2DBoxF.h`. Each nonempty `old_text` must occur exactly once in the target file. An empty `old_text` creates a file.

Use the control's `00baa870.cpp` and its `// Leaf:` and `// Flags:` entries for native VC7.1 grading. Compile the control's `00baa870_test.cpp` against that same patched tree with `/Od /W3 /GR-`. `GRADE.json` records the top-level patch fingerprint, the source and leaf digests, and the unchanged relocation sequence. `AUDIT.json` records all linked operand, local transfer, Direct3D and helper checks. `TEST_RESULT.json` identifies the exact source, fixture, compiler flags and replayed header. The portable fixture uses the recorded GCC C++03 flags and sanitizer environment.

# Landing 2026-11-08r: chunk 868 (Fable retry of ten parks), 2 matches

Both RELOCATION_MATCH + PASS. Neither needed a header change. Hand-back: `pending/2026-11-08_linux_hand868/`.

| Row | Device |
|---|---|
| 0x00D18DF0 NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main | per-row flag `/d2inlS15` (the smallest N that expands both implicit MovieBase dtor sites; same TU and N as the landed 0x00D17F20 / 0x00D19BD0) plus the follow loop's terminate check at the loop top. VC7.1 inverts the loop and peels that check, and the peeled return is the one that joins retail's speech-pad tail (the last residue in chunk 865, a single jne displacement). |
| 0x00ECBDF0 NScript::CV_SickChildScript::CTalkingTrader1::Main | plain C++: a `do { if (T) return; } while (0)` terminate check at the main loop head (EXTRA_RULES 2026-11-03 lever; line 3 notes that retail's construct is unproven). It makes the loop exit hot (~CTimer expanded) and the hit-block speech returns cold (the shared ~CTimer call), as retail. |

- **Parked (8):** 0x00D11930 (24 lines, one 0.5-frequency MovieBase dtor site; every /d2inlS permutes the frame), 0x00DCFA60 (41),
  0x00B92300 / 0x00C52BB0 / 0x00ECE7F0 (4 lines each), 0x00D45060 (375, baseline only), 0x007A1B20 (rebuilt on shared headers,
  490 lines), and 0x00A2F740 (not rebuilt: the mesh-file chunk finders and the C3DMeshStats headers do not exist yet).
- **Names:** no change (every label the agent proved was already correct). vland's two RELOC_NAME warnings are queued in land0811m.
- Test gotcha: under VC7.1 `World w = World();` leaves the scalar members of a class with an implicit ctor and std::vector members
  unset (g++ zeroes them): give the test model an explicit zeroing ctor.

# Landing hand3c3 b5: 0x0077A430 CTCDExperienceOrb::FrameUpdate (Account B, 2026-11-20)

RELOCATION_MATCH, test PASS, strict vland `--land`. Audited by the rolling auditor ($S/stage/audit_roll).

- **TU contents:** the row's TU carries the GetAllTCDsInRoughArea<CTCDExperienceOrb,...> instance 0x0077ADF0, using the landed row's define set.
  It grades RELOCATION_MATCH from the same compile.
- **What closed the row:** `<=` in the touch test, marked `// Inferred:`, plus the existing C3DVector opt-ins NORMALISE_SQUARE_HELPER and
  NONZERO_SQUARED_XYZ.
- **Audit changes:**
  - GAME_ACTION_ATTRACT_EXPERIENCE_ORBS replaces a cast;
  - one test line was added so the touch-radius mutant, which the delivered test let through, is now killed.
- **Mutants:** 93/93.
- **Headers** (hdrcheck2 default=same; the only DIFF is the new opt-in, which no landed row defines; no new header, so no landed row is
  newly flagged):
  - CTCDExperienceOrb.h FABLE_RETAIL_CTCDEXPERIENCEORB_FRAMEUPDATE: virtual FrameUpdate and the private CombineWithExperienceOrb(CThing&);
  - CPlayer.h FABLE_RETAIL_PLAYER_PEEKPLAYERINTERFACE: the const accessor on its own.
- **Note for the C3DVector.h owner:** the default C3DVector::IsNonZero (no opt-in) is a fitted three-statement running sum, while Ego_d has it
  as one statement. Not changed here.
- **Name fixes:**
  - 0x0077A130 is the private CombineWithExperienceOrb(CThing&); the landed local model takes CThing*;
  - 0x0129BA3C is still labelled g_NormaliseEpsilon.

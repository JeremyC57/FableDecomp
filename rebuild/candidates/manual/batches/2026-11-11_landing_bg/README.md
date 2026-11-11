# 2026-11-11 landing bg -- chunk 1023: 4 of 7 (two parked, one phantom)

Source: `pending/2026-11-11_linux_hand1023/` (NOTES, STATUS, LESSONS, LABELS, NAME_FIXES, SPLITS, ATTEMPTS, differ/, hdr/libpk drafts).
vland --land: 4 wins (RELOCATION_MATCH), tests PASS.
- 0x006E3EC0 ?Initialise@NSpeechGainManager@@YIXABVCWorld@@@Z (catalog placeholder CWorld_InitGuiTreePane): new engine/NSpeechGainManager.h.
  Inline ladder: plain C++ and a /d2inlT scan failed (no threshold expands the CResponseTimes(float,float) ctor but not vector::erase;
  T77 erase expanded / ctor called, T80+ both expanded), so the ctor carries FABLE_FORCEINLINE with an `// Inline-budget:` note. Debt: the
  forceinline is unconditional in the new header (its only user); move it under an opt-in if another row includes the header.
- 0x007CB8B0 ?OnSerialise@CTCHeroReceiveItems@@MAEXAAVCPersistContext@@_N@Z (placeholder Save_TransferPickedUpItems), 508 bytes
  (catalog 608 split; BOUNDS_FIX).
- 0x0061A6A0 ?OnCreate@CTCInventoryQuests@@UAEXXZ and 0x00B67270 ?BuildBlendingTables@CEngineLandscapeRenderer@@AAEXXZ (the two rows
  chunk 1022 never graded; 0x00B67270 has a `// Retail-asm:` line for GFFloatToLongFloor).
Parked: 0x00417747 UpdateForcedTickSpeed DIFFER(600v596) -- retail's `fld [m]; fld1; fdivrp st(1)` reciprocal pair; 20+ probes (constants,
helpers, pow, /G5-/G7, /Op, /O2) found no C++ spelling; only a float carried in a register across a loop (std::_Pow_int's shape) gives it.
0x00C1C460 (libpk positional-sound update; owner class unproven, draft headers kept in the pending folder, NOT landed) DIFFER(445v445),
an edi/ebx swap + one 3-dword copy schedule. 0x0074B77C is a phantom (interior of 0x0074B770; BOUNDS_FIX skip).
Headers: base 4f0189cb3, clean merge, hdrcheck2 default=same; new CQuestCardItem.h, NSpeechGainManager.h. Queued land1011bg: 4 name fixes
+ two to settle (0x00C1C460 bound three ways by landed callers; 0x00C33FD0 label looks propagated).

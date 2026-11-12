# Chunk H40k notes (eleventh pass, 22 rows)

Every grade used the header overlay `FABLE_HDR_OVERLAY=$A/hdr` (gr.sh / tools/pg.sh / tools/sweep.sh). No hdr/ change this pass.
51 grades in ATTEMPTS.log (one line each). No row matched; src/ and test/ are empty. Every row of ROWS.tsv has a differ/ file;
lines 2 / 3 of each carry an appended "H40k" note with what was graded.

## Improved
- **0x00D13580** NScript::CQ_BountyHuntScript::Main: 119 -> 106 masked lines (DIFFER(6875v6917)).
  - Lever: per-row `// Flags: /d2inlT75`. The inline trace (tools/inl.sh) scored the cutscene-1 movie dtor s74 against T74,
    one point over; retail expands it at 0x00D13D94. Owner ladder step 2 (plain C++ had no spelling that moves that site
    alone; `if (T && true) return` moves slots, H40j). The differ draft now spells FABLE_FORCEINLINE.
  - Left, measured:
    (a) cutscene 2 (0x00D14518..0x00D14649): retail EXPANDS that movie dtor too (`call 0x006E7AB0 ; GROBase vtable store ;
        call 0x0099A430` at 0x00D14607), ours calls ~MovieBase: the site's frequency is f0.1 (s3722 at c19), it needs
        f >= 4.6. The frequency collapses across the lookout loop (break exit nested in region test / IsAlive && text test /
        T check) and the four post-loop T checks. A call there leaves the movie weight 6 < the three STBs' 7, so the
        packer gives cutscene 2's movie a new slot (+0x118) and hostage1 the shared +0xE8 slot; retail: movie +0xE8
        (shared with cutscene 1's), hostages +0xF8 / +0x108 / +0x118. Fixing the frequency should fix both.
        Probes (no grade, inl.sh): all wait loops as `for(;;){if(c)break;NSF;if(T)return;}` = same f;
        `&& true` on all 38 T checks = cutscene 1 f12.3, cutscene 2 f0.6 (still far).
    (b) main quest loop: ours rotates `while (!QS && !QF)` (QS test copied to the bottom, 0x00D14B2C); retail's bottom is
        `T ; je head ; jmp exit` (0x00D14B5A). `for(;;){if(QS||QF)break;...}` is byte-identical to the while;
        `if (!T) continue; return;` gives a separate exit with every dtor expanded (9049 bytes, 1238 lines).
    (c) the inline CCharString == text arms: retail places the non-null arm of the "FromLookoutPoint" compare after the
        `ret` (0x00D13F92) and reloads the string pointer at 0x00D14858; plus alignment pads that follow the loop layout.

## Measured, no gain
- **0x00DB97A0** (125): the cross-jump residue is layout, not frequency. Retail lays the outro (0x00DBB0E4..) directly before
  the exit block 0x00DBB2E8 and ends it with a bare `call IsActiveThreadTerminating` (the post-loop if / return folds), so the
  T-return pads (`pause dtor ; call ~movie 0x006E7B80 ; lea ecx,[esp+10h] ; call ~item ; jmp 0x00DBB2E8`) have no fall-through
  twin and stay apart. Ours puts the exit block (0x00DBB045) before the outro, right after the last pad, and c2 cross-jumps
  every pad into that pad's tail (`jmp 0x00DBB037`). `|| false` / `&& true` on the outro test are byte-identical.
  Next: what makes c2 order the outro before the exit block.
- **0x00E50FD0** (99): the second entry zero store (0x00E50FE8, from ebp) and the first (0x00E50FDF, from ebx) write the same
  frame word: the conditional-temporary flags home that the `CCharString("")` temporary of the first AddSpawnedFunction
  reuses (0x00E510A9 / destroyed 0x00E510BC). A function-scope `long answer = 0;` is removed as a dead store (byte-identical).
- **0x00E5FD40** (163): the ~CScriptThing call / expand split is already retail's (inline trace: s46 / s64 expand, s78 x2 /
  s882 x2 call). The residue is block placement (retail's 0x00E610AF..0x00E61118 block sits earlier) and the movie exit at
  0x00E607F9, which retail cross-jumps into the shared timer-dereg tail 0x00E6213C while ours keeps a private epilogue.
- /d2inlT73 / T75 sweep over 0x00E3BB70, 0x00E33EA0, 0x00E4B5B0, 0x00CF68C0, 0x00E20600, 0x00E50FD0, 0x00D0F640,
  0x00E5FD40, 0x00D973D0: byte-identical everywhere (no site near the threshold); 0x00DB97A0 T73 worse.
- Row-local `inline CScriptThing::~CScriptThing() { }` (the 0x00E5FD40 lever) on 0x00E3BB70 / 0x00E4B5B0 / 0x00E50FD0:
  neutral; on 0x00D973D0 / 0x00E33EA0: worse (560 / 565).
- 0x00507C30: `buffer[file.GetLength()] = 0` instead of `text[...]` worse (51 lines).

## Baseline only (unchanged)
0x00C4F600 85, 0x00F0FB70 895, 0x0061E6D0 1295, 0x00B10B10 788, 0x00B03CA0 1022, 0x00DF6010 1486, 0x006096E0 902,
0x00502E90 SRC_FAIL (11 errors, hand879 opt-ins still not ported: CThingAICreature RELATIONS, CMainGameComponent
_SAMPLEBANK_WORLDSEED rename, CThingBuilding _PEEKSIMDEF rename, CWorld POPULATE / PeekDefinitionManager / World),
0x0078FD60 SRC_FAIL. 0x00603000 not graded (coordinator: skip).

## Identities / names / splits
No new identities. LABELS.tsv, NAME_FIXES.tsv and SPLITS.tsv are the earlier passes' files, unchanged.

## Tools added
- tools/bdis.py `<addr> <from> <to>`: disassemble the last q.py build of a row at retail addresses (needs env.sh sourced).

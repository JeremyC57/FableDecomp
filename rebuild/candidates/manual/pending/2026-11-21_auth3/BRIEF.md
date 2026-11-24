# AUTH3: Account A authoring agent, round 3 (2026-11-21): 0x00C73050 ONLY

Owner orders, unchanged: do not park; keep working until the row matches. Stalls go to the advisor through the coordinator.
You have exactly ONE row: 0x00C73050 Audio_PlaySourceAndProcessQueue (CAudioSource::UpdateStream, 672 B). Take it to a terminal state
(matched, or a true stall per the rule below) and nothing else. Two agents before you ran out of context on multi-row briefs: be frugal.

## Read first
1. `$S/w22/auth1/BRIEF.md` (Paths, Tools, Hard rules, Hand-back: all apply; your outputs go to `$S/w22/auth3/`), with
   `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.
2. `$S/w22/auth2/00c73050/RESUME.md` (your starting point) and `LOG.md`, `blk.txt`; `$S/w22/auth1/00c73050/ESCALATE.md`.
   Hook kit: `$S/w22/auth2/c2x/c2prio_x.py` (C2X_FORCE, priority readout `--blocks`), `$S/w22/auth2/objcmp.py`.
3. The p25N / p25R negatives: `rebuild/candidates/manual/pending/2026-11-20_p25_open/00c73050/` (all notes there). Never repeat a recorded negative.
4. `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` (redundant-read terms, the 0x00E9E420 precedent) binds.

## Known (confirmed by the c2 hook, AUTH2)
Best `$S/w22/auth2/00c73050/b0.cpp` with /d2inlT150 (keep it, with an `// Inline-budget:` line): DIFFER(672v672), 3 lines (item #32 edi v
retail edx). Colouring queue #9 (prio 23) AFTER #22/#31/#32 (prio 20) gives a RELOCATION_MATCH object. A natural text needs queue's
priority <= 20 with its definition still in block 1, and #22/#31/#32 unchanged.

## Method (advisor, 2026-11-21)
1. FIRST one forced run with #9 moved further down (e.g. C2X_FORCE=22:9,31:9,32:9,19:9 and lower): is the target a window or a threshold?
2. Then targeted respellings. For EVERY variant, read queue's priority with the hook's readout BEFORE spending a grade; grade only variants
   whose readout reaches the target. Ego_d has no record of this function, so text is unconstrained (but must be sensible, natural C++).
   An extra read of `queue` only on the 0x00E9E420 redundant-read terms. No invented helpers, no devices.
3. Name: keep the catalogue name; it is descriptive (no PDB second source). Mark it descriptive in NOTES.

## Stall rule
25 graded variants with no residue reduction, OR 40 hook readouts with no variant reaching priority <= 20, OR every idea left breaks a rule:
write `$S/w22/auth3/00c73050/ESCALATE.md` and return. On a match: source + `_test.cpp` (prints `00c73050_TEST PASS`, check with vt.py) +
NOTES.md + MATCHES.tsv line, then return. Keep STATUS.tsv / LOG.md current after every step. Never write into /home/user/FableDecomp.

# AUTH4: Account A authoring agent (2026-11-21): 0x00A4EFC0 ONLY

Owner orders, unchanged: do not park; keep working until the row matches. Stalls go to the advisor through the coordinator.
ONE row: 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (3491 B). Take it to a terminal state and nothing else. Be frugal
with context (earlier agents ran out): residue counts and differing hunks only, never whole listings.

## Read first
1. `$S/w22/auth1/BRIEF.md` (Paths, Tools, Hard rules, Hand-back: all apply; your outputs go to `$S/w22/auth4/`),
   `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.
2. This row's history: `$S/w22/auth1/00a4efc0/{ESCALATE,LOG}.md`, `$S/w22/auth2/00a4efc0/{ESCALATE,LOG}.md` (committed copies:
   `rebuild/candidates/manual/pending/2026-11-21_auth1/`, `_auth2/`).
3. HOW the last row was matched: `rebuild/candidates/manual/batches/2026-11-21_landing_auth3/README.md`,
   `rebuild/candidates/manual/pending/2026-11-21_auth3/00c73050/{LOG,NOTES}.md`, and the GOTCHAS line "(landing auth3, 2026-11-21)".
   The c2 hook kit: `$S/w22/auth2/c2x/c2prio_x.py` (priority readout per candidate, `--blocks`; C2X_FORCE to force colouring order),
   `$S/w22/auth2/objcmp.py`.
4. `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` binds (fitted assert arms, Inferred locals: the 0x00E9E420 / 0x00A1E5C0 terms).

## Facts to respect
- Ego_d and FableWin are VC10 builds: their line sizes BOUND a text but do not select one; do not try to calibrate helper texts by size
  (AUTH2 closed that lever). Locals, types and symbol spellings from Ego_d still count as evidence.
- Start from AUTH2's V1 draft (`$S/w22/auth2/00a4efc0/a2.cpp`, /Gr, overlay `$S/w22/auth2/00a4efc0/hdr`, 16 lines). The V1 AllocateType
  opt-in must get a NEW macro name (the old name `FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD` holds AUTH1's rejected, fitted text in the
  committed record): use e.g. `FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_IFBODY`. V1 is a CANDIDATE: do not refit 0x00A50510 unless the row
  is one step from matching. Rule-clean fallback: `$S/w22/auth1/00a4efc0/a1.cpp` (26 lines).

## Method (advisor, 2026-11-21)
1. Run the c2 hook on the row: dump the colouring priorities / symbol numbers of the position block's compressed / uncompressed size temps
   (the residue at 0xA4F7DF..0xA4F810) and of the equivalent scaling / rotation block temps (which match). Find what differs.
2. Force the order with C2X_FORCE to prove which change gives retail's object (objcmp.py). Only then hunt a natural text.
3. Try the INLINED HELPERS' texts first (that is where the 0x00C73050 fix was): a text change inside an expanded helper is admissible only
   if every landed owner and inliner of that helper still regrades equal with it (check them all, as AUTH3 did in its cons/ folder).
4. Score each variant with the hook's readout before spending a grade on it.

## Stall rule
25 graded variants with no residue reduction, OR 40 hook readouts with no variant reaching the target, OR every idea left breaks a rule:
write `$S/w22/auth4/00a4efc0/ESCALATE.md` and return. On a match: source + `_test.cpp` + NOTES.md + MATCHES.tsv line + the list of
helper users you regraded, then return. Keep STATUS.tsv / LOG.md current. Never write into /home/user/FableDecomp.

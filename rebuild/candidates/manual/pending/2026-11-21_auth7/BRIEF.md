# AUTH7: Account A authoring agent (2026-11-21): 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData, the ONE-SHOT step

Owner orders: do not park; keep working until the row matches; stalls go to the advisor through the coordinator. Locks read AUTH7.
ONE row. Be frugal with context: residue counts and differing hunks only.

## Read first
1. `$S/w22/auth1/BRIEF.md` (Paths, Tools, Hard rules, Hand-back apply; outputs go to `$S/w22/auth7/`),
   `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.
2. The row's history: `rebuild/candidates/manual/pending/2026-11-21_auth1/`, `_auth2/`, `_auth4/` READMEs and their `00a4efc0/`
   (ESCALATE, LOG); AUTH4's working files `$S/w22/auth4/00a4efc0/` (a2.cpp, hdr, IL-key kit `$S/w22/auth4/tools/il/`, `ord.sh`, `c2x/`).
3. The VC10 evidence lane: docs/BUILDING.md "VC10 SP1", odall10.py docstring, FINDINGS_LOG 2026-11-21 VC10 entries. New since AUTH4: the
   helper texts now calibrate EXACTLY against Ego_d (SetScaling 34, GetMaximumAbsoluteComponent 259, GetBoneName 49, C3DVectorWord::operator==
   86, SetupIdentityFlag 97: the earlier 47 / 82 gaps were a parser bug), and `--stl` reproduces Ego_d's Dinkumware STL. So the
   "unresolved" VC10 note in the auth4 README is now resolvable.
4. ADVISOR_DECISIONS_B (Inferred-local terms 0x00E9E420 / 0x00A1E5C0; the 0x008DBC80 size test).

## The step (advisor ruling, binding: NO open search)
The residue is c2's tree-order hash tie on the position block's `compressed_size < uncompressed_size` operands; any text change re-rolls it,
so a variant chosen because the readout flips is fitted by construction.
1. Measure first with VC10 (odall10.py, the row's Ego_d library preset; `--stl` standalone for any STL statement): find every place where
   the current best text (a2.cpp) differs from Ego_d in line sizes, named locals (S_BPREL32 / block tree) or symbol spellings. Each
   correction must have its own Ego_d evidence (exact VC10 line size, a named local, a block scope), never the hash readout.
2. Apply ALL such corrections as ONE set, grade once (VC7.1, rg1.sh / q.py) and read the order once with the hook.
3. If it matches: land-ready hand-back (source + test + NOTES + MATCHES.tsv). If not: test whether fields #2493 / #2484 move under an
   unrelated declaration-only opt-in (header set dependence). Then write ESCALATE.md with the result (fitted-dead-temporary ruling or owner).
Do not iterate on the hash. Keep STATUS.tsv / LOG.md current. Never write into /home/user/FableDecomp.

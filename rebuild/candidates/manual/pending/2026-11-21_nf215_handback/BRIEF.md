# Name-fix agent NF215 (2026-11-21, round 10 continued): the advisor-decided held items, then the ??8CBasicStringChar family

Owner (2026-11-21): "start an agent to continue the naming". This supersedes the HANDOFF line "no more agents or naming for now".
You are the ONLY naming agent. One authoring agent, AUTH1, runs at the same time (locks `AUTH1` in `$S/w22/nflocks/`; its overlay
`$S/w22/auth1/hdr/` is read-only for you). Account B's rows are all released; there is no `acctB` lock left. Account A = branch FullDecomp.

## Your brief is NF214's, with the overrides below
Read `$S/w22/agentNF215/NF214_BRIEF.md` in full (the NF214 brief, copied from
`rebuild/candidates/manual/pending/2026-11-21_nf214_handback/BRIEF.md`) and everything it says to read first. All of it applies, with these
substitutions: every `agentNF214` path becomes `$S/w22/agentNF215`, `NF214` becomes `NF215`, and the sections named below are replaced.
Ignore its Account B lines (no B rows remain; "never author a new byte match" still holds: record `blocked: authoring`).

## Lane (replaces NF214's "Lane" section: this container's scratch differs)
Start every Bash call with:

    export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp

- `$S/tools/` holds current copies of the repo's `tools/decomp_pipeline/linux_lane/lane_tools/` (q.py, vland.py, recheck.py, xdis.py, fg.py, vt.py ...).
- `$S/w22/nftools/` here is an OLDER set: `callers.py <addr>...` (E8/E9 sites mapped to landed rows), `absrefs.py <addr-file>` (absolute
  references mapped to landed rows), `mkpay.py <out.json> <addr>...` (recheck payload from the LANDED rows: build your own payload for
  rows/<addr>.cpp by editing its source_cpp/test_cpp), `xref.py <addr> <obj> [leaf]`, `apply_rows.py`, `xrow.sh`. There is NO refs.py,
  gate.py or selfcheck.py here. Build REFS.tsv yourself (callers.py + absrefs.py; check it reproduces a known census, e.g. the 11 referencers
  of 0x00485DBC NF213 counted, before trusting it). Do the selfcheck by hand: every hand-back row RELOCATION_MATCH + PASS in recheck_final.log,
  the defined-symbol diff (objdump -t) vs HEAD, no new split (REFS census per renamed address), RENAMES section 1/2 complete.
- Grade one row: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]` (add
  `FABLE_HDR_OVERLAY=$S/w22/agentNF215/hdr` when you use your overlay; `LANDVERIFY_WORK=$S/w22/agentNF215/wd1` for vland runs).
- Grade + test for every handed-back row: a payload with your rows (source + test) through
  `timeout 1800 python3 -u $S/tools/vland.py <payload.json> <oracle.tsv> --recheck-landed` (oracle rows for landed addresses come from
  `rebuild/oracles/auto-re-candidates.tsv`). Must say MATCH or RELOCATION_MATCH and PASS.
- Disk is tight (~6 GB free, fixed allowance): delete each `wd*` dir once its log is saved. One compile at a time, inside `timeout`.
  Never run git. Never write under /home/user/FableDecomp. Never land anything.

## NF214's hand-back is being landed by the coordinator now
Its rows (0x00A2ED10 0x00A2F0F0 0x00A2F120 0x00A2F150 0x007E4B60 0x006116E0 0x00485DBC 0x0048514C) are locked `NF214`: do not touch them.
Its header opt-ins (`pending/2026-11-21_nf214_handback/hdr/`: engine/C3DMeshStats.h, stlport/fable_containers.h) will land in the repo
during your run. If you need to overlay either of those two headers, base your copy on the NF214 hdr/ version, not on HEAD, and say so.

## NF215 specifics (advisor decisions, owner delegated, 2026-11-21; ENTRIES.tsv has the queue lines)
Rows locked to you: rows.txt (0x0096FA90 0x006E9310 0x006EA050 0x006D41F0 0x006D4180 0x006ED9E0 0x004115A0 0x00EE2A80). Claim others with plain mkdir.
Earlier drafts: grep each address in `rebuild/candidates/manual/naming/2026-11-2*_agentNF*/` (NOTES.md, STATUS.tsv, work/) and FINDINGS_LOG.
1. **q12300** (0x006E9310 CCombatManager::IsAttackerOn): the held binder 0x0096FA90 needs NO cast: its targeting call is vtable slot 29 =
   `GetPBestEnemyCreatureForAttack`, returning `CThingCreatureBase*`. 0x0096FA90 binds 0x006E9310 and 0x006EA050; move them together
   (no new split).
2. **q11760 / q11873** (0x006D41F0 CPersistTraits<pair<CCharString,long>>::TransferIn, held with its caller 0x006D4180 for the inline budget):
   try a plain inline `CCharString::operator!=(const char*)` (both PDBs spell it) for the `"("` test. It is a header decision: if it needs a
   new member in fable_string.h, make it an add-only member-specific opt-in; if it needs a changed member, record "needs header claim".
3. **q12339** (was mislabelled q12340) (0x006ED9E0 CCombatManager::DeregisterDestroyedCombatant): filler. Respell its local `IsCombatant() // 0x00662430` binder onto
   `?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ` (the shared header opt-in FABLE_RETAIL_CTHINGCREATUREBASE_ISINMELEEASATTACKED exists),
   so the address goes down one spelling.
4. **q12262** (then q12265): a binder FAMILY pass on the old `??8?$CBasicString@D@@QBE_NPBD@Z` leaf whose owner is 0x004115A0 (88-byte
   out-of-line copy, Compare expanded) against the 20-byte TEXT_EQ_INLINE pick-any copy many rows carry. Census first (every landed row
   that defines or binds that symbol, and which body it emits); lock every family row up front; the unit is a row; prove the shared
   rewrite (an expanded-body opt-in giving the owner's 88-byte body) on 2-3 rows, then script it and grade the rest in payloads of 20-30.
   q12265 (0x00EE2A80) lands only if the copies it defines equal the owner's bytes and relocation targets. If the family is larger than
   your context allows, hand back at a row boundary with the census written down.
Do them in this order, one to a terminal state before the next. Then report.

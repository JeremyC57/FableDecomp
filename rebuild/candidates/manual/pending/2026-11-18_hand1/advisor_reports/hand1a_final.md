hand1a final report (w23). All files are under /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w23/hand1a. One row is matched, one is ready to land if the name swap is accepted, and the other 8 are escalated. I did not run git, did not touch the repo, and did not use --land or --recheck-landed. The lv/ directory has been deleted.

**Matched**
- **0x00B0D360 NParticleEngine::CPSCUpdateNormal::DatabaseSerialise: RELOCATION_MATCH PASS** in the strict read-only vland check (log: tmp/vland_00b0d360.log).
  - What closed it: the Fable advisor's c2_full_2arg candidate. The CPSCLight 0x00B1A870 opt-in preamble makes the `CPersistContext::Transfer<T>` body visible. Retail still calls the instances (0x004045C0 bool, 0x00410620 float), but the visible body lets VC7.1 fold the bool temporary (`xor cl,[esp+28h]` at 0x00B0DA91).
  - Shape follows Ego_d: one block per bitfield, the float transfer outside the block, and no defaultScale local. Defaults go through the 2-arg Transfer and the GetDefaultVal bodies.
  - Header opt-ins: existing repo ones only. FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE is reused, with a note in the source on what it gates here.
  - Flags: none. No names were changed.
  - Test: test/00b0d360.cpp is a standalone fixture over the same body covering load, save and defaults. Each member has a distinct value. It kills 11 mutants: member swaps, the 1000 / 1.0f defaults, the CRC, the IsSaving branch, dropped transfers and the bitfield write-back.
  - Hand-over: src/00b0d360.cpp, test/00b0d360.cpp, payload.json and oracle.tsv (this row only).

**Ready to land if the name swap is accepted**
- **0x0066A9E0 CTCSoundPlayer::PlayCollisionSound.** Source: differ/0066a9e0_fablewin_shape.cpp (FableWin shape).
  - The strict read-only vland check gives RELOCATION_MATCH PASS with only two warnings: RELOC_CONFLICT and RELOC_NAME on `??BCDefString` → 0x00415DD0 versus 0x00415D70. That is exactly the binding question under review.
  - Test: test/0066a9e0.cpp, 8/8 mutants killed. Payload and log are separate: tmp/payload_66.json and tmp/vland_0066a9e0.log.
  - Its tmp/ESCALATE_0066a9e0.md is updated. I am holding it for your outcome.

**Escalated** (each has tmp/ESCALATE_<addr>.md with the exact constraint, the c2prio / DAG numbers and what was measured; differ/<addr>.cpp holds the best candidate with its line-2 grade)

| Row | Best grade | Residue |
|---|---|---|
| 0x00918A40 | 114v116 | Needs a join after the GetTC store with a single presence test. Probe n gets retail's join but keeps a redundant HasTC test (129). Landed 0x00929F10 shows the mechanism: a differently spelled presence test. |
| 0x008EEC70 | 1129v1129, 2 hunks | The post-`bounce<0` block is one 81-tuple scheduler region and needs one more no-code tuple. Every probe left the region sizes unchanged. |
| 0x006E89B0 | 344v344, 2 hunks | The return-arm epilogue needs one more no-code tuple ready at cycle 1 with priority above 0x2800. |
| 0x00B6DC40 | 775v771 Ego_d-faithful; draft 771v771 with an unconditional store | The false-edge epilogue gets cross-jumped into the final one. The /d2inlT200 device itself may be distorting this row, since CloseStaticMap matches at plain flags. |
| 0x00AE3C30 | draft 2310v2310, 1 hunk; no-local spelling 2278 | Only the draft's begin-first `data` local (absent from Ego_d) keeps retail's allocation. |
| 0x005AC10E | 3532v3532, 2 hunks | All Ego_d locals and scopes are applied (probe/5a_d.cpp). The reload order follows split-piece ids: pframe is #67, the children ref #125, and retail needs the children ref first. |
| 0x00680050 | 439v439, 2 hunks | Reload order: retail needs the pappearance_def piece before the cleanup-flags piece. |
| 0x00F3ABF0 | 605v605, register swap only (Ego_d-faithful, no `left`) | `this` has priority 58 against 56 for the strength-reduced temp. The `left` draft's block terms show where the missing +2 would come from. |

**Other hand-over files**
- NOTES.md, STATUS.tsv, ATTEMPTS.log and LESSONS.md are current. The lessons cover visible callee bodies, the calibration of same-TU bodies, and reload order following piece ids. The paren lesson is labelled as inferred because I ran no positive control.
- NAME_FIXES.tsv and LABELS.tsv contain headers only; no name fixes were proven.
- hdr/ is empty apart from README.md. The experimental GetTC if-form opt-in was removed because it had no effect.
- Unmatched drafts were moved to tmp/drafts/. I deleted the tmp/q_*.txt files, which contained retail bytes.

I am waiting for the Fable advisor candidates for 0x00918A40 and 0x008EEC70; each will be graded, tested and run through the read-only vland check when it arrives.
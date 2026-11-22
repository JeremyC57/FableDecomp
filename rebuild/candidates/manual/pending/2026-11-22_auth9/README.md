# AUTH9 (Account A authoring agent, 2026-11-22)

Brief: `BRIEF.md`; state: `STATUS.tsv`, `MATCHES.tsv`.
- Landed: 0x00763060 FractureShape and 0x00B30B50 SetCameraState (`batches/2026-11-22_landing_auth9_*`), both with P1 comment patches on
  their TU-mate owners 0x007634E0 / 0x00B31160. 0x007634E0's optional P2 (owner-verbatim callee copies) is in `007634e0/`, not applied.
- 0x008E18D0: unfinished (context), not escalated. Best b0.cpp DIFFER(2045v2073); `008e18d0/HANDOVER.md` names the next steps. Continued by AUTH10.
- NAME_FIXES.tsv: 0x0093D270's UNMATCHED name column held the mangled string. Fixed to IsRunnable at commit.
- `tools/leafrel.py`: for each relocation in a leaf, the retail target and a comparison of the object's copies (covers targets with no binding line).

# 0x008E18D0 UpdateLightningBeams: matched at stock flags (AUTH14 + coordinator, 2026-11-22); landing pending

- **e5n = AUTH11's e5 without C5** grades **RELOCATION_MATCH** under stock VC7.1 with `/Gr` and the DEFAULT inline threshold, no hook, no
  forcing. The overlay is AUTH10's add-only IMPLICIT_DTOR opt-in in engine/NParticleEngine_CParticleUpdateParameters.h (`hdr.diff`; the
  CThing.h GETTC wrapper (C5) changes no bytes and is left out). e5 with C5 also matches.
  - Without the overlay, e5n grades DIFFER(2012v2073), so the opt-in is needed.
  - The coordinator pre-registered and ran these grades after AUTH14's decision table showed every callee's call / expand window overlaps
    at T in {72..74} (74 = default) (`DECISIONS.md`, `LOG.md`).
- **Corrections to the record:**
  - AUTH10's "e4 = 2044v2073" was taken WITHOUT the overlay; e4 with it is 2165v2073.
  - `/d2inlT65` was a wrong premise. Of AUTH10's four forced decisions, only two were ever needed: the sky `__uninitialized_copy`
    remainders are dead code once the overflow is expanded.
- **Pick-any** (`e5n_t74_leafrel.txt`, `e5n_t74_pickany.tsv`): leafrel reports 0 problems.
  - `~CParticleUpdateParameters` equals 0x006E2500 (73 B; it differed under T65), and `_M_insert_overflow` equals 0x0076ADA0.
  - pk.py: 10 EQ, 89 no owner. 2 copies (bitset::test v 0x00410DE0, CThing::HasTC v 0x004118C0) are byte-equal; their only unbound
    relocation is the string literal "bitset", which retail holds at 0x0122E068.
- **Locals** (`egod_locals.txt`): all 18 Ego_d names are in e5n, and e5n declares no named local Ego_d lacks. Ego_d's frame is about
  552 B, so the unexplained +10-B prolog is not `__chkstk`.
- **Left for landing:** a behaviour test (none exists); NOTES on the remaining Ego_d differences (layout +3 at 3279 / 3305, the four
  IsDebuggerPresent assert lines, the prolog); the opt-in's PDB evidence and a regrade of the landed users of 0x006E2500 / sibling
  0x007A1380 with it defined; the Flags line (`/Gr`, against Ego_d's compile line and TU-mate 0x008DD7E0); a strict-headers dry run and
  the header proof.

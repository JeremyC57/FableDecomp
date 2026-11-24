# Landing rename 1 (Account A, 2026-11-24): 7 callers stop calling the right function by a made-up name (7 table replaces)

`access_fix.py --rename` covers a landed caller that binds a spelling W to target T, where:
- T's own row defines the PDB spelling P;
- W and P have the same scope, access letter and tail, and differ only in the member's identifier.
The caller's local model misnamed the member it calls. Every whole-word use of the identifier in the caller is renamed.

**Proof:**
- objsec ALL_EQ under the one rename W -> P, so nothing else moved;
- the caller's own graded symbol unchanged, grade PASS;
- HEAD and new at one path.

**Identity:**
- P is in both PDB name sets;
- T's own row defines P;
- the table binds W to T, which is the relocation.

**Run rn1.** 13 accepted.
- 3 were held before landing because ref_scan found other landed rows still referencing W: 0x00AF5900, 0x008FAF70, 0x007E9360. They need
  the unit route.
- 3 held at landing:
  - 0x007868A0 and 0x00BF84C0 are stale, because unit1 changed their sources after the run (land_scripted.py's new staleness check);
  - 0x00DC0300 is under a NF314 lock.
- 0x00BDC180 was rejected (link failure); it is queued as `rename1`.

## Recheck
`logs/recheck.log`: 7 PASS; table: 7 replaces, 0 misses.

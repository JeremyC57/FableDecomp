# NF333 lessons

- Re-check every tracker "stand-in" table line against the witness's CURRENT object before planning a source edit. 12 of the
  standin_alias witnesses (the CClass_ / CTransferScope_ Alloc factories and two CCountedPointer Reset rows) no longer contained the
  stand-in at all: the table line was stale. A HEAD `hgrade` plus `nf.py xr` on the witness settles it, and the fix is table-only
  (NF309 Q4). tools/sacensus.py + the PRESENT/ABSENT token check found them in seconds.
- A stale line can hide a DIFFERENT wrong spelling: five "stale" CClass_ witnesses bound the real class, but with `CThing*` (PAV)
  where the PDB has `CThing&`, or in the global scope where the PDB has `NParticleEngine::`. Read the relocation at the target, not
  just "is the stand-in gone".
- An enum-only include can renumber the compiler-local `$Lnnnn` labels of a switch's jump table (0x0095C1F0): bytes, relocation
  targets and `.drectve` stay equal, but nf.py / proof332 report NOT_EQ on the label names. tools/lblproof.py compares local labels by
  (section, value).
- An enum's only shared home may drag STLport in: engine/CWoundMorphs.h -> CVectorMap.h adds `/DEFAULTLIB:"libcp"` to `.drectve`
  (0x00757F90), which the T1 agent standard does not allow. Check `.drectve` before choosing which header supplies an enum.
- Tests that link the row object (the self-contained link fails first) break when a callee's spelling moves, even with zero
  instruction change (0x00401F30: the test stubbed `CCharString(CAFile*, long)`). Grades and objsec do not see it; only the vland
  recheck does. Run the recheck before calling a group done.
- `__stdcall` -> `__fastcall` with only `unsigned __int64` arguments is byte-identical at the call site (both go on the stack, callee
  pops), so a stand-in free function's convention can move onto the PDB's in place (0x0051E210).

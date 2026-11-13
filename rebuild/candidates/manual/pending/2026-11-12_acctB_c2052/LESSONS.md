# Chunk 2052 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- 0x004A21F0: "every instruction equal, frame 4 bytes short" was an early `if (SerialisingHeroState) return true;` written as an
  `if (!SerialisingHeroState) { tail }` block: the extra return copy is cross-jumped (no code) but adds destructor references to every
  live destructible local (file, parser ...), which changes the frame packer's density sort. Read the packer trace (agent2009 pk.py) and
  try sim2 `--w` what-ifs: when the needed weights are "+N on every object live at a point", look for an early return there.
- 0x0049F4C0: `StartTime` read before GFGetTime (`fld [esi+180h] ; fstp [esp+18h]` ahead of the call) = a named `double start_time =
  StartTime;` copy; `total = GFGetTime() - start_time + TimePlayed; TimePlayed = total;` stores the local before the member.
- 0x0049F4C0: an address-taken local (the CRC word) declared at function scope gets its own frame word; inside a block that starts at
  the first statement after the variables it depends on it shares the earlier temporaries' slot -- try block boundaries one statement
  apart (here header_length .. Close()).
- 0x0049F4C0 v 0x004A21F0 (same TU): the two-argument CPersistContext::Transfer<CCharString / CWideString / C3DVector> is expanded in the
  save row and called out of line (0x00411010 / 0x00411060 / 0x00485B44) in the load row; VC7.1 reproduces both from the same header
  text in single-row TUs.
- VC7.1 bug: `CPersistTraits() : DefaultVal() {}` cannot value-initialise a NESTED enum (CWorld::ESaveType): C2440 when a global enum of
  the same name exists, C2065 otherwise.
- `buffer.size() > 0 ? &front() : this` (CArray::GetAsCArray) = retail's `sub ecx,eax ; jne ; lea eax,[vector]` data pointer.

Review at landing:
- Both rows emit ??$Transfer@W4ESaveType@@@ for 0x004AB640 (PDB: W4ESaveType@CWorld@@); see NOTES.md header debt.
- NAME_FIXES.tsv: StartAutoSaveProgress / StopAutoSaveProgress are IAE (landed QAE), Transfer<CWideString> / <double> are template
  instances, 0x0049F180 is ActivateWorld(bool) (CMainGameComponent_PostInit_00416953 declares it PostLoad(long)).
- Rows 0x0048E78E and 0x007CACB0 not attempted.

# NF292 lessons (2026-11-23)

- **"Same scope" in the table is not "same class" in the binder.** Entry 153's stand-ins bind `?GetProgress...@CCombatActionBase@@`, a
  derived class; the PDB member lives on CCreatureActionBase. Respelling the leaf gives the wrong scope. Read the HEAD object's symbol
  (or let proof2.py's explicit map fail) before claiming a row: the map must land on the exact PDB spelling.
- **Build the objsec rename map from the respell, not by sorting.** tools/proof2.py rewrites `?old@` to `?new@` on each HEAD-only symbol
  and requires the result in the new object; a const change (QAE -> QBE) needs an explicit full-symbol pair.
- **Compile path changes anonymous-namespace hashes.** A row compiled from rows/ gets a different `?A0x...` than the repo copy; map the
  hash pair explicitly (same name and type code) rather than calling it a diff.
- **The table names one witness per spelling; xsites finds the rest.** Entry 54's SetGroups had 7 binders, 188's WriteUShort 9, 61's
  Reset 2: an entry is only `done` when every exe caller binds the PDB name.
- **Binder rows can belong to another agent's ENTRIES** even when no lock is held: grep the neighbour's ENTRIES for the binder address
  too, not only the targets.
- **A pin-only equiv run first** makes the assert pin and the opt-in two separate proofs (0 differ each); one planted control over the full
  tree then shows every branch user is seen.

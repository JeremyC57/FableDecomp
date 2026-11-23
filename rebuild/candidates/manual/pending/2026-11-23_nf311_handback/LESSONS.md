# NF311 lessons (2026-11-23)

- **The ENTRIES `spellings` column is cut at 160 characters.** Long template / AI-action names lose their parameter lists, so a cut
  stand-in can look access-only (0x009804A0's AIAction_MoveToThingProximity) or fail to demangle. Read the FULL spellings from
  reloc_bindings.tsv by target (tools/census311.py does); the demangler then parses all of them.
- **Stale witnesses hide behind QUAL / SIG differences.** An identifier screen (stalescreen) cannot see access, const or pointer v
  reference; 4 of 6 compiled "suspect" witnesses already bound the PDB form or another stand-in. When a binder includes the shared
  header that declares the PDB member (HDRFLAG), compile its HEAD object before calling it a port.
- **A table line keeps one witness per spelling.** Before deleting, list every exe call site of the target and grep each landed caller's
  source for the old declaration (tools/dtorcensus.py for dtors); entry 85's QAE line still had 0x00BD9250 behind the witness I fixed,
  so it is a rewitness, not a delete.
- **`virtual` on a local model is cheap only when the TU builds no object of that class.** A member-only use (a dtor run on a member)
  or a qualified base call keeps objsec ALL_EQ; a row with a `new T` / MakeOne helper, an inline ctor, or a derived dtor of its own emits
  or changes a vftable and is a port / decision.
- **Making a dtor protected renames the scalar deleting destructor too** (??_G...MAE), so a row that emits the vftable changes its slot
  relocation.
- **The `__imp_` U?$char_traits lines are MSVCP71 exports** (Dinkumware): they are correct as the exe imports them; only game-code lines
  are STLport V candidates.
- Shell slip to avoid: a `sed 's#.*/tests/##'` over prep.sh output also eats the PASS / FAIL word; print the status column separately.

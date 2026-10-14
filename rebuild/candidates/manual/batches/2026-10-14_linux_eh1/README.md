# Batch `2026-10-14_linux_eh1` — hand-authored small EH-framed destructors: 6 bodies (6 landed)

The smallest open EH-framed (`push -1 ; push offset __ehhandler`) bodies, authored by hand with the
compiler in the loop (Wine lane, DDK 13.10.4035 `cl.exe`, `/EHsc`): the four "shutdown then base"
destructors of `CScriptDefinitionManager` and its relatives (vtable store, EH state 0 over an
out-of-line Shutdown on `this`, out-of-line base destructor at state -1), `CConsoleVariable::~CConsoleVariable`
(unregisters from the registry at +0x14, then the inline base destructor restores the base vtable)
and the pair-node destructor `_Dest_val<pair<CCharString,CScriptInfo>>` (second at +0x1c, then the
first on `this`; the user-declared pair destructor is the landed unit). Their unwind funclets land
through `families/funclets.py` on the next pass.

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14. Note for the oracle: two family-scan rows carried a
trailing `int3` in their byte column (length 80 for a 79-byte body); the lander compares the full row,
so such rows are stripped before landing.

# Batch `2026-10-14_linux_eh3` — hand-authored small EH-framed bodies: 5 bodies (5 landed)

Third hand pass over the smallest open EH-framed bodies (Wine lane, `/EHsc`): a second map-node
pair destructor (`_Dest_val<pair<CCharString,CScriptInfo>>` @ 0x00C2E140: the second at +0x3c, then
the first's implicit destructor inlined to its member at +0xc), `CGameScriptInterface::EntitySetAsOpinionSource`
(a 4-byte stack visitor with an inline empty destructor -- the EH state exists only for it --
handed to virtual slot 10 when slot 15 reports an entity), `CProfile::CProfile` (a `novtable`
destructible root, own vtable, one member built out of line) and the two sub-primitive list
constructors (the root keeps the node pointer, the node is built out of line with 0 -- the shape
of `2026-10-14_linux_i1130` without the derived Init call).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; the unwind funclets follow through
`families/funclets.py` (batch `2026-10-14_linux_funclets8`).

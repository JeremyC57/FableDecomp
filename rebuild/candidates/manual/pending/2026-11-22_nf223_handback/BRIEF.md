# Name-fix agent NF223 (2026-11-22, round 10): the CCountedPointer tail (DeleteData, GetFont / OpenGraphicBank stand-ins) and two
# split follow-ups

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. An authoring agent (AUTH10) is
running; its locks read AUTH10: leave them alone. Codex pass 27 rows (UNMATCHED_ROWS "reserved (Codex pass 27 ...)") are off limits.

## Your brief is NF222's with overrides
Read `$S/w22/agentNF222/BRIEF.md`, `$S/w22/agentNF221/BRIEF.md`, `$S/w22/agentNF215/BRIEF.md` ("Lane" applies unchanged) and
`$S/w22/agentNF223/NF214_BRIEF.md` in full, with everything they say to read first. Paths -> `$S/w22/agentNF223`, names -> `NF223`. On a
conflict: this file > NF222 > NF221 > NF215 > NF214. The ReleaseData family LANDED as nf222 (`batches/2026-11-22_landing_nf222/README.md`;
hand-backs `pending/2026-11-22_nf221_handback/`, `_nf222_handback/`, including NF222's `tools/pk.py` pick-any checker): start from the repo
tree as it is now, never from NF221 / NF222's rows. Ego_r.pdb / FableWin.pdb paths: `$S/dl_debug/files2/` (repack4k.py first).

## Entries (ENTRIES.tsv), in this order; claim each row's lock with plain mkdir (released = free)
1. **q12551-q12553: the three DeleteData self rows** 0x00419036 (CGraphicDataBank), 0x00428AE7 (CMeshDataBank), 0x0042F864 (const CFontBank).
   Self leaves are placeholders (`@DeleteIfSet@4`, `sub_0042f864`); the binders (Reset rows, OpenGraphicBank 0x009F83D0, OpenMeshBank
   0x00A09F20) store their address. Census every binder, then move each self row and every binder onto the FableWin spelling in one unit.
2. **q12554 / q12555: GetFont 0x009E2C80 and OpenGraphicBank 0x009F83D0 binders still on stand-ins** (e.g. 0x00416C8A, 0x0041E3F6 for
   OpenGraphicBank / CreateAndInitialize). One spelling each at the end; remove the stale table lines.
3. **q12556: 0x0041E641 ~CManager**: 0x0042DB20 binds `?Dtor@CManager@@QAEXXZ`; respell onto the header's `??1CManager@NUISystem@@IAE@XZ`.
4. **q12557: 0x00416C8A pick-any debt** (ShareData, `??4<CGraphicDataBank>` copies differ from their owners at HEAD): try the existing
   ShareData / assign declaration-only opt-ins in engine/CCountedPointer.h; compare with pk.py.
5. **q12558: the AcquireFont `mutable` Font member**: settle from the PDB member type record (keep or drop; report only if no byte changes).
6. **q12548: 0x00765B70** vector<vector<C3DVector> >::_M_insert_overflow: the table holds only the July placeholder
   `?InsertGrow@CRetailVector_CElement12_006f0810@@...` (witness push_back 0x00765C80); landed 0x00763060 binds the STLport spelling.
   Respell the placeholder binders and the self row (if landed) onto the header spelling.
7. **q12550: 0x00B2FD60** label: `ghidra_out/labels_forge_autoname4.tsv` says CPlane_InitializeFromCorners; both PDBs say
   CEngineCamera::SetupGamut. Hand back the label line fix (LABELS.tsv) and any landed binder respell.
For every entry: grade every changed row (recheck), compare every new pick-any copy with its owner's retail bytes (pk.py), census each
touched address's spellings (one each at the end, or no growth), link-check, and list table lines to add / delete in TABLE_CHANGES.tsv.
One entry to a terminal state before the next. Never write into /home/user/FableDecomp; never run git. Report briefly when done or when your
context gets heavy (hand-back files current first).

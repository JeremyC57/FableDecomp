# NF321 lessons (2026-11-23/24, body and callee ports onto the real class model)
- Triage by script before any draft (tools/tri4.py): for each entry, the header declaration, the local models, every direct callee with its
  landed row / Leaf / RTTI label / table spellings, and every virtual slot. Then screen the callee TARGETS against stay-off: 17 entries were
  blocked because a callee they must respell (or the row itself) sits in a T3_PORT_QUEUE line. Count a target as stay-off only when the port changes its
  spelling (a CCharString ctor bound the same way before and after is not touched).
- The donor FableWin.exe call list of the PDB member (tools/dcalls.py, from naming_batch/agent_tools/nf303) settles stand-in callee
  identities in one call: EndCutFade -> EndFadeScreen + Rest, ForceUnload -> UnloadLevel, CWADFile::GetAlignment -> CBankFile::GetAlignment,
  CEngine::SetCamera -> SetGameCamera + Process. With both PDB name sets that is the NF318 Q4 second source.
- Most "callee identity" rows needed only an add-only declaration of the callee (or the override) in the class header: one `_DECL` macro
  per member, inserted inside an access region that already has the PDB access, plus a clang `override` probe and a made-up control.
- A header inline accessor can add a pick-any COMDAT even when every call is expanded (CWorld::GetMusicManager): read the header member the
  accessor returns instead, and check the DEF set (tools/symdiff.py) after every grade, not only the code.
- A float argument faked as an int constant (`helperB(0x3f000000)`) becomes the PDB float with the same `push imm32`, but the object gains
  the UND `__fltused`: record it, it is the only symbol-table difference.
- Build the new include tree once (cp -r rebuild/include work/tree_new), edit headers only there, keep hdr/ and orig/ copies in step, and
  prove tree_new differs from the repo only in those headers (diff -rq) before the equiv_check.
- Link the HEAD and new sets together with the HEAD objects of every respelled callee's owner: it shows which new PDB names resolve and that
  the rest are unresolved only because the owner defines a stand-in (NAME_FIXES), never a reference HEAD resolved.

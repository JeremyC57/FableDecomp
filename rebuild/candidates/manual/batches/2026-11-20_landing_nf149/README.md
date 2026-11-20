# Landing nf149 (Account A, 2026-11-20): CIndexBuffer / CVertexBuffer names (27 rows)

Agent NF149 (round 10) handed back q4552, q4555..q4560 and q6203; its files are in `naming/2026-11-20_agentNF149/`.

- 26 hand-back rows plus two coordinator additions, all on engine/CIndexBuffer.h / CVertexBufferBase.h / CVertexBufferWin32.h:
  - 0x00BE91E0: its local CEngineResourceManager model respelled in place from the `GetIndexBufferPool` stand-in to the PDB
    ?GetIndexBufferMemPool@CEngineResourceManager@@QAE?AW4EIndexBufferPool@@XZ (NF152's NAME_FIXES; `logs/respell_00be91e0.log`).
  - 0x009E2890 (??_G): moved off its local model onto the header retail branch with nf86 scaffolding, first form
    (`logs/port_009e2890.log`). Its local model emitted an empty ??0CVertexBufferWin32@@QAE@XZ and a one-slot vtable beside the header copies
    (0x00B8DCD0 already emitted the header copy at HEAD).
- Self renames (RENAMES section 1, 11): each old self leaf is bound by no landed row and each new leaf is already bound by landed rows, so
  every address's spelling set shrinks (advisor decision, owner delegated: the nf120 / nf139 rule applied to all four pending
  hand-backs). 0x00A63150 keeps one stand-in binder, 0x00BF3380 (CVertexBufferWin32Native; port queued).
- Headers: CVertexBufferWin32.h and CEngineLandscapeMemoryManager.h add-only (the three removed lines are asserts re-pinned at their old
  lines); overlays' `orig` equal HEAD.
- Recheck (`logs/recheck.log`): 4 MATCH + 23 RELOCATION_MATCH, every test PASS. rebind: removed 64, added 33; bindings_multi_check 1
  (baseline). Split gate: 0 created (`logs/split_gate.txt`). tablecheck (`logs/tablecheck.txt`): ten addresses shrink; 0x0099A2F0 shows
  2 -> 3 only because the table now lists 0x009E2E30's CFontManager stand-in, which its HEAD object already bound (gate SAME; queued).
- Includers (`logs/includer_recheck.txt`): 92 good, 2 bad, both July landscape rows at their HEAD grades (0x00BE7D70 1437v2983,
  0x00BFE050 791v2218).
- Link (`logs/link_landing.log`): the 27 objects + owners 0x00401630 / 0x00405C10 / 0x009B1F50 / 0x009E3090 / 0x009E3100 + 0x00B8DCD0: one
  LNK2005, ~CDiskFileWin32 (0x00401630 vs 0x00B61FF0); the HEAD 0x00B61FF0 object gives the same clash (`logs/link_control_head.log`), so
  it is pre-existing; queued as DEBT.
- Queued: the 0x00BF3380 port, 0x00BC5420's vtable stand-in, the 0x009E2E30 CFontManager stand-in, the CIndexBuffer::PrimitiveType
  header claim, the ~CDiskFileWin32 debt, and NF149's NAME_FIXES.tsv.

# Linux-lane scratch tools (saved 2026-10-14 so a fresh session can resume)

These scripts lived in the cloud session's scratch directory (`$S`) and are the day-to-day
workflow around `verify_and_land.py`. They contain no retail data. Every one expects
`cwd = repo root`, `S=<scratch lane root>` in the environment and `source $S/wine/env.sh`
(see `env.sh.template`; the lane itself comes from `../setup_vc71_wine.sh`).

| script | purpose |
|---|---|
| `q.py <addr8> <src.cpp> [leaf] [--flags /EHsc /GS /Oa]` | grade one source against the oracle (`RELOCATION_MATCH` / `DIFFER(ours v retail)` with side-by-side disassembly). Oracle rows are looked up in `$S/batch/*_oracle.tsv` and every `rebuild/candidates/manual/batches/*/oracle.tsv`. |
| `udiff.py` | `python3 udiff.py <(python3 q.py ...)` prints only the differing instructions. |
| `gp.py payload.json oracle.tsv [addr...]` | grade a whole payload through the lane with the lander's pragma/flag sweep. |
| `xdis.py <va-hex> <len-hex>` | Capstone disassembly straight from `Fable.exe` (path inside: `$S/dl/game/Fable The Lost Chapters/Fable.exe`). |
| `fun.py 0x<va>` | an EH-framed body's handler, unwind states and funclets. |
| `rstr.py 0x<va>` / `iat.py 0x<iat-slot>` | a string literal / an import name at an address. |
| `next.py N [x]` | the next open EH-framed bodies by size (needs `$S/batch/eh_open_oracle.tsv` from `mk_eh_oracle.py` and `$S/w23/parked.txt`). |
| `pick.py`, `info.py`, `skel.py`, `famscan.py` | target picking, batch annotation, opcode-skeleton lookup of landed twins, family scanning (`famscan.py 512` writes the families JSON the chunk cutter reads). |
| `chunk_pool/BRIEF.md` | the brief handed to the single hand-authoring sub-agent (tools, rules, test template, output format). |
| `chunk_pool/mkchunk.py N [K]` | cut the next K 40-row chunks of the class-labelled 17–96-byte pool from `$S/batch/families512.json` into `$S/w22/chunkNN.tsv` + `$S/batch/chunkNN_oracle.tsv`. |
| `chunk_pool/land_agent.sh agentNN <batchname>` | copy an agent's payload into a batch dir and land it (sequential). |
| `eh_pass_example/build.py`, `tests.py` | the per-pass batch builder of a hand EH pass (names dict + behaviour tests); copy and edit for the next pass. |
| `eh_pass_example/parked*.txt` | the parked EH bodies with the evidence of why they do not match on this compiler build. |

Resume recipe: `docs/HANDOFF.md`.

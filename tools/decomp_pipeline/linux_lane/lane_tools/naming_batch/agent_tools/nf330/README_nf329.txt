NF329 tools (2026-11-24). Repo root, lane env sourced, S and A (= this folder) exported.
flatten_pilot.py  v5 of NF324's v4 (agent_tools/nf324): + `_VIRTUAL_MODEL` opt-ins as switches (own / landed), the `// Needs` line, the new-slot
                  gate (model tsv), the nf324-Q1 T3 release, `export --only`. Usage as v4 (docstring); --agent NF329.
genhdr.sh         per class: orig copy, genmodel.py (family CA: CCombatActionBase branch 1; CS: CCombatSequenceBase), pin_asserts, copy into work/tree.
genmodel.py       NF325's generator (paths and comment retargeted; logic unchanged).
vprobe2.py        NF325's emitted-vftable probe (+ PROBE_TREE for controls); vtcmp.py / nf.py its helpers.
probesum.py       strict probe summary: slot counts equal, the only DIFF allowed is slot 0 ??_E v ??_G with the same tail.
declsrc.py        every generated declaration: RTTI label + PDB name set (two sources).
hdrcheck.py       add-only check: drop the opt-in and the pins, compare with orig/ byte for byte.
retriage.py       the Q1 list against the current tree (state per address).
includers.py / plant.py / ctlcover.py / eqrun.sh   equiv_check closure, planted control, per-header control coverage, the run.
linkcheck.sh / inclass_census.py / treesync.py / lockall.sh / evcopy.py / bytescan.py   NF324's helpers retargeted.
assemble_nf329.py STATUS / RENAMES / APPLIED / REJECTED / TABLE_CHANGES / rows / HEADERS_<batch>.tsv from the batches.
assemble.py, verify_final.py, libcp.py, namecensus.py   unchanged copies (reference).

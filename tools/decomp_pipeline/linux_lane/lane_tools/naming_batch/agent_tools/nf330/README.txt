NF330 tools (2026-11-24): NF329's agent_tools/nf329 copied (paths retargeted to agentNF330; README_nf329.txt is theirs) plus:
families.tsv / genhdr2.sh  per-family base options (base, base header, base sizeof, Needs, forward declarations, pre-includes) for genmodel.py.
genmodel.py      + --preinc (headers a base header needs first); declares a non-public virtual destructor (??_G..@@MAE / EAE) with its access.
flatten_pilot.py + --own-optins FILE: tree-only base opt-ins named in a `// Needs` line count as this run's own (route header).
vtdump.py        retail vftable(s) of a class through RTTI, slot / address / RTTI-port label (genmodel's reader).
hdrcheck.py      #else matched by #if depth; inserted _RTTI_DECLS blocks in existing models checked too.
plant.py         planted token nf330_*; #else by depth; anchors on _RTTI_DECLS blocks.
probesum.py      the ??_E v ??_G alias is accepted at the destructor's slot, whichever slot.
declsrc.py       also checks override-dtor declarations.
assemble_nf330.py hand-back tables and per-batch hdr / orig / PROBES / EQUIV files.

# Naming batch toolkit

Shared helpers for the naming-tracker batch agents (brief: `rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md`).

- `lock.sh <AGENT> <addr> [release]`: row locks in `$S/w22/nflocks/<addr>/owner`.
- `clean.py <agent folder> <paths...>`: deletes only inside the agent folder (agents never `rm` a variable path).
- `apply_rows.py <handback>/rows [--dry]` (coordinator): copies `<addr>.cpp` / `<addr>_test.cpp` over the landed files found by address.
- `apply_table.py <TABLE_CHANGES.tsv> [--dry]` (coordinator): delete / add / replace / rewitness on `rebuild/oracles/reloc_bindings.tsv`,
  all-or-nothing, and reports any symbol that gains a second target.
- `agent_tools/nf2xx/`: each batch agent's own helpers, committed at its landing so a container restart cannot lose them.
  **Known bug (fixed in `agent_tools/nf294/`):** the `flags.sh` / `srcof.sh` copies in `nf287/`, `nf289/`, `nf291/` can read the NEXT
  catalog entry's CompilerFlags / Source for a row that has none of its own (0x00997800 picked up 0x00CAF500's `/EHsc`). Use nf294's.
  Their HEAD v new comparisons stay valid (the same flags on both sides); their grades against retail for such rows do not. Landing rechecks
  use `recheck.py` / vland, which read the catalog correctly.

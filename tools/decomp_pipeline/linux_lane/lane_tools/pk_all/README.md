# pk_all.py: all-copies pick-any checker (agent NF340, 2026-11-24)

Proposed home: `tools/pk_all.py` (it imports `tools/decomp_pipeline/verify_and_land.py` and `reloc_identity.py` from `$FABLE_ROOT`).

```
source $S/wine/env.sh; unset TMPDIR TMP
python3 -I -B tools/pk_all.py <row.obj> <row-address> [--leaf=SUBSTR] [--row-length=N] [--funclet] [--no-leaf-relocs] [--tsv] [--addrs=FILE]
```

It checks every function the row's object defines in a code section, except the row's own leaf, against Fable.exe. The
comparison masks relocation fields on both sides, the same way `pickany_vs_retail.py` does. It finds each copy's retail
address from five sources:

| source | where the address comes from |
|---|---|
| `a` | The row's own leaf relocations. It runs `reloc_identity.check`, the grader's symbol-to-retail-target mapping (the same call `verify_and_land` / `vland` make), on the leaf graded against the retail row bytes. This only works when the leaf has the retail length (MATCH / RELOCATION_MATCH). |
| `a2` | Transitive: the relocations of a copy already proven EQ, using the same formula at that copy's retail address. |
| `b` | `rebuild/oracles/reloc_bindings.tsv` (every target listed for the symbol). |
| `c` | `rebuild/oracles/self_symbols.tsv` (each landed row's graded symbol; the last line per address wins). |
| `o` | `--addrs=FILE` (`symbol<TAB>hex address` lines): addresses the caller supplies, authoritative like `a`. For controls whose leaf no longer grades (a row recompiled without its `/d2inl` flag, a reduced probe TU). |

TU-local names (storage class 3, such as `_$E2`, and the anonymous-namespace `?A0x` names) are resolved only by `a` / `a2`.

There is one output line per (copy, address). The **verdict** is per copy:

- If an `a` / `a2` / `o` address exists, it decides. EQ requires every such address to be EQ. A `b` / `c` address that differs
  from it is printed as `ADDR_CONFLICT(..)` and does not count.
- Without one, the copy is EQ when its single `b` / `c` address is EQ, and `EQ_AMBIG` when one of several table addresses is EQ.
  A COMDAT has exactly one retail address, so the other table addresses are stand-in or mis-graded bindings (for example, six
  landed rows grade as `??_GCWideString@@QAEPAXI@Z`).
- Otherwise the verdict is NEQ, or `NEQ_AMBIG` when there are several addresses.
- The remaining verdicts are NOADDR (no source gives an address), NOREAD and SELF.

The other columns:

| column | values |
|---|---|
| `kind` | `pickany` (COMDAT select-any), `strong` (COMDAT nodup or plain extern), `comdat<N>`, `static` |
| `catalog_len` | the catalog length (auto-re-candidates.tsv) at the retail address; a NEQ copy of a different length is a different function body, one of the same length needs a closer look |
| `leafref` | `y` when the leaf relocates directly to the symbol |
| `reach` | `y` when the leaf reaches it through other copies |

A **leaf-referenced NEQ** is a copy the leaf calls that differs from retail. The final link can pick that copy and break the
row (docs/NAMING_AND_HEADERS.md section 6).

## How it differs from `pickany_vs_retail.py`

The old checker misses three things:

1. **Copies with no `reloc_bindings.tsv` line.** It only checks copies whose name is in that table. Source `a` adds every
   copy the leaf calls, at the exact address retail calls.
2. **The wrong leaf.** It takes the largest function as the leaf, not the grader's `// Leaf:` selection, so its
   `LEAF-REFERENCED` tag can be wrong.
3. **Conflicting table addresses.** It prints every table address separately and has no notion of a conflict.

## Audit driver

```
python3 -I -B tools/audit_run.py <addrs.txt> <set> <results.jsonl> <workdir> [--control-from=<flagged results.jsonl>]
```

`--control-from` is the `/d2inl` control. It drops every `/d2inl*` flag and passes the flagged run's `a` / `a2` addresses as
`o`, because the stripped leaf usually no longer grades.

The driver compiles each landed row the way `recheck.py` + `vland --recheck-landed` do:

- `recheck.py` builds the payload: catalog CompilerFlags, the `/Oy-` rule, the Leaf, and the funclet offset.
- It compiles once with `V.parity_of(V.vc71(src))`, with a 180 s tool limit and no sweep.

It then runs `pk_all.check` and the official `pickany_vs_retail.py` on the object. Results go to one JSON line per row and
it resumes from where it stopped.

`report.py <outdir> <results.jsonl>... [--control=<control.jsonl>]` writes AUDIT.tsv, DEBT_QUEUE.tsv, ADDR_CONFLICTS.tsv and
counts.json. `select_rows.py` writes the row
sets: `/d2inl` catalog entries plus a seed-340 sample. `one.py` compiles a single source for validation.

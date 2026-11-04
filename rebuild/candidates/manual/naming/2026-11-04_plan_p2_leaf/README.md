# Plan phase 2, wave 1 (2026-11-04): `// Leaf:` cleanup, 3,679 rows

The `// Leaf:` line names the function vland grades. In 3,872 landed rows (index status LEAFGUESS) it named no emitted symbol, so
vland fell back to the largest function. That fallback is unchecked: if a row grows a second function, it silently grades the wrong
one.

## Result
- **3,679 rows respelled.** Each change is the `// Leaf:` comment and nothing else; the working-tree diff is one line per file.
- **Same function, before and after.** The plan agent checked every row with gate.py (identical length, masked sha1 and
  relocation targets) and with vland on both the landed and the respelled source (same parity, no leaf warning left).
- **Coordinator re-grade from the working tree:**
  - gate.py: 3,679 / 3,679 PASS (`logs/regrade_gate.tsv`, 140 s).
  - vland `--recheck-landed`: 3,679 / 3,679 (1,037 MATCH, 2,642 RELOCATION_MATCH, all PASS; `logs/regrade_vland_summary.log`).
- **Anonymous-namespace symbols:** five rows have `?A0x` symbols, so their leaf is cut at `?A0x`. The hash after it depends on the
  compile path.
- **Deferred:** 194 rows (`DEFERRED.tsv`).
  - 132 grade another function's PDB or catalog name: base-class `??_G` rows graded for derived classes, and forwarders that define
    their callee.
  - 55 already carry the PDB spelling as their leaf while the source emits a near miss, so the source needs the fix.
  - 6 contain several functions of the same size.
  - 1 is 0x00A9C960, which grades DIFFER(48v27) even from its landed source. It goes to phase 3.
- **Visible debt:** 3,623 of the passing rows grade a stand-in symbol (`?Get@T@@QAEHXZ` style) rather than their catalog name
  (`STANDIN.tsv`). They are a plan step-6 input.

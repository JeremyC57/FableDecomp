# NF339 lessons
- A T3 line can outlive its fix: 135 of 227 "self respell" lines were already fixed by later naming landings that went in through
  apply_rows (no `--release-t3`), so nothing closed them. Re-read the queue against today's objects before porting anything.
- `rebuild/oracles/self_symbols.tsv` lags the landings of the same day (03:05 here; 111 rows it calls "old" define the PDB symbol at HEAD).
  Prove a self from the HEAD object's DEF set and the graded symbol, not from the Leaf or that table.
- binders1 RESULTS lines whose reject came before an object existed ("no object") carry the CALLER's own Leaf on both sides; the real
  callee comes from the reloc_bindings witness column.
- A derived class calling its base's private member (PDB AAE) is the NF305 Q5 friend case: `friend class <Derived>` in the local base model
  keeps objsec ALL_EQ.
- Keep `os.chdir(<work>)` in every helper that imports verify_and_land, and quote heredocs even for one-off Python (a backticked word in an
  unquoted heredoc runs as a command).

#!/usr/bin/env python3
"""descriptive_check.py [registry.tsv] [--extra extra.tsv]: consistency gate for best-guess names (owner, 2026-11-25:
"we just need to be certain our naming is consistent so as to not cause conflicts with naming").

Reads rebuild/oracles/descriptive_names.tsv (address, symbol, basis, note), plus an optional batch file of the same shape
to check before landing. Each line is checked against the binding table (rebuild/oracles/reloc_bindings.tsv), the landed
self symbols (rebuild/oracles/self_symbols.tsv) and the two PDB name sets (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv).

FAIL kinds:
  DUP_ADDRESS     the registry lists one address twice
  DUP_SYMBOL      one best-guess symbol is listed for two addresses
  SYMBOL_ELSEWHERE the symbol is bound or defined at another retail address (table or self symbols): a name clash
  ADDRESS_PROVEN  the address already carries a PDB name OTHER than the listed symbol (table or self): a guess may never replace it
  (A PDB name is allowed as a guess: the name sets carry FableWin / Ego_r addresses, so membership says nothing about where the
  name lives in retail; R-NF440-1, 2026-11-25. Reuse is caught by SYMBOL_ELSEWHERE.)
  OTHER_SPELLING  the table or the self symbol spells this address differently: callers and self must all use the one name
  PLACEHOLDER     the symbol is a placeholder (VirtualNN_, Unproven, sub_, CVtable_, FUN_, _gapscan, stub)
  BAD_BASIS       basis is not one-source:<which> or style
Exit 1 on any FAIL. Run it on every batch that adds best-guess names, and on the registry at each landing.
"""
import os, re, sys

R = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
P = lambda *a: os.path.join(R, *a)
PLACE = re.compile(r"VirtualNN_|Virtual\d+_|Unproven|(^|[?@_])sub_[0-9a-f]{6,8}|CVtable_[0-9a-fA-F]{8}|FUN_[0-9a-fA-F]{8}|_gapscan|@stub|Stub_[0-9a-f]{8}")


def norm(a):
    return a.strip().lower().replace("0x", "").zfill(8)


def tsv(p):
    if not os.path.exists(p): return []
    return [l.split("\t") for l in open(p, encoding="utf-8").read().splitlines()[1:] if l.strip()]


def main():
    av = sys.argv[1:]
    args = [a for i, a in enumerate(av) if not a.startswith("--") and not (i and av[i - 1] == "--extra")]
    reg = args[0] if args else P("rebuild", "oracles", "descriptive_names.tsv")
    extra = sys.argv[sys.argv.index("--extra") + 1] if "--extra" in sys.argv else ""
    lines = [("registry", f) for f in tsv(reg)] + ([("extra", f) for f in tsv(extra)] if extra else [])
    pdb = set()
    for fn in ("egor_pdb_names.tsv", "fablewin_pdb_names.tsv"):
        for f in tsv(P("ghidra_out", fn)):   # address \t name
            if len(f) > 1 and f[1]: pdb.add(f[1].strip())
    sym2addr, addr2sym = {}, {}
    for f in tsv(P("rebuild", "oracles", "reloc_bindings.tsv")):
        if len(f) > 1: sym2addr.setdefault(f[0], set()).add(norm(f[1])); addr2sym.setdefault(norm(f[1]), set()).add(f[0])
    for f in tsv(P("rebuild", "oracles", "self_symbols.tsv")):
        if len(f) > 1 and f[1]: sym2addr.setdefault(f[1], set()).add(norm(f[0])); addr2sym.setdefault(norm(f[0]), set()).add(f[1])
    fails, seen_a, seen_s = [], {}, {}
    for src, f in lines:
        if len(f) < 3: fails.append(("SHORT_LINE", src, "\t".join(f))); continue
        a, s, basis = norm(f[0]), f[1].strip(), f[2].strip()
        if a in seen_a and seen_a[a] != s: fails.append(("DUP_ADDRESS", a, s + " vs " + seen_a[a]))
        if s in seen_s and seen_s[s] != a: fails.append(("DUP_SYMBOL", a, s + " also at " + seen_s[s]))
        seen_a[a], seen_s[s] = s, a
        if PLACE.search(s): fails.append(("PLACEHOLDER", a, s))
        if not (basis == "style" or basis.startswith("one-source:")): fails.append(("BAD_BASIS", a, basis))
        other = sorted(x for x in sym2addr.get(s, ()) if x != a)
        if other: fails.append(("SYMBOL_ELSEWHERE", a, s + " bound at " + ",".join(other[:5])))
        here = addr2sym.get(a, set())
        proven = sorted(x for x in here if x in pdb and x != s)
        if proven: fails.append(("ADDRESS_PROVEN", a, proven[0]))
        diff = sorted(x for x in here if x != s)
        if diff and not proven: fails.append(("OTHER_SPELLING", a, " | ".join(diff[:4])))
    for k in fails: print("FAIL", *k, sep="\t")
    print("descriptive_check: %d lines, %d FAIL" % (len(lines), len(fails)))
    sys.exit(1 if fails else 0)


if __name__ == "__main__":
    main()

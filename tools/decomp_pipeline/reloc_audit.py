#!/usr/bin/env python3
"""Catalog-wide relocation-target identity audit (see reloc_identity.py).

    python3 tools/decomp_pipeline/reloc_audit.py [--addrs file] [--out rebuild/oracles/reloc_audit.tsv] [--limit N]

Recompiles each catalog row with its recorded flags, recovers (symbol, retail target) for every
relocation of the graded function and writes one line per row: address, parity, HARD/WARN
findings, binding count. Bindings go to reloc_bindings.tsv (via record_bindings) so the landing
verifier can flag later conflicts. Resumable: rows already in --out are skipped.
"""
import csv, os, re, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import verify_and_land as V, reloc_identity as RI

def catalog_entries(ps1):
    text = Path(ps1).read_text(encoding="utf-8-sig"); ents = {}
    for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", text, re.S):
        d = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", m.group(1)))
        if "Address" in d: ents[d["Address"].lower()] = d
    return ents

def main():
    args = sys.argv[1:]
    out = Path(args[args.index("--out") + 1]) if "--out" in args else V.ROOT / "rebuild" / "oracles" / "reloc_audit.tsv"
    limit = int(args[args.index("--limit") + 1]) if "--limit" in args else None
    ents = catalog_entries(V.ROOT / "rebuild" / "build_candidates.ps1")
    orc = {r["address"].lower(): r for r in csv.DictReader(open(V.ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t")}
    addrs = [l.split()[0].lower() for l in open(args[args.index("--addrs") + 1]) if l.strip()] if "--addrs" in args else sorted(ents)
    done = set()
    if out.exists():
        done = {l.split("\t")[0] for l in open(out, encoding="utf-8")}
    else:
        out.write_text("address\tparity\tfindings\tbindings\n", encoding="utf-8")
    e = V.env(); n = 0
    for a in addrs:
        if a in done or a not in ents or a not in orc: continue
        if limit is not None and n >= limit: break
        n += 1; d = ents[a]
        if d.get("Grade") == "asm_bake":
            open(out, "a", encoding="utf-8").write(f"{a}\tASM_BAKE\t-\t0\n"); continue
        src = (V.ROOT / "rebuild" / "src" / "compiled" / d["Source"]).read_text(encoding="utf-8", errors="replace")
        retail = bytes.fromhex(orc[a]["bytes"])
        m = re.search(r"^//\s*Leaf:\s*(\S+)", src, re.M)
        leaf = m.group(1) if m else orc[a]["name"].rsplit("::", 1)[-1]
        extra = [f for f in d.get("CompilerFlags", "").split() if f not in V.BASE_FLAGS]
        comp = d.get("Compiler", "")
        if comp == "qfe4035": V.use_qfe()
        else: V.use_default()
        work = V.shard_directory(V.WORK_ROOT / "reloc_audit", a, leaf=True); work.mkdir(parents=True, exist_ok=True)
        try:
            st, built, sec = V.parity_of(V.vc71(src), a, leaf, retail, work, e, extra=tuple(extra))
        except Exception as ex:
            open(out, "a", encoding="utf-8").write(f"{a}\tERROR {ex}\t-\t0\n"); continue
        findings = []; pairs = []
        if st.startswith(("MATCH", "RELOCATION")):
            try:
                hard, warn, pairs = RI.check(V.ROOT, V.OBJDUMP, work / f"{a}.obj", leaf, a, retail, built=built)
                findings = hard + warn
                RI.record_bindings(V.ROOT, a, [(p[2], p[3]) for p in pairs])
            except Exception as ex: findings = [f"RELOC_CHECK_ERROR: {ex}"]
        open(out, "a", encoding="utf-8").write(f"{a}\t{st}\t{' | '.join(findings) or '-'}\t{len(pairs)}\n")
        print(a, st, len(pairs), findings[:2], flush=True)

if __name__ == "__main__": main()

#!/usr/bin/env python3
"""Rename CRT-thunk aliases in landed rows to the function the relocation really targets.

    python3 tools/decomp_pipeline/fix_crt_aliases.py [--audit rebuild/oracles/reloc_audit.tsv] [--limit N] [--apply]

The catalog relocation audit (reloc_audit.py) flags landed rows whose source calls an import
thunk under an alias (`free_engine`, `Free1`, `AutoTinyVectorDelete`, `fable_op_delete_7c8114`)
that names a different CRT function than the thunk (`MSVCR71.DLL::operator_delete`, `free`, ...).
The bytes are right; the name is wrong. This tool rewrites the alias declaration and every call
in the source and its test to the real function (`operator delete(p)`, `free(p)`, ...), re-runs
the landing verifier on the rewritten pair (bytes, behaviour test, relocation identity) and,
with --apply, writes the pair back only when it still grades MATCH/RELOCATION_MATCH + PASS.
Rows written as true CRT symbols (`??2`, `??3`, `_free`, `_malloc`) but calling another CRT
function are semantic defects and are left for hand repair. Resumable: results append to
rebuild/oracles/reloc_alias_fixes.tsv.
"""
import csv, json, os, re, subprocess, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import verify_and_land as V, reloc_identity as RI
from reloc_audit import catalog_entries

DECL = {"operator delete": "void operator delete(void*);", "operator delete[]": "void operator delete[](void*);",
        "operator new": "void* operator new(unsigned int);", "operator new[]": "void* operator new[](unsigned int);",
        "malloc": "void* __cdecl malloc(unsigned int);", "free": "void __cdecl free(void*);"}
TRUE_CRT = re.compile(r"^(\?\?[23]@|\?\?_[UV]@|_?(malloc|free|realloc|calloc)$)")

def alias_name(sym):
    if sym.startswith("??"): return None
    if sym.startswith("?"): return sym[1:].split("@")[0]
    return sym[1:] if sym.startswith("_") else sym

def rewrite(text, name, target):
    """Replace the alias declaration with the CRT declaration and every use with the CRT name."""
    decl = DECL[target]; out = []; changed = False
    for line in text.split("\n"):
        if re.search(rf"\b{re.escape(name)}\s*\(", line) and ";" in line and "{" not in line and re.match(r"^\s*(extern\b|void\b|int\b|char\b|unsigned\b|static\b|[\w:]+\s*\*)", line) and "=" not in line.split("(")[0]:
            comment = line[line.index("//"):] if "//" in line else ""
            out.append(decl + ("   " + comment if comment else "")); changed = True; continue
        new = re.sub(rf"\b{re.escape(name)}\b", target, line)
        if new != line:
            changed = True
            if target.startswith("operator") and 'extern "C"' in new: new = new.replace('extern "C"', "").replace("__cdecl ", "")
        out.append(new)
    return "\n".join(out), changed

def main():
    args = sys.argv[1:]
    audit = Path(args[args.index("--audit") + 1]) if "--audit" in args else V.ROOT / "rebuild" / "oracles" / "reloc_audit.tsv"
    limit = int(args[args.index("--limit") + 1]) if "--limit" in args else None
    apply = "--apply" in args
    out = V.ROOT / "rebuild" / "oracles" / "reloc_alias_fixes.tsv"
    done = {l.split("\t")[0] for l in open(out, encoding="utf-8")} if out.exists() else set()
    if not out.exists(): out.write_text("address\talias\ttarget\tresult\n", encoding="utf-8")
    ents = catalog_entries(V.ROOT / "rebuild" / "build_candidates.ps1")
    orc = {r["address"].lower(): r for r in csv.DictReader(open(V.ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t")}
    jobs = []
    for r in csv.DictReader(open(audit, encoding="utf-8"), delimiter="\t"):
        a = r["address"]
        if a in done or a not in ents: continue
        fixes = []
        for m in re.finditer(r"RELOC_IDENTITY: (\S+) -> [0-9a-f]{8} is the import thunk 'MSVCR71\.DLL::([^']+)'", r["findings"]):
            sym, thunk = m.group(1), m.group(2).replace("operator_", "operator ")
            if TRUE_CRT.match(sym) or thunk not in DECL: continue
            name = alias_name(sym)
            if name: fixes.append((name, thunk))
        if fixes: jobs.append((a, fixes))
    if limit: jobs = jobs[:limit]
    work = V.WORK_ROOT / "alias_fix"; work.mkdir(parents=True, exist_ok=True)
    auth = []; orows = ["address\tname\tlength\tbytes"]; meta = {}
    for a, fixes in jobs:
        d = ents[a]; sp = V.ROOT / "rebuild" / "src" / "compiled" / d["Source"]; tp = V.ROOT / "rebuild" / "tests" / d["TestSource"]
        src = sp.read_text(encoding="utf-8", errors="replace"); tst = tp.read_text(encoding="utf-8", errors="replace"); ch = False
        for name, thunk in fixes:
            src, c1 = rewrite(src, name, thunk); tst, c2 = rewrite(tst, name, thunk); ch = ch or c1
        if not ch:
            open(out, "a", encoding="utf-8").write(f"{a}\t{fixes[0][0]}\t{fixes[0][1]}\tNO_TEXT_MATCH\n"); continue
        m = re.search(r"^//\s*Leaf:\s*(\S+)", src, re.M)
        c = {"address": a, "module": d.get("Module", "_global"), "name": orc[a]["name"], "source_cpp": src, "test_cpp": tst, "pass_pattern": d.get("PassPattern", f"{a}_TEST PASS")}
        fl = [f for f in d.get("CompilerFlags", "").split() if f not in V.BASE_FLAGS]
        if fl: c["flags"] = fl
        if m: c["leaf"] = m.group(1)
        auth.append(c); orows.append("\t".join([a, orc[a]["name"], orc[a]["length"], orc[a]["bytes"]])); meta[a] = (fixes, sp, tp, src, tst)
    if not auth: print("nothing to do"); return
    (work / "payload.json").write_text(json.dumps({"result": {"authored": auth}}), encoding="utf-8")
    (work / "oracle.tsv").write_text("\n".join(orows) + "\n", encoding="utf-8")
    log = subprocess.run([sys.executable, "-u", str(Path(__file__).resolve().parent / "verify_and_land.py"), str(work / "payload.json"), str(work / "oracle.tsv"), "--recheck-landed"], capture_output=True, text=True).stdout
    (work / "verify.log").write_text(log, encoding="utf-8")
    status = {}
    for line in log.splitlines():
        m = re.match(r"^([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", line)
        if m and m.group(2) != "WARN" and not m.group(2).startswith("RELOC_IDENTITY:"): status[m.group(1)] = (m.group(2), m.group(3))
    ok = 0
    for a, (fixes, sp, tp, src, tst) in meta.items():
        st, beh = status.get(a, ("NO_RESULT", "-"))
        good = st.startswith(("MATCH", "RELOCATION")) and beh == "PASS"
        if good and apply: sp.write_text(src, encoding="utf-8"); tp.write_text(tst, encoding="utf-8")
        ok += good
        open(out, "a", encoding="utf-8").write(f"{a}\t{fixes[0][0]}\t{fixes[0][1]}\t{'APPLIED' if good and apply else 'OK' if good else st + '/' + beh}\n")
    print(f"{len(meta)} rewritten, {ok} verified{' and applied' if apply else ''}; log {work/'verify.log'}")

if __name__ == "__main__": main()

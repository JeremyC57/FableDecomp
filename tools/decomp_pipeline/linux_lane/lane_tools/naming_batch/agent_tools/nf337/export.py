#!/usr/bin/env python3
"""export.py (NF337) <batch> <run> <copyaddr>...: land_scripted-style batch dir batches/<batch>/ from work/<run>/ (UNITS.tsv + GRADE.tsv):
rows/<a>.cpp (new source), tests/<a>_test.cpp (moved test), orig/rows + orig/tests (the landed files each replaces; a file with a retail
byte dump is left out and listed), head/payload.json (HEAD entries of the Copy rows; byte-dump ones left out -> head/HEAD_FROM_REPO.txt),
RESULTS.tsv, TABLE_CHANGES.tsv (the impl line of each row: replace impl -> ??4, or delete when a ??4 line for that target exists),
UNITS.tsv (Copy row + operator= mate), rows.txt. No retail bytes are written."""
import os, re, sys, json, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import drv
S = os.environ["S"]; A = f"{S}/w22/agentNF337"; W = f"{A}/work"; R = drv.R
batch, run, addrs = sys.argv[1], sys.argv[2], [x.lower() for x in sys.argv[3:]]
B = f"{A}/batches/{batch}"
for d in ("rows", "tests", "orig/rows", "orig/tests", "head"): os.makedirs(f"{B}/{d}", exist_ok=True)
dump = re.compile(r'(?i)(?<![0-9a-z_])(?:[0-9a-f]{2}[ ,]\s*){3,}[0-9a-f]{2}(?![0-9a-z_])|(?:0x[0-9a-f]{2}\s*,\s*){3,}0x[0-9a-f]{2}|(?:\\x[0-9a-f]{2}){4,}|e9 <rel32>|e9 00000000')
U = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(f"{W}/{run}/UNITS.tsv").read().splitlines()[1:]}
G = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(f"{W}/{run}/GRADE.tsv").read().splitlines()[1:]}
tbl = [l.rstrip("\n").split("\t") for l in open(f"{R}/rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]]
res, tch, units, left, headout = [], [], [], [], []
hp = {x["address"]: x for x in json.load(open(f"{W}/{run}/head/payload.json"))["result"]["authored"]}
for a in addrs:
    u = U[a]; g = G[a]
    assert u[3] == "OK" and g[2] in ("MATCH PASS", "RELOCATION_MATCH PASS") and g[3].endswith("LINK_FAIL"), (a, u, g)
    C, op, oc, hself, himpl, opsym, sel = u[1], u[2], u[4], u[5], u[6], u[7], u[8]
    pdb = f"?Copy@{C}@@UAEXPBVCDefClassBase@@@Z"
    shutil.copy(f"{W}/pre/{a}/new.cpp", f"{B}/rows/{a}.cpp"); shutil.copy(f"{W}/pre/{a}/ntest.cpp", f"{B}/tests/{a}_test.cpp")
    for src, dst in ((f"{W}/pre/{a}/head.cpp", f"{B}/orig/rows/{a}.cpp"), (f"{W}/pre/{a}/test.cpp", f"{B}/orig/tests/{a}_test.cpp")):
        if dump.search(drv.rd(src)): left.append(f"{dst[len(B) + 1:]}\tretail byte dump in the landed file (D-f): not copied")
        else: shutil.copy(src, dst)
    h = hp[a]
    if dump.search(h.get("source_cpp", "")) or dump.search(h.get("test_cpp", "")): headout.append(a)
    else: headout.append(None); h = h
    route = "header" if "#include \"engine/" in drv.rd(f"{W}/pre/{a}/new.cpp") else "local"
    res.append([a, "ACCEPT", hself, pdb, f"objcheck {oc} under {{self -> PDB, {himpl} -> {opsym}}}" + (" (+libcp only, NF318 Q1)" if oc == "LIBCP" else "") +
                f", {g[2]}; moved test, control HEAD+moved test {g[3]} [route {route}; unit with operator= {op} ({sel}), unchanged]"])
    have = any(t[0] == opsym and t[1].lower().replace("0x", "") == op for t in tbl)
    line = [t for t in tbl if t[0] == himpl and t[1].lower().replace("0x", "") == op]
    if not line: tch.append(["MISSING", himpl, op, a, "no table line binds the impl"]); continue
    if have: tch.append(["delete", himpl, op, line[0][2], f"NF337: the made-up impl spelling; {opsym} already binds {op}"])
    else: tch.append(["replace", f"{himpl} -> {opsym}", op, a, f"NF337: Copy {a} now binds the PDB operator= of {C} (defined by the landed row {op})"])
    units.append([a, op, C, pdb, opsym, sel, "Copy row moves; operator= row unchanged (defines the PDB operator=)"])
heads = [hp[a] for a, o in zip(addrs, headout) if o is None]
json.dump({"result": {"authored": heads}}, open(f"{B}/head/payload.json", "w"))
open(f"{B}/head/HEAD_FROM_REPO.txt", "w").write("".join(o + "\n" for o in headout if o))
open(f"{B}/orig/LEFT_OUT.txt", "w").write("".join(l + "\n" for l in left))
open(f"{B}/RESULTS.tsv", "w").write("address\tverdict\told\tnew\treason\n" + "".join("\t".join(r) + "\n" for r in res))
open(f"{B}/TABLE_CHANGES.tsv", "w").write("action\tsymbol\ttarget\twitness\tnote\n" + "".join("\t".join(r) + "\n" for r in tch))
open(f"{B}/UNITS.tsv", "w").write("copy\toperator=\tclass\tcopy_symbol\toperator=_symbol\toperator=_definition\tnote\n" + "".join("\t".join(r) + "\n" for r in units))
open(f"{B}/rows.txt", "w").write("".join(a + "\n" for a in addrs))
print(len(res), "rows;", sum(1 for o in headout if o), "HEAD entries from repo;", len(left), "orig files left out;", sum(1 for t in tch if t[0] == "MISSING"), "missing table lines")

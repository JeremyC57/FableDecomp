#!/usr/bin/env python3
"""export2.py (NF337) <batch> <spec.tsv>: land_scripted-style batch dir for the respell units, body-form rows and ruling batches.
spec columns: addr, run, role (copy | op | body), class, mate (the unit's other row or -), objcheck verdict, old self, new self, reason.
Per row: rows/<a>.cpp (work/<run>/../pre new source), tests/<a>_test.cpp only when the test moved (work/pre/<a>/ntest.cpp), orig/rows +
orig/tests (the landed files each replaces; a file with a retail byte dump is left out and listed in orig/LEFT_OUT.txt), head/payload.json
(the HEAD entries from work/<run>/head/payload.json; byte-dump ones left out -> head/HEAD_FROM_REPO.txt), RESULTS.tsv, rows.txt.
The grade is read from work/<run>/GRADE.tsv and asserted unchanged (head grade == new grade, test PASS; a moved test's control LINK_FAIL).
TABLE_CHANGES.tsv / UNITS.tsv / RENAMES.tsv are written by the caller. No retail bytes are written."""
import os, re, sys, json, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import drv
S = os.environ["S"]; A = f"{S}/w22/agentNF337"; W = f"{A}/work"
batch, spec = sys.argv[1], sys.argv[2]
B = f"{A}/batches/{batch}" if not batch.startswith("held/") else f"{A}/{batch}"
for d in ("rows", "tests", "orig/rows", "orig/tests", "head"): os.makedirs(f"{B}/{d}", exist_ok=True)
dump = re.compile(r'(?i)(?<![0-9a-z_])(?:[0-9a-f]{2}[ ,]\s*){3,}[0-9a-f]{2}(?![0-9a-z_])|(?:0x[0-9a-f]{2}\s*,\s*){3,}0x[0-9a-f]{2}|(?:\\x[0-9a-f]{2}){4,}|e9 <rel32>|e9 00000000')
res, left, heads, headout, addrs = [], [], [], [], []
for l in open(spec).read().splitlines():
    if not l.strip() or l.startswith("#"): continue
    a, run, role, C, mate, oc, old, new, why = l.split("\t")
    G = {x.split("\t")[0]: x.split("\t") for x in open(f"{W}/{run}/GRADE.tsv").read().splitlines()[1:]}[a]
    hg, ng, cg = G[1], G[2], G[3]
    moved = os.path.exists(f"{W}/pre/{a}/ntest.cpp")
    assert hg == ng and ng.endswith(" PASS"), (a, G)
    if moved: assert cg.endswith("LINK_FAIL"), (a, G)
    assert drv.rd(f"{W}/pre/{a}/new.cpp") == drv.rd(f"{W}/gen/{a}.cpp"), ("stale gen", a)
    shutil.copy(f"{W}/pre/{a}/new.cpp", f"{B}/rows/{a}.cpp")
    pairs = [(f"{W}/pre/{a}/head.cpp", f"{B}/orig/rows/{a}.cpp")]
    if moved:
        shutil.copy(f"{W}/pre/{a}/ntest.cpp", f"{B}/tests/{a}_test.cpp"); pairs.append((f"{W}/pre/{a}/test.cpp", f"{B}/orig/tests/{a}_test.cpp"))
    for src, dst in pairs:
        if dump.search(drv.rd(src)): left.append(f"{dst[len(B) + 1:]}\tretail byte dump in the landed file (D-f): not copied")
        else: shutil.copy(src, dst)
    h = {x["address"]: x for x in json.load(open(f"{W}/{run}/head/payload.json"))["result"]["authored"]}[a]
    if dump.search(h.get("source_cpp", "")) or dump.search(h.get("test_cpp", "")): headout.append(a)
    else: heads.append(h)
    res.append([a, "ACCEPT", role, C, mate, old, new, f"objcheck {oc}; grade {ng} (HEAD {hg})" + (f"; moved test, control HEAD+moved test {cg}" if moved else "; landed test unchanged") + f"; {why}"])
    addrs.append(a)
json.dump({"result": {"authored": heads}}, open(f"{B}/head/payload.json", "w"))
open(f"{B}/head/HEAD_FROM_REPO.txt", "w").write("".join(o + "\n" for o in headout))
open(f"{B}/orig/LEFT_OUT.txt", "w").write("".join(l + "\n" for l in left))
open(f"{B}/RESULTS.tsv", "w").write("address\tverdict\trole\tclass\tunit_mate\told_self\tnew_self\treason\n" + "".join("\t".join(r) + "\n" for r in res))
open(f"{B}/rows.txt", "w").write("".join(a + "\n" for a in addrs))
print(batch, len(res), "rows;", len(headout), "HEAD entries from repo;", len(left), "orig files left out")

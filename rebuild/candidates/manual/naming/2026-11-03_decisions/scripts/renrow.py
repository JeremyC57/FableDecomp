# renrow.py <renames.tsv-in> <RENAMES.tsv-out(append)>: in = addr<TAB>new_module<TAB>new_func<TAB>new_name<TAB>evidence.
# Moves source + test to the make_base() names, updates the build_candidates.ps1 block (Source/TestSource/Module) and
# appends RENAMES rows (naming-wave shape). Leaves file content unchanged.
import sys, os, re, csv
sys.path.insert(0, "tools/decomp_pipeline")
from verify_and_land import make_base
P = "rebuild/build_candidates.ps1"; ps = open(P, encoding="utf-8").read()
cat = {l.split("\t")[0]: l.split("\t")[1] for l in open("rebuild/oracles/auto-re-candidates.tsv")}
out = open(sys.argv[2], "a")
for line in open(sys.argv[1]):
    if not line.strip(): continue
    a, mod, fn, newname, ev = line.rstrip("\n").split("\t")
    blk = re.search(r"(\[pscustomobject\]@\{\n        Address = '%s'\n.*?\n    \})" % a, ps, re.S); b = blk.group(1)
    om = re.search(r"Module = '([^']*)'", b).group(1); osrc = re.search(r"\n        Source = '([^']*)'", b).group(1)
    otst = re.search(r"TestSource = '([^']*)'", b).group(1)
    base = make_base(mod, fn, a); d = f"{a[:2]}/{a[2:4]}"
    nsrc, ntst = f"{d}/{base}.cpp", f"{d}/{base}_test.cpp"
    nb = b.replace(f"Source = '{osrc}'", f"Source = '{nsrc}'", 1).replace(f"TestSource = '{otst}'", f"TestSource = '{ntst}'", 1)
    nb = re.sub(r"Module = '[^']*'", "Module = '%s'" % mod.replace("'", "''"), nb, 1)
    ps = ps.replace(b, nb, 1)
    if osrc != nsrc: os.rename("rebuild/src/compiled/" + osrc, "rebuild/src/compiled/" + nsrc)
    if otst != ntst: os.rename("rebuild/tests/" + otst, "rebuild/tests/" + ntst)
    out.write("\t".join([a, cat.get(a, ""), newname, om, mod, "rebuild/src/compiled/" + osrc, "rebuild/src/compiled/" + nsrc,
                         "rebuild/tests/" + otst, "rebuild/tests/" + ntst, ev]) + "\n")
    print("renamed", a, "->", base)
open(P, "w", encoding="utf-8").write(ps)

# chk.py <addr>...: compile each landed row's source (copy in $S/decisions/tmp) and compare its find/_M_find symbols with canon.tsv
import sys, os, subprocess, re
S = os.environ["S"]; D = S + "/decisions/d1"
canon = dict(l.rstrip("\n").split("\t") for l in open(D + "/canon.tsv"))
src = {l.split("\t")[0]: l.rstrip("\n").split("\t")[1] for l in open(D + "/respell_src.tsv")}
for a in sys.argv[1:]:
    p = "rebuild/src/compiled/" + src[a]; t = f"{S}/decisions/tmp/{a}.cpp"
    open(t, "w").write(open(p).read())
    err = subprocess.run([S + "/decisions/tools/clx.sh", t], capture_output=True, text=True).stdout
    if "error" in err: print(a, "COMPILE ERROR\n" + "\n".join(l for l in err.splitlines() if "error" in l)[:1500]); continue
    o = subprocess.run(["objdump", "-t", t[:-4] + ".obj"], capture_output=True, text=True).stdout
    syms = sorted({l.split()[-1] for l in o.splitlines() if re.search(r"\?(\$)?(_M_find|find)@", l.split()[-1] if l.split() else "")})
    for s in syms:
        hit = [k for k, v in canon.items() if v == s]
        print(a, "CANON " + hit[0] if hit else "other", s[:160])

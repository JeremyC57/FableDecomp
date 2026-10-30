#!/usr/bin/env python3
"""fam.py apply <family> <script.py> [script args] -- <file>... : snapshot each file to work/pre/<family>/ (pre-image),
run the family script on them, write work/fam_<family>.txt (edited files) and logs/apply_<family>.txt.
fam.py revert <family> <mcheck log>... : every edited row of <family> whose latest mcheck status is not SAME /
RELOC_RENAMED is restored to its pre-image (so another family's passing edit made before stays); appends rev.tsv.
fam.py revertrows <family> <reason> <addr>... : restore the pre-image of the listed rows."""
import sys, os, re, shutil, subprocess
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]; W = S + "/mig7/work"
cmd, fam = sys.argv[1], sys.argv[2]
P = W + "/pre/" + fam
def addr(f): return re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1)
if cmd == "apply":
    a = sys.argv[3:]; i = a.index("--"); script, sargs, files = a[0], a[1:i], a[i+1:]
    os.makedirs(P, exist_ok=True)
    for f in files:
        dst = P + "/" + addr(f) + ".cpp"
        if not os.path.exists(dst): shutil.copyfile(os.path.join(R, f), dst)
    out = subprocess.run([sys.executable, S + "/mig7/scripts/" + script] + sargs + files, capture_output=True, text=True, cwd=R)
    open(S + "/mig7/logs/apply_%s.txt" % fam, "a").write(out.stdout + out.stderr)
    ed = [l.split()[1] for l in out.stdout.splitlines() if l.startswith("edit ")]
    with open(W + "/fam_%s.txt" % fam, "a") as fh:
        for f in ed: fh.write(f + "\n")
    print(fam, "edited", len(ed), "of", len(files)); sys.stderr.write(out.stderr[-2000:])
elif cmd in ("revert", "revertrows"):
    files = {addr(l.strip()): l.strip() for l in open(W + "/fam_%s.txt" % fam) if l.strip()}
    bad = {}
    if cmd == "revert":
        last = {}
        for lg in sys.argv[3:]:
            for l in open(lg):
                q = l.rstrip("\n").split("\t")
                if len(q) >= 2 and len(q[0]) == 8 and q[0] in files: last[q[0]] = (q[1], q[2] if len(q) > 2 else "")
        bad = {a: s + " " + d for a, (s, d) in last.items() if s not in ("SAME", "RELOC_RENAMED")}
    else:
        bad = {a: sys.argv[3] for a in sys.argv[4:]}
    with open(W + "/rev.tsv", "a") as out:
        for a, why in sorted(bad.items()):
            shutil.copyfile(P + "/" + a + ".cpp", os.path.join(R, files[a]))
            out.write(f"{a}\t{fam}\t{why[:300]}\n")
    # drop reverted rows from the family list
    keep = [f for a, f in files.items() if a not in bad]
    open(W + "/fam_%s.txt" % fam, "w").write("".join(f + "\n" for f in keep))
    print(fam, "reverted", len(bad), "kept", len(keep))

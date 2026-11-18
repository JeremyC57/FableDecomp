#!/usr/bin/env python3
"""rebase_overlay.py <old-rev> <overlay-dir> <out-dir>   (repo root)
Rebases a header overlay made against rebuild/include at <old-rev> onto today's rebuild/include: three-way merge per header
(git merge-file: today's header, the <old-rev> header, the overlay copy), NEW headers copied, then pin_asserts.py against today's
tree. Prints CONFLICT / NEWNOW lines for anything that needs a human; exit 1 if any."""
import os, subprocess, sys, shutil, tempfile
old, H, O = sys.argv[1:4]; bad = 0
if os.path.isdir(O): shutil.rmtree(O)
files = [os.path.relpath(os.path.join(d, f), H) for d, _, fs in os.walk(H) for f in fs if f.endswith(('.h', '.inl'))]
merged = []
for r in sorted(files):
    src = f"{H}/{r}"; dst = f"{O}/{r}"; os.makedirs(os.path.dirname(dst), exist_ok=True)
    cur = f"rebuild/include/{r}"
    oldtxt = subprocess.run(["git", "show", f"{old}:rebuild/include/{r}"], capture_output=True).stdout if subprocess.run(["git", "cat-file", "-e", f"{old}:rebuild/include/{r}"], capture_output=True).returncode == 0 else None
    if oldtxt is None:
        if os.path.exists(cur): print("NEWNOW", r, "(new in the overlay, but today's tree now has it)"); bad = 1
        shutil.copy(src, dst); continue
    if not os.path.exists(cur): print("GONE", r, "(header removed from today's tree)"); bad = 1; shutil.copy(src, dst); continue
    with tempfile.NamedTemporaryFile(delete=False) as t: t.write(oldtxt); base = t.name
    res = subprocess.run(["git", "merge-file", "-p", cur, base, src], capture_output=True)
    os.unlink(base); open(dst, "wb").write(res.stdout)
    if res.returncode != 0: print("CONFLICT", r, res.returncode); bad = 1
    else: merged.append(r)
if merged:
    p = subprocess.run(["python3", "tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py", "rebuild/include", O] + merged, capture_output=True, text=True)
    for l in p.stdout.splitlines():
        if "!!" in l or ("pinned" in l and not l.rstrip().endswith(" 0")): print("  pin:", l.strip())
print("rebased", len(files), "headers ->", O, "OK" if not bad else "NEEDS ATTENTION"); sys.exit(bad)

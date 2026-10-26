# leafscan.py <lvw dir> [addr...]: for each graded row, check its source's mangled `// Leaf:` names a symbol in its object.
import sys, os, re, subprocess, glob
lvw = sys.argv[1]; want = set(a.lower() for a in sys.argv[2:])
ps = open("rebuild/build_candidates.ps1", encoding="utf-8-sig").read(); src = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: src[kv["Address"].lower()] = kv.get("Source")
bad = 0; n = 0
for obj in glob.glob(f"{lvw}/*/*/*/*.obj"):
    a = os.path.basename(obj)[:8].lower()
    if obj.endswith(".t.obj") or a not in src or (want and a not in want): continue
    t = open("rebuild/src/compiled/" + src[a], encoding="utf-8", errors="replace").read()
    m = re.search(r"^//\s*Leaf:\s*(\S+)", t, re.M)
    if not m or not m.group(1).endswith("Z"): continue
    n += 1
    syms = subprocess.run(["objdump", "-t", obj], capture_output=True, text=True).stdout
    if m.group(1) not in syms: bad += 1; print(a, m.group(1)[:140])
print("checked", n, "leaf-not-in-object", bad, file=sys.stderr)

# Apply retail class keys (keyconv.py + leaffix.py, KEYS.tsv) to an agent payload.json's source/test text, in place.
# usage: keypayload.py payload.json   (owner 2026-10-26: class key = retail's)
import json, sys, os, subprocess, tempfile
H = os.path.dirname(os.path.abspath(__file__)); p = sys.argv[1]; d = json.load(open(p)); n = 0
with tempfile.TemporaryDirectory() as t:
    for i, c in enumerate(d["result"]["authored"]):
        for k in ("source_cpp", "test_cpp"):
            if not c.get(k): continue
            f = f"{t}/{i}_{k}.cpp"; open(f, "w", encoding="utf-8", errors="surrogateescape").write(c[k])
            subprocess.run([sys.executable, H + "/keyconv.py", f], capture_output=True)
            if k == "source_cpp": subprocess.run([sys.executable, H + "/leaffix.py", f], capture_output=True)
            s = open(f, encoding="utf-8", errors="surrogateescape").read()
            if s != c[k]: c[k] = s; n += 1
json.dump(d, open(p, "w"), indent=1); print("keypayload:", n, "texts changed")

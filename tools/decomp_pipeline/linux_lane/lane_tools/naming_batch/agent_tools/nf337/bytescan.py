import re, os, glob, json, sys
pat = re.compile(r'(?i)(?<![0-9a-z_])(?:[0-9a-f]{2}[ ,]\s*){3,}[0-9a-f]{2}(?![0-9a-z_])|(?:0x[0-9a-f]{2}\s*,\s*){3,}0x[0-9a-f]{2}|(?:\\x[0-9a-f]{2}){4,}')
files = [f for g in ["rows/*", "batches/*/rows/*", "hdr/**/*", "orig/**/*", "evidence/**/*", "*.tsv", "*.md", "*.txt", "*.log", "tools/*", "batches/*/*.tsv", "batches/*/head/*.txt", "batches/**/*.cpp", "batches/**/*.h", "batches/**/*.diff", "held/**/*", "*.tsv"] for f in glob.glob(g, recursive=True) if os.path.isfile(f)]
hits = 0
for f in files:
    for i, l in enumerate(open(f, errors="replace"), 1):
        m = pat.search(l)
        if m: hits += 1; print("HIT", f, i, l.strip()[:140]) if hits <= 40 else None
for p in glob.glob("batches/*/head/payload.json") + glob.glob("held/*/head/payload.json"):
    for x in json.load(open(p))["result"]["authored"]:
        for k in ("source_cpp", "test_cpp"):
            for l in (x.get(k) or "").splitlines():
                if pat.search(l): hits += 1; print("HIT", p, x["address"], k, l.strip()[:140]) if hits <= 80 else None
print("files", len(files), "hits", hits)

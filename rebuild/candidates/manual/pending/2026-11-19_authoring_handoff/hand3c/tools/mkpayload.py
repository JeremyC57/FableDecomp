#!/usr/bin/env python3
# mkpayload.py [addr ...] : payload.json + oracle.tsv from src/<addr>.cpp + test/<addr>.cpp (default: every src/*.cpp)
import sys, os, json, csv, re, glob
csv.field_size_limit(10**8)
W = os.environ["W"]; S = os.environ["S"]
rows = {r["address"]: r for r in csv.DictReader(open(f"{W}/rows.tsv"), delimiter="\t")}
orc = {r["address"]: r for r in csv.DictReader(open(f"{S}/batch/hand2c_oracle.tsv"), delimiter="\t")}
if os.path.exists(f"{W}/oracle_override.tsv"):
    for r in csv.DictReader(open(f"{W}/oracle_override.tsv"), delimiter="\t"): orc[r["address"]] = r
addrs = sys.argv[1:] or sorted(os.path.basename(p)[:-4] for p in glob.glob(f"{W}/src/*.cpp"))
out = []; ol = []
for a in addrs:
    if not os.path.exists(f"{W}/test/{a}.cpp"): print("skip (no test)", a); continue
    src = open(f"{W}/src/{a}.cpp").read(); tst = open(f"{W}/test/{a}.cpp").read()
    r = rows.get(a, {"module": "", "name": ""})
    mod, nm = r["module"], r["name"]
    m1 = re.match(r"// ((?:\w+::)*\w+)::(\w+)\(", src)
    if m1: mod, nm = m1.group(1), m1.group(2)   # line 1 carries the proven name (NAME_FIXES), e.g. 0x00F36010 Get2DNoise
    else:
        m2 = re.match(r"// (\w+)\(", src)
        if m2: mod, nm = "_global", m2.group(1)
    e = {"address": a, "module": mod, "name": nm, "source_cpp": src, "test_cpp": tst, "pass_pattern": f"{a}_TEST PASS"}
    m = re.search(r"^// Leaf:\s*(\S+)", src, re.M)
    if m: e["leaf"] = m.group(1)
    m = re.search(r"^// Flags:\s*(.*)$", src, re.M)
    if m and m.group(1).strip().lower() not in ("", "none"): e["compiler_flags"] = m.group(1).split(); e["flags"] = m.group(1).split()
    out.append(e)
    o = dict(orc[a]); b = bytes.fromhex(o["bytes"])
    L = int(o["length"]); o["bytes"] = b[:L].hex(); o["name"] = mod + "::" + nm; ol.append(o)
json.dump({"result": {"authored": out}}, open(f"{W}/payload.json", "w"), indent=1)
with open(f"{W}/oracle.tsv", "w") as f:
    f.write("address\tname\tlength\tbytes\n")
    for o in ol: f.write(f"{o['address']}\t{o['name']}\t{o['length']}\t{o['bytes']}\n")
print(len(out), "rows")

# Payload + oracle from an agent MATCHES.tsv (addr src test module name leaf flags oracle_length note); needs FABLE_EXE.
import csv, json, os, sys
M = sys.argv[1]; out = sys.argv[2]
exe = open(os.environ["FABLE_EXE"], "rb").read()  # retail Fable.exe, SHA-1 39fd416b...
auth = []; orc = ["address\tname\tlength\tbytes"]; seen = set()
F = ["addr","src","test","module","name","leaf","flags","oracle_length","note"]
for r in csv.DictReader(open(M), delimiter="\t", fieldnames=F):
    if r["addr"] == "addr": continue
    if "::" in r["name"]: r["module"], r["name"] = r["name"].rsplit("::", 1)
    a = r["addr"].lower()
    if a in seen: continue
    seen.add(a); n = int(r["oracle_length"]); off = int(a, 16) - 0x400000
    e = dict(address=a, module=r["module"], name=r["name"], source_cpp=open(r["src"]).read(), test_cpp=open(r["test"]).read(),
             pass_pattern=f"{a}_TEST PASS", leaf=r["leaf"] or r["name"].rsplit("::", 1)[-1])
    if r["flags"].strip(): e["flags"] = r["flags"].split()
    auth.append(e); orc.append(f"{a}\t{r['module']}::{r['name']}\t{n}\t{exe[off:off+n].hex()}")
    print(a, n, exe[off+n:off+n+16].hex())
json.dump({"result": {"authored": auth}}, open(f"{out}/payload.json", "w"))
open(f"{out}/oracle.tsv", "w").write("\n".join(orc) + "\n")

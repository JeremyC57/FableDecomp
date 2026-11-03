# usage: mkpayload.py <plan.tsv> <outdir> <addr-file>  -> recheck payload whose leaf is the COMPLETED symbol
import sys, json, subprocess, csv, os
plan = {r["address"]: r for r in csv.DictReader(open(sys.argv[1]), delimiter="\t")}
out = sys.argv[2]; addrs = [l.split()[0] for l in open(sys.argv[3]) if l.strip()]
os.makedirs(out, exist_ok=True)
subprocess.run([sys.executable, "tools/decomp_pipeline/linux_lane/lane_tools/recheck.py", out] + addrs, check=True)
p = json.load(open(f"{out}/payload.json"))
for e in p["result"]["authored"]:
    r = plan[e["address"]]
    assert e["leaf"] == r["leaf"], (e["address"], e["leaf"], r["leaf"])
    e["leaf"] = r["completed"]
json.dump(p, open(f"{out}/payload.json", "w"))
print(len(p["result"]["authored"]), "leaves completed in payload")

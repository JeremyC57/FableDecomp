# usage (repo root): pairs.py <rk dir> > pairs.tsv : for every graded row of an rk.sh run, the (symbol, retail target)
# relocation pairs of the graded function (reloc_identity.check, same as vland's landing record).
import sys, json, glob, os
sys.path.insert(0, "tools/decomp_pipeline")
import reloc_identity as R
F = sys.argv[1]
for pd in sorted(glob.glob(F + "/rk_part_0*")) + ([F] if os.path.exists(F + "/payload.json") else []):
    if not os.path.isdir(pd): continue
    pl = json.load(open(pd + "/payload.json"))["result"]["authored"]
    orc = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(pd + "/oracle.tsv").read().splitlines()[1:]}
    for e in pl:
        a = e["address"]; objs = glob.glob(f"{pd}/work/**/{a}.obj", recursive=True)
        if not objs or a not in orc: print(f"{a}\tNOOBJ\t\t"); continue
        retail = bytes.fromhex(orc[a][3])
        try: hard, warn, pairs = R.check(".", "objdump", objs[0], e.get("leaf"), a, retail)
        except Exception as ex: print(f"{a}\tERR\t{ex}\t"); continue
        seen = set()
        for off, kind, sym, t in pairs:
            if (sym, t) in seen: continue
            seen.add((sym, t)); print(f"{a}\t{t:08x}\t{sym}\t{kind}")

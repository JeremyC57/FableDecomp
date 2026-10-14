#!/usr/bin/env python3
"""Grade a payload.json through the Wine lane quickly (base flags + lander sweep), printing a
side-by-side disassembly for every DIFFER. Usage: gp.py payload.json oracle.tsv [addr ...]"""
import sys, os, csv, json
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = Path(os.environ["S"]); pl = json.load(open(sys.argv[1]))["result"]["authored"]
orc = {r["address"].lower(): r for r in csv.DictReader(open(sys.argv[2], encoding="utf-8-sig"), delimiter="\t")}
only = set(a.lower() for a in sys.argv[3:]); e = V.env(); md = Cs(CS_ARCH_X86, CS_MODE_32)
PR = ["", '#pragma optimize("s",on)', '#pragma optimize("t",on)', '#pragma optimize("g",on)', '#pragma optimize("",off)']
wins = 0
for c in pl:
    a = c["address"].lower()
    if only and a not in only: continue
    o = orc[a]; retail = bytes.fromhex(o["bytes"]); leaf = c["name"].rsplit("::",1)[-1]
    work = S/"wine"/"q"/a; work.mkdir(parents=True, exist_ok=True); src0 = V.vc71(c["source_cpp"])
    st, built, _ = V.parity_of(src0, a, leaf, retail, work, e); tag = ""
    if not st.startswith(("MATCH","RELOCATION")):
        for xf in V.EXTRA_FLAGSETS:
            for P in PR:
                if not xf and not P: continue
                st2, b2, _ = V.parity_of((P+"\n"+src0) if P else src0, a, leaf, retail, work, e, extra=xf)
                if st2.startswith(("MATCH","RELOCATION")): st, built, tag = st2, b2, f"[{(P.split('(')[1].split(',')[0] if P else '')}{' '.join(xf)}]"; break
            if tag: break
    beh = V.behaviour_of(V.vc71(c["test_cpp"]), a, c["pass_pattern"], work, e, work/f"{a}.obj") if st.startswith(("MATCH","RELOCATION")) else "-"
    print(f"{a} {st}{tag} {beh} {c['module']}::{c['name']}")
    if st.startswith(("MATCH","RELOCATION")) and beh == "PASS": wins += 1
    if st == "SRC_FAIL":
        cp = V.cl(["/nologo","/c"]+V.BASE_FLAGS+[f"/Fo{work/(a+'.obj')}", str(work/(a+'.cpp'))], e); print("   ", (cp.stdout+cp.stderr)[-1500:].replace("\n","\n    "))
    elif not st.startswith(("MATCH","RELOCATION")) and built:
        for lab, b in (("retail", retail), ("built", built)):
            print(f"   --- {lab} ({len(b)})"); print("\n".join(f"    {i.address:08x} {i.bytes.hex():20} {i.mnemonic} {i.op_str}" for i in md.disasm(b, int(a,16))))
print("WINS", wins)

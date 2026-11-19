#!/usr/bin/env python3
# egod_refs.py [--types] : add egod_locals.txt / egod_tu.txt to the hand2 agents' ref dirs (mkagent_egod.py logic)
import csv, os, re, subprocess, sys, collections
ROOT="/home/user/FableDecomp"; S=os.environ["S"]; PDB="/tmp/claude-0/egod_pdb/Ego_d.pdb"; TYP="/tmp/claude-0/egod_pdb/types.txt"
L=f"{ROOT}/rebuild/candidates/manual/pending/2026-11-18_hand1/lane"
named={r["address"]:r for r in csv.DictReader(open(f"{ROOT}/rebuild/candidates/manual/pending/2026-11-17_egod_parked/EGOD_NAMED.tsv"),delimiter="\t")}
cat=collections.defaultdict(list)
for l in open(ROOT+"/rebuild/oracles/auto-re-candidates.tsv"):
    p=l.split("\t")
    if len(p)>2 and re.match(r"^[0-9a-f]{8}$",p[0]): cat[p[1]].append(p[0])
funcs=collections.defaultdict(list)
for l in open(ROOT+"/ghidra_out/egod_carved_functions.tsv"):
    if l.startswith("#"): continue
    o,off,ln,k,n=l.rstrip("\n").split("\t",4); funcs[o.split("\\")[-1].lower()].append((int(off),int(ln),n))
for ag in ("hand2a","hand2b"):
    for ad in os.listdir(f"{S}/w24/{ag}/ref"):
        R=f"{S}/w24/{ag}/ref/{ad}"; r=named[ad]; en=r["egod_name"]
        objs=[o for o in r["egod_objects"].split(" | ") if o]; out=[]
        for ob in objs:
            fl=sorted(funcs.get(ob.split("\\")[-1].lower(),[]))
            out.append(f"== {ob}: {len(fl)} functions in Ego_d object order (debug length; retail address(es) when the catalog has the name)")
            for off,ln,n in fl:
                hits=cat.get(n) or cat.get(n.split("<")[0]) or []
                out.append(f"{'>>' if n==en else '  '} {ln:6d}  {n}"+(f"   retail {' '.join(hits[:4])}" if hits else ""))
        open(f"{R}/egod_tu.txt","w").write("\n".join(out)+"\n")
        raw=subprocess.run(["python3","-I",L+"/egod_all.py",PDB,en],capture_output=True,text=True)
        open(f"{R}/egod_locals.raw.txt","w").write(raw.stdout)
        if "--types" in sys.argv:
            rl=subprocess.run(["python3","-I",L+"/egod_type.py",TYP,"--locals",f"{R}/egod_locals.raw.txt"],capture_output=True,text=True).stdout
            open(f"{R}/egod_locals.txt","w").write(rl)
        print(ag,ad,en,len(out),"tu lines;",len(raw.stdout.splitlines()),"raw local lines",raw.stderr[-200:])

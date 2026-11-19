#!/usr/bin/env python3
# mk_rows.py <agent> <addr>... : workspace for any UNMATCHED_ROWS rows, $S/w24/<agent>/
import csv, os, re, shutil, subprocess, sys, glob, collections
ROOT="/home/user/FableDecomp"; S=os.environ["S"]; M=f"{ROOT}/rebuild/candidates/manual"
L=f"{M}/pending/2026-11-18_hand1/lane"
sys.path.insert(0, ROOT+"/tools/decomp_pipeline/linux_lane/lane_tools/bounds"); import extent
name=sys.argv[1]; ADDRS=[a.lower() for a in sys.argv[2:]]; A=f"{S}/w24/{name}"
for d in ("src","test","differ","tmp","probe","ref","hdr"): os.makedirs(f"{A}/{d}",exist_ok=True)
U=[r for r in csv.DictReader(open(f"{M}/UNMATCHED_ROWS.tsv"),delimiter="\t") if r["address"] in ADDRS]
PI={r["addr"]:r for r in csv.DictReader(open(f"{M}/PARKED_INDEX.tsv"),delimiter="\t")}
EN={r["address"]:r for r in csv.DictReader(open(f"{M}/pending/2026-11-17_egod_parked/EGOD_NAMED.tsv"),delimiter="\t")}
egor=collections.defaultdict(list)   # proc short name -> (obj, idx, size)
for l in open(f"{ROOT}/ghidra_out/egor_obj_functions.tsv"):
    if l.startswith("#"): continue
    o,i,va,sz,k,n,p=l.rstrip("\n").split("\t"); egor[n].append((o,i,sz))
E=extent.Exe(); rows=[]; lines=[]
for n,r in enumerate(U,1):
    ad=r["address"]; R=f"{A}/ref/{ad}"; os.makedirs(R,exist_ok=True); have=["retail_dis.txt"]
    m=re.search(r'DIFFER\((\d+)v(\d+)\)',r["best_grade_on_record"])
    ext=E.scan(int(ad,16))["extent"]; ln=int(m[2]) if m else ext
    rows.append((ad,r["module"],r["name"],ln))
    open(f"{R}/retail_dis.txt","w").write(subprocess.run(["python3",f"{S}/tools/xdis.py",ad,hex(ln)],capture_output=True,text=True).stdout)
    cv=r["closest_version"]
    if cv and os.path.isfile(f"{ROOT}/{cv}"):
        shutil.copy(f"{ROOT}/{cv}",f"{R}/draft"+(".test.cpp" if cv.endswith(".test.cpp") else ".cpp")); have.append("draft")
        base=re.sub(r'(\.test|\.alt|\.speaker_draft)?\.cpp$','',os.path.basename(cv)); fold=os.path.dirname(f"{ROOT}/{cv}")
        for f in glob.glob(f"{fold}/{base}*.cpp")+glob.glob(f"{os.path.dirname(fold)}/test/{base}*.cpp"):
            shutil.copy(f,f"{R}/prior_"+os.path.basename(f))
        top=os.path.dirname(fold) if fold.endswith(("/differ","/src","/hdr")) else fold
        notes=[]
        for f in glob.glob(f"{top}/*.md")+glob.glob(f"{top}/*.tsv"):
            try:
                for l in open(f,errors="replace"):
                    if ad in l.lower() or ad.upper() in l: notes.append(f"{os.path.relpath(f,ROOT)}: {l.rstrip()[:1500]}")
            except Exception: pass
        open(f"{R}/prior_notes.txt","w").write("\n".join(notes)+"\n"); have.append("prior_*.cpp, prior_notes.txt")
    pi=PI.get(ad,{})
    open(f"{R}/parked_index.txt","w").write("\n".join(f"{k}: {v}" for k,v in pi.items())+"\n"); have.append("parked_index.txt")
    en=EN.get(ad,{}).get("egod_name") or (r["module"]+"::"+r["name"] if not r["name"].startswith("?") and r["module"]!="_global" else "")
    if en:
        raw=subprocess.run(["python3","-I",L+"/egod_all.py","/tmp/claude-0/egod_pdb/Ego_d.pdb",en],capture_output=True,text=True).stdout
        if raw.strip():
            open(f"{R}/raw.txt","w").write(raw)
            open(f"{R}/egod_locals.txt","w").write(subprocess.run(["python3","-I",L+"/egod_type.py","/tmp/claude-0/egod_pdb/types.txt","--locals",f"{R}/raw.txt"],capture_output=True,text=True).stdout)
            os.remove(f"{R}/raw.txt"); have.append("egod_locals.txt")
        hits=egor.get(en,[])
        if hits: open(f"{R}/egor_object.txt","w").write("".join(f"Ego_r: {o} index {i} size {s}\n" for o,i,s in hits)); have.append("egor_object.txt")
    lines.append(f"{n}. 0x{ad.upper()} {r['module']}::{r['name'][:90]} ({ln} bytes; extent scan {ext}; best {r['best_grade_on_record'][:40] or '-'}; residue: {pi.get('residue','')[:200]})\n   ref/{ad}/: "+", ".join(have))
with open(f"{A}/rows.tsv","w") as f:
    f.write("address\tmodule\tname\tlength\n"); [f.write("\t".join(map(str,x))+"\n") for x in rows]
subprocess.run(["python3",f"{ROOT}/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/oracle_from_exe.py",f"{A}/rows.tsv",name],stdout=subprocess.DEVNULL)
open(f"{A}/ROWS_BLOCK.md","w").write("\n".join(lines)+"\n"); print(len(lines),"rows")

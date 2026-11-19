#!/usr/bin/env python3
# mk_hand2.py <agent> <addr>... : build $S/w24/<agent>/ for the hand1 continuation (no Ego_d.pdb on disk this session)
import csv, os, shutil, subprocess, sys, glob
ROOT="/home/user/FableDecomp"; S=os.environ["S"]
H1=f"{ROOT}/rebuild/candidates/manual/pending/2026-11-18_hand1"
LEN={"0066a9e0":626,"00ae3c30":2310,"00b0d360":2240,"006e89b0":344,"00f3abf0":605,"00680050":439,"005ac10e":3532,"00b6dc40":771,"008eec70":1129,"00918a40":116}
name=sys.argv[1]; addrs=sys.argv[2:]
A=f"{S}/w24/{name}"
for d in ("src","test","differ","tmp","probe","ref","hdr"): os.makedirs(f"{A}/{d}",exist_ok=True)
res={r["address"]:r for r in csv.DictReader(open(f"{H1}/RESERVED.tsv"),delimiter="\t")}
unm={r["address"]:r for r in csv.DictReader(open(f"{ROOT}/rebuild/candidates/manual/UNMATCHED_ROWS.tsv"),delimiter="\t")}
st={r["address"]:r for r in csv.DictReader(open(f"{H1}/STATUS.tsv"),delimiter="\t")}
rows=[]; lines=[]
for n,ad in enumerate(addrs,1):
    r=res[ad]; R=f"{A}/ref/{ad}"; os.makedirs(R,exist_ok=True); have=[]
    ln=LEN[ad]; mod,nm=r["module"],r["name"]
    rows.append((ad,mod,nm,ln))
    open(f"{R}/retail_dis.txt","w").write(subprocess.run(["python3",f"{S}/tools/xdis.py",ad,hex(ln)],capture_output=True,text=True).stdout); have.append("retail_dis.txt")
    for f in glob.glob(f"{H1}/candidates/{ad}*")+glob.glob(f"{H1}/candidates/*{ad}*"):
        shutil.copy(f,R); have.append("hand1 candidate "+os.path.basename(f))
    cv=unm.get(ad,{}).get("closest_version","")
    if cv and os.path.isfile(f"{ROOT}/{cv}"):
        shutil.copy(f"{ROOT}/{cv}",f"{R}/codex_draft.cpp"); have.append("codex_draft.cpp (Codex pass 24 parked source)")
        for t in glob.glob(os.path.dirname(f"{ROOT}/{cv}")+"/*"):
            if t!=f"{ROOT}/{cv}" and os.path.isfile(t): shutil.copy(t,f"{R}/codex_"+os.path.basename(t))
    for f in glob.glob(f"{H1}/advisor_reports/*{ad}*")+glob.glob(f"{H1}/escalations/*{ad}*"):
        shutil.copy(f,R); have.append(os.path.basename(f))
    open(f"{R}/hand1_attempts.log","w").write("".join(l for l in open(f"{H1}/ATTEMPTS.log") if l.startswith(ad)))
    s=st.get(ad,{})
    lines.append(f"{n}. 0x{ad.upper()} {mod}::{nm} ({ln} bytes; hand1 state: {s.get('grade','-')} {s.get('bytes','')} -- {s.get('notes','')[:220]})\n   ref/{ad}/: "+", ".join(have)+", hand1_attempts.log")
with open(f"{A}/rows.tsv","w") as f:
    f.write("address\tmodule\tname\tlength\n"); [f.write("\t".join(map(str,x))+"\n") for x in rows]
subprocess.run(["python3",f"{ROOT}/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/oracle_from_exe.py",f"{A}/rows.tsv",name],stdout=subprocess.DEVNULL)
print("\n".join(lines))
open(f"{A}/ROWS_BLOCK.md","w").write("\n".join(lines)+"\n")

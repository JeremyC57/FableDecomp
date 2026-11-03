# usage: filter.py <B>: build payload_land.json from verify_readonly.log + offcheck2 (expected offset in the parent's own $L block)
import sys,json,re,subprocess,os
B=sys.argv[1]; S=os.environ["S"]
oc=subprocess.run([sys.executable,f"{S}/funclets/offcheck2.py",f"{B}/payload.json",f"{B}/oracle.tsv",f"{B}/work"],capture_output=True,text=True).stdout
open(f"{B}/offcheck2.txt","w").write(oc)
off={l.split()[0]:l.split()[1:] for l in oc.splitlines() if l.strip()}
st={}
for l in open(f"{B}/verify_readonly.log"):
    m=re.match(r"^([0-9a-f]{8})\s+(\S+)\s+(\S+)\s+Unwind@",l)
    if m: st[m.group(1)]=(m.group(2),m.group(3))
bad=set(open(f"{S}/funclets/badpush.txt").read().split())
d=json.load(open(f"{B}/payload.json")); keep=[]; rep=[]
for c in d["result"]["authored"]:
    a=c["address"]; s,b=st.get(a,("NONE","-")); oo=off.get(a,["-"])
    ok= s in ("MATCH","RELOCATION_MATCH") and b=="PASS" and oo[0]=="OK" and a not in bad
    if a in bad: oo=["PUSH-OUTSIDE-PARENT"]+oo[1:]
    rep.append("\t".join([a,s,b]+oo[:1]+[oo[2] if len(oo)>2 else "-","PASS" if ok else "PARK"]))
    if ok: keep.append(c)
json.dump({"result":{"authored":keep}},open(f"{B}/payload_land.json","w"),indent=1)
open(f"{B}/grades.tsv","w").write("\n".join(rep)+"\n"); print(B.split("/")[-1],len(keep),"of",len(rep),"pass")

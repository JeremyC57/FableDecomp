# mkstatus.py: rewrite STATUS.tsv attempts/best/grade columns from tmp/attempts.log (chunk-859 attempts only)
import re,collections
A="/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent859"
cnt=collections.Counter(); best={}
for l in open(A+"/tmp/attempts.log"):
    f=l.rstrip("\n").split("\t")
    if len(f)<5: continue
    addr,tag,grade,md=f[1],f[2],f[3],f[4]
    if grade.startswith("SRC_FAIL") or grade.startswith("ERR"): continue
    cnt[addr]+=1
    m=re.search(r"md=(\d+)",md); mdv=int(m.group(1)) if m else 999
    if "MATCH" in grade: mdv=0
    if addr not in best or mdv<best[addr][0]: best[addr]=(mdv,grade,tag)
lines=open(A+"/STATUS.tsv").read().splitlines()
out=[lines[0]]
for l in lines[1:]:
    f=l.split("\t"); a=f[0]
    f[3]=str(cnt.get(a,0))
    if a in best:
        mdv,grade,tag=best[a]
        f[2]="matched" if mdv==0 else "parked"
        f[4]=grade if mdv==0 else "DIFFER"
        f[5]=re.sub(r"DIFFER\((\w+)\)",r"\1",grade) if mdv>0 else grade.split("(")[0]
        f[6]=str(mdv)
    out.append("\t".join(f))
open(A+"/STATUS.tsv","w").write("\n".join(out)+"\n")
print("\n".join(out))

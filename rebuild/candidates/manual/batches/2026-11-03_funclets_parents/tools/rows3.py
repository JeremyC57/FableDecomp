import json,collections,os,sys
S=os.environ["S"]
ehm=json.load(open(f"{S}/funclets/ehmap.json"))
rows=[l.split("\t") for l in open(f"{S}/funclets/ROWS2.tsv").read().splitlines()]
hdr=rows[0]+["block_base","offset","len","len_note","thunk"]
out=[hdr]; c=collections.Counter()
for r in rows[1:]:
    if r[2]!="unwind": out.append(r+["","","","",""]); continue
    fi=ehm[r[3]]; acts=sorted(set(a for k,to,a in fi["acts"] if a)); th=fi["thunks"][0]
    A=int(r[0],16); base=acts[0]; i=acts.index(A)
    end=acts[i+1] if i+1<len(acts) else th
    ln=end-A; note=[]
    if th<=acts[-1]: note.append("thunk-before-last")
    if r[9] and int(r[9])!=ln: note.append(f"cov{r[9]}")
    # contiguity of whole block: nothing else between
    out.append(r+["%08x"%base,"0x%x"%(A-base),str(ln),";".join(note),"%08x"%th]); c[bool(note)]+=1
open(sys.argv[1],"w").write("\n".join("\t".join(r) for r in out)+"\n"); print(c)

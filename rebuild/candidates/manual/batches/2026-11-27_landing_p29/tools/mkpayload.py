import sys,json,struct,ast,os
S,X,out=sys.argv[1],sys.argv[2],sys.argv[3]
exe=open(S+"/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; ns=struct.unpack_from("<H",exe,pe+6)[0]; oh=struct.unpack_from("<H",exe,pe+20)[0]
ib=struct.unpack_from("<I",exe,pe+24+28)[0]; secs=[]
for i in range(ns):
    o=pe+24+oh+40*i; va,vs,rs,rp=struct.unpack_from("<IIII",exe,o+12-4+4) if False else (struct.unpack_from("<I",exe,o+12)[0],struct.unpack_from("<I",exe,o+8)[0],struct.unpack_from("<I",exe,o+16)[0],struct.unpack_from("<I",exe,o+20)[0])
    secs.append((va,vs,rs,rp))
def read(va,n):
    rva=va-ib
    for v,vs,rs,rp in secs:
        if v<=rva<v+max(vs,rs): return exe[rp+rva-v:rp+rva-v+n]
cat={}
for l in open("/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv",encoding="utf-8").read().splitlines()[1:]:
    f=l.split("\t"); cat[f[0]]=f[1]
st=[l.split("\t") for l in open(X+"/zip/STATUS.tsv").read().splitlines()[1:]]
rows=[];orc=[]
for r in st:
    a=r[0]
    if r[1] not in ("RELOCATION_MATCH","retail_asm"): continue
    d=X+"/zip/rows/"+a; mg=json.load(open(d+"/MAIN_REGRADE.json"))
    fl=mg.get("flags") or []
    if isinstance(fl,str): fl=ast.literal_eval(fl) if fl.strip() else []
    n=int(r[3]); b=read(int(a,16),n)
    full=cat.get(a,"Unknown::"+a); mod,_,name=full.rpartition("::")
    row={"address":a,"module":mod or "Unknown","name":name,"source_cpp":open(d+"/"+a+".cpp",encoding="utf-8").read(),
         "test_cpp":open(d+"/"+a+"_test.cpp",encoding="utf-8").read(),"pass_pattern":a+"_TEST PASS"}
    if fl: row["flags"]=fl
    rows.append(row); orc.append("\t".join([a,full,str(len(b)),b.hex()]))
os.makedirs(out,exist_ok=True)
json.dump({"result":{"authored":rows}},open(out+"/payload.json","w"),indent=1)
open(out+"/oracle.tsv","w").write("address\tname\tlength\tbytes\n"+"\n".join(orc)+"\n")
print(len(rows),"rows; flags used:",sorted({" ".join(r.get("flags",[])) for r in rows}))

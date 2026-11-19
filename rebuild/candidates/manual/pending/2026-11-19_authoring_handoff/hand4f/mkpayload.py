# mkpayload.py: build payload.json + oracle.tsv from ROWS (matched rows) list
import json,csv,sys,re
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f"
S="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"
rows=[l.split("\t") for l in open(W+"/matched.tsv").read().strip().split("\n") if l and not l.startswith("#")]
auth=[];orc=[]
o={r["address"]:r for r in csv.DictReader(open(S+"/batch/hand4f_oracle.tsv"),delimiter="\t")}
for r in rows:
    addr,mod,name=r[0],r[1],r[2]
    src=open(f"{W}/src/{addr}.cpp").read(); tst=open(f"{W}/test/{addr}.cpp").read()
    d={"address":addr,"module":mod,"name":name,"source_cpp":src,"test_cpp":tst,"pass_pattern":f"{addr}_TEST PASS"}
    m=re.search(r"^// Flags:\s*(.*)$",src,re.M)
    if m: d["flags"]=m.group(1).split(); 
    m=re.search(r"^// Leaf:\s*(\S+)",src,re.M)
    if m: d["leaf"]=m.group(1)
    auth.append(d)
    x=o[addr]; orc.append("\t".join([x["address"],x["name"],x["length"],x["bytes"]]))
json.dump({"result":{"authored":auth}},open(W+"/payload.json","w"),indent=1)
open(W+"/oracle.tsv","w").write("address\tname\tlength\tbytes\n"+"\n".join(orc)+"\n")
print(len(auth),"rows")

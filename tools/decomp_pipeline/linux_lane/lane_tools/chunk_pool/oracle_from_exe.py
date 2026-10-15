# usage: python3 oracle_from_exe.py <rows.tsv> [<name>]
# Rebuild grader oracle rows (address, name, length, bytes) for a staged chunk's rows straight from Fable.exe and
# write them to $S/batch/<name>_oracle.tsv (SCRATCH ONLY -- never commit the output: retail bytes of unlanded
# functions stay out of the repo). q.py then grades the parked differ/ sources of that chunk.
import os,sys,struct,csv
S=os.environ["S"]
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,rs,ro))
def rd(va,n):
    for sva,rs,ro in secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return exe[off:off+n]
src=sys.argv[1]; name=sys.argv[2] if len(sys.argv)>2 else os.path.basename(os.path.dirname(os.path.abspath(src)))
out=f"{S}/batch/{name}_oracle.tsv"
with open(out,"w") as f:
    f.write("address\tname\tlength\tbytes\n")
    for r in csv.DictReader(open(src),delimiter="\t"):
        n=int(r["length"]); b=rd(int(r["address"],16),n)
        f.write(f"{r['address']}\t{r['module']}::{r['name']}\t{n}\t{b.hex()}\n")
print("wrote",out)

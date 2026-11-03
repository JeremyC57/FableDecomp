import sys,re,glob
F=sys.argv[1]; out=[]
for p in sorted(glob.glob(F+"/rk_part_0*.log")):
    for l in open(p,errors="replace"):
        m=re.match(r"^([0-9a-f]{8})\s+(\S+)\s+(\S+)",l)
        if m and m.group(2)!="WARN" and not l.startswith("  "): out.append("\t".join(m.groups()))
open(F+"/grades.tsv","w").write("\n".join(out)+("\n" if out else ""))

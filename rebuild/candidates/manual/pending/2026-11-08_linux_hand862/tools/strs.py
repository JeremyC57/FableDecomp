import os,sys,struct,re
S=os.environ["S"]
exec(open(f"{S}/w22/agent862/bin/rdf.py").read().split("va=int")[0])
for line in open(sys.argv[1]):
    for m in re.finditer(r"0x([0-9a-f]{7,8})", line.split(None,2)[-1]):
        v=int(m.group(1),16)
        if 0x1200000<=v<0x1400000:
            b=rd(v,80) or b""
            s=b.split(b"\0")[0]
            if len(s)>=2 and all(32<=c<127 for c in s): print(line.split()[0], hex(v), s.decode()); break

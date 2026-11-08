# rot.py <probe.out> <funcsubstr>: print the rotation list (and candidate table) of one function from a c2prio output
import sys,re
t=open(sys.argv[1]).read(); f=sys.argv[2]
secs=t.split("="*110)
for s in secs:
    if f in s.split("\n",2)[1] if len(s.split("\n",2))>1 else False:
        i=s.find("expression temporaries")
        print(s[:s.find("not in the list")] if "not in the list" in s else s[:i])
        print(s[i:].split("\n",4)[4] if i>=0 else "(no rotation section)")

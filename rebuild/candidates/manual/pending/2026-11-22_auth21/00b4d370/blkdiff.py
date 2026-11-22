# blkdiff.py A.log B.log [retail]: align built listings of two q.py logs (or retail of A) by instruction text, print per-hunk byte deltas
import sys,re,difflib
def load(p,which):
    L=open(p).read().split('\n'); out=[]; sec=None
    for l in L:
        if l.startswith('--- retail'): sec='r'; continue
        if l.startswith('--- built'): sec='b'; continue
        m=re.match(r'^([0-9a-f]{8})  ([0-9a-f]+)\s+(.*)$',l)
        if m and sec==which:
            t=re.sub(r'0x[0-9a-f]{5,}','X',m.group(3).split(';')[0].strip())
            out.append((int(m.group(1),16),len(m.group(2))//2,t))
    return out
a=load(sys.argv[1],'b'); b=load(sys.argv[2],'r' if len(sys.argv)>3 else 'b')
sm=difflib.SequenceMatcher(None,[x[2] for x in a],[x[2] for x in b],autojunk=False)
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=='equal': continue
    da=sum(x[1] for x in a[i1:i2]); db=sum(x[1] for x in b[j1:j2])
    at=a[i1][0] if i1<len(a) else a[-1][0]
    print(f"{op:7} A@{at:08x} {i2-i1}ins/{da}B  B {j2-j1}ins/{db}B  d={da-db:+d}")

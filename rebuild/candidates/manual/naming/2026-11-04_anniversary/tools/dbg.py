import sys
exec(open(sys.argv[3]).read().split("# validation: hide string anchors")[0].replace("W, M = sys.argv[1], sys.argv[2]","W, M = sys.argv[1], sys.argv[2]"))
anchors = dict(rtti_anchor); anchors.update(str_anchor)
pts = sorted(anchors.items())
print(len(pts), sum(1 for i in range(len(pts)-1) if pts[i+1][1] > pts[i][1]))
import collections
gs=collections.Counter()
for (t1,a1),(t2,a2) in zip(pts,pts[1:]):
    if a2<=a1: continue
    tg=[v for v in tnr if t1<v<t2]; ag=[v for v in asorted if a1<v<a2]
    gs[(min(len(tg),5),min(len(ag),5))]+=1
print(sorted(gs.items())[:40])
eq=sum(1 for t,a in str_anchor.items() if tnr.get(t)==aslots[a]); print("slot-equal string pairs", eq, len(str_anchor))
import collections
print(collections.Counter((tnr.get(t),aslots[a]) for t,a in str_anchor.items()).most_common(12))
import bisect as b
pts = sorted(anchors.items())
tails_idx=[]; prev=[None]*len(pts)
for i,(t,a) in enumerate(pts):
    j=b.bisect_left([pts[k][1] for k in tails_idx], a)
    if j==len(tails_idx): tails_idx.append(i)
    else: tails_idx[j]=i
    prev[i]=tails_idx[j-1] if j>0 else None
print("LIS len", len(tails_idx))

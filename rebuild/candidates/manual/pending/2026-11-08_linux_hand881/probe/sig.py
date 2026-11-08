import sys,re
def sig(fn):
    cases=open(fn).read().split('=== case at ')[1:]
    res=[]
    for c in cases:
        calls=re.findall(r'CALL S[0-9a-f]+ \+= \(in(-?\d+) \* c(\d+)\)',c)
        outs=len(re.findall(r'CALL out\[\d+\] \+=',c)); shr=len(re.findall(r'>>call',c))//2
        res.append((sorted((int(b) for a,b in calls)),outs, len(re.findall(r'CALL S[0-9a-f]+ = ',c))))
    return res
a=sig(sys.argv[1]); b=sig(sys.argv[2])
for i,(x,y) in enumerate(zip(a,b)):
    print('case',i,'retail calls',len(x[0]),'outs',x[1],'shr',x[2],'| ours',len(y[0]),'outs',y[1],'shr',y[2])
    print('   R only', sorted(set(x[0])-set(y[0])), ' ours only', sorted(set(y[0])-set(x[0])))

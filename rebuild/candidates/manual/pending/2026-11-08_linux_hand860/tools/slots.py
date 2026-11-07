# slots.py <addr> <tag>: frame-slot map retail -> ours from instruction pairs that align after slot masking
import sys,os,re,csv,difflib,collections
sys.path.insert(0,os.path.dirname(__file__))
S=os.environ['S']; A=S+'/w22/agent860'; a,tag=sys.argv[1:3]
import importlib.util
sys.argv=[sys.argv[0],a,tag]
src=open(os.path.join(os.path.dirname(__file__),'sd.py')).read().split("ra,oa=dis(rb)")[0]
exec(src)
ra,oa=dis(rb),dis(ob); rn=[norm(x[1]) for x in ra]; on=[norm(x[1]) for x in oa]
sm=difflib.SequenceMatcher(None,rn,on,autojunk=False)
pat=re.compile(r"\[(ebp|esp) ([-+]) (0x[0-9a-f]+|[0-9])\]")
m=collections.defaultdict(collections.Counter)
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op!='equal': continue
    for k in range(i2-i1):
        r=pat.search(ra[i1+k][1]); o=pat.search(oa[j1+k][1])
        if r and o and r.group(1)=='ebp': m[r.group(2)+r.group(3)][o.group(2)+o.group(3)]+=1
for k in sorted(m,key=lambda s:int(s.replace('+','').replace('-','-'),16) if 0 else (s[0]=='+', int(s[1:],16))):
    print(k,'->',dict(m[k]))

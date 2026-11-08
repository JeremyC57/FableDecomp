import sys,re
# cs.py <out> : per-case call signature for retail vs built (q.py output)
t=open(sys.argv[1]).read()
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
def sig(L):
    cases=[];cur=None
    for l in L:
        p=l.split(None,2)
        if len(p)<3: continue
        ins=p[2]
        if re.search(r'mov e.x, dword ptr \[ebp \+ 0x14\]',ins):
            cur=[int(p[0],16),[]]; cases.append(cur); continue
        if cur and ins.startswith('call'):
            tgt=ins.split()[-1]
            cur[1].append(tgt)
    return cases
import subprocess,os
S=os.environ.get('S','/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad')
addr=sys.argv[2] if len(sys.argv)>2 else '00c812f0'; leaf=sys.argv[3] if len(sys.argv)>3 else 'ResampleStereo'
m=subprocess.run(['python3',os.path.dirname(os.path.abspath(__file__))+'/relmap.py',S+'/wine/q/%s/%s.obj'%(addr,addr),leaf,addr],capture_output=True,text=True).stdout
mp={}
T={'??YIs32vec4':'0xc7b870','??5Is32vec4':'0xc7b8b0','??_2Is32vec4':'0xc7b8f0','__security_check':'0xbfe9f9'}
for l in m.split('\n'):
    if not l: continue
    a,s2=l.split(); a=int(a,16)
    for k,v in T.items():
        if k in s2: mp[a-1]=v
    if a-1 not in mp: mp[a-1]=s2
nb=[]
for l in b:
    p=l.split(None,2)
    try:
        a=int(p[0],16)
        if a in mp and 'call' in l: l=l.split('call')[0]+'call '+mp[a]
    except Exception: pass
    nb.append(l)
b=nb
R=sig(r); B=sig(b)
names={}
for i,(x,y) in enumerate(zip(R,B)):
    def comp(c):
        s=''.join({'0xc7b870':'+','0xc7b8b0':'>','0xc7b8f0':'=','0xbfe9f9':'$'}.get(z,'?'+z[:12]) for z in c[1])
        return s
    print(i,'R',comp(x)); print(i,'B',comp(y))

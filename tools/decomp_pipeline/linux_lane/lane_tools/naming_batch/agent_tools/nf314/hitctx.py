# hitctx.py <delete_audit.txt> [entry...]: for each HIT line, the enclosing class / struct (nearest preceding `class X` / `struct X` head
# whose brace is still open, comment-stripped) and the row's stay-off / T3 / lock state. NF314.
import sys,re,os,subprocess
A=os.path.dirname(os.path.abspath(__file__))+'/..'
so=dict(l.split('\t') for l in open(A+'/work/stayoff_why.tsv').read().splitlines())
t3=set(open(A+'/work/t3rows.txt').read().split())
want=set(sys.argv[2:]); cur=None; seen=set()
def src(a):
    if os.path.exists(A+'/rows/%s.cpp'%a): return A+'/rows/%s.cpp'%a
    return subprocess.run(['bash',A+'/tools/srcof.sh',a],capture_output=True,text=True).stdout.strip()
for l in open(sys.argv[1]):
    m=re.match(r'## entry (\d+) ',l)
    if m: cur=m.group(1); print(l.rstrip()[:160]); continue
    if want and cur not in want: continue
    m=re.match(r'\s+HIT (\w{8}):(\d+) (.*)',l)
    if not m: continue
    r,n=m.group(1),int(m.group(2)); lines=open(src(r),encoding='utf-8',errors='replace').read().splitlines()
    depth=0; stack=[]
    for i,x in enumerate(lines[:n]):
        c=x.split('//')[0]
        for mm in re.finditer(r'\b(class|struct)\s+(\w+)[^;{]*\{|\{|\}',c):
            if mm.group(1): stack.append(mm.group(2))
            elif mm.group(0)=='{': stack.append(None)
            else:
                if stack: stack.pop()
    cls=[s for s in stack if s]
    lk=open('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/nflocks/%s/owner'%r).read().strip() if os.path.exists('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/nflocks/%s/owner'%r) else '-'
    print('   %s:%d in %s | so=%s t3=%s lock=%s | %s'%(r,n,'::'.join(cls) or '?',so.get(r,'-'),'T3' if r in t3 else '-',lk,m.group(3)[:90]))

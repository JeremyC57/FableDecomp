# objdiff.py ADDR [--work DIR] : disassembly diff of the gated function, index object (cc/<addr>/x.obj, the landed bytes)
# against the gate work object (DIR/<addr>/x.obj, the wave's rewrite). Prints the symbols of both objects first.
import sys,os,re,subprocess,difflib
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
a=sys.argv[1]; work=L.P+'/gate_work'
if '--work' in sys.argv: work=sys.argv[sys.argv.index('--work')+1]
OBJDUMP=os.environ.get('OBJDUMP','objdump')
def funcs(obj):
    out=subprocess.run([OBJDUMP,'-d','-z','-M','intel',obj],capture_output=True,text=True).stdout
    fns={}; cur=None
    for line in out.splitlines():
        m=re.match(r'^([0-9a-f]+) <(.+)>:$',line)
        if m:
            if int(m.group(1),16)==0 or cur is None: cur=m.group(2); fns[cur]=[]
            continue
        m=re.match(r'^\s*[0-9a-f]+:\s+((?:[0-9a-f]{2} )+)\s*(.*)$',line)
        if m and cur: fns[cur].append((m.group(1).strip(),m.group(2).strip()))
    return fns
leaf=L.ent()[a].get('Leaf') or ''
t=L.read(L.ROOT+'/'+L.src_rel(a)); m=re.search(r'^//\s*Leaf:\s*(\S+)',t,re.M); leaf=m.group(1) if m else ''
A=funcs(L.IDX+'/cc/'+a+'/x.obj'); B=funcs(work+'/'+a+'/x.obj')
def pick(F):
    c=[k for k in F if leaf and leaf in k] or list(F)
    return max(c,key=lambda k:sum(len(x[0].split()) for x in F[k]))
ka=pick(A); kb=pick(B)
print('index  :',ka,sum(len(x[0].split()) for x in A[ka]),'bytes'); print('rewrite:',kb,sum(len(x[0].split()) for x in B[kb]),'bytes')
if '--syms' in sys.argv:
    print('--- index symbols:',*sorted(A),sep='\n  '); print('--- rewrite symbols:',*sorted(B),sep='\n  ')
la=[f'{x[0]:30} {x[1]}' for x in A[ka]]; lb=[f'{x[0]:30} {x[1]}' for x in B[kb]]
for l in difflib.unified_diff(la,lb,'index','rewrite',lineterm='',n=2): print(l)

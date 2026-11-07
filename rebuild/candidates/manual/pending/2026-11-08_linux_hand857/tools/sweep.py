# sweep.py <addr> <base.cpp> <marker> <kinds,comma> [max] : per-site mechanical variants inside the function body after <marker>
# kinds: dw (do{}while(0) round one statement), blk ({ } round one statement), eqt (`if (X())` -> `if (X() == true)`),
#        dinit (`T x = e;` -> `T x(e);`), cst (const on a scalar local), forw (`while (c) {` -> `for (; c; ) {`)
import sys,os,re,subprocess,concurrent.futures as cf
A=os.environ['S']+'/w22/agent857'
addr,base,marker,kinds=sys.argv[1:5]; mx=int(sys.argv[5]) if len(sys.argv)>5 else 999
kinds=kinds.split(',')
lines=open(base).read().split('\n')
start=next(i for i,l in enumerate(lines) if marker in l and not l.lstrip().startswith('//'))
var=[]
for i in range(start+1,len(lines)):
    l=lines[i]; s=l.strip(); ind=l[:len(l)-len(l.lstrip())]
    if s.startswith('//') or not s: continue
    simple = s.endswith(';') and not re.match(r'(return|if|while|for|else|case|default|break|continue)\b|[{}]',s) and '(' in s
    decl = re.match(r'^(const )?[A-Za-z_][\w:<>, ]*[\w>]\s*[&*]*\s*[A-Za-z_]\w*\s*(=\s*.+|\(.*\))?;$',s) and not s.startswith(('return',))
    if 'dw' in kinds and simple and not decl:
        var.append((f'dw{i+1}',f'sweep do-while(0) round L{i+1}',i,f'{ind}do {{ {s} }} while (0);'))
    if 'blk' in kinds and simple and not decl:
        var.append((f'bk{i+1}',f'sweep block round L{i+1}',i,f'{ind}{{ {s} }}'))
    if 'eqt' in kinds:
        m=re.search(r'if \((!?)(IsActiveThreadTerminating\(\))\)',s)
        if m:
            rep = '(IsActiveThreadTerminating() == false)' if m.group(1) else '(IsActiveThreadTerminating() == true)'
            var.append((f'eq{i+1}',f'sweep T-check ==bool L{i+1}',i,l.replace(m.group(0),'if '+rep,1)))
    if 'dinit' in kinds and decl:
        m=re.match(r'^(\s*)((?:const )?[A-Za-z_][\w:<>, ]*[\w>]\s+\**[A-Za-z_]\w*)\s*=\s*(.+);$',l)
        if m and not m.group(3).startswith('{'):
            var.append((f'di{i+1}',f'sweep direct-init L{i+1}',i,f'{m.group(1)}{m.group(2)}({m.group(3)});'))
    if 'cst' in kinds and decl and re.match(r'^\s*(long|int|unsigned long|unsigned int|bool|float|unsigned short|short)\s',l):
        var.append((f'cs{i+1}',f'sweep const local L{i+1}',i,l.replace(s,'const '+s,1)))
    if 'forw' in kinds:
        m=re.match(r'^(\s*)while \((.*)\) \{$',l)
        if m: var.append((f'fw{i+1}',f'sweep while->for L{i+1}',i,f'{m.group(1)}for (; {m.group(2)}; ) {{'))
    if 'post' in kinds:
        m=re.search(r';\+\+(\w+)\)',l)
        if m: var.append((f'po{i+1}',f'sweep postfix increment L{i+1}',i,l.replace(m.group(0),';'+m.group(1)+'++)',1)))
    if 'neq' in kinds:
        m=re.search(r';(\w+)!=([\w.>-]+\(\));',l)
        if m: var.append((f'ne{i+1}',f'sweep !(a==b) loop test L{i+1}',i,l.replace(m.group(0),';!('+m.group(1)+'=='+m.group(2)+');',1)))
done={l.split(chr(9))[2] for l in open(A+'/tmp/attempts.log') if l.split(chr(9))[1:2]==[addr]}
var=[v for v in var if v[0] not in done]
only=os.environ.get('ONLY'); var=[v for v in var if not only or v[0] in only.split(',')][:mx]
jobs=[]
for tag,note,i,new in var:
    t=list(lines); t[i]=new
    if tag.startswith('dw'):
        t.insert(i, ind_ if (ind_:=lines[i][:len(lines[i])-len(lines[i].lstrip())]+'// Inline-split: sweep probe') else '')
    p=f'{A}/probe/{addr}_{tag}.cpp'; open(p,'w').write('\n'.join(t)); jobs.append((tag,note,p))
def run(j):
    tag,note,p=j
    r=subprocess.run(['python3',A+'/bin/gr.py',addr,p,tag,'--note',note,'-q'],capture_output=True,text=True,timeout=400)
    line=[l for l in r.stdout.splitlines() if '\t'+addr+'\t' in l]
    return (line[0].split('\t',2)[2] if line else 'ERR '+r.stdout[-300:]+r.stderr[-300:])
with cf.ThreadPoolExecutor(2) as ex:
    for j,res in zip(jobs,ex.map(run,jobs)): print(res[:140],flush=True)

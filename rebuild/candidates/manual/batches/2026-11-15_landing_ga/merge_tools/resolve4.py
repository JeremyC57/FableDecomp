# Apply the versions merge4 rejected, resolving each conflict hunk by block union (see the coordinator notes in the landing README).
import json,hashlib,subprocess,re,collections
S=os.environ['S']
INC='/home/user/FableDecomp/rebuild/include/'; W=S+'/lc/w'; OUT=S+'/lc/inc'
rep=json.load(open(S+'/lc/merge4_report.json'))
code=lambda s:[x for x in s if not x.strip().startswith('//')]
def blocks(lines):
    out=[];i=0
    while i<len(lines):
        s=lines[i].strip()
        if re.match(r'#\s*if',s):
            d=0;j=i
            while j<len(lines):
                t=lines[j].strip()
                if re.match(r'#\s*if',t): d+=1
                elif re.match(r'#\s*endif',t):
                    d-=1
                    if d==0: break
                j+=1
            out.append(('B',lines[i:j+1])); i=j+1
        elif re.match(r'#\s*(endif|else|elif)',s): out.append(('C',[lines[i]])); i+=1
        else: out.append(('L',[lines[i]])); i+=1
    return out
def opener_names(b):
    m=re.findall(r'defined\s*\(\s*(\w+)\s*\)',b[0]) or re.findall(r'#\s*ifn?def\s+(\w+)',b[0]); return m
def body_code(b): return [re.sub(r'\b(\w+)\)',')',x) for x in code(b[1:])]  # crude: param names ignored for decl compare
DECL=re.compile(r'(\w+)\s*\(')
def members(b): return set(m.group(1) for x in code(b[1:-1]) for m in [DECL.search(x)] if m)
def resolve(O,T,rel):
    bo,bt=blocks(O),blocks(T)
    closers=[];seen=set()
    for k,l in bo+bt:
        if k=='C':
            key=re.sub(r'\s*//.*','',l[0]).strip()
            if key not in seen: seen.add(key); closers.append(l[0])
    res=[x for k,l in bo if k=='B' for x in l]
    oblk=[l for k,l in bo if k=='B']
    for k,l in bt:
        if k!='B': continue
        tn=opener_names(l)
        same=[ob for ob in oblk if opener_names(ob)==tn]
        if same:
            ob=same[0]
            if code(ob)!=code(l) and set(code(l))-set(code(ob)): print('   !! same opt-in, different code:',rel,tn)
            extra=[x for x in l if x.strip().startswith('//') and x not in ob]
            if extra: i=res.index(ob[0]); res[i+1:i+1]=extra
            continue
        ov=[ob for ob in oblk if members(ob) and members(ob)==members(l)]
        if ov:
            ob=ov[0]; i=res.index(ob[0]); names=opener_names(ob)+[n for n in tn if n not in opener_names(ob)]
            res[i]='#if '+' || '.join('defined(%s)'%n for n in names)
            res[i+1:i+1]=[x for x in l if x.strip().startswith('//')]
            print('   condmerge',rel,names); continue
        res+=l
    loose=[x for k,l in bo if k=='L' for x in l]+[x for k,l in bt if k=='L' for x in l if x not in [y for k2,l2 in bo if k2=='L' for y in l2]]
    return closers+res+loose
for rel,d,_ in rep:
    if d=='UNBALANCED': continue
    o=W+'/'+hashlib.md5((rel+d).encode()).hexdigest()+'.u4'; cur=OUT+'/'+rel
    r=subprocess.run(['git','merge-file','-p','--diff3',cur,INC+rel,o],capture_output=True); t=r.stdout.decode('latin-1').split('\n')
    if r.returncode==0: open(cur,'w',encoding='latin-1').write('\n'.join(t)); print('clean now',rel,d); continue
    out=[];st=None;Ol=[];Bl=[];Tl=[]
    for l in t:
        if st is None and l.startswith('<<<<<<< '): st='o';Ol=[];Bl=[];Tl=[];continue
        if st=='o' and l.startswith('||||||| '): st='b';continue
        if st=='b' and l=='=======': st='t';continue
        if st=='t' and l.startswith('>>>>>>> '):
            if Bl: print('   !! non-empty base hunk',rel,d,Bl[:2])
            out+=resolve(Ol,Tl,rel); st=None; continue
        if st=='o': Ol.append(l)
        elif st=='b': Bl.append(l)
        elif st=='t': Tl.append(l)
        else: out.append(l)
    open(cur,'w',encoding='latin-1').write('\n'.join(out)); print('resolved',rel,d)
def balanced(p):
    d=0
    for l in open(p,encoding='latin-1'):
        s=l.strip()
        if re.match(r'#\s*if',s): d+=1
        elif re.match(r'#\s*endif',s): d-=1
        if d<0: return False
    return d==0
items=json.load(open(S+'/lc/items.json'))
print('unbalanced:',[r for r in items if not balanced(OUT+'/'+r)])

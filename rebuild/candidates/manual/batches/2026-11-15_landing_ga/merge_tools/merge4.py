# Per header: versions = patch(base, folder diff) with folder assert pins undone. Start from the version that adds the most lines;
# 3-way merge (git merge-file) each other version into it. A version whose added lines (normalised: #elif/#ifdef -> #if defined, and
# widened conditions split into names) are all present already is skipped. Remaining conflicts are written out for hand resolution.
import json,os,shutil,collections,hashlib,re,subprocess
S=os.environ['S']
R='/home/user/FableDecomp'; INC=R+'/rebuild/include/'
items=json.load(open(S+'/lc/items.json')); OUT=S+'/lc/inc'; W=S+'/lc/w'; CF=S+'/lc/conf4'
shutil.rmtree(OUT,ignore_errors=True); shutil.copytree(INC,OUT); shutil.rmtree(CF,ignore_errors=True); os.makedirs(CF)
PIN=re.compile(r'FABLE_STATIC_ASSERT_AT\(\s*\d+\s*,\s*')
def unpin(lines,bset):
    out=[]
    for l in lines:
        if 'FABLE_STATIC_ASSERT_AT(' in l and l not in bset:
            u=PIN.sub('FABLE_STATIC_ASSERT(',l,1)
            if u in bset: l=u
        out.append(l)
    return out
def norm_tokens(lines):
    out=collections.Counter()
    for l in lines:
        s=l.strip()
        m=re.match(r'#\s*(?:el)?if(?:def)?\b(.*)',s)
        if m and not s.startswith('#endif'):
            ns=re.findall(r'defined\s*\(\s*(\w+)\s*\)',m.group(1)) or re.findall(r'^\s*(\w+)\s*$',m.group(1))
            if ns:
                for n in ns: out['COND:'+n]+=1
                continue
        out[s]+=1
    return out
def balanced(text):
    d=0
    for l in text.split('\n'):
        s=l.strip()
        if re.match(r'#\s*if',s): d+=1
        elif re.match(r'#\s*endif',s): d-=1
        if d<0: return False
    return d==0
report=[]
for rel,v in sorted(items.items()):
    cur=OUT+'/'+rel
    if v[0][0]=='new':
        best=max((open(x[2],encoding='latin-1').read() for x in v),key=len)
        os.makedirs(os.path.dirname(cur),exist_ok=True); open(cur,'w',encoding='latin-1').write(best); continue
    btxt=open(INC+rel,encoding='latin-1').read(); B=btxt.split('\n'); bset=set(B); bc=norm_tokens(B)
    vers=[]
    for kind,d,diff in v:
        o=W+'/'+hashlib.md5((rel+d).encode()).hexdigest()
        V=unpin(open(o,encoding='latin-1').read().split('\n'),bset)
        f=o+'.u4'; open(f,'w',encoding='latin-1').write('\n'.join(V))
        vers.append((len(V),d,f,norm_tokens(V)-bc))
    vers.sort(key=lambda x:-x[0])
    shutil.copy(vers[0][2],cur); have=vers[0][3]; used=[vers[0][1]]
    for n,d,f,add in vers[1:]:
        if not (add-have): continue
        r=subprocess.run(['git','merge-file','-p',cur,INC+rel,f],capture_output=True)
        if r.returncode==0:
            open(cur,'wb').write(r.stdout); have=norm_tokens(r.stdout.decode('latin-1').split('\n'))-bc; used.append(d)
        else:
            report.append((rel,d,sorted((add-have).elements())[:4]))
            open(CF+'/'+rel.replace('/','_')+'__'+d,'wb').write(r.stdout)
    if not balanced(open(cur,encoding='latin-1').read()): report.append((rel,'UNBALANCED',''))
json.dump(report,open(S+'/lc/merge4_report.json','w'),indent=1)
print('problems',len(report))
for x in report: print(x[0],x[1],[s[:90] for s in x[2]] if x[2] else '')

# cls.py <from> <to>: classification read per ENTRIES line: every stand-in table line (full, from reloc_bindings.tsv) with its witness
# binder: source file, OFFLIMITS mark, the code lines naming the leaf identifier, the scope token's struct/class/namespace definition
# line, and the binder's engine includes; plus whether engine/<PDB class>.h exists and which of its lines name the leaf (with the
# nearest enclosing #ifdef macro).
import re,sys,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF295'
R='/home/user/FableDecomp/'
ps=open(R+'rebuild/build_candidates.ps1',encoding='utf-8').read()
off=set(open(A+'/work/offlimits.txt').read().split())
sib=set(open(A+'/work/sibwit.txt').read().split())
def src(a):
    i=ps.find("Address = '%s'"%a)
    if i<0: return None
    j=ps.find("[pscustomobject]",i); blk=ps[i:j if j>0 else i+700]   # NF295: bounded to the row's own block
    m=re.search(r"\bSource = '([^']+)'",blk); return R+'rebuild/src/compiled/'+m.group(1) if m else None
T={}
for l in open(R+'rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2]))
def leaf_scope(sym):
    if sym.startswith('@') or sym.startswith('_'): return re.split(r'@',sym.lstrip('@_'))[0],''
    s=sym[1:]
    if s.startswith('?'):
        m=re.match(r'\?(\w\w?)(.*)',s); s2=m.group(2) if m else s
        parts=[p for p in s2.split('@') if p]; return ('op?'+(m.group(1) if m else ''), parts[0].lstrip('?$') if parts else '')
    parts=s.split('@'); return parts[0], (parts[1].lstrip('?$') if len(parts)>1 else '')
L=open(A+'/ENTRIES.tsv').read().splitlines()
for n in range(int(sys.argv[1]),int(sys.argv[2])+1):
    c=L[n].split('\t'); t,pdb,self_leaf=c[0],c[6],c[7]
    pl,pc=leaf_scope(pdb)
    print('#%d %s [%s] self=%s :: %s'%(n,t,c[4],'Y' if self_leaf else '-',pdb))
    if t in off: print('   TARGET OFFLIMITS')
    cls=c[4]; h=R+'rebuild/include/engine/%s.h'%cls
    if os.path.exists(h):
        lines=open(h,encoding='utf-8',errors='replace').read().splitlines(); stack=[]
        hits=[]
        for i,l in enumerate(lines,1):
            m=re.match(r'\s*#\s*(ifdef|ifndef|if)\s+(.*)',l)
            if m: stack.append(m.group(2).strip()[:50])
            elif re.match(r'\s*#\s*endif',l) and stack: stack.pop()
            elif re.match(r'\s*#\s*else',l) and stack: stack[-1]='!'+stack[-1]
            if pl and re.search(r'\b%s\b'%re.escape(pl),l.split('//')[0]): hits.append('%d[%s]'%(i,stack[-1] if stack else '-'))
        print('   header engine/%s.h: leaf lines %s'%(cls,' '.join(hits[:6]) or 'NONE'))
    else: print('   header engine/%s.h: absent'%cls)
    for s,w in T.get(t,[]):
        if s==pdb: print('   = PDB wit %s'%w); continue
        sl,sc=leaf_scope(s)
        f=src(w)
        print('   S %s wit=%s%s%s'%(s[:150],w,' OFFLIMITS' if w in off else '',' SIBWIT' if w in sib else ''))
        if not f: print('      no landed source'); continue
        txt=open(f,encoding='utf-8',errors='replace').read(); code=[l.split('//')[0] for l in txt.splitlines()]
        print('      %s (%d lines)'%(f.split('/')[-1][:80],len(code)))
        inc=[l.strip() for l in code if re.match(r'\s*#\s*include\s*"(engine|stlport|fable)',l)]
        print('      inc: %s'%' '.join(x.split()[-1] for x in inc)[:200])
        if sc:
            d=[(i+1,l.strip()) for i,l in enumerate(code) if re.search(r'\b(struct|class|namespace)\s+%s\b'%re.escape(sc),l)]
            for i,l in d[:2]: print('      def %d: %s'%(i,l[:170]))
        k=0
        for i,l in enumerate(code):
            if sl and re.search(r'\b%s\b'%re.escape(sl),l):
                print('      %d| %s'%(i+1,l.strip()[:180])); k+=1
                if k>=3: break

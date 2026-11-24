# tmplcls.py <lo> <hi>: for template-PDB entries (pdb spelling starts ??$), per non-PDB table spelling: witness, stay-off flag, whether the
# witness source DEFINES the stand-in identifier (a `Ident(...) {` body or `::Ident(` definition) or only declares it (DECL), and whether
# the stand-in itself is a template instance (??$). Source reading only. NF309.
import re,sys,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309'
R='/home/user/FableDecomp/rebuild/'
why=dict(l.split('\t') for l in open(A+'/work/stayoff_why.tsv').read().splitlines())
T={}
for l in open(R+'oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2].lower()))
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
def src(a): return subprocess.run(['bash',A+'/tools/srcof.sh',a],capture_output=True,text=True).stdout.strip()
for n in range(int(sys.argv[1]),int(sys.argv[2])+1):
    t=L[n][0]; P=L[n][6]
    if not P.startswith('??$'): print('#%d %s not-template'%(n,t)); continue
    out=[]
    for s,w in T.get(t,[]):
        if s==P: continue
        m=re.match(r'\?+\$?([A-Za-z_]\w*)@',s); ident=m.group(1) if m else '?'
        f=src(w); kind='NOSRC'
        if f:
            code='\n'.join(x.split('//')[0] for x in open(f,encoding='utf-8',errors='replace').read().splitlines())
            # NF309 fix: a definition = the identifier as a declarator (line text before it has no call / control context), its
            # balanced parameter list, optional const, then a body `{`; the defining line is printed so the verdict can be read.
            kind='DECL'
            for m in re.finditer(r'\b%s\s*\('%re.escape(ident),code):
                ls=code.rfind('\n',0,m.start())+1; pre=code[ls:m.start()]; pre=re.sub(r'__declspec\([^)]*\)','',pre)
                if re.search(r'\b(if|while|return|for|switch)\b|->|\.\s*$|[=(!&|?]',pre): continue
                k=m.end(); d=1
                while k<len(code) and d: d+= {'(':1,')':-1}.get(code[k],0); k+=1
                rest=code[k:k+60].lstrip()
                rest=rest[5:].lstrip() if rest.startswith('const') else rest
                if rest.startswith('{'): kind='DEF[L%d %s]'%(code.count('\n',0,m.start())+1,code[ls:m.start()+len(ident)].strip()[:80]); break
        out.append('%s:%s%s:%s:%s'%(w,kind,('/SO='+why[w]) if w in why else '','TMPL' if s.startswith('??$') else 'plain',s[:70]))
    print('#%d %s  %s'%(n,t,'  '.join(out)))

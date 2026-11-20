"""tnc.py <addr>...: make the row-local CBaseClassNonCopyable default ctor protected (binds ??0CBaseClassNonCopyable@@IAE@XZ)."""
import sys,re,os
R='/home/user/FableDecomp/rebuild/src/compiled/'; D='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF181/'
bc=open('/home/user/FableDecomp/rebuild/build_candidates.ps1',encoding='utf-8-sig').read()
for a in sys.argv[1:]:
    m=re.search(r"Address = '%s'[^\n]*\n(?:\s*Module[^\n]*\n)?\s*Source = '([^']*)'"%a,bc,re.I)
    if not m: print(a,'NOSRC'); continue
    p=D+'rows/'+a+'.cpp'
    s=open(p,encoding='utf-8',errors='surrogateescape').read() if os.path.exists(p) else open(R+m.group(1),encoding='utf-8',errors='surrogateescape').read()
    h=list(re.finditer(r'(?:class|struct)\s+CBaseClassNonCopyable\b[^;{]*\{',s))
    if len(h)!=1: print(a,'HEADS',len(h)); continue
    i=h[0].end(); depth=1; j=i
    while depth and j<len(s):
        if s[j]=='{': depth+=1
        elif s[j]=='}': depth-=1
        j+=1
    body=s[i:j-1]
    dm=list(re.finditer(r'(?m)^([ \t]*)(CBaseClassNonCopyable\s*\(\s*(?:void)?\s*\)\s*)(;|\{\s*\})(.*)$',body))
    if len(dm)!=1:
        # one-line class body: decl not at line start
        dm=list(re.finditer(r'()(CBaseClassNonCopyable\s*\(\s*(?:void)?\s*\)\s*)(;|\{\s*\})()',body))
        if len(dm)!=1: print(a,'DECLS',len(dm)); continue
    d=dm[0]
    if d.group(3)!=';': print(a,'INLINEBODY'); continue
    new=body[:d.start(2)]+'protected: '+d.group(2)+d.group(3)+' public:'+body[d.end(3):]
    s=s[:i]+new+s[j-1:]
    s=s.replace(d.group(4) if False else '', '')
    note="// NF181 (2026-11-20, q11902): the local CBaseClassNonCopyable default ctor is protected (Ego_r.pdb / FableWin.pdb\n//   ??0CBaseClassNonCopyable@@IAE@XZ, engine/CBaseClassNonCopyable.h); access only, was public (QAE).\n"
    lines=s.split('\n'); k=0
    while k<len(lines) and lines[k].startswith('//'): k+=1
    lines.insert(k,note.rstrip('\n'))
    open(p,'w',encoding='utf-8',errors='surrogateescape').write('\n'.join(lines)); print(a,'ok')

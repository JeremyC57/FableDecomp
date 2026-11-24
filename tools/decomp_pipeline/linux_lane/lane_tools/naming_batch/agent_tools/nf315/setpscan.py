# setpscan.py: for every landed source declaring SetPItem(CThing*), the enclosing class / namespace of that declaration (nearest
# preceding class|struct head, brace depth) -> work/setp_scan.tsv; candidates = enclosing class exactly CBaseIntelligentPointer, global
import re,os,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
R='/home/user/FableDecomp/rebuild/src/compiled/'
off={l.split()[0] for l in open(A+'/work/offwhy.tsv')}
out=[]
for rel in open(A+'/work/setp_declarers.txt').read().split():
    f=R+rel[2:]; a=re.search(r'([0-9a-f]{8})\.cpp$',f).group(1)
    t=open(f,encoding='utf-8',errors='replace').read()
    t2=re.sub(r'//[^\n]*','',t); t2=re.sub(r'/\*.*?\*/','',t2,flags=re.S)
    stack=[]; i=0; res=None
    toks=re.finditer(r'(namespace\s+(\w+)\s*\{)|((?:class|struct)\s+(\w+)[^;{(]*\{)|(\{)|(\})|(SetPItem\s*\(\s*(?:class\s+)?CThing\s*\*[^)]*\)\s*;)',t2)
    for m in toks:
        if m.group(1): stack.append(('ns',m.group(2)))
        elif m.group(3): stack.append(('cls',m.group(4)))
        elif m.group(5): stack.append(('blk',''))
        elif m.group(6):
            if stack: stack.pop()
        elif m.group(7):
            res='::'.join(n for k,n in stack if k!='blk'); break
    lk=''
    try: lk=open('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/nflocks/%s/owner'%a).read().strip()
    except Exception: pass
    out.append('%s\t%s\t%s\t%s'%(a,res,'OFF' if a in off else '-',lk or '-'))
open(A+'/work/setp_scan.tsv','w').write('\n'.join(out)+'\n')
import collections; print(collections.Counter(l.split('\t')[1] for l in out))
for l in out:
    if l.split('\t')[1]=='CBaseIntelligentPointer': print(l)

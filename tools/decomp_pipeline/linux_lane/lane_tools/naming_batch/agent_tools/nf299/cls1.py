# cls1.py: first-pass class of the T1 "other" entries (ENTRIES lines 7..): per stand-in table line, compare the stand-in's scope
# (the mangled name between the leaf and '@@') with the PDB spelling's scope. SAMESCOPE = only the leaf (and perhaps the signature)
# differs, so a declaration respell in the binder's local model may do; otherwise the scope is a stand-in (port, T3).
import re,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299'
T={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: T.setdefault(r[1].lower(),[]).append((r[0],r[2]))
def scope(s):
    if s.startswith('??$') or s.startswith('??'): return 'OP:'+s.split('@@')[0][3:] if False else None
    if s.startswith('?'):
        m=re.match(r'\?([^@]+)@(.*?)@@',s)
        if m: return m.group(1),m.group(2)
        m=re.match(r'\?([^@]+)@@',s)
        if m: return m.group(1),''
    if s.startswith('@'): return s[1:].split('@')[0],'FASTCALLFREE'
    return None
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
lo=int(sys.argv[1]) if len(sys.argv)>1 else 1; hi=int(sys.argv[2]) if len(sys.argv)>2 else len(L)-1
for n in range(lo,hi+1):
    c=L[n]; pdb=c[6]; ps=scope(pdb)
    for s,w in T.get(c[0].lower(),[]):
        if s==pdb: continue
        ss=scope(s)
        if ps and ss and ss[1]==ps[1]: k='SAMESCOPE'
        elif ps and ss and ss[1]=='' and ps[1]!='': k='FREE_V_MEMBER'
        elif ss is None or ps is None: k='TEMPLATE/OP'
        else: k='OTHERSCOPE'
        print('%d\t%s\t%s\t%s\t%s\t%s\t%s'%(n,c[0],k,w,(ss or ('',''))[0],s[:110],pdb[:110]))

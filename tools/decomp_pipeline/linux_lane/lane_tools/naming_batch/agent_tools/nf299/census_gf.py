# census_gf.py: every landed source naming a GF[AV][XY]To[AV][XY] converter: exe call targets of the row (from the evidence xsites
# files), identifiers it calls and their calling convention at HEAD and in rows/<addr>.cpp (if changed); then a consistency check
# (each called address must be spelled by its PDB name: 0x009E1DA0 GFAXToVX, 0x009E1DD0 GFAYToVY, 0x009E1E10 GFVXToAX, 0x009E1E40 GFVYToAY).
import re,subprocess,collections,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299'
R='/home/user/FableDecomp/'
N={'009e1da0':'GFAXToVX','009e1dd0':'GFAYToVY','009e1e10':'GFVXToAX','009e1e40':'GFVYToAY'}
calls=collections.defaultdict(set)
for f in ('xsites_009e1e40.tsv','xsites_009e1dd0_009e1e10.tsv','xsites_009e1da0.tsv'):
    for l in open(A+'/evidence/'+f):
        t,k,site,row=l.rstrip('\n').split('\t')
        if row!='-': calls[row[:8]].add(t)
ps=open(R+'rebuild/build_candidates.ps1',encoding='utf-8').read(); src={}
for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'[^}]*?\bSource = '([^']+)'",ps): src[m.group(2)]=m.group(1).lower()
out=subprocess.run(['grep','-rlE','GF[AV][XY]To[AV][XY]','rebuild/src/compiled'],capture_output=True,text=True,cwd=R).stdout.split()
def scan(p):
    code='\n'.join(l.split('//')[0] for l in open(p,encoding='utf-8',errors='replace').read().splitlines())
    return (','.join(sorted(set(re.findall(r'\b(GF[AV][XY]To[AV][XY]\w*)\s*\(',code)))),
            ','.join(sorted(set(re.findall(r'(__stdcall|__fastcall|FABLE_FASTCALL)\s+GF[AV][XY]To[AV][XY]\w*',code)))))
print('# row\texe_calls\tHEAD_used\tHEAD_conv\tnew_used\tnew_conv\tsource'); bad=0
for f in sorted(out):
    rel=f.replace('rebuild/src/compiled/',''); a=src.get(rel,'?')
    u,c=scan(R+f); nu,nc=u,c
    if os.path.exists(A+'/rows/'+a+'.cpp'): nu,nc=scan(A+'/rows/'+a+'.cpp')
    cl=calls.get(a,set()); print('\t'.join([a,','.join(sorted(cl)) or '-',u,c,nu,nc,rel]))
    exp=set(N[x] for x in cl); got=set(x for x in nu.split(',') if x)
    if cl and got and got!=exp: bad+=1; print('# MISMATCH',a,sorted(cl),nu)
    if 'stdcall' in nc: bad+=1; print('# STDCALL',a,nu)
print('# consistency problems:',bad)

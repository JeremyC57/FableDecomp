import sys,collections,re
sys.path.insert(0,sys.argv[2])
import pdb as P
p=P.PDB(sys.argv[1]); t=p.tpi
eg=collections.OrderedDict(); size={}
for l in open(sys.argv[3],encoding='latin-1'):
    s=l.rstrip('\n').split('\t')
    if l.startswith('@') or len(s)<5: continue
    size[s[0]]=int(s[1]); eg.setdefault(s[0],[])
    if s[4] not in ('_padding_',''): eg[s[0]].append(s[4])
n=same_list=same_size=0; ratios=[]; ex=[]
for c,mem in eg.items():
    if not re.match(r"(N[A-Z]\w*::)*C[A-Z]",c) or c.startswith(("CComPtr","CD3D","CWin")): continue
    ti=t.resolve(c)
    if ti is None: continue
    r=t.tag(ti)
    if not r or r[2] is None: continue
    xm=[f['name'] for f in t.fields(r[3]) if f['kind']=='member']
    if not mem or not xm: continue
    n+=1
    if xm==mem: same_list+=1
    if r[2]==size[c]: same_size+=1
    ratios.append(len(set(mem)&set(xm))/len(set(mem)))
    if xm!=mem and len(ex)<8 and len(mem)>3 and len(set(mem)&set(xm))/len(set(mem))<0.8: ex.append((c,size[c],r[2],[m for m in mem if m not in xm][:4],[m for m in xm if m not in mem][:4]))
ratios.sort()
print('classes in both with members',n,'identical member list',same_list,'same size',same_size)
print('share of TLC member names kept: median %.2f, p25 %.2f'%(ratios[len(ratios)//2],ratios[len(ratios)//4]))
for e in ex: print(e)

#!/usr/bin/env python3
import os,re,glob,csv
I=os.path.dirname(os.path.abspath(__file__))
match={}
for mf in ('/matches.tsv','/matches2.tsv'):
  for ln in open(I+mf):
    a,f,n=ln.rstrip('\n').split('\t'); match[a]=(f,n)
best={}
for ln in open(I+'/greedy_best.txt'):
    p=ln.split(); best[p[0]]=(p[2],' '.join(p[4:6]))
ex={}
cur=None
if os.path.exists(I+'/exall.log'):
    for ln in open(I+'/exall.log'):
        if ln.startswith('=='): cur=ln.split()[1]; ex.setdefault(cur,None)
        elif ln.startswith('best') and cur and ex.get(cur) is None: ex[cur]=' '.join(ln.split()[3:5])
plain={}
for ln in open(I+'/g_pd.txt'):
    p=ln.split(); plain[p[0]]=p[1]+' '+p[2]
rows=[l.split('\t')[0] for l in open(I+'/rows90.tsv')]
out=open(I+'/summary.tsv','w')
out.write('row\told_grade\tnew_grade\tlever\tattempts\tbest_source\n')
for a in rows:
    d=f"{I}/rows/{a}"
    att=len(set(glob.glob(d+'/g/*.cpp')+glob.glob(d+'/ex/*.cpp')+glob.glob(d+'/x/*.cpp')+glob.glob(d+'/xs/*.cpp')+glob.glob(d+'/ex3/*.cpp')+glob.glob(d+'/go2/g/*.cpp')+[f for f in glob.glob(d+'/*.cpp') if not f.endswith('landed.cpp')]))
    if a in match:
        f,n=match[a]; new='RELOCATION_MATCH+PASS (vland recheck)'; lever=n; src=f"out/{a}.cpp"
    else:
        cand=[]
        if a in best: cand.append(best[a][1]+' mask '+best[a][0])
        if ex.get(a): cand.append('exhaustive best '+ex[a])
        new='; '.join(cand) if cand else 'plain '+plain.get(a,'?')
        lever='-'; src='-'
    out.write(f"0x{a.upper()}\tRELOCATION_MATCH (inline_depth pragma)\t{new}\t{lever}\t{att}\t{src}\n")
P={'00f3ea50':('DIFFER(2603v2379)','DIFFER(2443v2379) dl=1011 (call sequence = retail)','loop guard if (section_strip.size() != 0) before the strip append loop','12','parked/00f3ea50/best.cpp'),
   '005ff190':('DIFFER(4604v4176)','DIFFER(4128v4176) dl=1144 (all 9 Reset sites = retail)','loop guards on loop 1 (outer + inner) + loop 2 as while(it != end)','12','parked/005ff190/best.cpp'),
   '0074bc20':('DIFFER(1415v1290)','DIFFER(1323v1290) (call sequence = retail)','if (OpenPositions.size() > 0) loop guard + FABLE_RETAIL_CRGBCOLOUR; residue frame 0xC8 v 0xB0','10','parked/0074bc20/best.cpp'),
   '00f3d980':('DIFFER(2301v1947)','DIFFER(1789v1947) dl=435','continue; at the end of the fan-walk for(;;) (section.push_back now called); rings.push_back nested-call residue','13','parked/00f3d980/best.cpp'),
   '00e2f170':('DIFFER(2163v2781)','DIFFER(2344v2781) dl=443','do-while(0) checks + GetPos through PImp.IsNull()/operator->','18','parked/00e2f170/best.cpp'),
   '00e2ec30':('DIFFER(1053v1336)','DIFFER(1093v1336) dl=459','FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ; masks do not help','13','parked/00e2ec30/best.cpp'),
   '00603000':('DIFFER(9852v9571)','DIFFER(9847v9571) dl=3649','loop guard on the cached-marker erase loop; UI_INTERESTING Reset still expanded','10','parked/00603000/best.cpp')}
for a,(o,n,l,t,s) in P.items():
    out.write(f"0x{a.upper()}\t{o} (parked)\t{n}\t{l}\t{t}\t{s}\n")
print('ok')

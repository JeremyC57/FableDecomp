# Vendor naming inventory: TU strings (__FILE__) per function, binding spellings per target, manifest names.
import sys,os,re,csv,bisect,collections
sys.path.insert(0,'tools/decomp_pipeline/linux_lane/lane_tools/bounds')
from extent import Exe
E=Exe(); OUT=sys.argv[1]
LO,HI=0xC08AE0,0xCB8000
# strings in .rdata
rd=[s for s in E.secs if s[0]=='.rdata'][0]; name,va,vs,raw,rs=rd
data=E.d[raw:raw+rs]
strs={}
for m in re.finditer(rb'[\x20-\x7e]{5,}\x00',data):
    s=m.group()[:-1].decode()
    if re.search(r'\.(cpp|h)$',s,re.I) or re.search(r'nRefs|IsValidString|GetData\(\)|m_pchData',s):
        strs[va+m.start()]=s
text=E.text; tlo=E.tlo
fn=collections.defaultdict(set)
for sva,s in strs.items():
    pat=sva.to_bytes(4,'little')
    for m in re.finditer(re.escape(pat),text):
        a=tlo+m.start(); i=bisect.bisect_right(E.sstarts,a)-1
        if i<0: continue
        f=E.sstarts[i]
        if LO<=f<HI: fn[f].add(s.split('\\')[-1])
man={x['address'].lower():x for x in csv.DictReader(open('rebuild/manifest/functions.tsv',encoding='utf-8-sig'),delimiter='\t')}
syms=collections.defaultdict(collections.Counter)
for r in csv.DictReader(open('rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'):
    t=r['target'].lower()
    if re.fullmatch(r'[0-9a-f]{8}',t) and LO<=int(t,16)<HI: syms[t][r['symbol']]+=1
def rng(v):
    return ('libpk' if v<0xC57C90 else 'gamecoda' if v<0xC5E180 else 'libpk2' if v<0xC89000 else 'vorbis' if v<0xC9F000 else 'c9f-cb8')
addrs=sorted(set(int(a,16) for a in man if LO<=int(a,16)<HI)|set(fn)|set(int(t,16) for t in syms))
with open(OUT,'w') as f:
    f.write('address\trange\tlanded\tmodule\tname\ttu_strings\tn_spellings\tspellings\n')
    for v in addrs:
        a='%08x'%v; m=man.get(a,{})
        sp=syms.get(a,collections.Counter())
        f.write('\t'.join([a,rng(v),'1' if m.get('compiled_source') else '0',m.get('module',''),m.get('name',''),
            '|'.join(sorted(fn.get(v,()))),str(len(sp)),' | '.join('%s(%d)'%(k,c) for k,c in sp.most_common())])+'\n')
print(len(strs),'strings',len(fn),'functions with TU strings',len(addrs),'rows')

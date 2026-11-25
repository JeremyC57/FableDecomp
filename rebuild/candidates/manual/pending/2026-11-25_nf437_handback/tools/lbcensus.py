#!/usr/bin/env python3
# lbcensus.py: landed rows whose retail bytes call 0x0040F020 / 0x004365B0, with the identifiers the row's source uses there.
import sys, os, re, csv, glob, struct, collections
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF423/tools'); import nf
R='/home/user/FableDecomp'
cat = nf.catalog()
T = {0x0040f020: 'NC', 0x004365b0: 'C'}
def calls(a):
    r = cat.get(a)
    if not r or not r.get('bytes'): return set()
    b = bytes.fromhex(r['bytes']); va = int(a,16); out=set()
    for i in range(len(b)-4):
        if b[i] in (0xE8,0xE9):
            t=(va+i+5+struct.unpack_from('<i',b,i+1)[0])&0xffffffff
            if t in T: out.add(T[t])
    return out
files = {}
for f in glob.glob(R+'/rebuild/src/compiled/*/*/*.cpp'):
    m = re.search(r'_([0-9a-f]{8})\.cpp$', f)
    if m: files.setdefault(m.group(1), f)
tab = collections.defaultdict(list)
for r in csv.DictReader(open(R+'/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
    if r['target'] in ('0040f020','004365b0'): tab[r['symbol']].append(r['address'])
out = open(sys.argv[1],'w')
n=0
for a,f in sorted(files.items()):
    k = calls(a)
    if not k: continue
    x = open(f, encoding='utf-8', errors='replace').read()
    exp = 'GETTC_EXPANDED' if 'FABLE_RETAIL_CTHING_GETTC_EXPANDED' in x else ''
    lb = sorted(set(re.findall(r'\b(\w*(?:LowerBound|lower_bound|Find|FindConst|find)\w*)\s*\(', x)))
    cls = sorted(set(re.findall(r'\b(MazeTable|CCreatureInterfaceMap|CInterfaceMapB?|CTCMap(?:_Retail)?|MapT|HeroMap|Sub|CTCInterfaceMap|ComponentMap|DoorMap|NCondition00935160|NFactory008385b0|Row00868b40|N00809d60|CVectorMap)\b', x)))
    out.write('\t'.join([a, ','.join(sorted(k)), exp, ' '.join(cls), ' '.join(lb)[:200], os.path.relpath(f,R)])+'\n'); n+=1
print('rows', n)

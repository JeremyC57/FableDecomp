"""li_a.py <addr>...: sub-family A transform (rows declaring a global struct CLayerInfo + extern LayerInfo_0137dad8[]): the struct line becomes
the engine/NThingMap_CThingMap.h FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY include, the extern line goes, LayerInfo_0137dad8[ -> NThingMap::LayerInfo[.
Reads the repo row (or work/li/<addr>.cpp if present? no: always the repo row), writes work/li/<addr>.cpp."""
import sys,re,os
D='$S/w22/agentNF100'
os.chdir('/home/user/FableDecomp')
src={}
for l in open(D+'/work/li_pp.tsv'):
    f=l.rstrip('\n').split('\t'); src[f[0]]=f[3]
os.makedirs(D+'/work/li',exist_ok=True)
NOTE=('// Name fix (NF100, round 9, q11463 / q11282): the layer table is NThingMap::LayerInfo (Ego_d.pdb data symbol '
      '?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B, retail 0x0137DAD8;\n//   element NThingMap::CLayerInfo, Ego_r.pdb 16 bytes, ScaleShift +4), declared by engine/NThingMap_CThingMap.h '
      '(opt-in FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY:\n//   the table alone, beside this row\'s local NThingMap model). It was the global stand-in ?LayerInfo_0137dad8@@3PAUCLayerInfo@@A.\n')
for a in sys.argv[1:]:
    s=open('rebuild/src/compiled/'+src[a],encoding='utf-8',errors='surrogateescape').read()
    L=s.split('\n'); out=[]; n_struct=n_ext=0
    for l in L:
        if re.match(r'^\s*struct CLayerInfo\s*\{.*\};\s*$',l):
            out.append(NOTE+'#define FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY\n#include "engine/NThingMap_CThingMap.h"'); n_struct+=1; continue
        if re.match(r'^\s*extern CLayerInfo LayerInfo_0137dad8\[\];',l): n_ext+=1; continue
        if l.startswith("// The table's real element type is NThingMap::CLayerInfo"): continue
        out.append(l)
    t='\n'.join(out)
    k=t.count('LayerInfo_0137dad8[')
    t=t.replace('LayerInfo_0137dad8[','NThingMap::LayerInfo[')
    left=len(re.findall(r'LayerInfo_0137dad8|\bCLayerInfo\b(?!@)',re.sub(r'//[^\n]*','',t)))
    open(D+'/work/li/%s.cpp'%a,'w',encoding='utf-8',errors='surrogateescape').write(t)
    print(a,'struct',n_struct,'extern',n_ext,'uses',k,'leftover',left)

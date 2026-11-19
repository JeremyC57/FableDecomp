"""li_b.py <addr>...: sub-family B transform (rows declaring struct SMapwhoLevel + extern g_MapwhoLevels[]): the struct becomes the
engine/NThingMap_CThingMap.h FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY include, the extern goes, g_MapwhoLevels[i].Shift -> NThingMap::LayerInfo[i].ScaleShift,
.CellSize -> .Size. Writes work/li/<addr>.cpp from the repo row."""
import sys,re,os
D='$S/w22/agentNF100'
os.chdir('/home/user/FableDecomp')
src={}
for l in open(D+'/work/li_pp.tsv'):
    f=l.rstrip('\n').split('\t'); src[f[0]]=f[3]
os.makedirs(D+'/work/li',exist_ok=True)
NOTE=('// Name fix (NF100, round 9, q11463 / q11282): the level table is NThingMap::LayerInfo (Ego_d.pdb data symbol '
      '?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B, retail 0x0137DAD8;\n//   element NThingMap::CLayerInfo, Ego_r.pdb 16 bytes: Size +0, ScaleShift +4), declared by engine/NThingMap_CThingMap.h '
      '(opt-in FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY:\n//   the table alone, beside this row\'s local NThingMap model). It was the global stand-in ?g_MapwhoLevels@@3PAUSMapwhoLevel@@A.\n')
INC='#define FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY\n#include "engine/NThingMap_CThingMap.h"\n'
for a in sys.argv[1:]:
    s=open('rebuild/src/compiled/'+src[a],encoding='utf-8',errors='surrogateescape').read()
    def rs(m):
        pre,post=m.group(1),m.group(2)
        out=''
        if pre.strip(): out+=pre.rstrip()+'\n'
        out+=NOTE+INC
        post=re.sub(r'^\s*extern SMapwhoLevel g_MapwhoLevels\[\];','',post)
        if post.strip(): out+=post.lstrip()+'\n'
        return out.rstrip('\n')
    # struct (single- or multi-line), possibly sharing its line with other declarations
    s,n1=re.subn(r'(?m)^([^\n]*?)struct SMapwhoLevel\s*\{[^}]*\};([^\n]*)$',rs,s)
    # extern line: whole line when alone, else just the declaration
    s,n2=re.subn(r'(?m)^\s*extern SMapwhoLevel g_MapwhoLevels\[\];[^\n]*\n','',s)
    s,n3=re.subn(r'extern SMapwhoLevel g_MapwhoLevels\[\];','',s)
    k1=s.count('].Shift'); 
    s,u1=re.subn(r'\bg_MapwhoLevels\[([^\]]*)\]\.Shift\b',r'NThingMap::LayerInfo[\1].ScaleShift',s)
    s,u2=re.subn(r'\bg_MapwhoLevels\[([^\]]*)\]\.CellSize\b',r'NThingMap::LayerInfo[\1].Size',s)
    code=re.sub(r'//[^\n]*','',s)
    left=len(re.findall(r'g_MapwhoLevels|g_mapwhoLevels|SMapwhoLevel',code))
    open(D+'/work/li/%s.cpp'%a,'w',encoding='utf-8',errors='surrogateescape').write(s)
    print(a,'struct',n1,'extern',n2+n3,'uses',u1+u2,'leftover',left)

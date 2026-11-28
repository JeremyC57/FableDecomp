import re,os,sys,json,collections,glob
sys.path.insert(0,'tools/decomp_pipeline'); 
import importlib.util
spec=importlib.util.spec_from_file_location('V','tools/decomp_pipeline/verify_and_land.py'); V=importlib.util.module_from_spec(spec); spec.loader.exec_module(V)
S=sys.argv[1]
ps=open('rebuild/build_candidates.ps1',encoding='utf-8-sig').read()
ents={}
for b in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}",ps,re.S):
    e=dict(re.findall(r"(\w+) = '([^']*)'",b))
    if 'Address' in e: ents[e['Address'].lower()]=e
pdbcls=set()
for l in open('ghidra_out/struct_layouts_egor.tsv',encoding='utf-8',errors='replace'):
    if l.startswith('@STRUCT\t'): pdbcls.add(l.split('\t')[1].split('::')[-1])
roots=V.shared_header_roots()
DEF=re.compile(r"\b(?:class|struct)\s+([A-Za-z_]\w*)\s*(?::[^{;()]*)?\{")
PAD=re.compile(r"\b(?:char|unsigned char|BYTE|unsigned __int8|uint8_t)\s+_?(?:pad|unk|gap|_pad|filler|reserved)\w*\s*\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]",re.I)
STANDIN=re.compile(r"\b\w*(?:Unproven|_Retail\b|_[0-9a-f]{8}\b|CVtable_[0-9a-f]+)\w*",re.I)
out={}
for a,e in ents.items():
    p='rebuild/src/compiled/'+e.get('Source','')
    if not os.path.exists(p): continue
    src=open(p,encoding='utf-8',errors='replace').read()
    t=V._strip_comments_and_strings(src)
    redef=V.shared_header_redefinitions(src,roots)
    defs=[m.group(1) for m in DEF.finditer(t)]
    hdrless_pdb=[d for d in set(defs) if d in pdbcls and not any((r/f'engine/{d}.h').exists() or (r/f'libpk/{d}.h').exists() for r in roots)]
    pads=len(PAD.findall(t))
    standin=sorted(set(x for x in STANDIN.findall(t) if not re.fullmatch(r'(?:sub|loc|FUN|DAT)_[0-9a-f]{8}',x,re.I)))[:8]
    lm=re.search(r'^//\s*Leaf:\s*(\S+)',src,re.M)
    out[a]=dict(redef=[r[0] for r in redef],hdrless_pdb=hdrless_pdb,local_defs=len(set(defs)),pads=pads,standin_tokens=standin,
                leaf=lm.group(1) if lm else '',fidelity='// Fidelity:' in src,asm=bool(re.search(r'Retail-asm:',src)))
json.dump(out,open(S+'/inv/rows.json','w'))
c=collections.Counter()
for a,r in out.items():
    if r['redef']: c['redefines a class that has a shared header']+=1
    if r['hdrless_pdb']: c['defines a PDB class locally that has no shared header yet']+=1
    if r['pads']: c['local model with pad/unknown byte arrays']+=1
    if r['standin_tokens']: c['uses stand-in identifiers (address-suffixed / Unproven / CVtable_)']+=1
    if r['local_defs']: c['defines any local class/struct']+=1
    if not r['leaf']: c['no Leaf line']+=1
    elif not r['leaf'].startswith('?'): c['plain (C / extern) Leaf']+=1
    if r['fidelity']: c['Fidelity note']+=1
print(len(out)); [print(v,k) for k,v in c.most_common()]

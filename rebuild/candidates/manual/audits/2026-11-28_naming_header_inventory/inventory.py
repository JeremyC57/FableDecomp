import csv,json,os,re,sys,collections,importlib.util
csv.field_size_limit(sys.maxsize)
S=sys.argv[1]; OUT=sys.argv[2]; os.makedirs(OUT,exist_ok=True)
os.environ['FABLE_ROOT']=os.getcwd(); os.environ['FABLE_HDR_OVERLAY']=''
spec=importlib.util.spec_from_file_location('V','tools/decomp_pipeline/verify_and_land.py'); V=importlib.util.module_from_spec(spec); spec.loader.exec_module(V)
rows=json.load(open(S+'/inv/rows.json'))
ans={}
for r in csv.DictReader(open(S+'/codex31/NAME_ANSWERS.tsv'),delimiter='\t'): ans[r['address']]=r
t3=collections.defaultdict(list)
for r in csv.reader(open('rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv'),delimiter='\t'):
    if len(r)>2 and r[0]!='source':
        for x in set([r[1]]+re.findall(r'0x[0-9a-fA-F]{8}',r[2])): t3[x.lower().replace('0x','')].append(r[0])
desc={l.split('\t')[0].lower() for l in open('rebuild/oracles/descriptive_names.tsv') if l[:1].isalnum() and not l.startswith('address')}
backlog={r['address']:r['bucket'] for r in csv.DictReader(open('rebuild/candidates/manual/pending/2026-11-28_catalog_name_review/BACKLOG.tsv'),delimiter='\t')}
calls=collections.defaultdict(collections.Counter)
for r in csv.DictReader(open('rebuild/oracles/reloc_bindings.tsv'),delimiter='\t'):
    v=ans.get(r['target'],{}).get('verdict','NOT_IN_ANSWERS'); calls[r['address']][v]+=1
pdbcls=set()
for l in open('ghidra_out/struct_layouts_egor.tsv',encoding='utf-8',errors='replace'):
    if l.startswith('@STRUCT\t'): pdbcls.add(l.split('\t')[1].split('::')[-1])
def leafkind(lf):
    if not lf: return 'no Leaf line (grades the catalog name)'
    if not lf.startswith('?'): return 'plain C / extern Leaf'
    if lf.endswith('@') or not re.search(r'@@[A-Z]',lf): return 'prefix Leaf (matched by prefix, not checkable in full)'
    return 'full mangled Leaf'
def selfname(a,lf):
    x=ans.get(a)
    if not x: return 'no tracker record'
    v=x['verdict']
    if v=='PROVEN': return 'PROVEN'
    if v=='PROVEN_RTTI': return 'PROVEN_RTTI (respell to the RTTI slot name)' if lf!=x['use_this_spelling'] else 'PROVEN_RTTI'
    if v=='PROVEN_LANDED_WRONG': return 'PROVEN_LANDED_WRONG (self leaf wrong)' if lf and lf!=x['use_this_spelling'] else 'PROVEN_LANDED_WRONG (self right, callers wrong)'
    return v
F=['address','kind','group','self_name','in_T3_queue','catalog_backlog','leaf_kind','callees_unsettled','callees_landed_wrong','redefines_shared','pdb_classes_without_header','pad_arrays','standin_tokens','fidelity_note']
out=[]
def rec(a,kind,group,r):
    c=calls.get(a,{})
    out.append(dict(address=a,kind=kind,group=group,self_name=selfname(a,r['leaf']),in_T3_queue=','.join(sorted(set(t3.get(a,[]))))[:60],
        catalog_backlog=backlog.get(a,''),leaf_kind=leafkind(r['leaf']),callees_unsettled=c.get('UNSETTLED',0),callees_landed_wrong=c.get('PROVEN_LANDED_WRONG',0),
        redefines_shared=' '.join(r['redef'][:12]),pdb_classes_without_header=' '.join(r['hdrless_pdb'][:12]),pad_arrays=r['pads'],
        standin_tokens=' '.join(r['standin_tokens'][:6]),fidelity_note='y' if r['fidelity'] else ''))
for a,r in rows.items(): rec(a,'landed','',r)
# held rows that match
DEF=re.compile(r"\b(?:class|struct)\s+([A-Za-z_]\w*)\s*(?::[^{;()]*)?\{")
PAD=re.compile(r"\b(?:char|unsigned char|BYTE|unsigned __int8|uint8_t)\s+_?(?:pad|unk|gap|_pad|filler|reserved)\w*\s*\[\s*(0x[0-9a-fA-F]+|\d+)\s*\]",re.I)
ST=re.compile(r"\b\w*(?:Unproven|_Retail\b|_[0-9a-f]{8}\b|CVtable_[0-9a-f]+)\w*",re.I)
from pathlib import Path
for h in json.load(open(S+'/inv/held.json')):
    src=open(h['source'],errors='replace').read(); t=V._strip_comments_and_strings(src)
    roots=([Path(h['overlay'])] if h['overlay'] else [])+[Path('rebuild/include')]
    redef=[x[0] for x in V.shared_header_redefinitions(src,roots)]
    defs=set(m.group(1) for m in DEF.finditer(t))
    hdrless=[d for d in defs if d in pdbcls and not any((r/f'engine/{d}.h').exists() or (r/f'libpk/{d}.h').exists() for r in roots)]
    lm=re.search(r'^//\s*Leaf:\s*(\S+)',src,re.M)
    rr=dict(redef=redef,hdrless_pdb=hdrless,pads=len(PAD.findall(t)),standin_tokens=sorted(set(x for x in ST.findall(t) if not re.fullmatch(r'(?:sub|loc|FUN|DAT)_[0-9a-f]{8}',x,re.I)))[:8],
            leaf=lm.group(1) if lm else '',fidelity='// Fidelity:' in src)
    rec(h['address'],'held, matches',h['group'],rr)
w=csv.DictWriter(open(OUT+'/ROWS.tsv','w',newline=''),fieldnames=F,delimiter='\t',lineterminator='\n'); w.writeheader(); [w.writerow(x) for x in out]
# summaries
def cnt(kind,f): return collections.Counter(f(x) for x in out if x['kind']==kind)
summ={}
for kind in ('landed','held, matches'):
    X=[x for x in out if x['kind']==kind]
    summ[kind]=dict(rows=len(X),
      self_name=collections.Counter(x['self_name'] for x in X).most_common(),
      leaf_kind=collections.Counter(x['leaf_kind'] for x in X).most_common(),
      in_T3=sum(1 for x in X if x['in_T3_queue']), catalog_backlog=sum(1 for x in X if x['catalog_backlog']),
      rows_with_unsettled_callees=sum(1 for x in X if x['callees_unsettled']), unsettled_callee_bindings=sum(x['callees_unsettled'] for x in X),
      rows_with_landed_wrong_callees=sum(1 for x in X if x['callees_landed_wrong']), landed_wrong_callee_bindings=sum(x['callees_landed_wrong'] for x in X),
      redefines_shared=sum(1 for x in X if x['redefines_shared']), pdb_classes_without_header=sum(1 for x in X if x['pdb_classes_without_header']),
      pad_arrays=sum(1 for x in X if x['pad_arrays']), standin_tokens=sum(1 for x in X if x['standin_tokens']), fidelity=sum(1 for x in X if x['fidelity_note']),
      no_open_item=sum(1 for x in X if x['self_name'] in ('PROVEN','PROVEN_RTTI','PROVEN_LANDED_WRONG (self right, callers wrong)') and not x['in_T3_queue'] and not x['callees_unsettled'] and not x['callees_landed_wrong'] and not x['redefines_shared'] and not x['pdb_classes_without_header'] and not x['standin_tokens'] and x['leaf_kind']=='full mangled Leaf'))
json.dump(summ,open(OUT+'/summary.json','w'),indent=1)
# class tables
rc=collections.Counter(); hc=collections.Counter()
for x in out:
    for c in x['redefines_shared'].split(): rc[c]+=1
    for c in x['pdb_classes_without_header'].split(): hc[c]+=1
open(OUT+'/CLASSES_REDEFINED.tsv','w').write('class\trows_defining_it_locally (shared header exists)\n'+''.join(f'{c}\t{n}\n' for c,n in rc.most_common()))
open(OUT+'/CLASSES_WITHOUT_HEADER.tsv','w').write('class\trows_defining_it_locally (PDB layout exists, no shared header)\n'+''.join(f'{c}\t{n}\n' for c,n in hc.most_common()))
print(json.dumps(summ,indent=1)[:6000])

import sys,os,csv,re,struct
sys.path.insert(0,'tools/decomp_pipeline/linux_lane/lane_tools/bounds')
from extent import Exe
E=Exe()
conf=sys.argv[1]; out=sys.argv[2]
raw=open(E.path if hasattr(E,'path') else os.environ['S']+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
cat={l.split('\t')[0]:l.split('\t')[1] for l in open('rebuild/oracles/auto-re-candidates.tsv')}
man={r['address']:(r['module']+'::'+r['name']) for r in csv.DictReader(open('rebuild/manifest/functions.tsv'),delimiter='\t')}
landed=set(re.findall(r"Address = '([0-9a-f]{8})'",open('rebuild/build_candidates.ps1',encoding='utf-8').read()))
pdb=set()
for f in ('ghidra_out/fablewin_pdb_names.tsv','ghidra_out/egor_pdb_names.tsv'):
    for l in open(f,errors='replace'):
        c=l.rstrip('\n').split('\t')
        if len(c)>1: pdb.add(c[1])
pdbtxt='\n'.join(pdb)
def refs(fva):
    return raw.count(struct.pack('<I',fva))
LAY={'def':{0:'sdd',18:'Transfer',19:'Copy',20:'GetSizeofClass'},
     'level':{0:'sdd',1:'RegisterMain',2:'Main',3:'Init'},
     'entity':{0:'sdd',1:'Main',2:'Init',3:'GetParentScript',4:'OnPersist',5:'OnPredicateFail'}}
PH=re.compile(r"sub_|CVtable_|CUnprovenDef|vfunc_|^\?|_gapscan|None")
rows=[];rep=[]
for r in csv.DictReader(open('rebuild/candidates/manual/naming/2026-11-04_anniversary/candidate_names.tsv'),delimiter='\t'):
    if r['confidence']!=conf: continue
    m=re.search(r'vtable 0x([0-9A-F]+)',r['source_evidence']); va=int(m.group(1),16)
    full=r['name']; parts=full.split('::'); chain='@'.join(reversed(parts))  # inner@outer@ns
    has=lambda meth: ('?%s@%s@@'%(meth,chain)) in pdbtxt
    sdd_ok=('??_G%s@@'%chain) in pdbtxt or ('??1%s@@'%chain) in pdbtxt
    kind='def' if has('Transfer') else 'level' if has('RegisterMain') else 'entity' if has('GetParentScript') or has('OnPredicateFail') else None
    classin=('@%s@@'%chain) in pdbtxt
    rep.append(f'== {full} vt={va:08x} kind={kind} pdb_class={classin} pdb_sdd={sdd_ok}')
    if not kind or not classin: continue
    for slot,meth in LAY[kind].items():
        fva=E.dword(va+4*slot); f='%08x'%fva; cur=cat.get(f) or man.get(f) or 'None'
        n=refs(fva)
        pdbok = sdd_ok if meth=='sdd' else has(meth)
        name=full+'::'+("`scalar deleting destructor'" if meth=='sdd' else meth)
        note=''
        if meth=='GetSizeofClass':
            b=E.read(fva,6); note='body=mov eax,imm;ret' if b[0]==0xb8 and b[5]==0xc3 else 'BODY_MISMATCH'
        ok = pdbok and n==1 and note!='BODY_MISMATCH'
        act = 'RENAME' if ok and (PH.search(cur) or cur.split('::')[-1]==name.split('::')[-1] and cur!=name) else ('SAME' if cur==name else ('SKIP' if not ok else 'KEEP'))
        rep.append(f'  {slot:2d} {f} {meth:16s} refs={n} pdb={pdbok} landed={f in landed} cur={cur} -> {act} {note}')
        if act=='RENAME':
            rows.append((f,name,f'Anniversary wave 2026-11-04 ({conf}): vtable 0x{va:08X} slot {slot}; Anniversary RTTI {full} (string-literal match, validated 257/258); PDB name set has the {meth} member of {full}; slot layout from named TLC siblings; function referenced by this vtable only{"; "+note if note else ""}', f in landed))
open(out,'w').write('address\tname\tevidence\n'+''.join(f'{a}\t{n}\t{e}\n' for a,n,e,l in rows))
print('\n'.join(rep)); print('renames',len(rows),'landed',sum(1 for x in rows if x[3]), [x[0] for x in rows if x[3]])

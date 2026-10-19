from pathlib import Path
import runpy,sys,csv,struct,re,json,pefile
D=Path(__file__).resolve().parent;h=runpy.run_path(str(D/'coff_helpers.py'));phase='root'
p=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');u=lambda a:struct.unpack('<I',p.get_data(a-p.OPTIONAL_HEADER.ImageBase,4))[0]
native={int(x['address'],16):x['name']for x in csv.DictReader(Path('rebuild/manifest/functions.tsv').open(),delimiter='\t')};allnames=[x.split('\t',1)[1]for x in Path('ghidra_out/fablewin_pdb_names.tsv').read_text().splitlines()if '\t'in x];out=[];errors=[]
def norm(s):
 s=re.sub(r'(public: |protected: |private: |virtual |class |struct |enum )','',s)
 s=s.replace('EAIStateGroupType','long').replace('EScriptAIPriority','long')
 return re.sub(r'\s+','',s)
for a in ['00da6240','00da6740']:
 obj=next((D/'dry_work').rglob(a+'.t.obj'));ss,secs=h['coff'](obj)
 for name,val,sec in ss.values():
  if name not in ['??_7CScriptThing@@6B@','??_7CGameScriptInterface@@6B@']:continue
  owner='CScriptThing'if 'CScriptThing' in name else 'CGameScriptInterface';vt=0x1238c8c if owner=='CScriptThing'else 0x1260f0c;count=79 if owner=='CScriptThing'else 109;size,rels=secs[sec-1];actual=[rels[k]for k in range(val,size,4)if k in rels];assert len(actual)==count,(name,len(actual))
  for i,s in enumerate(actual):
   target=u(vt+4*i);n=native[target];expected=n if n.startswith('?') else None
   if owner=='CGameScriptInterface' and i==100:expected='?CreateEffect@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABVC3DVector@@0M_N2@Z'
   if owner=='CGameScriptInterface' and i==101:expected='?CreateEffect@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABV2@00_N2@Z'
   if i==0:expected='destructor'
   elif not expected:
    meth=n.split('::')[-1];opts=[q for q in allnames if q.startswith('?'+meth+'@'+owner+'@@')];expected=opts[0]if len(set(opts))==1 else None
   dem=h['und'](s);ed=h['und'](expected)if expected else 'UNRESOLVED '+n
   if i and norm(dem)!=norm(ed):errors.append([a,i,hex(target),dem,ed])
   out.append({'address':a,'slot':i,'table':name,'target':hex(target),'compiled':dem,'expected':ed})
assert len(out)==376,len(out)
(D/('script_virtual_signatures_'+phase+'.json')).write_text(json.dumps({'slots':out,'mismatches':errors},indent=2));print('audited',len(out),'slots;mismatches',len(errors));print(json.dumps(errors,indent=2))

assert not errors,errors

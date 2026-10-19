from pathlib import Path
import runpy,sys,csv,struct,re,json
r=Path('work/codex-lane/w22');o=r/'coordinator_retry177';h=runpy.run_path(str(o/'coff_helpers.py'));sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
native={int(x['address'],16):x['name']for x in csv.DictReader(Path('rebuild/manifest/functions.tsv').open(),delimiter='\t')};allnames=[x.split('\t',1)[1]for x in Path('ghidra_out/fablewin_pdb_names.tsv').read_text().splitlines()if '\t'in x];out=[];errors=[]
def norm(s):
 s=re.sub(r'(public: |protected: |private: |virtual |class |struct |enum )','',s)
 s=s.replace('EAIStateGroupType','long').replace('EScriptAIPriority','long')
 return re.sub(r'\s+','',s)
for a in ['00e33b00','00e7b970']:
 ss,secs=h['coff'](next((o/'dry_work').rglob(a+'.t.obj')))
 for name,val,sec in ss.values():
  if name not in ['??_7CScriptThing@@6B@','??_7CGameScriptInterface@@6B@']:continue
  owner='CScriptThing'if 'CScriptThing' in name else 'CGameScriptInterface';vt=0x1238c8c if owner=='CScriptThing'else 0x1260f0c;count=79 if owner=='CScriptThing'else 100;size,rels=secs[sec-1];actual=[rels[k]for k in range(val,size,4)if k in rels];assert len(actual)==count,(name,len(actual))
  for i,s in enumerate(actual):
   target=u(vt+4*i);n=native[target];expected=n if n.startswith('?') else None
   if i==0:expected='destructor'
   elif not expected:
    meth=n.split('::')[-1];opts=[q for q in allnames if q.startswith('?'+meth+'@'+owner+'@@')];expected=opts[0]if len(set(opts))==1 else None
   dem=h['und'](s);ed=h['und'](expected)if expected else 'UNRESOLVED '+n
   if i and norm(dem)!=norm(ed):errors.append([a,i,hex(target),dem,ed])
   out.append({'address':a,'slot':i,'table':name,'target':hex(target),'compiled':dem,'expected':ed})
(o/'script_virtual_signatures.json').write_text(json.dumps({'slots':out,'mismatches':errors},indent=2));print(json.dumps(errors,indent=2))

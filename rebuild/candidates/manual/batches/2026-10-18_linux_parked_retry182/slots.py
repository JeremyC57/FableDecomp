from pathlib import Path
import runpy,sys,csv,struct,re,json
D=Path(__file__).resolve().parent;h=runpy.run_path(str(D/'coff_helpers.py'));sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
native={int(x['address'],16):x['name']for x in csv.DictReader(Path('rebuild/manifest/functions.tsv').open(),delimiter='\t')};pdb=[x.split('\t',1)[1]for x in Path('ghidra_out/fablewin_pdb_names.tsv').read_text().splitlines()if '\t'in x]
replacements={'Native::FreeableResource':'CEngineLandscapeFreeableResource','Native::SubNodeBase':'CLandscapeBackgroundSubNodeBase','Native::Movable':'CMovableResource','MovableNative':'CMovableResource','Native::Patch':'CLandscapeBackgroundPatch','Native::Water':'CEngineWaterBackgroundSubPatch','Native::Tree':'CLandscapeBackgroundTreeNode','QuadTreeElement':'CLocalDetailCacheMap::CQuadTreeElement','Native::LANDSCAPE_EDGE_STRIP_SIDE':'LANDSCAPE_EDGE_STRIP_SIDE'}
mapvt={'Movable':(0x129b860,4),'MovableNative':(0x129b860,4),'Patch':(0x12a803c,9),'Water':(0x12a8334,4),'SubNodeBase':(0x12a8014,9),'FreeableResource':(0x12a8228,5),'Tree':(0x12a8240,5),'QuadTreeElement':(0x12a7df0,4)}
def norm(s):
 for a,b in replacements.items():s=re.sub(r'(?<![A-Za-z])'+re.escape(a),b,s)
 s=re.sub(r'(public: |protected: |private: |virtual |class |struct |enum )','',s)
 return re.sub(r'\s+','',s)
out=[];errors=[]
for a in ['00bdfa10','00be6090','00bf67f0']:
 sy,secs=h['coff'](next((D/'dry_work').rglob(a+'.t.obj')))
 for name,val,sec in sy.values():
  if not name.startswith('??_7'):continue
  owner=name[4:].split('@')[0]
  if owner=='TestChild':continue
  assert owner in mapvt,name
  vt,count=mapvt[owner];size,rels=secs[sec-1];actual=[rels[k]for k in range(val,size,4)if k in rels];assert len(actual)==count,(name,len(actual))
  for i,s in enumerate(actual):
   target=u(vt+4*i);n=native.get(target,'');expected=n if n.startswith('?')else None
   if target==0xbfea08:assert s=='__purecall';expected='__purecall'
   elif i==0:assert s.startswith(('??_E','??_G'));expected='deleting destructor'
   elif owner=='QuadTreeElement'and i in [1,2]:
    meth='RelocateData'if i==1 else'CanRelocateData';opts=set(q for q in pdb if q.startswith('?'+meth+'@CQuadTreeElement@CLocalDetailCacheMap@@'));assert len(opts)==1;expected=opts.pop()
   elif not expected:
    parts=n.split('::');assert len(parts)==2,(name,i,n);cl,meth=parts;opts=set(q for q in pdb if q.startswith('?'+meth+'@'+cl+'@@') and q.split('@@',1)[1][0] in 'UEM');assert len(opts)==1,(name,i,n,opts);expected=opts.pop()
   dem=h['und'](s);ed=h['und'](expected)
   if i and norm(dem)!=norm(ed):errors.append([a,i,hex(target),dem,ed])
   out.append({'address':a,'table':name,'slot':i,'target':hex(target),'compiled':dem,'expected':ed})
assert len(out)==57,len(out)
(D/'native_virtual_signatures.json').write_text(json.dumps({'slots':out,'mismatches':errors,'excluded':'TestChild is a custom fixture subclass; inherited slot6 is identical audited SubNodeBase declaration.'},indent=2));print('audited',len(out),'native/PDB slots;mismatches',len(errors));print(json.dumps(errors,indent=2));assert not errors

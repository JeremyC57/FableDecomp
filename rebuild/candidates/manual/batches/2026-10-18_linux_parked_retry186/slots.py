from pathlib import Path
import runpy,sys,csv,struct,re,json
D=Path(__file__).resolve().parent;h=runpy.run_path(str(D/'coff_helpers.py'));sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
native={int(x['address'],16):x['name']for x in csv.DictReader(Path('rebuild/manifest/functions.tsv').open(),delimiter='\t')};pdb=[x.split('\t',1)[1]for x in (Path('ghidra_out/fablewin_pdb_names.tsv').read_text()+Path('ghidra_out/egor_pdb_names.tsv').read_text()).splitlines()if '\t'in x]
mapvt={'CSpecialAbilitiesBerserkDef':(0x12341fc,25),'CSpecialAbilitiesLightningSpellDef':(0x123460c,25),'CEnginePrimitiveManagerWeaponTrails':(0x12a6aec,13),'CEnginePrimitiveManager':(0x12a53e8,13),'CShaderBase':(0x129a030,2),'CVertexShader':(0x129b3d0,3),'CVertexShaderData':(0x129da50,1),'CShaderResource':(0x129d580,1),'CEnginePrimitiveRenderUtil':(0x12a3bd0,1),'CBaseClassNonCopyable':(0x129a7c4,1),'CBaseClass':(0x1231710,1)}
def norm(s):return re.sub(r'\s+','',re.sub(r'(public: |protected: |private: |virtual |class |struct |enum )','',s))
out=[];errors=[];excluded=[]
for a in ['004573fd','00457ca0','00bcc5f0']:
 sy,secs=h['coff']((D/(a+'_emit_abi.obj') if a!='00bcc5f0' else next((D/'dry_work').rglob(a+'.t.obj'))))
 for name,val,sec in sy.values():
  if not name.startswith('??_7') or sec<=0:continue
  owner=name[4:].split('@')[0]
  if a!='00bcc5f0' and not owner.startswith('CSpecialAbilities'):continue
  if owner not in mapvt:
   excluded.append([a,name,'Destruction-only external interface view or fixture subclass; not a complete native object claim.']);continue
  vt,count=mapvt[owner]
  if '6BCDeviceResetCallback' in name:vt,count=(0x122f174 if owner=='CMainGameComponent' else 0x122eca4),2
  size,rels=secs[sec-1];actual=[rels[k]for k in range(val,size,4)if k in rels];assert len(actual)==count,(name,len(actual))
  for i,s in enumerate(actual):
   target=u(vt+4*i);n=native.get(target,'');n={0x987cf0:owner+'::GetDebugName',0x9be210:'CVertexShader::CountReferences'}.get(target,n);expected=n if n.startswith('?')else None
   if target==0xbfea08:assert s=='__purecall';expected='__purecall'
   elif s.startswith(('??_E','??_G')):assert i==0;expected='deleting destructor'
   elif not expected:
    parts=n.split('::');assert len(parts)==2,(name,i,hex(target),n);cl,meth=parts;opts=set(q for q in pdb if q.startswith('?'+meth+'@'+cl+'@@') and q.split('@@',1)[1][0] in 'UEM')
    if target in [0x9ad4a0,0x9ad510]:opts={q for q in opts if ('@@UAE' in q)==(target==0x9ad4a0)}
    assert len(opts)==1,(name,i,n,opts);expected=opts.pop()
   dem=h['und'](s);ed=h['und'](expected)
   if expected!='deleting destructor' and norm(dem)!=norm(ed):errors.append([a,i,hex(target),dem,ed])
   out.append({'address':a,'table':name,'slot':i,'target':hex(target),'compiled':dem,'expected':ed})
(D/'native_virtual_signatures.json').write_text(json.dumps({'slots':out,'mismatches':errors,'excluded':excluded},indent=2));print('audited',len(out),'native/PDB slots;mismatches',len(errors));print(json.dumps(errors,indent=2));assert not errors

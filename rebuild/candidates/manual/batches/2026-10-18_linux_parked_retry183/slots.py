from pathlib import Path
import runpy,sys,csv,struct,re,json
D=Path(__file__).resolve().parent;h=runpy.run_path(str(D/'coff_helpers.py'));sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
native={int(x['address'],16):x['name']for x in csv.DictReader(Path('rebuild/manifest/functions.tsv').open(),delimiter='\t')};pdb=[x.split('\t',1)[1]for x in Path('ghidra_out/fablewin_pdb_names.tsv').read_text().splitlines()if '\t'in x]
mapvt={'CMainGameComponent':(0x122f180,10),'CGameComponent':(0x122ecb0,5),'CDiskFileWin32':(0x122d06c,15),'CNetworkClient':(0x122edac,2),'CDisplayEngine':(0x1231574,3),'CFrameRateSmoother':(0x122f168,2),'CGame':(0x122e59c,2),'CBaseClass':(0x1231710,1),'CBaseClassNonCopyable':(0x129a7c4,1),'CInitBaseClass':(0x129a7cc,2),'CAFile':(0x122eccc,15),'CIVCountedPointeeBase':(0x12354d0,2)}
def norm(s):return re.sub(r'\s+','',re.sub(r'(public: |protected: |private: |virtual |class |struct |enum )','',s))
out=[];errors=[];excluded=[]
for a in ['00417418','00418b7a','0041d8e7','0041d98a']:
 sy,secs=h['coff'](next((D/'dry_work').rglob(a+'.t.obj')))
 for name,val,sec in sy.values():
  if not name.startswith('??_7') or sec<=0:continue
  owner=name[4:].split('@')[0]
  if owner not in mapvt:
   excluded.append([a,name,'Destruction-only external interface view or fixture subclass; not a complete native object claim.']);continue
  vt,count=mapvt[owner]
  if '6BCDeviceResetCallback' in name:vt,count=(0x122f174 if owner=='CMainGameComponent' else 0x122eca4),2
  size,rels=secs[sec-1];actual=[rels[k]for k in range(val,size,4)if k in rels];assert len(actual)==count,(name,len(actual))
  for i,s in enumerate(actual):
   target=u(vt+4*i);n=native.get(target,'');expected=n if n.startswith('?')else None
   if target==0xbfea08:assert s=='__purecall';expected='__purecall'
   elif s.startswith(('??_E','??_G')):assert i==0;expected='deleting destructor'
   elif not expected:
    parts=n.split('::');assert len(parts)==2,(name,i,hex(target),n);cl,meth=parts;opts=set(q for q in pdb if q.startswith('?'+meth+'@'+cl+'@@') and q.split('@@',1)[1][0] in 'UEM');assert len(opts)==1,(name,i,n,opts);expected=opts.pop()
   dem=h['und'](s);ed=h['und'](expected)
   if expected!='deleting destructor' and norm(dem)!=norm(ed):errors.append([a,i,hex(target),dem,ed])
   out.append({'address':a,'table':name,'slot':i,'target':hex(target),'compiled':dem,'expected':ed})
(D/'native_virtual_signatures.json').write_text(json.dumps({'slots':out,'mismatches':errors,'excluded':excluded},indent=2));print('audited',len(out),'native/PDB slots;mismatches',len(errors));print(json.dumps(errors,indent=2));assert not errors

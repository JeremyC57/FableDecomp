from pathlib import Path
import runpy,sys,csv,struct,re,json
D=Path(__file__).resolve().parent;h=runpy.run_path(str(D/'coff_helpers.py'));sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
native={int(x['address'],16):x['name']for x in csv.DictReader(Path('rebuild/manifest/functions.tsv').open(),delimiter='\t')};pdb=[x.split('\t',1)[1]for x in (Path('ghidra_out/fablewin_pdb_names.tsv').read_text()+Path('ghidra_out/egor_pdb_names.tsv').read_text()).splitlines()if '\t'in x]
mapvt={'BaseClassNative':(0x1231710,1),'MaintainedEvent':(0x129c0e0,1),'BaseClass':(0x1231710,1),'BaseNonCopyable':(0x129a7c4,1),'CHandleBasedMemoryPool':(0x129c964,4),'DataResource':(0x129d3a4,10),'IVNative':(0x12354d0,2),'ResourceNative':(0x12354a4,10)}
replacements={'BaseClassNative':'CBaseClass','MaintainedEvent':'CMaintainedEvent','BaseClass':'CBaseClass','BaseNonCopyable':'CBaseClassNonCopyable','LipSyncEntry::DataResource':'CLipSyncDataBankEntry::CData','IVNative':'CIVCountedPointeeBase','ResourceNative':'CResource'}
def norm(s):
 for a,b in replacements.items():s=re.sub(r'(?<![A-Za-z])'+re.escape(a),b,s)
 return re.sub(r'\s+','',re.sub(r'(public: |protected: |private: |virtual |class |struct |enum )','',s))
out=[];errors=[];excluded=[]
for a in ['009e4470','00a02c00','00a42980']:
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
   target=u(vt+4*i);n=native.get(target,'');n={0xa42a80:'CLipSyncDataBankEntry::CData::UnloadResource',0xa42810:'CLipSyncDataBankEntry::CData::IsResourceLoaded',0xa42fb0:'CLipSyncDataBankEntry::CData::OnResourceLoaded'}.get(target,n);expected=n if n.startswith('?')else None
   if target==0xbfea08:assert s=='__purecall';expected='__purecall'
   elif s.startswith(('??_E','??_G')):assert i==0;expected='deleting destructor'
   elif not expected:
    parts=n.split('::');assert len(parts)>=2,(name,i,hex(target),n);cl='@'.join(reversed(parts[:-1]));meth=parts[-1];opts=set(q for q in pdb if q.startswith('?'+meth+'@'+cl+'@@') and q.split('@@',1)[1][0] in 'UEM');assert len(opts)==1,(name,i,n,opts);expected=opts.pop()
   dem=h['und'](s);ed=h['und'](expected)
   if expected!='deleting destructor' and norm(dem)!=norm(ed):errors.append([a,i,hex(target),dem,ed])
   out.append({'address':a,'table':name,'slot':i,'target':hex(target),'compiled':dem,'expected':ed})
(D/'native_virtual_signatures.json').write_text(json.dumps({'slots':out,'mismatches':errors,'excluded':excluded},indent=2));print('audited',len(out),'native/PDB slots;mismatches',len(errors));print(json.dumps(errors,indent=2));assert not errors

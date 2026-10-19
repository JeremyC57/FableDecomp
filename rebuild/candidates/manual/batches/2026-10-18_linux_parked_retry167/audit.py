from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry167';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?InitializeChangingStateComponent':0x52cc50,'?EditBoxVTable':0x1246b8c,'?EditBoxObserverVTable':0x1246b64,'?EditBoxSecondaryVTable':0x1246b5c,'?AllocateActionListNode':0xbfea0e,'??0CWideString':0x99aed0,'?GetUIManager':0x41e5f2,'?ObserveEvent':0x52da20,'?LoadAndReturnDefResultStorage':0x530ec0,'__ftol2':0xbfea70,'?AreaChanged':0x8f5fa0,'?LowerBound@EventMap':0x6faf80,'__real@38d1b717':0x129ba3c}
for a,n in [('005407b0',13),('0081e810',5),('006f9430',2)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert u(0x1230134+8)==0x55ca40;assert rd(0x5408dc,3)==bytes.fromhex('ff5204');assert rd(0x530f1b,7)==bytes.fromhex('8bc65e59c20400');assert rd(0x6fafcc,6)==bytes.fromhex('8bc65ec20400');assert rd(0x6fafb1,7)==bytes.fromhex('3bdf8d1496730c');assert u(0x129ba3c)==0x38d1b717
out={}
for vt,offset in [(0x1246b8c,0),(0x1246b64,4),(0x1246b5c,24),(0x1245de4,0),(0x1230134,0)]:
 col=u(vt-4);assert u(col+4)==offset;ch=u(col+16);h=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);h.append([rd(u(bc)+8,160).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 out[hex(vt)]={'complete_object_offset':offset,'bases':h}
assert out['0x1246b8c']['bases'][1]==['.?AVCChangingStateComponent@NUISystem@@',0];assert out['0x1246b8c']['bases'][-2:]==[['.?AVCObserver@NUISystem@@',4],['.?AVCActionParam@NUISystem@@',24]];assert out['0x1230134']['bases'][1]==['.?AVCObservable@NUISystem@@',0]
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2));(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n01230134\t2\t0055ca40\n')
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');b=rd(0xbfea0e,6);assert b[:2]==b'\xff\x25';iat=struct.unpack('<I',b[2:])[0];assert [(e.dll,i.name)for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports if i.address==iat]==[(b'MSVCR71.dll',b'malloc')]
pdb=Path('ghidra_out/fablewin_pdb_names.tsv').read_text();assert '?GetRefCount@CIVCountedPointeeBase@@QBEJXZ'in pdb;assert '?Delete@CIVCountedPointeeBase@@UBEXXZ'in pdb;assert '023f9630\t??0CEvent@NAnimationEvents@@QAE@XZ'in pdb;assert '023fa5c0\t??0CManager@NAnimationEvents@@QAE@ABVCMeshDataBank@@ABVCWideString@@_N@Z'in pdb
records=[]
for d,a,n in [('astra235_whole','005407b0',2),('astra240_whole','0081e810',3),('astra240_whole','006f9430',5)]:
 rows=[json.loads(l)for l in(r/d/'ATTEMPTS.jsonl').read_text().splitlines()if json.loads(l)['address']==a];assert len(rows)==n
 for x in rows:
  p=Path(x['snapshot'])/'source.cpp'if 'snapshot'in x else Path(x['command'][5]);assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['command']and(x.get('base_flags')or x.get('effective_flags'));records.append(x)
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS20relocations/5RTTItables/observer+constDelete/hiddenresult/malloc/constiterator+epsilon/10immutablegrades')

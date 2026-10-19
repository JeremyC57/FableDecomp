from pathlib import Path
import struct,runpy,sys,json,hashlib,re,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry193';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];H=runpy.run_path(str(o/'coff_helpers.py'))
M={'?resize@FrameArray':0xa016f0,'?Add@GraphicArray':0xa01650,'??0MemoryInput':0x994930,'?Load@GraphicHeader':0x9fd160,'??0GraphicInfo':0x9fd700,'?NotifyEntryAdded@CBankFileNative':0x9cbc20,'??1InputStream':0x993b10,'??_7CGraphicDataBank@@6BCBankFileAsync':0x129c8cc,'??_7CGraphicDataBank@@6BCResourceBank':0x129c938,'?erase@GraphicArray':0xa00e60,'?erase@FrameArray':0xa00ed0,'?Close@CBankFileNative':0x9ce360,'??1NativeTexture':0x9f9f70,'??3@':0xbfe9bc,'??_7BankState':0x129c82c,'_free':0xbfea14,'??1CResourceBank':0x9fc690,'??1CBankFileAsync':0x9d6490}
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  assert[val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==19
pdb=[l.split('\t',1)[1]for l in(Path('ghidra_out/egor_pdb_names.tsv').read_text()+Path('ghidra_out/fablewin_pdb_names.tsv').read_text()).splitlines()if'\t'in l]
alias={'CBankFileNative':'CBankFile','BankArrayView':'CArray','BankTypeCount':'CBankFileDataTypeCount','StatePointer':'CCountedPointer<CBankStateBlock>','CharArray':'CArray<char>','OutputStream':'CDataOutputStream','BankState':'CBankStateBlock','GraphicBankState':'CGraphicBankStateBlock','GraphicInfoFrame':'CGraphicInfoFrame','DataStream':'CDataStream','InputStream':'CDataInputStream','MemoryInput':'CMemoryDataInputStream'}
def normalize(s):
 s=re.sub(r'public: |protected: |private: |virtual |class |struct |enum ','',s)
 for a,b in sorted(alias.items(),key=lambda x:-len(x[0])):s=re.sub(r'\b'+a+r'\b',b,s)
 return re.sub(r'\s+','',s)
tables={'??_7CBaseClass@@6B@':(0x1231710,1),'??_7CBaseClassNonCopyable@@6B@':(0x129a7c4,1),'??_7CBankFileNative@@6B@':(0x129b54c,20),'??_7CBankFileAsync@@6B@':(0x129b8d4,20),'??_7CResourceBank@@6B@':(0x129c814,1),'??_7CGraphicDataBank@@6BCResourceBank@@@':(0x129c938,1),'??_7CGraphicDataBank@@6BCBankFileAsync@@@':(0x129c8cc,26),'??_7GraphicInfoFrame@@6B@':(0x129c878,1),'??_7DataStream@@6B@':(0x129a69c,1),'??_7InputStream@@6B@':(0x129a6b0,11),'??_7MemoryInput@@6B@':(0x123675c,11),'??_7GraphicBankState@@6B@':(0x129c888,4),'??_7BankState@@6B@':(0x129c82c,4)}
slots=[];seen=set();known_targets={}
for a in obs:
 sy,se=H['coff'](next((o/'dry_work').rglob(a+'.t.obj')))
 for table,value,sec in sy.values():
  if not(table.startswith('??_7')and sec>0):continue
  assert table in tables,table
  vt,count=tables[table];size,rel=se[sec-1];symbols=[rel[k]for k in range(value,size,4)if k in rel];assert len(symbols)==count,(a,table,len(symbols),count);seen.add(vt)
  for i,sym in enumerate(symbols):
   target=u(vt+4*i);comp=H['und'](sym)
   if i==0:
    assert sym.startswith(('??_G','??_E'));expected='genuine deleting destructor; native RTTI table identity'
    if vt==0x129c938:assert 'adjustor{356}'in comp
   elif target==0xbfea08:expected='HOLD: native purecall declaration mismatch' if sym!='__purecall' else 'native purecall'
   else:
    method,cl=sym[1:].split('@')[:2];original=alias.get(cl,cl);cand=[p for p in pdb if p.startswith('?'+method+'@'+original+'@@')];hits=[H['und'](p)for p in cand if normalize(H['und'](p))==normalize(comp)];assert hits,(table,i,comp,[H['und'](p)for p in cand]);expected=hits[0]
   slots.append(dict(address=a,table=table,slot=i,target=hex(target),compiled=comp,expected=expected))
# Shared/inherited slots agree with corresponding native primary bank table.
bank=[u(0x129b54c+4*i)for i in range(20)];asyn=[u(0x129b8d4+4*i)for i in range(20)];graphic=[u(0x129c8cc+4*i)for i in range(26)]
assert asyn[6:]==bank[6:]
for i in [1,2,3,4]:assert graphic[i]==asyn[i]
for i in [6,7,9,15,17,18,19]:assert graphic[i]==bank[i]
assert graphic[12]==0x9ff590 and graphic[5]==0x9fece0 and bank[12]==0x9cbc20 and bank[5]==0x9ce360
assert [u(0x129c888+i*4)for i in range(4)]==[0xa00920,0x9fcef0,0xa00940,0x9fd020]
assert u(0x129c878)==0xa009a0
hier={}
for vt in seen:
 col=u(vt-4);ch=u(col+16);items=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+i*4);items.append([rd(u(bc)+8,180).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 hier[hex(vt)]={'offset':u(col+4),'hierarchy':items}
assert hier['0x129c938']['offset']==0x164 and hier['0x129c8cc']['offset']==0
assert hier['0x129c8cc']['hierarchy']==[['.?AVCGraphicDataBank@@',0],['.?AVCBankFileAsync@@',0],['.?AVCBankFile@@',0],['.?AVCBaseClassNonCopyable@@',0],['.?AVCBaseClass@@',0],['.?AVCResourceBank@@',356],['.?AVCBaseClassNonCopyable@@',356],['.?AVCBaseClass@@',356]]
assert hier['0x129c878']['hierarchy']==[['.?AVCGraphicInfoFrame@@',0],['.?AVCBaseClass@@',0]]
assert [x[0]for x in hier['0x123675c']['hierarchy']]==['.?AVCMemoryDataInputStream@@','.?AVCDataInputStream@@','.?AVCDataStream@@','.?AVCBaseClassNonCopyable@@','.?AVCBaseClass@@']
(o/'native_virtual_signatures.json').write_text(json.dumps(slots,indent=2));(o/'native_hierarchy_audit.json').write_text(json.dumps(hier,indent=2))
records=[]
for a,total in [('009ff440',4),('009ff590',5),('009febc0',2)]:
 for i in range(1,total+1):
  p=r/'astra239_whole/snapshots'/f'{a}_{i:02d}';q=json.loads((p/'grade.json').read_text());assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==q['sha256'];assert q['command']and q['effective_flags']==['/O2','/Oy','/W3'];assert (p/'grade.log').exists();q.update(address=a,source=str(p/'source.cpp'),log=str(p/'grade.log'));records.append(q)
q=json.loads((o/'root_amendments/grade.json').read_text());assert hashlib.sha256(Path(q['source']).read_bytes()).hexdigest()==q['sha256'];assert 'RELOCATION_MATCH'in(o/'root_amendments/grade.log').read_text();records.append(q);assert len(records)==12;(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
assert(o/'dry.log').read_text().count('RELOCATION_MATCH PASS')==3
print('DIAGNOSTIC ONLY: unresolved purecall HOLD; remaining19relocations/'+str(len(slots))+'fullVTsignatures/'+str(len(hier))+'RTTIhierarchies/12immutablegrades; source/fixture review required')

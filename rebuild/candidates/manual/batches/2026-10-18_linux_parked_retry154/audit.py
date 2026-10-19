from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,csv,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry154';w=r/'luna234_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'?GetPItem':0xa01b50,'?LowerBound':0x40f020,'??0CBaseIntelligentPointer':0xa01b10,'??_7?$CIntelligentPointer':0x1238c6c,'?SetPItem':0xa01b90,'?PushBack':0x4ad520,'??1?$CIntelligentPointer':0xa01c10,'?Erase':0x4ac0f0,'?FinishAction':0x859a20,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'??0CStringFileArgument':0x99ec30,'??BCCharString':0x99e4c0,'__TI2?AVCBBBFileException@@':0x1369380,'__CxxThrowException@8':0xbfeb84,'?WriteFileImport':0x143fda4,'?DebugMessage':0x13bca00,'??_C@_0N@IMNODCAP':0x12c5d94,'??_C@_0CP@KCEDELNN':0x12df6b8,'??_C@_0BL@INOMAHCP':0x12df69c,'??_C@_0BD@PKOJAOF':0x1258ab0,'??_C@_0BD@DOMBEDCF':0x129a850,'??_C@_0BN@NIIGHKML':0x129a878,'??_C@_00CNPNBAHC':0x122d70e,'??_C@_0BD@FAJOICCE':0x129a864}
count=0
for a,n in [('008e6940',12),('00e03bf0',16),('0099aa90',24)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
table=list(csv.DictReader((w/'probe/008e6940_vtable_audit.tsv').open(),delimiter='\t'));assert len(table)==81
for row in table:assert struct.unpack('<I',rd(0x12874ec+int(row['slot'])*4,4))[0]==int(row['native'],16)
s=subprocess.check_output(['objdump','-r',str(next((o/'dry_work').rglob('008e6940.t.obj')))],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);vs=[]
for i in range(1,len(b),2):
 if b[i]=='.rdata' and '?Clone@CCreatureAction_ReleaseStrengthBerserk' in b[i+1]:vs.append(re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b[i+1],re.M))
assert len(vs)==1 and len(vs[0])==81
for row,(off,typ,sym)in zip(table,vs[0]):
 assert int(off,16)==4*int(row['slot']);method=re.search(r'(\w+)\(',row['method']).group(1)
 if int(row['slot']):assert sym.startswith('?'+method+'@'),(row,sym)
(o/'008e6940_native_vtable_audit.tsv').write_text((w/'probe/008e6940_vtable_audit.tsv').read_text())
slots=[(0x126683c,21,0x72d9f0),(0x1260f0c,296,0x896a30),(0x1260f0c,321,0x891920),(0x1260f0c,325,0x891ad0),(0x1260f0c,350,0x891f50),(0x1260f0c,655,0x891880)]
for vt,k,v in slots:assert struct.unpack('<I',rd(vt+4*k,4))[0]==v
assert rd(0x72d9f0,6)==bytes.fromhex('b8a7000000c3')
for a,t in [(0x12c5d94,'BarrowFields'),(0x12df6b8,'TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_04'),(0x12df69c,'HUD_QUEST_ICON_EARTH_TROLL'),(0x1258ab0,'HUD_ORB_QUEST_CORE'),(0x129a850,'Null dst parameter'),(0x129a878,'File is not open for writing'),(0x129a864,'Error writing file'),(0x122d70e,'')]:assert rd(a,len(t)+1)==t.encode()+b'\0'
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');assert [(e.dll,i.name)for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports if i.address==0x143fda4]==[(b'KERNEL32.dll',b'WriteFile')]
records=[]
for a,g in [('008e6940',2),('00e03bf0',2),('0099aa90',4)]:
 for i in range(1,g+1):
  p=w/'snapshots'/f'{a}_g{i}.cpp';m=(w/'logs'/f'{a}_g{i}.meta').read_text();h=hashlib.sha256(p.read_bytes()).hexdigest();assert 'source_sha256='+h in m and 'command='in m and 'flags='in m;records.append(dict(address=a,grade=i,source_sha256=h,metadata=m))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));print('PASS',count,'relocations,87 native slots,81 emitted action slots,8 literals,WriteFile import,8 immutable grades/final hashes')

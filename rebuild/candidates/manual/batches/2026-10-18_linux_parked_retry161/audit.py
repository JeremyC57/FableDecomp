from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry161';w=r/'astra238_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'??0IntelligentPointerBase':0xa01b10,'??_7IntelligentPointer@@':0x1238c6c,'?Set@IntelligentPointerBase':0xa01b90,'?Get@IntelligentPointerBase':0xa01b50,'??1IntelligentPointerBase':0xa01c10,'??4CCharString':0x99efb0,'?PositionTolerance':0x129ba3c}
a='008cffe0';obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==18;out=[]
for off,typ,sym in rel:
 k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
u=lambda a:struct.unpack('<I',rd(a,4))[0]
assert u(0x129ba3c)==0x38d1b717;assert u(0x1238c6c)==0x4a9710;assert u(0x125d2c4+63*4)==0x6b1230
assert rd(0xa01b10,16)==bytes.fromhex('8bc1c7005cc92901c7400400000000c3')
assert rd(0x6b1230,7)==bytes.fromhex('d905dcde2201c3') and u(0x122dedc)==0
col=u(0x1238c6c-4);td=u(col+12);name=rd(td+8,200).split(b'\0')[0].decode();assert 'CIntelligentPointer' in name and 'CThing' in name,name
(o/'native_auxiliary_evidence.txt').write_text(f'VT1238C6C RTTI {name}, dtor4A9710. PhysicsVT125D2C4slot63=6B1230 baseanglezero. Tolerance129BA3C exact38D1B717. BaseIntelligent ctorA01B10 zeroInfo/vptr129C95C; nativeGet/Set/dtor inspected directly.\n')
grades=[json.loads(x)for x in(w/'ATTEMPTS.jsonl').read_text().splitlines()];selected=[g for g in grades if g['address']==a];assert len(selected)==10
for g in selected:assert g['prior']==0 and g['effective_flags'] and hashlib.sha256(Path(g['command'][5]).read_bytes()).hexdigest()==g['sha256']
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2));print('PASS18relocations,2virtualtargets,RTTI,tolerancepool,baseconstructor,10immutablegrades/finalhashes')

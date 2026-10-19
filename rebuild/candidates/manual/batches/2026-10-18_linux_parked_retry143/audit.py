from pathlib import Path
import struct,re,runpy,sys,csv,json,hashlib,subprocess
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
r=Path('work/codex-lane/w22');o=r/'coordinator_retry143';w=r/'astra238_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];md=Cs(CS_ARCH_X86,CS_MODE_32)
rows=json.loads((o/'payload.json').read_text())['result']['authored'];targets={'??_7ScriptMethods':0x1238c8c,'??3@':0xbfe9bc,'??1CBaseClassNative':0x99a2e0,'__real@00000000':0x122dedc,'__real@40a00000':0x12462a8,'__real@41200000':0x1237f0c,'?LowerBound':0x40f020,'??0NativeCharString':0x99ec30,'??4CCharString':0x99efb0,'??1CCharString':0x99eae0};count=0
for row in rows:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);(o/(a+'_coff_relocs.txt')).write_text(s);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);blocks=[(b[i],b[i+1])for i in range(1,len(b),2)];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max([x for sec,x in blocks if sec=='.text'],key=len),re.M);out=[]
 for off,typ,name in rel:
  off=int(off,16);expect=[v for k,v in targets.items()if name.startswith(k)];assert len(expect)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v;assert expect==[native],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\ttarget\tsymbol\n'+'\n'.join(out)+'\n')
 vr=[re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',x,re.M)for sec,x in blocks if sec=='.rdata'and '?IsEqualTo@ScriptMethods' in x];assert len(vr)==1 and len(vr[0])==79
 s=(w/'src'/(a+'.cpp')).read_text();decl=re.findall(r'virtual ([^;]+); //slot(\d+) 0x([0-9a-fA-F]+)',s);assert len(decl)==78;out=[]
 for declaration,slot,target in decl:
  slot=int(slot);target=int(target,16);assert struct.unpack('<I',rd(0x1238c8c+4*slot,4))[0]==target
  off,typ,sym=vr[0][slot];assert int(off,16)==slot*4;method=re.search(r'(\w+)\(',declaration).group(1);assert sym.startswith('?'+method+'@');out.append(f'{slot}\t{target:08x}\t{sym}\t{declaration}')
 assert struct.unpack('<I',rd(0x1238c8c,4))[0]==0x4ab170
 (o/(a+'_virtual_audit.tsv')).write_text('slot\ttarget\tsymbol\tdeclaration\n'+'\n'.join(out)+'\n')
# Retail accessor offsets, and interface slot identities, independently inspected.
fields={'008a2770':0x20,'008a2a30':0x24,'008a3a60':0x3c,'008a3fd0':0x44,'008a4ae0':0x54,'008a5300':0x60,'008a55c0':0x64,'008a76f0':0x7c}
for a,off in fields.items():assert rd(int(a,16)+0xaa,3)==bytes([0x8b,0x48,off]),(a,rd(int(a,16)+0xaa,3).hex())
assert rd(0x122dedc,4)==bytes(4)
assert struct.unpack('<f',rd(0x12462a8,4))[0]==5.0
assert struct.unpack('<f',rd(0x1237f0c,4))[0]==10.0
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
graderecords=[json.loads(x)for x in(w/'ATTEMPTS.jsonl').read_text().splitlines()];selected=[x for x in graderecords if x['address'] in{z['address']for z in rows}];assert len(selected)==18
for g in selected:
 p=Path(g['command'][5]);assert hashlib.sha256(p.read_bytes()).hexdigest()==g['sha256'];assert g['prior']==0 and g['effective_flags']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2))
print('PASS',count,'body relocations,1106 emitted/native virtual slots,8 accessor offsets, zero/5/10 constants, frozen hashes and18 immutable grade records.')

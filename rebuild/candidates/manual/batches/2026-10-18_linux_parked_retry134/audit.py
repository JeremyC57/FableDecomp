from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry134'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
targets={'?GetPItem':0xa01b50,'?LowerBound':0x4365b0,'?QuestManager':0x13b89fc}
def blocks(a):
 t=(o/f'{a}_coff_relocs.txt').read_text();b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',t)
 return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
for a,needle,targets,count in [(a,'?GetPItem',targets,5)for a in ['008d1ef0','008d22d0','008d2540','008d2610','008d27e0']]:
 selected=max([b for section,b in blocks(a)if section=='.text'and needle in b],key=len);rel=relocs(selected);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
virtuals=[(0x1274240,0x84b070,3,'CEventExpressionPerformedTo'),(0x1274210,0x84ae40,9,'CEventReceivedMoney'),(0x1253c8c,0x5bc3e3,40,'CEventReceivedItemInInventory'),(0x12723c0,0x811ee0,26,'CEventPerformedSpecialAbility')]
for vt,method,value,name in virtuals:
 assert struct.unpack('<I',rd(vt+4,4))[0]==method
 code=rd(method,6)
 if code[0]==0xb8:assert code==b'\xb8'+struct.pack('<I',value)+b'\xc3'
 else:assert code[:4]==bytes([0x6a,value,0x58,0xc3])
(o/'event_virtual_audit.tsv').write_text('native_vtable\tslot\tnative_method\ttype_value\tevent_class\n'+''.join(f'{vt:08x}\t1\t{m:08x}\t{v}\t{n}\n'for vt,m,v,n in virtuals))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
rows=[json.loads(x)for x in(r/'retry200a_sol/ATTEMPTS.jsonl').read_text().splitlines()];assert len(rows)==5
for x in rows:
 cmd=x['command'];assert hashlib.sha256(Path(cmd[5]).read_bytes()).hexdigest()==x['sha256'];assert x['uncertain_reserved']==4 and int(x['new_attempt'])==1
(o/'attempt_audit.txt').write_text('Whole5section: each1newsourcegrade+4UNCERTAINhistoricalreservations,5unusednewgrades retained. Five immutable pregrade hashes/commands verified; no root sourcegrades. Actualsourcehost/noelide/VC pass; originalinvalidplaceholders retained ashistory, never countasprovenauthoredgrades.\n')
print('PASS25relocations,4eventvirtualslot/typeimplementations,frozenhashes,5newgrades/20UNCERTAINreservations')

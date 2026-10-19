from pathlib import Path
import os,sys,runpy,re,struct,subprocess,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry141';a='00692f30'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
s=subprocess.check_output(['objdump','-r',str(o/'dry_work/00/69/00692f30/00692f30.obj')],text=True);(o/'00692f30_coff_relocs.txt').write_text(s)
b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);rel=max([b[i+1] for i in range(1,len(b),2) if b[i]=='.text'],key=len)
targets={'?HasPrecedingAction':0x692ec0,'?GetPItem':0xa01b50,'?LowerBound':0x4365b0,'?GetThingInPrimarySlot':0x6a1440,'??0CBaseIntelligentPointer':0xa01b10,'??_7CIntelligentPointer_CThing':0x1238c6c,'?SetPItem':0xa01b90,'??1CBaseIntelligentPointer':0xa01c10,'??2@':0xbfea1a,'??0CCreatureAction_SheatheItemToInventory':0x69ffd0}
rows=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',rel,re.M);assert len(rows)==13;out=[]
for off,typ,sym in rows:
 off=int(off,16);v=struct.unpack('<i' if typ=='DISP32' else '<I',rd(int(a,16)+off,4))[0];v=v+int(a,16)+off+4 if typ=='DISP32' else v;matches=[x for k,x in targets.items() if sym.startswith(k)];assert matches==[v],(sym,hex(v));out.append(f'{off:04x}\t{typ}\t{v:08x}\t{sym}')
(o/'relocation_audit.tsv').write_text('offset\ttype\tretail_target\tsymbol\n'+'\n'.join(out)+'\n')
assert struct.unpack('<I',rd(0x1238c6c,4))[0]==0x4a9710
s=(r/'luna233_whole/src/00692f30.cpp').read_text();slots=re.findall(r'// native slot(\d+) ([0-9A-Fa-f]{8})',s);assert len(slots)==69
for slot,target in slots:assert struct.unpack('<I',rd(0x1259d24+int(slot)*4,4))[0]==int(target,16),(slot,target)
assert struct.unpack('<I',rd(0x1259d24+4,4))[0]==0x99a400
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS 13 body relocations, 70 native action slots, genuine typed-pointer virtual destructor and frozen hashes')

from pathlib import Path
import struct,re,subprocess,runpy,sys,os,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry126'
sys.argv=['x','00d7f250','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
door={'?IsActiveThread':0xf35b30,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'??_C@_0BB@':0x125d1c8,'??_C@_0CD@':0x12d3f20,'??_C@_0CB@':0x12dc560}
dragon={'??_C@_0N@':0x12884bc,'??_C@_0BA@':0x123b47c,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'?SetAnimation':0x8574d0,'?Initialise@CCombatActionBase':0x857520,'?GetPItem':0xa01b50,'?SetThingIsAble':0x7e4330,'?LowerBound':0x40f020,'?AddTC':0x4c9d60,'?g_Game':0x13b86a0,'?GetLocalPlayer':0x449970,'?GetPlayerThing':0x487dd0,'?AddThingNotSolidTo':0x770fa0}
scream={'?GetPItem':0xa01b50,'??0CCombatActionBase@@QAE@PAUCThingCreatureBase@@PAUCThing@@':0x857f30,'??0CCombatActionBase@@QAE@PAUCThingCreatureBase@@J':0x858030,'??_7CCreatureAction_ScreamerDrainOutOf':0x12620c4,'?SetCurrentAction':0x6644f0,'??1CCombatActionBase':0x62e900,'?FinishAction@CCombatActionBase':0x859a20}
for a,needle,targets,count in [('00d7f250','?IsActiveThread',door,12),('00deefc0','?IsActiveThread',door,12),('008f40b0','?SetAnimation',dragon,20),('008b9010','?SetCurrentAction',scream,17),('009ca780','??3@',{'??3@':0xbfe9bc},1)]:
 txt=(o/f'{a}_coff_relocs.txt').read_text();blocks=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',txt)
 selected=max([blocks[i+1]for i in range(1,len(blocks),2)if blocks[i]=='.text'and needle in blocks[i+1]],key=len)
 rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',selected,re.M);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n');print(a,len(rel),'verified')
 if a=='008b9010':
  audit=list(csv.DictReader((r/'luna234_whole/probe/008b9010_vtable_audit.tsv').open(),delimiter='\t'));assert len(audit)==81
  slots=next(re.findall(r'^([0-9a-f]{8})\s+dir32\s+(\S+)',blocks[i+1],re.M)for i in range(1,len(blocks),2)if blocks[i]=='.rdata'and '?Initialise@CCreatureAction_ScreamerDrainOutOf' in blocks[i+1]);assert len(slots)==81
  vt=struct.unpack('<81I',rd(0x12620c4,324))
  for row,(off,name)in zip(audit,slots):
   i=int(row['slot']);method=re.search(r'([~\w]+)\(',row['method']).group(1);assert int(off,16)==i*4;assert vt[i]==int(row['outof'],16);assert name.startswith('??_E'if i==0 else '?'+method+'@'),(i,name,method)
  (o/'screamer_outof_vtable81.tsv').write_text('slot\tretail_va\tcompiler_symbol\n'+''.join(f'{i}\t{v:08x}\t{slots[i][1]}\n'for i,v in enumerate(vt)))
for va,s in [(0x125d1c8,b'SCRIPT_NAME_HERO'),(0x12d3f20,b'TEXT_QST_B04_BLOCKED_ARENA_DOOR_10'),(0x12dc560,b'TEXT_QST_055_WARDENS_DOOR_LOCKED'),(0x12884bc,b'HOVER_OUT_OF'),(0x123b47c,b'CTCSolidToThing')]:assert rd(va,len(s)+1)==s+b'\0'
for slot,va in [(518,0x89dc10),(115,0x8929d0),(40,0x894370),(7,0x6e7410)]:assert struct.unpack('<I',rd(0x1260f0c+slot*4,4))[0]==va
assert struct.unpack('<I',rd(0x1238c8c+20*4,4))[0]==0x4aab10
assert rd(0x9ca7ac,4)==bytes.fromhex('c20400cc')
print('62 body relocations,81 ScreamerOutOf slots,5 door dispatch slots,5 literals and corrected47-byte boundary verified')

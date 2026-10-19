from pathlib import Path
import struct,re,runpy,sys,os,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry127'
sys.argv=['x','008f2e00','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
common={'?GetPItem':0xa01b50,'?LowerBound':0x40f020}
flight={**common,'__real@00000000':0x122dedc,'?GetTotalTime':0x6925b0,'?RotateXY':0xa14040,'?FrameUpdate':0x859d80}
land={**common,'??_C@':0x1288488,'??0CCharString':0x99ebf0,'?SetAnimation':0x8574d0,'??1CCharString':0x99eae0,'?Initialise':0x857520,'?GetPosition':0x4c73d0,'?g_DragonLandingHeightDrop':0x128838c,'?SetSuspendAnimation':0x833a10}
charge={**common,'?FinishAction':0x859a20,'?AllowReactions':0x79e580,'?Kill':0x4c9b80,'??0CCreatureAction_ScorpionCharge':0x8c4ea0,'?SetCurrentAction':0x6644f0,'??1CCombatActionBase':0x62e900}
for a,needle,targets,count in [('008f2e00','?RotateXY',flight,8),('008f3ad0','?SetAnimation',land,17),('008c5490','?AllowReactions',charge,19)]:
 txt=(o/f'{a}_coff_relocs.txt').read_text();blocks=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',txt);selected=max([blocks[i+1]for i in range(1,len(blocks),2)if blocks[i]=='.text'and needle in blocks[i+1]],key=len)
 rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',selected,re.M);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n');print(a,len(rel),'verified')
for slot,va in [(31,0x6b01d0),(70,0x723c00),(75,0x7239c0)]:assert struct.unpack('<I',rd(0x1265f84+slot*4,4))[0]==va
assert rd(0x723c00,24)==bytes.fromhex('8b515c8b44240489108b51608950048b4964894808c20400')
assert rd(0x122dedc,4)==b'\0'*4
assert rd(0x128838c,4)==bytes.fromhex('e12ff141')
assert rd(0x1288488,5)==b'LAND\0'
assert struct.unpack('<I',rd(0x1265a3c+21*4,4))[0]==0x71a7e0
s=(r/'luna234_whole/src/008c5490.cpp').read_text()
def declarations(name):
 m=re.search(r'struct (?:__declspec\(novtable\) )?'+name+r'(?:\s*:[^{]+)?\s*\{',s);assert m,name
 body=s[m.end():s.index('\n};',m.end())];return re.findall(r'virtual\s+([^;]+);',body)
def key(decl):
 name=re.search(r'([~\w]+)\(',decl).group(1);return '~'if name.startswith('~')else name
base=declarations('CCreatureActionBase');assert len(base)==70
combat=base[:]
for x in declarations('CCombatActionBase'):
 keys=[key(d)for d in combat]
 if key(x)in keys:combat[keys.index(key(x))]=x
 else:combat.append(x)
assert len(combat)==81
for name,va in [('CCreatureAction_ScorpionCharge',0x1281024),('CCreatureAction_ScorpionChargeInto',0x1280ed4)]:
 decls=combat[:]
 for x in declarations(name):
  keys=[key(d)for d in decls]
  if key(x)in keys:decls[keys.index(key(x))]=x
  else:decls.append(x)
 audit=list(csv.DictReader((r/'luna234_whole/probe'/('008c5490_'+name+'_vt.tsv')).open(),delimiter='\t'));values=struct.unpack('<83I',rd(va,332));assert len(decls)==len(audit)==83
 for i,(x,y,v)in enumerate(zip(decls,audit,values)):
  assert int(y['slot'])==i and int(y['native'],16)==v and key(x)==key(y['method']),(i,x,y)
 (o/(name+'_vtable83.tsv')).write_text('slot\tnative\tactual_source_declaration\n'+''.join(f'{i}\t{v:08x}\t{d}\n'for i,(v,d)in enumerate(zip(values,decls))))
print('44 body relocations,3 physics slots,Mapwho identity,3 constants/literals and two genuine83-slot action declarations verified')

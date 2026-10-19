from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry139'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
def blocks(a):
 b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',(o/f'{a}_coff_relocs.txt').read_text());return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
common={'?GetPItem':0xa01b50,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'?LowerBound':0x40f020,'?GetThingInPrimarySlot':0x6a1440,'?SetProjectileWeaponLoaded':0x5dac50,'?PlayAnimation':0x7f90b0}
extra={'009bf350':{'?g_DisplayRenderingEnabled':0x13ca810,'?EndRender':0xa0b520,'?GetSurfaceLevel':0x9f9e00,'?GetPixelFormat':0x9f2f60,'?IsEquivalent':0x9e37f0,'?GetDimensions':0x9f2f90,'?CopyFromSurface':0x9f37e0,'?BeginRender':0xa0b480,'?g_CSurfaceVTable':0x122f84c,'?Release':0x9f2e20},'0055a3e0':{'?GetUIManager':0x41e5f2,'?UpdateClickable':0x55acb0,'?g_PositionEpsilon':0x129ba3c,'__real@00000000':0x122dedc},'008d5970':{'??_C@':0x1268a5c,'?MakeNoise':0x662110,'?GetShotAccuracyPercentage':0x663b10,'?LowerBound':0x4365b0,'?CreateShot':0x7c0ad0},'0078b6b0':{'??_C@_0BH@':0x12657bc,'??_C@_0BJ@':0x126bb64,'?AddQueuedAnimation':0x7f8c20},'0084a5f0':{'?GetCarryable':0x5cc10e,'?IsCarrySlotEmpty':0x6a1230,'?GetThingInCarrySlot':0x6a11f0,'?DropWeapon':0x845ce0,'?PutThingInCarrySlot':0x6a1f80}}
for a,needle,n in [('009bf350','?GetSurfaceLevel',15),('0055a3e0','?UpdateClickable',7),('008d5970','?CreateShot',15),('0078b6b0','?AddQueuedAnimation',16),('0084a5f0','?DropWeapon',16)]:
 targets=dict(common,**extra[a]);rel=relocs(max([b for sec,b in blocks(a)if sec=='.text'and needle in b],key=len));assert len(rel)==n,(a,len(rel));out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v;assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
for a,vt in [('008d5970',0x1285544),('0078b6b0',0x126b9d4),('0084a5f0',0x1270c3c)]:
 table=list(csv.DictReader((r/f'luna234_whole/probe/{a}_vtable_audit.tsv').open(),delimiter='\t'));assert len(table)==70
 for row in table:assert struct.unpack('<I',rd(vt+4*int(row['slot']),4))[0]==int(row['native'],16)
 (o/(a+'_native_vtable_audit.tsv')).write_text('vtable\tslot\tnative_target\tmethod\n'+''.join(f"{vt:08x}\t{x['slot']}\t{x['native']}\t{x['method']}\n"for x in table))
for vt,slot,target in [(0x124ba94,31,0x55a630),(0x124ba94,153,0x55a7c0)]:assert struct.unpack('<I',rd(vt+slot*4,4))[0]==target
for va,lit in [(0x1268a5c,b'SIMPLE_NOISE'),(0x12657bc,b'PROJECTILE_WEAPON_LOAD'),(0x126bb64,b'PROJECTILE_WEAPON_AIMING')]:assert rd(va,len(lit)+1)==lit+b'\0'
for va,f in [(0x129ba3c,0.0001),(0x122dedc,0.0)]:assert rd(va,4)==struct.pack('<f',f)
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
(o/'abi_audit.txt').write_text('D3D9 slots34 StretchRect,41 BeginScene,42 EndScene use native stdcall stack-passed this and native4-byte member pointers, full representation on host. Twelve fixture scenarios include failure, compatibility short circuits, mutable gate and balanced surface ownership. Draggable actual slots31/153 independently verified. Three complete70-slot native action tables verified; native Delete const slot1 retained for intrusive definition lifetimes. Fastcall CreateShot uses two register arguments plus8 stack arguments; DropWeapon uses two registers plusone stack argument.\n')
print('PASS69 body relocations,210 native action slots,2 UI virtual slots,3 literals,2 float pools and all frozen hashes.')

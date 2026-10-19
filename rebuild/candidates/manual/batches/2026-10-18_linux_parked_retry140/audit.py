from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry140'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
def blocks(a):
 b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',(o/f'{a}_coff_relocs.txt').read_text());return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
targets={'??2@':0xbfea1a,'??3@':0xbfe9bc,'??0CCombatActionBase':0x857f30,'??_7CCreatureAction_InterruptableMidAttack':0x1262edc,'?Reset':0x905fd0,'?AddAction':0x8b0b10,'??0CCreatureAction_VillagerAttackKnockdown':0x97cf90,'?OnCreate':0x8b0650,'?g_01375550':0x1375550,'??0CRetailString':0x99ebf0,'??1CCharString':0x99eae0,'??0CCreatureAction_PlayCombatAnimation':0x8b7f80,'??0CCreatureAction_CastPhysicalShieldSpell':0x8bb6f0,'??_C@_0BL@':0x12801b4,'??_C@_00':0x122d70e,'?ShouldPlayUnsheatheWeaponAction':0x8b7cf0,'?IsIncapacitated':0x85f0c0,'?IsKnockedDown':0x85f010,'?LowerBound':0x40f020,'??0CCreatureAction_UnsheatheWeapons':0x8b85d0,'?IsFreeToPerformAction':0x662210,'?PerformAction':0x8b7ba0,'??1CCombatActionBase':0x62e900,'?SetCurrentAction':0x6644f0,'??0CParentDefClassBase':0x46ee98,'??4CParentDefClassBase':0x431ed0}
for a,n in [('00908a20',16),('00908b50',16),('009092c0',17),('009093f0',17),('006ebdd0',10),('0047756c',1),('00477964',1)]:
 rel=relocs(max([b for sec,b in blocks(a)if sec=='.text'],key=len));assert len(rel)==n,(a,len(rel));out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v;assert native==match[0],(a,name,hex(native),hex(match[0]));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
table=list(csv.DictReader((r/'luna233_whole/handoff/COMBO_INTERRUPT81_VTABLE_AUDIT.tsv').open(),delimiter='\t'));assert len(table)==81
for a in ['00908a20','00908b50']:
 vb=[b for sec,b in blocks(a)if sec=='.rdata'and '?Clone@CCreatureAction_InterruptableMidAttack' in b];assert len(vb)==1;vr=relocs(vb[0]);assert len(vr)==81
 for row,(off,typ,sym) in zip(table,vr):
  slot=int(row['slot']);assert int(off,16)==4*slot;assert sym==row['emitted_symbol'];assert struct.unpack('<I',rd(0x1262edc+4*slot,4))[0]==int(row['native_address'],16)
 (o/(a+'_emitted_vtable_audit.tsv')).write_text((r/'luna233_whole/handoff/COMBO_INTERRUPT81_VTABLE_AUDIT.tsv').read_text())
unsheathe=[x.split('\t')for x in(r/'luna233_whole/handoff/UNSHEATHE_NATIVE81.tsv').read_text().splitlines()];assert len(unsheathe)==81
for slot,target in unsheathe:assert struct.unpack('<I',rd(0x127b1b4+int(slot)*4,4))[0]==int(target,16)
(o/'006ebdd0_native_vtable_audit.tsv').write_text('slot\ttarget\n'+'\n'.join('\t'.join(x)for x in unsheathe)+'\n')
assert rd(0x47757a,6)==b'\xc7\x06'+struct.pack('<I',0x123634c)
for slot,target in [(0,0x477943),(18,0x47769e),(19,0x47795f),(20,0x477698)]:assert struct.unpack('<I',rd(0x123634c+slot*4,4))[0]==target
assert rd(0x12801b4,len(b'ST_BUILDUP_PHYSICAL_SHIELD\0'))==b'ST_BUILDUP_PHYSICAL_SHIELD\0'
assert rd(0x122d70e,1)==b'\0'
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
ledger=list(csv.DictReader((r/'luna236_whole/GRADE_LEDGER.tsv').open(),delimiter='\t'))
for a,n in [('0047756c',3),('00477964',8)]:
 rows=[x for x in ledger if x['address']==a];assert sum(x['counts_toward_10']=='yes'for x in rows)==n
 for row in rows:
  p=r/'luna236_whole/attempts'/a/row['grade_dir'];assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==row['source_sha256'];assert(p/'command.txt').read_text().strip()
(o/'luna_attempt_audit.tsv').write_text((r/'luna236_whole/GRADE_LEDGER.tsv').read_text())
print('PASS78 body relocations,162 emitted combo slots,81 native Unsheathe slots,4 RTTI local-detail slots, explicit constructor vptr, two literals, frozen hashes and Luna grade-ledger snapshots.')

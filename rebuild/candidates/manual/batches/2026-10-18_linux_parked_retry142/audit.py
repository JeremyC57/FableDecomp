from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib,subprocess,importlib.util
r=Path('work/codex-lane/w22');o=r/'coordinator_retry142'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
def blocks(a,fixture=False):
 obj=next((o/'dry_work').rglob(a+('.t.obj'if fixture else'.obj')));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);(o/(a+('_fixture'if fixture else'')+'_coff_relocs.txt')).write_text(s)
 b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);return[(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
targets={'??_7CBaseClassNative':0x1231710,'??_7ScriptMethods':0x1238c8c,'??_C@':0x12caecc,'??0CCharString':0x99ebf0,'??3@':0xbfe9bc,'??1CBaseClassNative':0x99a2e0,'??1CCharString':0x99eae0,'??_7CIsThingAlive':0x12c32f0,'??0ScriptThingCopy':0x4abe90,'?SetPredicate':0xf35b10,'??1ScriptThingCopy':0x4aa840,'?IsActiveThreadTerminating':0xf35b30,'?RemoveFreeArea':0xc27f50,'?FindFreeArea':0xc27fd0,'?InsertFreeArea':0xc27ef0,'?MoveArea':0xc28580,'?CombineAdjacentAreas':0xc282e0,'__real@44ffe000':0x1288be4,'__real@00000000':0x122dedc,'?SetSizeZAt':0x825440,'?TransferAbility':0x452e01,'?TransferStat':0x469b8a,'?TransferLong@':0x431102,'?TransferVector2':0x4568bc,'?TransferLongVector':0x466a47,'?TransferFloat':0x431061,'?TransferBool':0x43314a,'?TransferDefIndex':0x4522f4}
for a,n in [('00d1e310',8),('00d864b0',8),('00a02020',0),('00c28120',0),('00c286f0',10),('008f60c0',3),('00469ab3',17)]:
 bs=[b for sec,b in blocks(a)if sec=='.text'];rel=relocs(max(bs,key=len))if bs else[];assert len(rel)==n,(a,len(rel));out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v;assert native==match[0],(a,name,hex(native),hex(match[0]));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
for a,owner,native,n in [('00d1e310','ScriptMethods',0x1238c8c,79),('00d1e310','GameMethods',0x1260f0c,845),('00d864b0','GameCalls',0x1260f0c,845)]:
 s=(r/'retry200d_sol/src'/(a+'.cpp')).read_text();start=s.index('struct '+owner+' {') if owner!='ScriptMethods' else s.index('struct ScriptMethods:CBaseClassNative');s=s[start:];s=s[:s.index('\n};')];decl=re.findall(r'virtual ([^;]+); //slot(\d+) 0x([0-9a-fA-F]+)',s);assert len(decl)==n-1,(owner,len(decl))
 vb=[relocs(b) for sec,b in blocks(a,True) if sec=='.rdata'];vb=[v for v in vb if len(v)==n and any('@'+owner+'@@'in sym for off,typ,sym in v)];assert len(vb)==1,(owner,len(vb));vr=vb[0];out=[]
 for declaration,slot,target in decl:
  slot=int(slot);target=int(target,16);assert struct.unpack('<I',rd(native+slot*4,4))[0]==target,(owner,slot)
  off,typ,sym=vr[slot];assert int(off,16)==4*slot;method=re.search(r'(\w+)\(',declaration).group(1);assert sym.startswith('?'+method+'@'),(owner,slot,method,sym)
  if method=='GetThingWithScriptName':assert ('@@0@Z' in sym)==(slot==73),(owner,slot,sym)
  out.append(f'{slot}\t{target:08x}\t{sym}\t{declaration}')
 assert struct.unpack('<I',rd(native,4))[0]==(0x4ab170 if owner=='ScriptMethods'else 0x6e7d50)
 (o/(a+'_'+owner+'_virtual_audit.tsv')).write_text('slot\tnative_target\temitted_symbol\tdeclaration\n'+'\n'.join(out)+'\n')
assert rd(0x12caecc,22)==b'VILLAGE_KNOTHOLEGLADE\0'
assert struct.unpack('<f',rd(0x1288be4,4))[0]==2047 and rd(0x122dedc,4)==bytes(4)
assert list(struct.unpack('<3I',rd(0x12c32f0,12)))==[0xcdf120,0xcdf130,0xcdf160]
# Independently extract the genuine helper from root's own object and compare all81 bytes.
spec=importlib.util.spec_from_file_location('landverify','tools/decomp_pipeline/verify_and_land.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
b,sec,sym=m.obj_text(next((o/'dry_work').rglob('00c286f0.obj')),'?InsertFreeArea@');assert b==rd(0xc27ef0,81) and len(b)==81
(o/'helper_audit.txt').write_text('C27EF0 InsertFreeArea:81 unmasked bytes exact in root independently built object; SHA256 '+hashlib.sha256(b).hexdigest()+'\n')
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
counts=[]
for lane,n in [('retry200d_sol',19),('retry200e_sol',13)]:
 grades=[json.loads(l)for l in(r/lane/'ATTEMPTS.jsonl').read_text().splitlines()];assert len(grades)==n,(lane,len(grades))
 for g in grades:assert hashlib.sha256(Path(g['snapshot']).read_bytes()).hexdigest()==g['sha256'];assert g['reserved_uncertain']==4 and int(g['new_attempt'])<=6;assert g['effective_flags'] and g['command']
 counts.append(f'{lane}: {n} immutable source/command/flag grades;20 UNCERTAIN historical reservation; {30-n} unused new allowances.\n')
(o/'attempt_audit.txt').write_text(''.join(counts))
print('PASS46 body relocations,1769 emitted/native virtual slots, Alive predicate, village literal,2floats,81-byte helper, frozen hashes and32 grade snapshots.')

from pathlib import Path
import struct,re,runpy,sys,json,hashlib,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry178';w=r/'astra241_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'??3@':0xbfe9bc,'??0CCharString':0x99e4b0,'?IsThingCarryingMeleeWeapon':0x6a15a0,'?LowerBound@ComponentMap':0x40f020,'?GetThingInPrimarySlot':0x6a1440,'?GetWeaponClass':0x5da7f0,'??4CCharString':0x99efe0,'??0NativeStringValue':0x99ec30,'?NotEqual@NativeStringValue':0x99e900,'??1CCharString':0x99eae0,'?SetAndHash@AnimationGroupFilter':0x73dff0,'?BaseFrameUpdate@':0x73ac50,'?GetPItem@BaseIntelligentPointer':0xa01b50,'?GetCurrentMode':0x73a6e0,'??0BaseIntelligentPointer':0xa01b10,'??_7IntelligentThingPointer':0x1238c6c,'?SetPItem@BaseIntelligentPointer':0xa01b90,'??1BaseIntelligentPointer':0xa01c10,'?RemoveMode@ModeManager':0x73aba0,'?Kill@ThingNative':0x4c9b80,'?StandDeactivate@ModeScreamer':0x861d00}
strings={0x12859d4:'BERSERK_WITH_HEAVY_SWORD',0x12859c0:'BERSERK_WITH_SWORD',0x1267f30:'BERSERK_NO_WEAPON'}
for a,s in strings.items():assert rd(a,len(s)+1)==s.encode()+b'\0'
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32' else u(int(a,16)+k)
  assert v==int(target,16)
  if sym.startswith('??_C@'):assert v in strings and strings[v] in sym
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==55
assert u(0x1267f00+5*4)==0x8621a0 and u(0x1267f00+6*4)==0x8d6830
assert rd(0x8d6830,5)==bytes.fromhex('e9cbb4f8ff')
assert rd(0x73a6e0,9)==bytes.fromhex('8b410c8b088b4108c3')
assert rd(0x5da7fe,3)==bytes.fromhex('8b712c')
assert rd(0x741435,2)==bytes.fromhex('6a00')and u(0x74143b)==0x1238c6c
assert u(0x1238c6c)==0x4a9710
assert u(0x1268234+4)==0x741460 and rd(0x741460,6)==bytes.fromhex('b845000000c3')
h=runpy.run_path(str(o/'coff_helpers.py'));out={}
for vt in [0x1238c6c,0x129c95c,0x1267f00]:
 col=u(vt-4);ch=u(col+16);names=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);names.append([rd(u(bc)+8,180).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 out[hex(vt)]={'hierarchy':names}
assert out['0x1238c6c']['hierarchy']==[['.?AV?$CIntelligentPointer@VCThing@@@@',0],['.?AVCBaseIntelligentPointer@@',0]]
for a in ['008da890','008da690']:
 sy,secs=h['coff'](next((o/'dry_work').rglob(a+'.obj')));tables={}
 for name,value,sec in sy.values():
  if name!='??_7IntelligentThingPointer@@6B@':continue
  size,rels=secs[sec-1];ss=[rels[k]for k in range(value,size,4)if k in rels];assert len(ss)==1 and ss[0].startswith(('??_G','??_E'));tables[name]=[h['und'](s)for s in ss]
 assert tables;out[a]={'emitted_virtuals':tables}
records=[json.loads(l)for l in (w/'ATTEMPTS.jsonl').read_text().splitlines()];records=[x for x in records if x['address']in obs];assert len(records)==7
for x in records:
 p=Path(x['snapshot']);assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['base_flags']==['/O2','/Oy','/W3']and not x['extra_flags'];assert (p/'stdout.txt').exists()and(p/'stderr.txt').exists()
assert collections.Counter(x['address']for x in records)=={'00861d00':1,'008d9530':1,'008da890':4,'008da690':1}
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS55relocations/3fullstrings/native mode+pointer ABI/3RTTIhierarchies/7immutablegrades')

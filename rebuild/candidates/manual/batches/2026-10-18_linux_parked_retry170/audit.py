from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry170';w=r/'astra239_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?GetPItem@CBaseIntelligentPointer@@QBE':0xa01b50,'?LowerBound@ComponentMap@@QBE':0x4365b0,'?QuestManager':0x13b89fc,'?GetNullScriptThing@':0x704730,'?GetScriptThing@CThingNative@@QBE':0x4c7e50,'?DefinitionStrings':0x13ca828,'?GetString@CDefStringTable@@QBE':0x9d49b0,'??4CCharString':0x99efb0,'??0CCharString@@QAE@ABU':0x99ec30,'??1CCharString':0x99eae0,'?GetPHitBy@HitEventBase@@QBE':0x810fc0,'?GetBuildupTime@HitEventBase@@QBE':0x810f30};expected_counts={'008d1b80':10,'008d1ca0':9,'008d1dd0':10,'008d21b0':10,'008d1890':10,'008d1330':8,'008d15b0':9};records=[];vtout={};total=0
for a,n in expected_counts.items():
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[];total+=len(rel)
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k)
  allowed=[v0 for p,v0 in M.items()if sym.startswith(p)]
  if sym.startswith('?GetEvent@'):allowed=[{'008d1890':0x8d4ed0,'008d1330':0x8d4a90,'008d15b0':0x8d4cd0}[a]]
  assert allowed==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
 src=(w/'src'/(a+'.cpp')).read_text();t=next((o/'dry_work').rglob(a+'.t.obj'));s=subprocess.check_output(['objdump','-r',str(t)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);blocks=[b[i+1]for i in range(1,len(b),2)if b[i]=='.rdata'];decl=re.findall(r'virtual ([^;]+); //slot(\d+) 0x([0-9a-fA-F]+)',src)
 if decl:
  assert len(decl)==78;v=next(x for x in blocks if'??_EScriptMethods@@'in x and'?IsEqualTo@ScriptMethods'in x);rr=re.findall(r'^([0-9a-f]{8})\s+dir32\s+(\S+)',v,re.M);assert len(rr)==79
  for declaration,slot,target in decl:
   k=int(slot);assert u(0x1238c8c+4*k)==int(target,16);assert int(rr[k][0],16)==4*k;name=re.search(r'(\w+)\(',declaration).group(1);assert rr[k][1].startswith('?'+name+'@')
  vtout[a+'_ScriptMethods79']=rr
 if a in ['008d1890','008d1330','008d15b0']:
  v=next(x for x in blocks if'??_EHitByEvent@@'in x and'?IsFlourish@HitEventBase'in x);rr=re.findall(r'^([0-9a-f]{8})\s+dir32\s+(\S+)',v,re.M);assert len(rr)==6;assert rr[1][1].startswith('?GetType@HitByEvent@@UBE?AW4');assert rr[3][1].startswith('?GetLifeTime@HitEventBase@@UBEJ');assert rr[5][1].startswith('?IsFlourish@HitEventBase@@UBE_N');vtout[a+'_HitBy6']=rr
 if a=='008d1ca0':
  v=next(x for x in blocks if'??_EExpressionDefNative@@'in x);rr=re.findall(r'^([0-9a-f]{8})\s+dir32\s+(\S+)',v,re.M);assert len(rr)==21;assert rr[1][1].startswith('?Delete@CParentDefClassBase@@UBEX');assert rr[10][1].startswith('?GetInstantiationName@CParentDefClassBase@@UBE?AU');assert rr[14][1].startswith('?GetPSubDefInfo@CParentDefClassBase@@UBE');assert rr[15][1].startswith('?GetPSubDefInfo@CParentDefClassBase@@UAE');vtout[a+'_Expression21']=rr
 for p in sorted((w/'snapshots').glob(a+'_[0-9]*')):
  x=json.loads((p/'grade.json').read_text());assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['effective_flags'];x['snapshot_path']=str(p);records.append(x)
assert total==66 and len(records)==20
native={}
for vt,targets in [(0x1272fa8,[0x83b0f0,0x83af50,0x57a4fc,0x810e50,0x57a4ff,0x810ef0]),(0x1233d1c,[0x456cc5,0x42d930,0x9ace90,0x9fbef0,0x42d950,0x42d960,0x42d970,0x42d980,0x9fbf00,0x9fbf10,0x4303b0,0x4303c0,0x9b00d0,0x9b0170,0x9ad4a0,0x9ad510,0x4303d0,0x42da80,0x4569a7,0x45d637,0x4569a1])]:
 assert [u(vt+4*i)for i in range(len(targets))]==targets;native[hex(vt)]=[hex(v)for v in targets]
assert rd(0x4c7e64,6)==bytes.fromhex('8bc65ec20400');assert rd(0x70474c,3)==bytes.fromhex('c20400');assert rd(0x810f30,7)==bytes.fromhex('8b4108d9402cc3');assert rd(0x810fc0,6)==bytes.fromhex('8b490883c150');assert rd(0x8d4ad3,3)==bytes.fromhex('ff5214')
# Actual bind4 order: name,flourish,before,after vs shot,name,before,after; bind3:hasthing,after,before.
for call,target in [(0x8d4ac6,0x811100),(0x8d4d06,0x810f70),(0x8d4d13,0x811100),(0x8d4f06,0x810f90)]:assert call+5+struct.unpack('<i',rd(call+1,4))[0]==target
(o/'native_hierarchy_audit.json').write_text(json.dumps({'native_tables':native,'emitted_tables':vtout},indent=2));(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for x in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(x[k]['path']).read_bytes()).hexdigest()==x[k]['sha256']
print('PASS66relocations/4x79scriptVT+3x6hitVT+21defVT/nativepredicateorder/hiddenreturn/lifetimes/immutablegrades',len(records))

# Actual inherited script signatures match the previously audited canonical interface, not only slot names.
prior=json.loads((r/'coordinator_retry149/payload.json').read_text())['result']['authored'];pattern=r'virtual ([^;]+); //slot(\d+) 0x([0-9a-fA-F]+)';ref=next(re.findall(pattern,x['source_cpp'])for x in prior if len(re.findall(pattern,x['source_cpp']))==78)
for a in ['008d1b80','008d1dd0','008d21b0','008d1890']:assert re.findall(pattern,(w/'src'/(a+'.cpp')).read_text())==ref

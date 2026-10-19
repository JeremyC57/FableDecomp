from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv,collections
r=Path('work/codex-lane/w22');o=r/'coordinator_retry177';w=r/'retry202d_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'??_7CLocalDetailPrimitiveRepeatedMesh':0x12a82ac,'?g_LocalDetailEngine':0x1436ea8,'?FreeVirtual@LandscapeMemoryMethods':0xbd9b90,'?FreeAlignedVirtual@LandscapeMemoryMethods':0xbd9ca0,'??_7CEnginePrimitiveHandle':0x122f598,'??1CBaseClass':0x99a2e0,'??_7CMovableResource':0x129b860,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'??_7CScriptThing':0x1238c8c,'??3@':0xbfe9bc,'?IsActiveThreadTerminating':0xf35b30,'?g_ScriptDef':0x143e90c,'??BCDefString':0x415d70,'??1CScriptThing':0x4aa840}
strings={0x12e30a8:'AC_DummyMoving',0x12ce8b4:'OBJECT_STRAW_DUMMY_01',0x12ea06c:'FishermanWasp'}
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
assert sum(map(len,obs.values()))==30
assert rd(0xf35b30,7)==bytes.fromhex('8b01ff500c8bc8')and rd(0xcb7940,14)==bytes.fromhex('8b412c85c0750332c0c38a4005c3')
for vt,k,v in [(0x1260f0c,7,0x6e7410),(0x1260f0c,91,0x8a9100),(0x1260f0c,99,0x8946e0),(0x1238c8c,3,0x4aa900),(0x1238c8c,4,0x4aa930),(0x1238c8c,6,0x4aa980),(0x12a3d60,3,0xb8ffc0),(0x12a3d60,1,0x42d930)]:assert u(vt+k*4)==v
# Independent PDB-to-emitted-COFF signature check of all179 script slots per fixture.
runpy.run_path(str(o/'slots.py'));scripts=json.loads((o/'script_virtual_signatures.json').read_text());assert not scripts['mismatches'];assert len(scripts['slots'])==358
h=runpy.run_path(str(o/'coff_helpers.py'));expected={0x1231710:[0x4371f0],0x122f598:[0x41c500],0x12354d0:[0x4664e0,0x42d930],0x12a3d60:[0xb8ffe0,0x42d930,0xbfea08,0xb8ffc0,0xb8ffd0],0x129b860:[0x9d5110,0xbfea08,0x9d50f0,0x9d5100],0x12a82ac:[0xbfbc50,0xbf9cc0,0xbf9a80,0x9d5100,0xbdf6d0,0xbfaba0,0xbdf6e0,0xbf9b40,0xbfa2a0,0xbfbb90]};out={}
for vt,values in expected.items():
 assert [u(vt+4*k)for k in range(len(values))]==values
 col=u(vt-4);ch=u(col+16);hier=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);hier.append([rd(u(bc)+8,140).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 out[hex(vt)]={'targets':list(map(hex,values)),'hierarchy':hier}
assert out['0x12a82ac']['hierarchy']==[['.?AVCLocalDetailPrimitiveRepeatedMesh@@',0],['.?AVCLocalDetailPrimitiveBase@@',0],['.?AVCMovableResource@@',0]]
methods=['destructor','RelocateData','CanRelocateData','Verify','RenderUpdate','AddObjectsToPrimitiveRenderer','NeedsRenderUpdate','OnPostRelocation','Load','GetType'];sy,secs=h['coff'](next((o/'dry_work').rglob('00bfaae0.obj')))
for name,value,sec in sy.values():
 if name!='??_7CLocalDetailPrimitiveRepeatedMesh@@6B@':continue
 size,rels=secs[sec-1];ss=[rels[k]for k in range(value,size,4)if k in rels];got=['destructor'if x.startswith(('??_G','??_E'))else x.split('@')[0][1:]for x in ss];assert got==methods;out['fresh_local_detail_signatures']=[h['und'](x)for x in ss]
out['script_interfaces']=scripts;out['limitation']='Native method projections and preexisting-interface fixture doubles do not claim to reproduce actual engine construction. Meaningful lifetime tests use genuine string/ScriptThing copy and destruction contracts; no full-game integration.'
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
records=[json.loads(l)for l in (w/'ATTEMPTS.jsonl').read_text().splitlines()];assert len(records)==18
for x in records:
 p=w/'snapshots'/f"{x['address']}_{x['attempt']}.cpp";assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['effective_flags']==['/O2','/Oy','/W3']and x['prior_reserved_uncertain']==4
assert collections.Counter(x['address']for x in records)=={'00bfaae0':1,'00e33b00':5,'00e7b970':6,'00873930':6}
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS30relocations/3fullstrings/358scriptCOFF-signature-checks/6RTTIhierarchies/10localdetailmethods/18immutablegrades+16UNCERTAINreservations')

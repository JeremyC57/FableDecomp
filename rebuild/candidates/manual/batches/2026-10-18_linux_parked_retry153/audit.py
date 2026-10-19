from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry153';w=r/'retry201a_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];rows=json.loads((o/'payload.json').read_text())['result']['authored'];count=0
M={'_malloc':0xbfea0e,'??0CBaseIntelligentPointer':0xa01b10,'??_7?$CIntelligentPointer':0x1238c6c,'?SetPItem':0xa01b90,'__ftol2':0xbfea70,'??_7CIsThingConscious':0x12c2fe8,'??0ScriptThing@':0x4abe90,'?SetPredicate':0xf35b10,'??1ScriptThing@':0x4aa840,'?IsActiveThreadTerminating':0xf35b30,'??_C@':0x122d70e,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0}
for row in rows:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len,default=''),re.M);out=[]
 for off,typ,sym in rel:
  n=int(off,16);native=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+n,4))[0];es=[v for k,v in M.items()if sym.startswith(k)];assert es==[native],(a,sym,hex(native));out.append(f'{off}\t{native:08x}\t{sym}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
slots=[(0x124bfb4,24,0x531090),(0x124bfb4,122,0x52eec0),(0x124bfb4,123,0x52ecc0),(0x1238c6c,0,0x4a9710),(0x12c2fe8,0,0xcddab0),(0x12c2fe8,1,0xcddae0),(0x12c2fe8,2,0xcddb10),(0x1238c8c,21,0x4aab30),(0x1238c8c,75,0x4ab130),(0x1238c8c,61,0x4aafa0),(0x1260f0c,7,0x6e7410)]
for vt,slot,target in slots:assert struct.unpack('<I',rd(vt+slot*4,4))[0]==target,(hex(vt),slot)
(o/'native_virtual_slots.tsv').write_text('vtable\tslot\ttarget\n'+''.join(f'{v:08x}\t{s}\t{t:08x}\n'for v,s,t in slots))
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');imports=[(entry.dll,imp.name)for entry in pe.DIRECTORY_ENTRY_IMPORT for imp in entry.imports if imp.address==0x1440158];assert imports==[(b'MSVCR71.dll',b'malloc')];assert rd(0xbfea0e,6)==bytes.fromhex('ff2558014401');assert rd(0x122d70e,1)==b'\0'
selected=[]
for a,g in [('0087ff60',2),('0055b8f0',5),('00d9c4a0',4),('0055bc30',6)]:
 ps=sorted((w/'snapshots').glob(a+'-??'));assert len(ps)==g
 for p in ps:
  m=json.loads((p/'command.json').read_text());assert m['command']and m['base_flags']and m['reserved_uncertain']==4;assert hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==m['source_sha256'];selected.append(dict(snapshot=str(p),manifest=m))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2));print('PASS',count,'relocs,11nativeVTslots,mallocimport,17immutablegrades includingcappedrow,finalhashes')

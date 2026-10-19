from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry152';w=r/'astra235_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];rows=json.loads((o/'payload.json').read_text())['result']['authored'];count=0
M={'??_7CIsThingAlive':0x12c32f0,'??_7CIsThingConscious':0x12c2fe8,'??0ScriptThingCopy':0x4abe90,'?SetPredicate':0xf35b10,'??1ScriptThingCopy':0x4aa840,'?IsActiveThreadTerminating':0xf35b30,'?InitializeCBaseObject':0x99a380,'?ResourceVTable':0x127094c,'?IsScripting':0xcd23b9,'?Release@ScriptResource':0xcd2770,'??1ScriptResource':0x7e74d0,'??_7CBaseClassNative':0x1231710,'??_7ScriptMethods':0x1238c8c,'??3@':0xbfe9bc,'?ResourceBaseVTable':0x126008c,'?DestroyCBaseObject':0x99a430,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'?IsWithinDistance':0xcbe2ff,'??1CBaseClassNative':0x99a2e0}
strings={'??_C@_00CNPNBAHC@?$AA@':(0x122d70e,b'\0'),'??_C@_0BP@EAAONMAD@NEW_RED_FORCEFIELD_IDLE_01_OFF?$AA@':(0x12c3470,b'NEW_RED_FORCEFIELD_IDLE_01_OFF\0'),'??_C@_0BP@MGJKKOKN@NEW_RED_FORCEFIELD_IDLE_02_OFF?$AA@':(0x12c3450,b'NEW_RED_FORCEFIELD_IDLE_02_OFF\0'),'??_C@_0BE@BPODFGED@Q_HobbeToothWhisper?$AA@':(0x12d6788,b'Q_HobbeToothWhisper\0')}
for row in rows:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len,default=''),re.M);out=[]
 for off,typ,sym in rel:
  n=int(off,16);native=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+n,4))[0]
  if sym in strings:expected,data=strings[sym];assert rd(expected,len(data))==data
  else:es=[v for k,v in M.items()if sym.startswith(k)];assert len(es)==1,(a,sym);expected=es[0]
  assert expected==native,(a,sym,hex(native));out.append(f'{off}\t{native:08x}\t{sym}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
# Parse actual root fixture COFF tables, not guessed object-section order.
vtcount=0
for a in ['00d408b0','00cdff50','00d9c2b0']:
 p=next((o/'dry_work').rglob(a+'.t.obj'));b=p.read_bytes();_,ns,_,syoff,nsy,opts,_=struct.unpack_from('<HHLLLHH',b);st=syoff+nsy*18
 def name(s):return b[st+struct.unpack_from('<L',s,4)[0]:].split(b'\0')[0].decode()if s[:4]==b'\0'*4 else s.split(b'\0')[0].decode()
 sy={};i=0
 while i<nsy:
  z=syoff+i*18;nm=name(b[z:z+8]);val,sec,typ,cl,aux=struct.unpack_from('<LhHBB',b,z+8);sy[i]=(nm,val,sec);i+=aux+1
 expected={'ScriptMethods':[],'GameMethods':[]};cls='ScriptMethods'
 for line in(w/'src'/(a+'.cpp')).read_text().splitlines():
  if line.startswith('struct GameMethods:'):cls='GameMethods'
  m=re.search(r'virtual ([^;]+); //slot(\d+) 0x([0-9a-fA-F]+)',line)
  if m:expected[cls].append((m[1],int(m[2]),int(m[3],16)))
 out={}
 for nm,val,sec in sy.values():
  if nm not in ['??_7ScriptMethods@@6B@','??_7GameMethods@@6B@']or sec<=0:continue
  z=20+opts+(sec-1)*40;size,raw,rel=struct.unpack_from('<LLL',b,z+16);nr=struct.unpack_from('<H',b,z+32)[0];entries=[]
  for j in range(nr):
   off,target,kind=struct.unpack_from('<LLH',b,rel+j*10)
   if off>=val:entries.append((off-val,sy[target][0]))
  c='GameMethods'if'GameMethods'in nm else'ScriptMethods';base=0x1260f0c if c=='GameMethods'else 0x1238c8c;assert len(entries)==len(expected[c])+1;assert entries[0][1].startswith(('??_G','??_E'));assert struct.unpack('<I',rd(base,4))[0]==(0x6e7d50 if c=='GameMethods'else 0x4ab170)
  for declaration,slot,target in expected[c]:
   assert struct.unpack('<I',rd(base+slot*4,4))[0]==target;off,sym=entries[slot];assert off==slot*4 and sym.startswith('?'+re.search(r'(\w+)\(',declaration)[1]+'@')
  out[c]=entries;vtcount+=len(entries)
 (o/(a+'_emitted_native_vtables.json')).write_text(json.dumps(out,indent=2))
slots=[(0x1260f0c,7,0x6e7410),(0x1260f0c,8,0x89b5b0),(0x1260f0c,376,0x88e4f0),(0x1260f0c,844,0x8a6dd0),(0x1260f0c,108,0x8910d0),(0x1260f0c,100,0x89f9e0),(0x12a66e4,12,0xbc61c0),(0x12a79b4,33,0xbcef80)]
for vt,slot,target in slots:
 actual=struct.unpack('<I',rd(vt+slot*4,4))[0];assert actual==target,(hex(vt),slot,hex(actual))
selected=[json.loads(l)for l in(w/'ATTEMPTS.jsonl').read_text().splitlines()if l.strip()];selected=[x for x in selected if x['address']in{z['address']for z in rows}];assert len(selected)==20,len(selected)
for g in selected:
 p=Path(g['snapshot']);m=json.loads((p/'before.json').read_text());assert m['command'] and m['base_flags'] and hashlib.sha256((p/'source.cpp').read_bytes()).hexdigest()==g['sha256']
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2));print('PASS',count,'relocs',vtcount,'emitted/nativeVTslots,8PMFslots,20grades,finalhashes')

from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry157';w=r/'astra238_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];rows=json.loads((o/'payload.json').read_text())['result']['authored'];count=0;vcount=0
M={'___security_cookie': 20564136, '?IsThingCarryingMeleeWeapon': 6952352, '?IsThingCarryingRangedWeapon': 6952480, '?FinishCurrentAction': 6694480, '?UnsheatheMeleeWeapon': 6703552, '?UnsheatheRangedWeapon': 6703632, '@__security_check_cookie@4': 12577273, '?CreatureUnsheathWeapon': 7257552, '?AddData@GameEventMethods': 10426208, '?g_MainGameComponent': 20678304, '?AddGameEvent': 4285596, '?LowerBound': 4255776, '??2@': 12577306, '??0EventStealing': 9091280, '??0EventPickingLock': 9091136, '??0EventPickpocketing': 9090992, '?AddEvent@EntityEventsMethods': 8459200, '?g_QuestManager': 20679164, '?SetQuestCardObjective@QuestManagerMethods': 4918576, '?IsQuestCardActive': 4912656, '?GetActiveQuestCardFromScriptName': 4918400, '?IsVignette': 7348960, '?GetMainPlayer': 4495728, '?GetHeroThing': 4750784, '?IsThingAlive': 4447184, '??_C@_09JHGFBHBK': 19097272, '??0CCharString': 10087408, '?PlaySoundEvent': 6733056, '??1CCharString': 10087136, '?AllowUIToOpen': 5679408, '?g_MovieTransitionState': 20678544, '?StartLetterBox@': 7238272, '??0MovieResource': 7239216, '?Reset@MovieResourceHandle': 9091440, '?g_MovieCameraID': 20450356, '?g_MovieState': 20688128, '?GetPosition': 5010384, '?PostDeed@HeroDeedLogMethods': 7104912, '?PostDeed@OpinionSourceMethods': 7803632, '??_C@_0BB@MHGLABCB': 19117596, '?AddTC': 5021024, '?Erase@ScriptVector': 9095632, '?g_HeroPerceptionManager': 20678544, '?GetPerceivingThings': 4421904, '__real@00000000': 19062492, '?GetScriptThing': 5013072, '?PushBack@ScriptVector': 9101200, '??1ScriptMethods': 4892736, '_free': 12577300}
for row in rows:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);blocks=[(b[i],b[i+1])for i in range(1,len(b),2)];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max([x for sec,x in blocks if sec=='.text'],key=len),re.M);out=[]
 for off,typ,sym in rel:
  n=int(off,16);native=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+n,4))[0];expected=[v for k,v in M.items()if sym.startswith(k)];assert len(expected)==1,(a,sym)
  if a=='008a8440'and sym.startswith('?LowerBound'):expected=[0x4365b0];assert '@@QBE' in sym
  assert native==expected[0],(a,sym,hex(native));out.append(f'{off}\t{native:08x}\t{sym}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
 src=(w/'src'/(a+'.cpp')).read_text();decl=re.findall(r'virtual ([^;]+); //slot(\d+) 0x([0-9a-fA-F]+)',src)
 if decl:
  vtobj=next((o/'dry_work').rglob(a+'.t.obj'));vtext=subprocess.check_output(['objdump','-r',str(vtobj)],text=True);vb=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',vtext);blocks=[(vb[i],vb[i+1])for i in range(1,len(vb),2)]
  assert len(decl)==78;vr=[re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',x,re.M)for sec,x in blocks if sec=='.rdata'and '??_EScriptMethods@@' in x and '?IsEqualTo@ScriptMethods' in x];assert len(vr)==1 and len(vr[0])==79;out=[]
  for declaration,slot,target in decl:
   slot=int(slot);target=int(target,16);assert struct.unpack('<I',rd(0x1238c8c+4*slot,4))[0]==target;off,typ,sym=vr[0][slot];assert int(off,16)==slot*4;method=re.search(r'(\w+)\(',declaration).group(1);assert sym.startswith('?'+method+'@');out.append(f'{slot}\t{target:08x}\t{sym}\t{declaration}')
  assert struct.unpack('<I',rd(0x1238c8c,4))[0]==0x4ab170;vcount+=79;(o/(a+'_virtual_audit.tsv')).write_text('slot\ttarget\tsymbol\tdeclaration\n'+'\n'.join(out)+'\n')
assert rd(0x12366b8,10)==b'LOG_ENTRY\0';assert rd(0x123b61c,17)==b'CTCOpinionSource\0';assert rd(0x122dedc,4)==bytes(4)
slots=[(0x1260f0c,7,0x6e7410),(0x1260f0c,396,0x89d690),(0x125ac0c,77,0xbfea08),(0x12457fc,77,0x52abe0)]
for vt,k,t in slots:assert struct.unpack('<I',rd(vt+4*k,4))[0]==t
assert rd(0x52abe0,3)==bytes.fromhex('b001c3');assert rd(0x6e7280,8)==bytes.fromhex('8b490ce918d9d4ff')
# Native event RTTI identifies the three constructors independently of propagated labels.
u=lambda a:struct.unpack('<I',rd(a,4))[0]
for vt,name in [(0x12781b0,'CEventHeroHasBeenSpottedPickpocketing'),(0x12781c8,'CEventHeroHasBeenSpottedPickingLock'),(0x12781e0,'CEventHeroHasBeenSpottedStealing')]:
 col=u(vt-4);td=u(col+12);assert name.encode()in rd(td+8,160).split(b'\0')[0]
import pefile
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe')
for thunk,iat,name in [(0xbfea1a,0x1440150,b'??2@YAPAXI@Z'),(0xbfea14,0x1440154,b'free')]:
 assert rd(thunk,6)==b'\xff\x25'+struct.pack('<I',iat);assert [(e.dll,i.name)for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports if i.address==iat]==[(b'MSVCR71.dll',name)]
(o/'native_auxiliary_evidence.txt').write_text('Game7/396,Creature/Player77,three eventRTTI identities,new/freeimports,LOG_ENTRY/CTCOpinionSource literals,floatzero,StartLetterBox forwarding are independently checked.\n')
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
grades=[json.loads(x)for x in(w/'ATTEMPTS.jsonl').read_text().splitlines()];selected=[g for g in grades if g['address']in{z['address']for z in rows}];assert len(selected)==sum(int(x.split("\t")[1])for x in(o/"attempts.tsv").read_text().splitlines()[1:]),len(selected)
for g in selected:assert g['prior']==0 and g['effective_flags'] and hashlib.sha256(Path(g['command'][5]).read_bytes()).hexdigest()==g['sha256']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2));print('PASS',count,'bodyrelocs',vcount,'ScriptThing slots,Game/Creature slots,event RTTI,literals/imports,immutable grades and hashes')

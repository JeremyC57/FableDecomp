from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry137';w=r/'retry200b_sol'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
def blocks(a):
 b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',(o/f'{a}_coff_relocs.txt').read_text());return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
common={'?GetPItem':0xa01b50,'?LowerBound':0x4365b0,'?QuestManager':0x13b89fc,'?HasEvent':0x8d4f80,'??0CCharString@@QAE@XZ':0x99e4b0,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??1CCharString':0x99eae0,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??4CCharString@@QAEABV0@PBD@Z':0x99efe0}
extra={'008d28d0':{},'008d3cf0':{},'00a63ee0':{'?ReadNextItem':0x9ba080,'?GetAsToken':0x9b9870,'?EmptyString':0x122d70e,'??_C@':0x122df24,'?Equals':0x4115a0,'?ReadAsStringUntilString':0x9ba110,'?GetAsString':0x9b9880},'00f00ae0':{'??_C@_0BC@':0x1270714,'??_C@_0CA@':0x12f7254,'?IsTerminating':0xf35b30,'??_7ScriptMethods':0x1238c8c,'??3@':0xbfe9bc,'??1CBaseClassNative':0x99a2e0},'00a63c00':{'??_C@_02NJPGOMH':0x129dad4,'??_C@_02DPKJAMEF':0x1265de8,'??_C@_04HCDDPBNL':0x122d6d4,'??_C@_05MAJJAKPI':0x122d6cc,'?Format':0x99f1f0,'??0NativeStringCopy':0x99ec30}}
for a,needle,count in [('008d28d0','?GetPItem',5),('008d3cf0','?HasEvent',6),('00a63ee0','?ReadNextItem',16),('00f00ae0','?IsTerminating',10),('00a63c00','?Format',17)]:
 targets=dict(common,**extra[a]);selected=max([b for sec,b in blocks(a)if sec=='.text'and needle in b],key=len);rel=relocs(selected);assert len(rel)==count;out=[]
 if a=='00a63c00':
  syms=subprocess.check_output(['objdump','-t',str(next((o/'dry_work').rglob(a+'.obj')))],text=True)
  for value,name in re.findall(r'0x([0-9a-f]{8}) (\$L\d+)$',syms,re.M):targets[name]=int(a,16)+int(value,16)
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native),hex(match[0]));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
s=(w/'src/00f00ae0.cpp').read_text();script=s[s.index('struct ScriptMethods:CBaseClassNative'):s.index('// Full first92')];game=s[s.index('struct GameMethods:CBaseClassNative'):];out=[]
for owner,vt,src,n in [('ScriptMethods',0x1238c8c,script,79),('GameMethods',0x1260f0c,game,92)]:
 methods=re.findall(r'virtual ([^;]+); //slot(\d+) 0x([0-9a-fA-F]+)',src);assert len(methods)==n-1,(owner,len(methods))
 if owner=='ScriptMethods':
  vb=[b for sec,b in blocks('00f00ae0')if sec=='.rdata'and '?IsEqualTo@ScriptMethods' in b];assert len(vb)==1;vr=relocs(vb[0]);assert len(vr)==79
 for declaration,slot,target in methods:
  slot=int(slot);target=int(target,16);assert struct.unpack('<I',rd(vt+slot*4,4))[0]==target
  if owner=='ScriptMethods':
   off,typ,sym=vr[slot];assert int(off,16)==4*slot;method=re.search(r'(\w+)\(',declaration).group(1);assert sym.startswith('?'+method+'@')
  out.append(f'{owner}\t{vt:08x}\t{slot}\t{target:08x}\t{declaration}')
assert struct.unpack('<I',rd(0x1238c8c,4))[0]==0x4ab170
assert struct.unpack('<I',rd(0x1260f0c,4))[0]==0x6e7d50
(o/'script_game_virtual_audit.tsv').write_text('owner\tvtable\tslot\ttarget\tdeclaration\n'+'\n'.join(out)+'\n')
for va,literal in [(0x1270714,b'OracleGamesmaster'),(0x12f7254,b'CREATURE_SS_ORACLE_GAMES_MASTER'),(0x129dad4,b'%f'),(0x1265de8,b'%d'),(0x122d6d4,b'TRUE'),(0x122d6cc,b'FALSE'),(0x122df24,b'"'),(0x122d70e,b'')]:assert rd(va,len(literal)+1)==literal+b'\0'
assert list(struct.unpack('<5I',rd(0xa63ca0,20)))==[0xa63c83,0xa63c9b,0xa63c14,0xa63c5b,0xa63c35]
assert struct.unpack('<I',rd(0x12723c0+4,4))[0]==0x811ee0
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
grades=[json.loads(l)for l in(w/'ATTEMPTS.jsonl').read_text().splitlines()];assert len(grades)==13
for g in grades:
 assert hashlib.sha256(Path(g['snapshot']).read_bytes()).hexdigest()==g['sha256'];assert g['reserved_uncertain']==4 and int(g['new_attempt'])<=6;assert g['effective_flags']
(o/'attempt_audit.txt').write_text('13 immutable source/command/flag grade records verified, including one compiler failure. 20 UNCERTAIN reservations preserved across five rows; 17 new allowances unused. Metadata-only correction retained separately; no root source grades.\n')
print('PASS 54 relocations including switch pointer and all five switch targets; 169 declared virtual methods plus both destructors; 79 emitted ScriptMethods entries; eight literals; frozen hashes and 13 grade snapshots.')

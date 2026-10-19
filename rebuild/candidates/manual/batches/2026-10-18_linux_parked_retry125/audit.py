from pathlib import Path
import struct,re,subprocess,runpy,sys,os,hashlib,json,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry125'
sys.argv=['x','008a6a20','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
common={'?LowerBound@ComponentMap':0x40f020,'??_7ScriptMethods':0x1238c8c,'??3@':0xbfe9bc,'??1CBaseClassNative':0x99a2e0,'??4CCharString':0x99efb0}
follow={**common,'?GetMainPlayer':0x449970,'?GetHeroThing':0x487dc0,'?IsFollower':0x6d4d70,'?RemoveFollower':0x6d5180,'?ForceAIReset':0x6d5ab0,'?RemoveIf':0x8ac890}
random={'??0CopyString':0x99ec30,'?GetAllWithScriptName':0x5ed860,'??1CCharString':0x99eae0,'??0CBaseClassNative':0x99a2d0,'??_7ScriptMethods':0x1238c8c,'?GetPRandomSeed':0x49d810,'?GFROR13':0x497890,'?GetScriptThing':0x4c7e50,'_free':0xbfea14}
ship={**common,'?GetAllWithTC':0x48d400,'?SetForceVisible':0x787740,'_free':0xbfea14}
for a,needle,targets,count in [('008a6a20','?LowerBound@',common,6),('008a77f0','?LowerBound@',common,5),('008a7980','?RemoveFollower@',follow,8),('008a7e00','?GetPRandomSeed@',random,9),('008a7f60','?SetForceVisible@',ship,4)]:
 txt=(o/f'{a}_coff_relocs.txt').read_text();blocks=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',txt)
 selected=max([blocks[i+1]for i in range(1,len(blocks),2)if blocks[i]=='.text'and needle in blocks[i+1]],key=len)
 rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',selected,re.M);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n');print(a,len(rel),'verified')
 for vb in [blocks[i+1]for i in range(1,len(blocks),2)if blocks[i]=='.rdata'and '?GetPThing@ScriptMethods' in blocks[i+1]]:
  vr=re.findall(r'^([0-9a-f]{8})\s+dir32\s+(\S+)',vb,re.M);assert len(vr)==79
  expected=list(csv.DictReader((r/'root_native_review/scriptthing79_audit.tsv').open(),delimiter='\t'));values=struct.unpack('<79I',rd(0x1238c8c,316));assert values[0]==0x4ab170
  for item,(off,symbol)in zip(expected,vr[1:]):
   i=int(item['slot']);assert int(off,16)==i*4 and symbol.startswith('?'+item['method']+'@')and values[i]==int(item['actual_native_address'],16),(item,symbol)
rows=[json.loads(x)for x in(r/'retry199f_sol/ATTEMPTS.jsonl').read_text().splitlines()];assert len(rows)==20
for x in rows:
 cmd=x['command'];p=Path(cmd[5]);assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert cmd[4]==x['address'];assert cmd[:3]==['timeout','200','python3'];assert x['uncertain_reserved']==4
(o/'attempt_audit.txt').write_text('20 immutable Sol source snapshots and full pregrade commands verified; 4 UNCERTAIN reservations per row retained. Counts2/2/5/5/6. Explicit-leaf same-source verifications separately recorded. No root source grades.\n')
print('32 body relocations, emitted ScriptThing79 vtables and20 pregrade hashes/commands verified')

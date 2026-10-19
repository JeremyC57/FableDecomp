from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry130'
sys.argv=['x','008a9020','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
common={'?LowerBound':0x40f020,'_free':0xbfea14}
family={**common,'?GetDefGlobalIndex':0x9ad410,'?PushBack':0x7ca160,'??4IndexVector':0x4e24ba}
filter={**common,'??0BaseIntelligentPointer':0xa01b10,'??_7IntelligentPointer':0x1238c6c,'?SetPItem':0xa01b90,'?PushBack':0x6a3c70,'??1BaseIntelligentPointer':0xa01c10,'?SetBypassScreenFilter':0x6b83e0,'?SetAllCarriedThingsAsBypassFilter':0x6a2520}
sort={'??MSymbolString':0x9e6530,'?MoveBackward':0x9e6410,'??4SymbolString':0x99efb0,'??0SymbolString':0x99ec30,'?UnguardedInsert':0x9e66f0,'??1CCharString':0x99eae0}
for a,needle,targets,count in [('008a9020','?GetDefGlobalIndex',family,5),('008a99f0','?SetBypassScreenFilter',filter,10),('009e6fa0','?MoveBackward',sort,12)]:
 txt=(o/f'{a}_coff_relocs.txt').read_text();blocks=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',txt);selected=max([blocks[i+1]for i in range(1,len(blocks),2)if blocks[i]=='.text'and needle in blocks[i+1]],key=len)
 rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',selected,re.M);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n');print(a,len(rel),'verified')
 if a!='009e6fa0':
  s=(r/'retry199g_sol/src'/f'{a}.cpp').read_text();body=s[s.index('struct ScriptMethods:CBaseClassNative{'):s.index('FABLE_STATIC_ASSERT(sizeof(ScriptMethods)')];decl=re.findall(r'virtual\s+([^;{]+)[;{]',body);assert len(decl)==79,len(decl)
  expected=list(csv.DictReader((r/'root_native_review/scriptthing79_audit.tsv').open(),delimiter='\t'));values=struct.unpack('<79I',rd(0x1238c8c,316));assert values[0]==0x4ab170
  for item,x in zip(expected,decl[1:]):
   i=int(item['slot']);assert re.search(r'(\w+)\(',x).group(1)==item['method']and values[i]==int(item['actual_native_address'],16),(item,x)
assert struct.unpack('<I',rd(0x1238c6c,4))[0]==0x4a9710
rows=[json.loads(x)for x in(r/'retry199g_sol/ATTEMPTS.jsonl').read_text().splitlines()];assert len(rows)==19
for x in rows:
 cmd=x['command'];p=Path(cmd[5]);assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert cmd[4]==x['address'];assert cmd[:3]==['timeout','200','python3'];assert x['uncertain_reserved']==4
(o/'attempt_audit.txt').write_text('19 immutable source/hash/pregrade commands verified, counts9E6FA0=3/8A9020=2/8A99F0=4/8A8010=6/8D0610=4, plus4UNCERTAINeach. Two unused8D0610newgrades preserved. Firsttwo effectiveflags metadata corrected separately inFLAGS_CORRECTION.md; actualunchangedtoolBASE_FLAGS/O2/Oy/W3, fullcommands/hashesaccurate. No root sourcegrades.\n')
print('27 body relocations,2 full79-slot ScriptThing declarations,typedpointer destructor slot and19grade hashes/commands verified')

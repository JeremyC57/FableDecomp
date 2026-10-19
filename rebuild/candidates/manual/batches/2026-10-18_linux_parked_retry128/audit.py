from pathlib import Path
import struct,re,runpy,sys,os,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry128'
sys.argv=['x','0087ed70','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
common={'?GetPItem':0xa01b50,'?LowerBound':0x4365b0}
stab={**common,'?GetPositionToMoveTo':0x746ec0,'?GetPosition@':0x4c73d0}
finish={**common,'?FinishAction':0x859a20,'?GetCurrentMode':0x73a6e0,'??0CCreatureAction_BeStunned':0x865940,'?SetCurrentAction':0x6644f0,'??1CCreatureActionBase':0x693ef0,'?GetDazedHealth':0x662090,'??0CCreatureAction_AlertToPlayer':0x865a00}
bank={'?GetEntryDataType':0x9cc210,'?FlushAsync':0x9d68b0,'?IsFinished':0x9d54e0,'?QueueGroupLoad':0x9c8220,'?BeginLoading':0x9c80f0,'?BuildGroup':0x9c8700,'?MakeFreeSpace':0x9fc4f0,'?ListRemove':0x9fbfb0,'?ListInsert':0x9fbf90}
for a,needle,targets,count in [('0087ed70','?GetPositionToMoveTo',stab,9),('008663d0','?GetDazedHealth',finish,18),('009c8890','?FlushAsync',bank,11)]:
 txt=(o/f'{a}_coff_relocs.txt').read_text();blocks=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',txt);selected=max([blocks[i+1]for i in range(1,len(blocks),2)if blocks[i]=='.text'and needle in blocks[i+1]],key=len)
 rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',selected,re.M);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n');print(a,len(rel),'verified')
assert struct.unpack('<I',rd(0x1265f84+66*4,4))[0]==0x723760
rows=list(csv.DictReader((r/'luna234_whole/probe/009c8890_vtable_audit.tsv').open(),delimiter='\t'));assert len(rows)==10
s=(r/'luna234_whole/src/009c8890.cpp').read_text();body=s[s.index('struct CResource {'):s.index('struct CResourceList {')];decl=re.findall(r'virtual\s+([^;]+);',body);assert len(decl)==10
for i,row in enumerate(rows):
 for name,va in [('CResource12354A4',0x12354a4),('CGroupEntry129B478',0x129b478)]:assert struct.unpack('<I',rd(va+4*i,4))[0]==int(row[name],16)
 if i:assert re.search(r'(\w+)\(',decl[i]).group(1)==row['method'].split()[0]
(o/'resource_vtable10.tsv').write_text((r/'luna234_whole/probe/009c8890_vtable_audit.tsv').read_text())
print('38 body relocations,physics setter slot66 and20 resource/group vtable entries verified')

from pathlib import Path
import struct,re,runpy,sys,os,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry129'
sys.argv=['x','009c80f0','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
targets={'?GetAsyncEntry':0x9d58d0,'?gDataBankReadPriority':0x1396314,'?GetEntryDataSize':0x9cc240,'?BeginReadDataAutoAlloc':0x9d6100,'??2@':0xbfea1a,'??0CGroupEntry':0x9c8060,'??0CTextEntry':0x9c8250,'?AddResource':0x9fc380,'?MakeFreeSpace':0x9fc4f0,'??3@':0xbfe9bc,'?ListRemove':0x9fbfb0,'?ListInsert':0x9fbf90}
for a in ['009c80f0','009c8340']:
 txt=(o/f'{a}_coff_relocs.txt').read_text();blocks=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',txt);selected=max([blocks[i+1]for i in range(1,len(blocks),2)if blocks[i]=='.text'and '?BeginReadDataAutoAlloc' in blocks[i+1]],key=len)
 rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',selected,re.M);assert len(rel)==12;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n');print(a,len(rel),'verified')
rows=list(csv.DictReader((r/'coordinator_retry128/resource_vtable10.tsv').open(),delimiter='\t'));assert len(rows)==10
for a in ['009c80f0','009c8340']:
 s=(r/'luna234_whole/src'/f'{a}.cpp').read_text();body=s[s.index('struct CResource {'):s.index('struct CResourceList {')];decl=re.findall(r'virtual\s+([^;]+);',body);assert len(decl)==10
 for i,row in enumerate(rows):
  for name,va in [('CResource12354A4',0x12354a4),('CGroupEntry129B478',0x129b478)]:assert struct.unpack('<I',rd(va+4*i,4))[0]==int(row[name],16)
  if i:assert re.search(r'(\w+)\(',decl[i]).group(1)==row['method'].split()[0]
 assert re.search(r'void\s*\(\s*(?:__fastcall|FABLE_FASTCALL)\s*\*\s*Destroy\)\s*\(void\*\)',s)
assert rd(0x9c8139,2)==bytes.fromhex('6a34') and rd(0x9c8389,2)==bytes.fromhex('6a44')
print('24 body relocations,real resource virtual declarations and native52/68-byte allocation sizes verified')

from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry175';w=r/'astra237_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'??0CTCBase':0x686800,'??_7CTCHeroOnlineScoreboard':0x124db34,'_malloc':0xbfea0e,'??0CCharString':0x99e4b0,'??0CWideString':0x99aed0,'?gScoreHost':0x13b8af0,'?gScorePath':0x13b8af4,'??4CCharString':0x99efe0,'?gScoreConfigured':0x13b8ae0,'?AddToGlobalUpdateTCs':0x686ed0,'?InitialiseActivityInfo':0x562710,'__ehhandler$':0x12170d6,'__except_list':0,'??0CBasePin':0xcac070,'??_7CDynamicOutputPin@@6BCUnknown@@@':0x12bf7a8,'??_7CDynamicOutputPin@@6BIPin@@@':0x12bf760,'??_7CDynamicOutputPin@@6BIQualityControl@@@':0x12bf748,'??_7CDynamicOutputPin@@6B@':0x12bf738,'__imp__InitializeCriticalSection@4':0x143fee4,'__imp__CreateEventW@16':0x143fdc8,'?AmGetLastErrorToHResult':0xcb0bc0,'?gWaterRenderer':0x1436e54,'?TesselatePatch@CWaterTessellator':0xbe3910,'$L806':0xbfd388,'$L808':0xbfd37c,'$L778':0xbfd303,'$L780':0xbfd311,'$L775':0xbfd324}
strings={0x124db14:'fableclangw.lionhead.com',0x124db08:'/index.php'}
for a,s in strings.items():assert rd(a,len(s)+1)==s.encode()+b'\0'
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32' else u(int(a,16)+k)
  assert v==int(target,16)
  if sym.startswith('??_C@'):assert v in strings
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==39
assert rd(0xbfd388,8)==bytes.fromhex('0000010101000200')
assert [u(0xbfd37c+i*4)for i in range(3)]==[0xbfd303,0xbfd311,0xbfd324]
assert rd(0x12170d6,5)==bytes.fromhex('b84c183701')
assert u(0x137184c)==0x19930520 and u(0x1371850)==2
unwind=u(0x1371854);assert [u(unwind+i*8+4)for i in range(2)]==[0x12170c0,0x12170c8]
assert rd(0x12170c8,9)==bytes.fromhex('8b4df081c1a4000000')
helpers=runpy.run_path(str(o/'coff_helpers.py'));slots=[];tables={}
for line in (w/'logs/00562ec0_native_vt_rtti_literals.txt').read_text().splitlines():
 m=re.match(r'VT124DB34 slot(\d+) ([0-9A-F]+)',line)
 if m:tables.setdefault(0x124db34,[]).append(int(m[2],16))
for line in (w/'logs/00cac300_native_imports_vtables.txt').read_text().splitlines():
 if line.startswith('VT'):vt=int(line[2:],16);tables[vt]=[]
 elif re.match(r'^\d+ [0-9A-F]{8}$',line):tables[vt].append(int(line.split()[1],16))
required={0x124db34:28,0x12bf738:4,0x12bf748:5,0x12bf760:18,0x12bf7a8:26}
vtmap={'??_7CTCHeroOnlineScoreboard@@6B@':0x124db34,'??_7CDynamicOutputPin@@6B@':0x12bf738,'??_7CDynamicOutputPin@@6BIQualityControl@@@':0x12bf748,'??_7CDynamicOutputPin@@6BIPin@@@':0x12bf760,'??_7CDynamicOutputPin@@6BCUnknown@@@':0x12bf7a8}
for a in ['00562ec0','00cac300']:
 syms,secs=helpers['coff'](next((o/'dry_work').rglob(a+'.obj')))
 for name,value,sec in syms.values():
  if name not in vtmap:continue
  vt=vtmap[name];size,rels=secs[sec-1];ss=[]
  for off in range(value,size,4):
   if off not in rels:break
   ss.append(rels[off])
  assert len(ss)==required[vt]==len(tables[vt])
  for i,s in enumerate(ss):
   assert u(vt+i*4)==tables[vt][i]
   if tables[vt][i]==0xbfea08:assert s=='__purecall'
   slots.append({'vtable':hex(vt),'slot':i,'target':hex(tables[vt][i]),'symbol':s,'signature':helpers['und'](s)})
assert len(slots)==81
col=u(0x124db30);chd=u(col+16);assert u(chd+8)==4
names=[]
for i in range(4):
 bc=u(u(chd+12)+4*i);td=u(bc);names.append(rd(td+8,100).split(b'\0')[0].decode())
assert names==['.?AVCTCHeroOnlineScoreboard@@','.?AVCTCBase@@','.?AVCBaseClassNonCopyable@@','.?AVCBaseClass@@']
(o/'native_hierarchy_audit.json').write_text(json.dumps({'scoreboard_hierarchy':names,'slots':slots,'DirectShow_reference':'Microsoft public amfilter/combase/wxutil classes; CBaseObject SDK identity is unrelated to Fable engine/CBaseObject.h. Full 53 slots manually signature-reviewed against SDK; native MI offsets C/10/A0 validated by fixture.','EH':'2 cleanup entries: base pin dtor CA85E0; CCritSec dtor CA4870 at offset A4. Frame handler BFEB34. Native fixture covers3 real Win32 outcomes; host5 includes injected C++ exceptions.'},indent=2))
records=[];status=list(csv.DictReader((w/'STATUS.tsv').open(),delimiter='\t'));assert len(status)==40
for row in status:
 a=row['address'];n=int(row['grades']);assert n<=10
 for i in range(1,n+1):
  d=w/'snapshots'/f'{a}_{i:02d}';m=json.loads((d/'manifest.json').read_text());assert hashlib.sha256((d/'source.cpp').read_bytes()).hexdigest()==m['sha256'];assert m['command'] and (m.get('compiler_flags') or m.get('compiler_command')) and (d/'output.txt').exists();records.append({'address':a,'grade':i,'snapshot':str(d),'metadata':m})
assert len(records)==179,len(records)
(o/'attempt_audit.json').write_text(json.dumps({'worker_grades':records,'root_extra_grades':[{'address':'00cac300','grade':4,'flags':['/','G','X'],'note':'Unintended flag serialization; initial nonmatch.'},{'address':'00cac300','grade':5,'flags':['/','G','X','/EHsc'],'note':'Automatic alternate matched; not landed. Logging limitation disclosed in FLAG_SERIALIZATION_INCIDENT.md.'}],'total':181},indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS: 39 masked operands, full literals, embedded switch tables, 81 native/COFF slots, 4-class RTTI, EH cleanup map, 179 worker grades plus 2 disclosed root flag attempts.')

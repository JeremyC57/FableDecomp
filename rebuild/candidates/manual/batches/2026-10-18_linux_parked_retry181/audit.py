from pathlib import Path
import struct,runpy,sys,json,hashlib,re,pefile
r=Path('work/codex-lane/w22');o=r/'coordinator_retry181';w=r/'astra239_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0];h=runpy.run_path(str(o/'coff_helpers.py'))
M={'??0CCharString@@QAE@XZ':0x99e4b0,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??1CCharString':0x99eae0,'?ReadNextItem':0x9ba080,'?GetSymbol':0x9b9880,'?EmptyString':0x129aaf4,'__stricmp':0xbfeba8,'?ReadFromParserAsFloat':0xa63d20,'?ReadFromParserAsString':0xa63ee0,'?ReadFromParserAsBool':0xa63db0,'?ReadFromParserAsSLONG':0xa63cc0,'?Initialise@PixelFormat@@QAEXJJJJ_N@Z':0x9e3790,'?Initialise@PixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830}
strings={0x122d6d4:'TRUE',0x122d6cc:'FALSE'}
for a,s in strings.items():assert rd(a,len(s)+1)==s.encode()+b'\0'
assert rd(0x129aaf4,1)==b'\0'
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 sy,secs=h['coff'](next((o/'dry_work').rglob(a+'.obj')));out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert v==int(target,16)
  if sym.startswith('??_C@'):assert v in strings and strings[v]in sym
  elif sym.startswith('$L'):
   local=[val for name,val,sec in sy.values()if name==sym];assert local==[v-int(a,16)]
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==62
assert [u(0xa63fe8+i*4)for i in range(5)]==[0xa63fde,0xa63fe3,0xa63fcf,0xa63fd9,0xa63fd4]
assert rd(0x9b9880,4)==bytes.fromhex('8d4108c3')
assert rd(0xbfeba8,6)==bytes.fromhex('ff25ec004401')
pe=pefile.PE('work/codex-lane/dl/game/Fable The Lost Chapters/Fable.exe');imports={x.address:(d.dll,x.name)for d in pe.DIRECTORY_ENTRY_IMPORT for x in d.imports};assert imports[0x14400ec]==(b'MSVCR71.dll',b'_stricmp')
assert rd(0x9fcf05,5)==bytes.fromhex('b918000000')and rd(0x9fcf0c,2)==bytes.fromhex('f3ab')
pdb=Path('ghidra_out/fablewin_pdb_names.tsv').read_text()
for sig in ['??0CParsedItem@@QAE@XZ','?GetSymbol@CParsedItem@@QBEABVCCharString@@XZ','?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z','?GetDefault@CGraphicBankStateBlock@@UAEXAAVCBankStateBlock@@@Z','?Initialise@CPixelFormat@@QAEXJJJJ_N@Z','?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z']:assert sig in pdb
records=[];counts={'00a64230':3,'00a63db0':3,'00a63fc0':2,'009fcef0':3}
for a,n in counts.items():
 ps=sorted((w/'snapshots').glob(a+'_[0-9][0-9]/grade.json'));assert len(ps)==n
 for p in ps:
  x=json.loads(p.read_text());assert hashlib.sha256((p.parent/'source.cpp').read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['effective_flags']==['/O2','/Oy','/W3'];assert(p.parent/'grade.log').exists();x['snapshot_path']=str(p.parent);records.append(x)
correction=json.loads((w/'snapshots/00a63fc0_COUNT_CORRECTION.json').read_text());assert correction['source_grades']==1 and correction['verification_count']==1
assert (w/'snapshots/00a63fc0_01/source.cpp').read_bytes()==(w/'snapshots/00a63fc0_02/source.cpp').read_bytes()
(o/'attempt_audit.json').write_text(json.dumps({'invocations':records,'counted_grades':10,'unchanged_verification':correction},indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS62relocations/TRUE+FALSE+emptystrings/5entryjumptable/localCOFFlabels/CRTimport/PDBsignatures/10grades+1same-sourceverification')

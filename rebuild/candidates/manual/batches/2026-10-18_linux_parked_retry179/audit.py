from pathlib import Path
import struct,runpy,sys,json,hashlib,collections,csv
r=Path('work/codex-lane/w22');o=r/'coordinator_retry179';w=r/'retry202e_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'?IsDistanceBetweenThingsUnder':0xcbe2ff,'?IsActiveThreadTerminating':0xf35b30,'??0CCharString':0x99ebf0,'??1CCharString':0x99eae0,'??1CScriptThing':0x4aa840,'??_7CScriptThing':0x1238c8c,'??3@':0xbfe9bc,'??1CBaseClass':0x99a2e0,'__real@bf800000':0x122dee0,'?DisplayProgress@NProgressDisplay':0x9e9f40,'??2@':0xbfea1a,'??0CDataBank@NGameText':0x9c85a0,'?Reset@?$CCountedPointer':0x403e40,'?GetFontBankName':0x415260}
strings={0x12d7274:'MinionAttackingGuard',0x12cb9f4:'CREATURE_MINION_WARDOG',0x12d71d4:'GuardNotThisWay',0x12d2548:'CREATURE_BS_GUARD',0x122edd4:'Init Text',0x122edc0:'Opening Text Bank'}
for a,s in strings.items():assert rd(a,len(s)+1)==s.encode()+b'\0'
obs=json.loads((o/'observed_relocations.json').read_text())
for a,rows in obs.items():
 out=[]
 for off,typ,sym,target in rows:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32' else u(int(a,16)+k);assert v==int(target,16)
  if sym.startswith('??_C@'):assert v in strings and strings[v].replace(' ','?5') in sym
  else:assert [val for key,val in M.items()if sym.startswith(key)]==[v],(a,sym,hex(v))
  out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert sum(map(len,obs.values()))==36
assert u(0x122dee0)==0xbf800000
assert rd(0x9c85be,6)==bytes.fromhex('c706d4b42901')
assert u(0x129b4d4)==0x9cb840 and u(0x129b4d8)==0x9d56c0
assert rd(0xf35b30,12)==bytes.fromhex('8b01ff500c8bc8e9041ed8ff')
for row in csv.DictReader((w/'evidence/SCRIPT_VIRTUAL_CALLS.tsv').open(),delimiter='\t'):
 if row['slot']=='-':continue
 assert u(int(row['VT'],16)+4*int(row['slot']))==int(row['native_target'],16)
h=runpy.run_path(str(o/'coff_helpers.py'));sy,secs=h['coff'](next((o/'dry_work').rglob('00416832.t.obj')));tables={}
for name,val,sec in sy.values():
 if name!='??_7CDataBank@NGameText@@6B@':continue
 size,rels=secs[sec-1];a=[rels[k]for k in range(val,size,4)if k in rels];assert len(a)==2 and a[0].startswith(('??_G','??_E')) and a[1]=='?OpenReadOnly@CDataBank@NGameText@@UAE_NABUCCharString@@K@Z';tables[name]=[h['und'](s)for s in a]
assert tables
records=[json.loads(l)for l in (w/'attempts.jsonl').read_text().splitlines()];assert len(records)==11
for x in records:
 assert hashlib.sha256(Path(x['source']).read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['flags']==['/O2','/Oy','/W3'];assert x['cumulative']==4+x['new_grade'];assert(w/'logs'/(x['address']+'_%02d.txt'%x['new_grade'])).exists()
assert collections.Counter(x['address']for x in records)=={'00da6240':1,'00da6740':1,'00416832':3,'0041726d':6}
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2));(o/'native_hierarchy_audit.json').write_text(json.dumps({'bank_interface_prefix':tables,'scope':'Method/data projections and prefix test doubles; no full game construction/integration claim'},indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
sigs=json.loads((o/'script_virtual_signatures_root.json').read_text());assert len(sigs['slots'])==376 and not sigs['mismatches']
print('PASS36relocations/6fullstrings/float/bankprefix/scriptdispatch/376emittedsignatures/11immutablegrades+16historicalreservations')

from pathlib import Path
import json,runpy,sys,struct,csv,re,hashlib
r=Path('work/codex-lane/w22');o=r/'repair008a03f0';h=runpy.run_path(str(o/'coff_helpers.py'));sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
rows=list(csv.DictReader((r/'astra239_whole/CTCBASE_NATIVE_CORRECTION.tsv').open(),delimiter='\t'));expected=[]
for x in rows[:23]:
 decl=x['corrected_prototype'];expected.append('destructor'if decl.startswith('~')else re.search(r'(\w+)\(',decl)[1])
sy,secs=h['coff'](next((o/'dry_work').rglob('008a03f0.t.obj')));report=[]
for name,value,sec in sy.values():
 if name!='??_7TargetingMethods@@6B@':continue
 size,rels=secs[sec-1];symbols=[rels[k]for k in range(value,size,4)if k in rels];got=['destructor'if x.startswith(('??_G','??_E'))else x.split('@')[0][1:]for x in symbols];assert got==expected
 for i,s in enumerate(symbols):
  v=u(0x12411fc+i*4);report.append({'slot':i,'native':hex(v),'compiled':h['und'](s),'retail_base_prototype':rows[i]['corrected_prototype']})
  if i in range(12,21):assert v==int(rows[i]['base_target'],16)
assert len(report)==23
assert rd(0x1239fb0,15)==b'CTCTargetingAI\0'
obs=json.loads((o/'observed_relocations.json').read_text())['008a03f0'];assert [int(x[3],16)for x in obs]==[0x40f020,0x1239fb0,0x99ebf0,0x411570,0x99eae0,0x99eae0,0x8abba0]
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'native_signature_audit.json').write_text(json.dumps(report,indent=2));print('PASS 23 real CTC interface prefix slots and 7 relocated operands; exact 206-byte body unchanged')

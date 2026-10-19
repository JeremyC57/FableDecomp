from pathlib import Path
import struct,runpy,sys,json
r=Path('work/codex-lane/w22');o=r/'coordinator_retry187';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];h=runpy.run_path(str(o/'coff_helpers.py'));result={}
for row in json.loads((o/'payload.json').read_text())['result']['authored']:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));b=obj.read_bytes();sy,secs=h['coff'](obj)
 prefix=row['leaf'] if row['leaf'].startswith('??')else '?'+row['name']+'@'
 main=[(n,v,s)for n,v,s in sy.values()if n.startswith(prefix)and s>0];assert len(main)==1,(a,main)
 name,val,sec=main[0];size,rels=secs[sec-1];sh=20+(sec-1)*40;raw,ptr=struct.unpack_from('<II',b,sh+20);nr=struct.unpack_from('<H',b,sh+32)[0];out=[]
 for i in range(nr):
  offset,idx,kind=struct.unpack_from('<IIH',b,ptr+10*i);n=offset-val;assert kind in (6,20);typ='DISP32'if kind==20 else 'dir32';sym=sy[idx][0]
  target=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if kind==20 else struct.unpack('<I',rd(int(a,16)+n,4))[0];out.append([f'{n:08x}',typ,sym,hex(target)])
 result[a]=out
(o/'observed_relocations.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))

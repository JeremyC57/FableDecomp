from pathlib import Path
import struct,re,runpy,sys,json,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry162';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];result={}
for row in json.loads((o/'payload.json').read_text())['result']['authored']:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);(o/(a+'_coff_relocs.txt')).write_text(s);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len,default=""),re.M);out=[]
 for off,typ,sym in rel:
  n=int(off,16);target=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+n,4))[0];out.append([off,typ,sym,hex(target)])
 result[a]=out
(o/'observed_relocations.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))

from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry162';w=r/'astra235_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd']
M={'?PeekGuiDef':0x438570,'_g_FloatZero':0x122dedc,'_g_AlphaScale255':0x1230014,'__ftol2':0xbfea70,'?g_CoordinateWidth':0x13961e8,'__real@3fd99999a0000000':0x125a288,'?g_CoordinateHeight':0x13961f0,'__real@3fea8f5c20000000':0x125a280,'??0CWideString':0x99aed0,'?GetPlayer@':0x4498c0,'?GetSpiritScoreText':0x44a2a0,'?GetMultiplayerColour':0x449b60,'__real@437a0000':0x125a278,'?Draw2DText':0x438010,'__real@3fbeb851e0000000':0x125a270,'??1CWideString':0x99b510};count=0
for a,n in [('006448a4',5),('00644424',14)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+k,4))[0];assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v],(a,sym,hex(v));out.append(f'{off}\t{v:08x}\t{sym}')
 count+=n;(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
for a,h in [(0x122dedc,'00000000'),(0x1230014,'437f0000'),(0x125a278,'437a0000')]:assert rd(a,4)==struct.pack('<I',int(h,16))
for a,h in [(0x125a288,'3fd99999a0000000'),(0x125a280,'3fea8f5c20000000'),(0x125a270,'3fbeb851e0000000')]:assert rd(a,8)==struct.pack('<Q',int(h,16))
assert struct.unpack('<I',rd(0x125a738,4))[0]==0x646762
assert rd(0x438010,11)==bytes.fromhex('8b8938020000e9bf362100')
assert rd(0x449b70,22)==bytes.fromhex('8b442404b1ff32d28850018810884802884803c20800')
(o/'native_auxiliary_evidence.txt').write_text('Native6literalpools checked including widenedfloat double precision; StatBarVT125A738slot0=646762. Draw2DText438010 forwards owner238 to64B6DA. Colour449B60 hiddenoutput stack4/index8/EAXoutput/ret8, native BGRA first case red=FF. Genuine seven-argument StatBar PDBsignature fablewin0210037F; playergetter4498C0 returnsreference, colourconst andtextconst signatures retained.\n')
grades=[json.loads(x)for x in(w/'ATTEMPTS.jsonl').read_text().splitlines()];selected=[g for g in grades if g['address']in ['006448a4','00644424']];assert len(selected)==9
for g in selected:assert g['base_flags'] and hashlib.sha256((Path(g['snapshot'])/'source.cpp').read_bytes()).hexdigest()==g['sha256'] and g['command']
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2));print('PASS19relocations,6pools,StatBarVT,colourhiddenresult,Draw2DTextthunk,9immutablegrades/finalhashes')

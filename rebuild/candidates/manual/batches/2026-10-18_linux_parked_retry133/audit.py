from pathlib import Path
import struct,re,runpy,sys,os,csv,json,hashlib
r=Path('work/codex-lane/w22');o=r/'coordinator_retry133'
sys.argv=['x','00d2c530','0'];rd=runpy.run_path(str(Path(os.environ['S'])/'tools/xdis.py'))['rd']
camera={'?GFABoxToVBox':0x9e2020,'?g_RenderManagerCore':0x1436e18,'?g_ShaderRenderManager':0x1436e14,'?SetViewportFloat':0xa0b560,'?SetViewTransform@CRenderManagerCore':0xa0b0a0,'?SetPerspectiveTransform':0xa0b320,'?SetViewTransform@CShader':0x988350,'?SetProjectionTransform':0x988540,'?SetFogColour':0x988320,'?SetWorldTransform@CRenderManagerCore':0xa0b140,'?SetWorldTransform@CShader':0x9881f0}
component={'?ConvertVector':0x52e580}
def blocks(a):
 t=(o/f'{a}_coff_relocs.txt').read_text();b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',t)
 return [(b[i],b[i+1])for i in range(1,len(b),2)]
def relocs(b):return re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b,re.M)
for a,needle,targets,count in [('0052eec0','?ConvertVector',component,1),('00b2fc50','?GFABoxToVBox',camera,17)]:
 selected=max([b for section,b in blocks(a)if section=='.text'and needle in b],key=len);rel=relocs(selected);assert len(rel)==count;out=[]
 for offset,typ,name in rel:
  off=int(offset,16);match=[v for k,v in targets.items()if name.startswith(k)];assert len(match)==1,(a,name)
  v=struct.unpack('<i'if typ=='DISP32'else'<I',rd(int(a,16)+off,4))[0];native=v+int(a,16)+off+4 if typ=='DISP32'else v
  assert native==match[0],(a,name,hex(native));out.append(f'{off:04x}\t{typ}\t{native:08x}\t{name}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\ttype\tretail_target\tcompiler_symbol\n'+'\n'.join(out)+'\n')
assert not any(relocs(b)for sec,b in blocks('00bc5ff0')if sec=='.text')
virtuals=[(0x124608c,102,0x52f1a0,'IsPositionIndependent'),(0x124608c,117,0x52f3b0,'IsUsingRelativePosition'),(0x124608c,122,0x52eec0,'GetTargetRelativeRenderPosition'),(0x124608c,123,0x52ecc0,'GetTargetRelativeRenderZoom'),(0x12a66e4,17,0xbc5ff0,'GetBoundingBoxWorldSpace')]
for va,slot,target,name in virtuals:assert struct.unpack('<I',rd(va+4*slot,4))[0]==target
(o/'virtual_audit.tsv').write_text('vtable\tslot\tnative_target\tmethod\n'+''.join(f'{v:08x}\t{s}\t{t:08x}\t{n}\n'for v,s,t,n in virtuals))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for kind in ['src','test']:assert hashlib.sha256(Path(row[kind]['path']).read_bytes()).hexdigest()==row[kind]['sha256']
print('PASS18 body relocations,zero groupbodyrelocations,5 realvirtualslots,frozenhashes')

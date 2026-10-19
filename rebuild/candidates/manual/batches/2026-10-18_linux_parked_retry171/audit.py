from pathlib import Path
import struct,re,runpy,sys,json,hashlib,csv,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry171';w=r/'retry202b_sol';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];u=lambda a:struct.unpack('<I',rd(a,4))[0]
M={'__real@00000000':0x122dedc,'?GetEdgeStrip@Patch@Native':0xbe5c60,'?GetPatch@Tree@Native':0xbf6130,'?GetSplitBranch@Tree@Native':0xbf6160,'?IsBackgroundAtFullDetailInArea@Tree@Native':0xbf6d50,'?gOcclusionManager@Native':0x1436e5c,'?RenderOcclusionBox@OcclusionManager@Native':0xb8d420}
for a,n in [('00bf6640',2),('00bf6930',1),('00bf6d50',3),('00bf7100',2)]:
 obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);bs=[b[i+1]for i in range(1,len(b),2)if b[i]=='.text'];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max(bs,key=len),re.M);assert len(rel)==n;out=[]
 for off,typ,sym in rel:
  k=int(off,16);v=struct.unpack('<i',rd(int(a,16)+k,4))[0]+int(a,16)+k+4 if typ=='DISP32'else u(int(a,16)+k);assert [v0 for k0,v0 in M.items()if sym.startswith(k0)]==[v];out.append(f'{off}\t{v:08x}\t{sym}')
 (o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
assert u(0x122dedc)==0
vtables={0x129b860:[0x9d5110,0xbfea08,0x9d50f0,0x9d5100],0x12a8014:[0xbe8d90,0xbfea08,0x9d50f0,0x9d5100,0xbfea08,0xbe8d50,0xbe8d60,0xbe8d70,0xbe8d80],0x12a803c:[0xbe8e60,0xbe65a0,0xbe5e50,0x9d5100,0xbe8dc0,0xbe61b0,0xbe63f0,0xbe5d80,0xbe8db0],0x12a8240:[0xbf8ce0,0xbf6b40,0xbf6b50,0x9d5100,0xbf7f70],0x12a8258:[0xbf8d10,0xbf8cd0,0x9d50f0,0x9d5100,0xbf8cc0,0xbf6c80,0xbf6cd0,0xbf6d10,0xbf6d30]}
names=['CMovableResource','CLandscapeBackgroundSubNodeBase','CLandscapeBackgroundPatch','CLandscapeBackgroundTreeNode','CLandscapeBackgroundSubNodeSplitBranch'];out={}
for (vt,values),name in zip(vtables.items(),names):
 assert [u(vt+4*k)for k in range(len(values))]==values
 col=u(vt-4);assert u(col+4)==0;ch=u(col+16);h=[]
 for i in range(u(ch+8)):
  bc=u(u(ch+12)+4*i);h.append([rd(u(bc)+8,160).split(b'\0')[0].decode(),struct.unpack('<i',rd(bc+8,4))[0]])
 assert h[0]==['.?AV'+name+'@@',0]and h[-1]==['.?AVCMovableResource@@',0]
 if 'TreeNode' in name:assert h[1][0]=='.?AVCEngineLandscapeFreeableResource@@'
 out[hex(vt)]={'hierarchy':h,'slots':[hex(x)for x in values]}
assert rd(0xbe8dc0,3)==bytes.fromhex('b001c3') and rd(0xbf8cc0,3)==bytes.fromhex('32c0c3')
(o/'native_hierarchy_audit.json').write_text(json.dumps(out,indent=2))
records=[json.loads(l)for l in (w/'ATTEMPTS.jsonl').read_text().splitlines()];assert len(records)==19
for x in records:
 p=w/'snapshots'/f"{x['address']}_{x['attempt']}.cpp";assert hashlib.sha256(p.read_bytes()).hexdigest()==x['sha256'];assert x['command']and x['effective_flags']==['/O2','/Oy','/W3']and x['prior_reserved_uncertain']==4
assert {a:sum(x['address']==a for x in records)for a in ['00bf6640','00bf6930','00bf6d50','00bf7100']}=={'00bf6640':6,'00bf6930':5,'00bf6d50':4,'00bf7100':4}
(o/'attempt_audit.json').write_text(json.dumps(records,indent=2))
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
print('PASS8relocations/36virtualslots+5RTTIhierarchies/zeroFloat/19immutablegrades+16UNCERTAINreservations')

from pathlib import Path
import struct,re,runpy,sys,json,hashlib,subprocess
r=Path('work/codex-lane/w22');o=r/'coordinator_retry149';w=r/'astra238_whole';sys.argv=['x','00d2c530','0'];rd=runpy.run_path('work/codex-lane/tools/xdis.py')['rd'];rows=json.loads((o/'payload.json').read_text())['result']['authored'];count=0;vcount=0
M={'?LowerBound':0x40f020,'?SetOwnedByPlayer':0x6c17b0,'?SetRented':0x6c1150,'?GetMainPlayer':0x449970,'?GetHeroThing':0x487dc0,'??0CBaseClassNative':0x99a2d0,'??_7ScriptMethods':0x1238c8c,'?GetPrimarySlotThing':0x6a1440,'?GetScriptThing':0x4c7e50,'??4ScriptMethods':0x8ab980,'??1ScriptMethods':0x4aa840,'??3@':0xbfe9bc,'??1CBaseClassNative':0x99a2e0,'?SetMoneyAsConfiscated':0x57c999,'?GetPerceivedThing':0x7446a0,'?GetSoundRadius':0x744440,'?GetPosition':0x4c73d0,'??_C@':0x12783b8,'??0CCharString':0x99ebf0,'?GetDefGlobalIndexFromName':0x9ad410,'??1CCharString':0x99eae0,'?CreateMarker':0x83d460,'?SetOverridden':0x640df0,'?SetColour':0x640bb0,'?SetInnerRadius':0x640c40,'?SetOuterRadius':0x640c70,'?SetFlicker':0x640ca0,'?SetActive':0x6412b0,'?ConfiscateAllItems@':0x5bfa8e,'?ConfiscateAllItemsOfType':0x5bfac6,'?Get@CGameDefinitionManager':0x44c6b0}
for row in rows:
 a=row['address'];obj=next((o/'dry_work').rglob(a+'.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);blocks=[(b[i],b[i+1])for i in range(1,len(b),2)];rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',max([x for sec,x in blocks if sec=='.text'],key=len),re.M);out=[]
 for off,typ,sym in rel:
  n=int(off,16);native=struct.unpack('<i',rd(int(a,16)+n,4))[0]+int(a,16)+n+4 if typ=='DISP32'else struct.unpack('<I',rd(int(a,16)+n,4))[0];expected=[v for k,v in M.items()if sym.startswith(k)];assert len(expected)==1,(a,sym)
  if a=='008976f0'and sym.startswith('?LowerBound'):expected=[0x4365b0];assert '@@QBE' in sym
  assert native==expected[0],(a,sym,hex(native));out.append(f'{off}\t{native:08x}\t{sym}')
 count+=len(rel);(o/(a+'_relocation_audit.tsv')).write_text('offset\tnative_target\tsymbol\n'+'\n'.join(out)+'\n')
 src=(w/'src'/(a+'.cpp')).read_text();decl=re.findall(r'virtual ([^;]+); //slot(\d+) 0x([0-9a-fA-F]+)',src)
 if decl:
  vtobj=next((o/'dry_work').rglob(a+'.t.obj'));vtext=subprocess.check_output(['objdump','-r',str(vtobj)],text=True);vb=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',vtext);blocks=[(vb[i],vb[i+1])for i in range(1,len(vb),2)]
  assert len(decl)==78;vr=[re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',x,re.M)for sec,x in blocks if sec=='.rdata'and '??_EScriptMethods@@' in x and '?IsEqualTo@ScriptMethods' in x];assert len(vr)==1 and len(vr[0])==79;out=[]
  for declaration,slot,target in decl:
   slot=int(slot);target=int(target,16);assert struct.unpack('<I',rd(0x1238c8c+4*slot,4))[0]==target;off,typ,sym=vr[0][slot];assert int(off,16)==slot*4;method=re.search(r'(\w+)\(',declaration).group(1);assert sym.startswith('?'+method+'@');out.append(f'{slot}\t{target:08x}\t{sym}\t{declaration}')
  assert struct.unpack('<I',rd(0x1238c8c,4))[0]==0x4ab170;vcount+=79;(o/(a+'_virtual_audit.tsv')).write_text('slot\ttarget\tsymbol\tdeclaration\n'+'\n'.join(out)+'\n')
assert rd(0x12783b8,13)==b'MARKER_LIGHT\0';assert struct.unpack('<I',rd(0x1253dbc+73*4,4))[0]==0x5bf91a;assert rd(0x744440,4)==bytes.fromhex('d9413cc3')
(o/'native_auxiliary_evidence.txt').write_text('Inventory VT1253DBC slot73 =5BF91A ReturnAllConfiscatedItems. MARKER_LIGHT literal12783B8. GetSoundRadius744440 reads3C. BGRA PDB struct_layouts_egor.tsv8271..8274. Component lower_bound4365B0 is const,40F020 mutable.\n')
for row in json.loads((o/'frozen_hashes.json').read_text()):
 for k in ['src','test']:assert hashlib.sha256(Path(row[k]['path']).read_bytes()).hexdigest()==row[k]['sha256']
grades=[json.loads(x)for x in(w/'ATTEMPTS.jsonl').read_text().splitlines()];selected=[g for g in grades if g['address']in{z['address']for z in rows}];assert len(selected)==18,len(selected)
for g in selected:assert g['prior']==0 and g['effective_flags'] and hashlib.sha256(Path(g['command'][5]).read_bytes()).hexdigest()==g['sha256']
(o/'attempt_audit.json').write_text(json.dumps(selected,indent=2));print('PASS',count,'bodyrelocs',vcount,'ScriptThing slots,Inventory73,literal/radius,18 immutable grades and all hashes')

from pathlib import Path
import ctypes,re,csv,subprocess,json
p=Path('work/codex-lane/w22/coordinator_retry142');lib=ctypes.CDLL('/lib/x86_64-linux-gnu/libLLVM-17.so');fn=getattr(lib,'_ZN4llvm17microsoftDemangleESt17basic_string_viewIcSt11char_traitsIcEEPmPiNS_15MSDemangleFlagsE');fn.argtypes=[ctypes.c_size_t,ctypes.c_char_p,ctypes.c_void_p,ctypes.c_void_p,ctypes.c_int];fn.restype=ctypes.c_char_p
def demangle(s):
 b=s.encode();x=fn(len(b),b,None,None,0);return x.decode()if x else s
def norm(s):
 s=re.sub(r'\benum [A-Za-z_][A-Za-z_0-9:]*','long',s);s=re.sub(r'\b(class|struct) ','',s);s=s.replace('ScriptMethods','CScriptThing').replace('GameMethods','CGameScriptInterface').replace('GameCalls','CGameScriptInterface').replace('protected:','public:');return s
native=[l.split('\t')for l in Path('ghidra_out/cgsi_vtable_slots.tsv').read_text().splitlines()]
for a,owner in [('00d1e310','GameMethods'),('00d864b0','GameCalls')]:
 obj=next((p/'dry_work').rglob(a+'.t.obj'));s=subprocess.check_output(['objdump','-r',str(obj)],text=True);b=re.split(r'RELOCATION RECORDS FOR \[([^]]+)\]:',s);v=[]
 for i in range(1,len(b),2):
  if b[i]!='.rdata':continue
  rel=re.findall(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)',b[i+1],re.M)
  if len(rel)==845 and owner in rel[0][2]:v=rel
 assert len(v)==845;out=[];bad=[]
 for slot in range(1,845):
  ns=demangle(native[slot+4][2]);es=demangle(v[slot][2]);equal=norm(ns)==norm(es);out.append(dict(slot=slot,native=ns,emitted=es,normalized_equal=equal))
  if not equal:bad.append(out[-1])
 (p/(a+'_full_signature_audit.json')).write_text(json.dumps(out,indent=2));print(a,'mismatches',len(bad));print(json.dumps(bad[:4],indent=2));assert not bad
print('PASS1688 non-destructor full emitted signatures match native after documented layout-view/enum/access normalization.')

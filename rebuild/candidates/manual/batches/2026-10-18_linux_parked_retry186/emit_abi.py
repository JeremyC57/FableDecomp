from pathlib import Path
import sys,json
sys.path.insert(0,'tools/decomp_pipeline');import verify_and_land as V
D=Path('work/codex-lane/w22/coordinator_retry186').resolve();e=V.env()
for a in ['004573fd','00457ca0']:
 # Supplemental ABI-only compile: turn off novtable to observe otherwise suppressed table order.
 # Production source/parity fixture remain unchanged and separately verified.
 p=D/(a+'_emit_abi.cpp');p.write_text('#define __declspec(x)\n'+(D/(a+'_test.cpp')).read_text());obj=D/(a+'_emit_abi.obj');args=['/nologo','/c','/Od','/W3',f'/Fo{obj}',str(p)]
 x=V.cl(args,e);(D/(a+'_emit_abi.log')).write_text(json.dumps(args)+'\n'+x.stdout+x.stderr);assert x.returncode==0
 print(a,'ABI-only compile PASS')
